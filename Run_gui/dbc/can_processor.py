import tkinter as tk
from tkinter import filedialog, simpledialog, messagebox
import subprocess
import os

# Global variables for configuration
message_id_hex = ""
dbc_file_path = ""
serial_port = ""

# Load the configuration if available
try:
    with open('config.txt', 'r') as f:
        exec(f.read())
except FileNotFoundError:
    pass

# Function to load DBC file and configure serial port
def configure():
    global dbc_file_path, serial_port
    dbc_file_path = filedialog.askopenfilename(title="Select DBC File")
    serial_port = simpledialog.askstring("Serial Port", "Enter Serial Port (e.g., /dev/ttyCH341USB0):")

# Function to save configuration and proceed to main screen
def save_config():
    global message_id_hex, dbc_file_path, serial_port
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
        display_window = tk.Toplevel(root)
        display_window.title("Decoded Values")

        # Labels to display decoded values
        for value in decoded_values:
            tk.Label(display_window, text=value).pack()

        # Back button to return to main screen
        tk.Button(display_window, text="Back", command=display_window.destroy).pack()

    except subprocess.CalledProcessError as e:
        messagebox.showerror("Error", f"Error displaying values: {e}")

# Function to switch to main screen
def show_main_screen():
    # Destroy current frame (if any)
    for widget in root.winfo_children():
        widget.destroy()
    
    # Create main screen frame
    main_frame = tk.Frame(root)
    main_frame.pack(fill="both", expand=True)

    # Configuration input fields
    tk.Label(main_frame, text="Enter Message ID (e.g., 0x28A):").pack()
    global entry_id
    entry_id = tk.Entry(main_frame)
    entry_id.pack()

    tk.Button(main_frame, text="Select DBC File", command=configure).pack()
    tk.Button(main_frame, text="Save Config", command=save_config).pack()
    tk.Button(main_frame, text="Run CAN Processing", command=run_code).pack()
    tk.Button(main_frame, text="Display Decoded Values", command=display_values).pack()

# Create the main window
root = tk.Tk()
root.title("CAN Message Processor")

# Show the main screen initially
show_main_screen()

root.mainloop()
