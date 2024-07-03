import tkinter as tk
from tkinter import filedialog, simpledialog, messagebox
import serial
import cantools
import datetime
import time

# Global variables for configuration and serial communication
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
        exec(open('latest_dbc.py').read())
        messagebox.showinfo("Success", "CAN message processed successfully!")
    except Exception as e:
        messagebox.showerror("Error", f"Error processing CAN message: {e}")

# Function to display decoded values
def display_values():
    try:
        exec(open('config.txt').read())
        exec(open('latest_dbc.py').read())

        # Create display frame
        display_frame = tk.Frame(root)
        display_frame.pack(fill="both", expand=True)

        # Labels to display decoded values
        tk.Label(display_frame, text="Expected Wheel Speed FR:").pack()
        tk.Label(display_frame, text=Expected_Wheel_Speed_FR).pack()
        tk.Label(display_frame, text="Expected Wheel Speed FL:").pack()
        tk.Label(display_frame, text=Expected_Wheel_Speed_FL).pack()
        tk.Label(display_frame, text="Expected Vehicle Speed:").pack()
        tk.Label(display_frame, text=Expected_Vehicle_Speed).pack()
        tk.Label(display_frame, text="Vehicle Speed Clock:").pack()
        tk.Label(display_frame, text=Expected_vehicleSpeedClock).pack()
        tk.Label(display_frame, text="Vehicle Speed CRC:").pack()
        tk.Label(display_frame, text=Expected_vehicleSpeedCRC).pack()

        # Back button to return to main screen
        tk.Button(display_frame, text="Back", command=show_main_screen).pack()

    except Exception as e:
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

