import serial

# Open the serial port
ser = serial.Serial('/dev/ttyUSB4', 115200, timeout=1)

try:
    while True:
        # Read data from serial port
        data = ser.readline().strip()

        # Check if data is not empty
        if data:
            # Assuming the received data is in a specific format,
            # you need to parse it according to your CAN message structure.
            # Here, we're just printing the raw received data.
            print("Received:", data.decode('utf-8'))  # Decode bytes to string

except KeyboardInterrupt:
    print("Exiting...")
    ser.close()

