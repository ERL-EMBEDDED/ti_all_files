import serial
import datetime

# Replace '/dev/ttyCH341USB0' with your actual serial port and configure other settings
ser = serial.Serial('/dev/ttyCH341USB0', baudrate=115200, timeout=0.2)

def twos_complement(value, bits=8):
    """Compute the two's complement of an integer."""
    if value & (1 << (bits - 1)):
        value -= 1 << bits
    return value

try:
    if ser.isOpen():
        print(f"Sending and receiving data on {ser.name}... Press Ctrl+C to stop.")
        
        while True:
            # Read data from the serial port if available
            if ser.in_waiting > 0:
                received_data = ser.read(ser.in_waiting)
                timestamp = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S.%f")[:-3]
                print(f"{timestamp} - Received Data: {' '.join([f'{byte:02X}' for byte in received_data])}")

                # Extract vehicle speed from received_data[1] and interpret it as a signed integer
                vehicle_speed = twos_complement(int.from_bytes(received_data[1:2], byteorder='big', signed=True))
                print(f"Vehicle Speed: {vehicle_speed}")

                # Echo back the received data
                ser.write(received_data)
                print(f"Sent Data: {received_data.hex()}")

except KeyboardInterrupt:
    pass
finally:
    ser.close()
