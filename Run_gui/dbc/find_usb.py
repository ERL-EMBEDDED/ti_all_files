import serial.tools.list_ports

# Function to find and return the device name of CH340 serial converter
def find_serial_device():
    ports = serial.tools.list_ports.comports()
    for port in ports:
        if port.vid == 0x1a86 and port.pid == 0x7523:  # Check for QinHeng Electronics CH340
            return port.device
    return None

# Example usage
serial_port = find_serial_device()
if serial_port is not None:
    print(f"Found CH340 serial converter at {serial_port}")
else:
    print("CH340 serial converter not found")
