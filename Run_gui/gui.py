import tkinter as tk
from tkinter import filedialog, simpledialog, messagebox
import subprocess
from PIL import ImageTk, Image

# Global variables for configuration
message_id_hex = ""
dbc_file_path = ""
serial_port = ""
entry_id = None  # Define entry_id as a global variable

# Function to handle logo clicks and switch frames
def handle_logo_click(message):
    if message == "Welcome":
        message_label.config(text=message)
        main_frame.pack_forget()  # Hide main frame
        message_frame.pack()  # Show message frame
    elif message == "Configuration":
        show_main_screen()
    elif message == "Run":
        run_code()  # Call run_code function to start CAN processing
    elif message == "Display":
        display_values()  # Call display_values function to show decoded values

# Function to go back to main window
def go_back():
    if message_frame.winfo_exists():
        message_frame.pack_forget()  # Hide message frame
    if 'display_window' in globals() and display_window.winfo_exists():
        display_window.destroy()  # Destroy display window

    main_frame.pack()  # Show main frame

# Function to load DBC file and configure serial port
def configure():
    global dbc_file_path, serial_port
    dbc_file_path = filedialog.askopenfilename(title="Select DBC File")
    serial_port = simpledialog.askstring("Serial Port", "Enter Serial Port (e.g., /dev/ttyCH341USB0):")

# Function to save configuration and proceed to main screen
def save_config():
    global message_id_hex, dbc_file_path, serial_port
    global entry_id  # Access entry_id as global
    message_id_hex = entry_id.get()
    with open('config.txt', 'w') as f:
        f.write(f"message_id_hex = '{message_id_hex}'\n")
        f.write(f"dbc_file_path = '{dbc_file_path}'\n")
        f.write(f"serial_port = '{serial_port}'\n")
    show_main_screen()

# Function to run CAN message processing
def run_code():
    try:
        subprocess.run(['python3', 'latest_dbc.py'], check=True)
        messagebox.showinfo("Success", "CAN message processed successfully!")
    except FileNotFoundError:
        messagebox.showerror("Error", "File 'latest_dbc.py' not found. Please ensure it exists.")
    except subprocess.CalledProcessError as e:
        messagebox.showerror("Error", f"Error processing CAN message: {e}")

# Function to display decoded values
def display_values():
    try:
        # Execute the latest_dbc.py file to process the CAN messages and decode values
        output = subprocess.check_output(['python3', 'latest_dbc.py', '--display'])
        decoded_values = output.decode('utf-8').strip().split('\n')

        # Create a new window to display the values
        global display_window
        display_window = tk.Toplevel(root)
        display_window.title("Decoded Values")

        # Labels to display decoded values
        for value in decoded_values:
            tk.Label(display_window, text=value).pack()

        # Back button to return to main screen
        tk.Button(display_window, text="Back", command=go_back).pack()

    except subprocess.CalledProcessError as e:
        messagebox.showerror("Error", f"Error displaying values: {e}")

# Function to switch to main screen
def show_main_screen():
    # Destroy current frame (if any)
    for widget in root.winfo_children():
        widget.destroy()
    
    # Create main screen frame
    global main_frame
    main_frame = tk.Frame(root)
    main_frame.pack(fill="both", expand=True)

    # Configuration input fields
    tk.Label(main_frame, text="Enter Message ID (e.g., 0x28A):").pack()
    global entry_id
    entry_id = tk.Entry(main_frame)
    entry_id.pack()

    # Buttons for configuration screen
    tk.Button(main_frame, text="Select DBC File", command=configure).pack()
    tk.Button(main_frame, text="Save Config", command=save_config).pack()

    # Back button to return to welcome screen
    tk.Button(main_frame, text="Back", command=go_back).pack()

# Function to handle window close event
def on_closing():
    if messagebox.askokcancel("Quit", "Do you want to quit?"):
        root.destroy()

# Create the main window
root = tk.Tk()
root.title("CAN Message Processor")

# Bind the close event
root.protocol("WM_DELETE_WINDOW", on_closing)

# Main Frame (initial screen)
main_frame = tk.Frame(root)

# Load and display Welcome logo (replace with absolute path)
welcome_logo_path = "erl.png"
welcome_logo_image = Image.open(welcome_logo_path)
welcome_logo_photo = ImageTk.PhotoImage(welcome_logo_image)
welcome_logo_label = tk.Label(main_frame, image=welcome_logo_photo)
welcome_logo_label.pack(side=tk.TOP, padx=10, pady=10)  # Pack at the top with padding

# Bind click event to Welcome logo label
welcome_logo_label.bind("<Button-1>", lambda event: handle_logo_click("Welcome"))

# Frame for company logos (to display them in a row)
company_logos_frame = tk.Frame(main_frame)
company_logos_frame.pack(side=tk.TOP, padx=10, pady=10)  # Pack at the top with padding

# Load and display first company logo (replace with absolute path)
company1_logo_path = "config.png"
company1_logo_image = Image.open(company1_logo_path)
company1_logo_photo = ImageTk.PhotoImage(company1_logo_image)
company1_logo_label = tk.Label(company_logos_frame, image=company1_logo_photo)
company1_logo_label.pack(side=tk.LEFT, padx=10)  # Pack to the left with padding

# Bind click event to first company logo label
company1_logo_label.bind("<Button-1>", lambda event: handle_logo_click("Configuration"))

# Load and display second company logo (replace with absolute path)
company2_logo_path = "run.png"
company2_logo_image = Image.open(company2_logo_path)
company2_logo_photo = ImageTk.PhotoImage(company2_logo_image)
company2_logo_label = tk.Label(company_logos_frame, image=company2_logo_photo)
company2_logo_label.pack(side=tk.LEFT, padx=220)  # Pack to the left with padding

# Bind click event to second company logo label
company2_logo_label.bind("<Button-1>", lambda event: handle_logo_click("Run"))

# Load and display third company logo (replace with absolute path)
company3_logo_path = "display.png"
company3_logo_image = Image.open(company3_logo_path)
company3_logo_photo = ImageTk.PhotoImage(company3_logo_image)
company3_logo_label = tk.Label(company_logos_frame, image=company3_logo_photo)
company3_logo_label.pack(side=tk.LEFT, padx=50)  # Pack to the left with padding

# Bind click event to third company logo label
company3_logo_label.bind("<Button-1>", lambda event: handle_logo_click("Display"))

# Message Frame (frame for displaying messages)
message_frame = tk.Frame(root)

# Label to display messages
message_label = tk.Label(message_frame, text="", font=("Arial", 18))
message_label.pack(pady=50)

# Pack main frame initially
main_frame.pack()

# Run the main loop
root.mainloop()

