import serial
import time

# Replace '/dev/ttyUSB0' with your specific port
ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)
time.sleep(2)  # Give some time for the connection to be established

def write_to_arduino(data):
    ser.write(data.encode())
    print(f"Sent: {data}")

def read_from_arduino():
    if ser.in_waiting > 0:
        line = ser.readline().decode('utf-8').rstrip()
        print(f"Received: {line}")
        return line
    return None

try:
    while True:
        user_input = input("Enter data to send to Arduino: ")
        write_to_arduino(user_input)
        
        time.sleep(1)  # Wait a bit before reading response
        read_from_arduino()
except KeyboardInterrupt:
    print("Program interrupted")
finally:
    ser.close()

