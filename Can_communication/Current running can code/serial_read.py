import serial
import datetime
import time

# Replace '/dev/ttyUSB0' with your actual serial port and configure other settings
ser = serial.Serial('/dev/ttyUSB1', baudrate=115200, timeout=0.2)

try:
    if ser.isOpen():
        print(f"Sending and receiving data on {ser.name}... Press Ctrl+C to stop.")

        while True:
            # Define the hex data to be sent
            send_hex_data = b"\x00\x00\x01\x23\x33\x33"
            
            # Send the hex data to the serial port
            ser.write(send_hex_data)
            
            # Get the current timestamp
            timestamp = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S.%f")[:-3]
            
            # Print the timestamp and the hex data sent
            print(f"{timestamp} - Sent Hex: {send_hex_data.hex()}")
            
            # Add a small delay before reading the response
            time.sleep(0.5)
            
            # Read data from the serial port if available
            if ser.in_waiting > 0:
                received_data = ser.read(ser.in_waiting)
                timestamp = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S.%f")[:-3]
                print(f"{timestamp} - Received Hex: {received_data.hex()}")

except KeyboardInterrupt:
    pass
finally:
    ser.close()

