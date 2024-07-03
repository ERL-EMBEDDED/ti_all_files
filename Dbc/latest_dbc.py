import serial
import datetime
import time
import cantools

class DecodedCANMessage:
    def __init__(self):
        self.wheelSpeedFR = 0.0
        self.wheelSpeedFL = 0.0
        self.vehicleSpeed = 0.0
        self.vehicleSpeedClock = 0
        self.vehicleSpeedCRC = 0

def extract_bits(data, start, length):
    """ Extract bits from data based on start position and length. """
    mask = (1 << length) - 1
    return (data >> start) & mask

def set_bits(data, start, length, value):
    """ Set bits in data based on start position and length. """
    mask = (1 << length) - 1
    value = (value & mask) << start
    data = data & ~(mask << start)  # Clear the bits
    data = data | value  # Set the new value
    return data

def signal_to_json(signal):
    return {
        "name": signal.name,
        "start_bit": signal.start,
        "bit_length": signal.length,
        "is_big_endian": signal.byte_order == 'big_endian',
        "is_signed": signal.is_signed,
        "scale": signal.scale,
        "offset": signal.offset,
        "minimum": signal.minimum,
        "maximum": signal.maximum,
        "unit": signal.unit
    }

def message_to_json(message):
    return {
        "name": message.name,
        "id_hex": f"0x{message.frame_id:03X}",
        "length": message.length,
        "signals": [signal_to_json(signal) for signal in message.signals]
    }

def find_message_by_id(messages, id_hex):
    for message in messages:
        if f"0x{message.frame_id:03X}" == id_hex:
            return message
    return None

def get_signal_details(message, signal_name):
    for signal in message.signals:
        if signal.name == signal_name:
            return signal.start, signal.length, signal.scale
    return None, None, None

# Load the DBC file
db = cantools.database.load_file('/home/ti/Dbc/Batch3_202108.dbc')

# List all messages and their IDs
print("Available messages in the DBC file:")
for message in db.messages:
    print(f"ID: 0x{message.frame_id:03X}, Name: {message.name}")

# Example: Find a specific message by its ID
message_id_hex = "0x28A"  # Replace with the ID you are looking for
specific_message = find_message_by_id(db.messages, message_id_hex)

if specific_message:
    print(f"\nSignals in message ID {message_id_hex}:")
    for signal in specific_message.signals:
        print(f"Name: {signal.name}, Start bit: {signal.start}, Length: {signal.length}")

    # Attempt to get signal details
    signal_names = ['WheelSpeedFR', 'WheelSpeedFL', 'VehicleSpeed', 'VehicleSpeedClock']
    signal_details = {}
    for name in signal_names:
        start_bit, length, scale = get_signal_details(specific_message, name)
        signal_details[name] = (start_bit, length, scale)
        if start_bit is None:
            print(f"Signal '{name}' not found in message {message_id_hex}")

    _config_wheelSpeedFR_start_bit, _config_wheelSpeedFR_length, _config_wheelSpeedFR_Scale = signal_details['WheelSpeedFR']
    _config_wheelSpeedFL_start_bit, _config_wheelSpeedFL_length, _config_wheelSpeedFL_Scale = signal_details['WheelSpeedFL']
    _config_vehicleSpeed_start_bit, _config_vehicleSpeed_length, _config_vehicleSpeed_Scale = signal_details['VehicleSpeed']
    _config_vehicleSpeedClock_start_bit, _config_vehicleSpeedClock_length, _ = signal_details['VehicleSpeedClock']
    
    if None in [_config_wheelSpeedFR_start_bit, _config_wheelSpeedFL_start_bit, _config_vehicleSpeed_start_bit, _config_vehicleSpeedClock_start_bit]:
        raise ValueError(f"Message with ID {message_id_hex} does not contain all required signals.")
else:
    raise ValueError(f"Message with ID {message_id_hex} not found.")

def decodeCANMessage(msg):
    decoded = DecodedCANMessage()
    
    # Assuming msg is a byte array representing the CAN message data
    raw_data = int.from_bytes(msg, byteorder='little')

    # Extract and decode values
    decoded.wheelSpeedFR = float(extract_bits(raw_data, _config_wheelSpeedFR_start_bit, _config_wheelSpeedFR_length) * _config_wheelSpeedFR_Scale)
    decoded.wheelSpeedFL = float(extract_bits(raw_data, _config_wheelSpeedFL_start_bit, _config_wheelSpeedFL_length) * _config_wheelSpeedFL_Scale)
    decoded.vehicleSpeed = float(extract_bits(raw_data, _config_vehicleSpeed_start_bit, _config_vehicleSpeed_length) * _config_vehicleSpeed_Scale)
    decoded.vehicleSpeedClock = int(extract_bits(raw_data, _config_vehicleSpeedClock_start_bit, _config_vehicleSpeedClock_length))

    # Extract Vehicle Speed CRC, combining parts
    decoded.vehicleSpeedCRC = int(extract_bits(raw_data, 1, 6) << 2)
    decoded.vehicleSpeedCRC |= (extract_bits(raw_data, 0, 1) & 0x01)
    decoded.vehicleSpeedCRC |= (extract_bits(raw_data, 63, 1) & 0x01) << 1

    return decoded

def encodeCANMessage(decoded):
    # Assuming we want to encode the values back into a CAN message format with subtraction of 1.0
    raw_data = 0
    Actual_speed =decoded.vehicleSpeed
    
    if decoded.vehicleSpeed >0:
        Actual_speed =decoded.vehicleSpeed-1.0
    # Subtract 1.0 from the values
    wheelSpeedFR_encoded = int((decoded.wheelSpeedFR) / _config_wheelSpeedFR_Scale)
    wheelSpeedFL_encoded = int((decoded.wheelSpeedFL) / _config_wheelSpeedFL_Scale)
    vehicleSpeed_encoded = int((decoded.vehicleSpeed-1.0) / _config_vehicleSpeed_Scale)
    vehicleSpeedClock_encoded = decoded.vehicleSpeedClock
    vehicleSpeedCRC = decoded.vehicleSpeedCRC

    # Encode the values into raw data
    raw_data = set_bits(raw_data, _config_wheelSpeedFR_start_bit, _config_wheelSpeedFR_length, wheelSpeedFR_encoded)
    raw_data = set_bits(raw_data, _config_wheelSpeedFL_start_bit, _config_wheelSpeedFL_length, wheelSpeedFL_encoded)
    raw_data = set_bits(raw_data, _config_vehicleSpeed_start_bit, _config_vehicleSpeed_length, vehicleSpeed_encoded)
    raw_data = set_bits(raw_data, _config_vehicleSpeedClock_start_bit, _config_vehicleSpeedClock_length, vehicleSpeedClock_encoded)

    # Combine the Vehicle Speed CRC parts back
    raw_data = set_bits(raw_data, 1, 6, (vehicleSpeedCRC >> 2) & 0x3F)
    raw_data = set_bits(raw_data, 0, 1, vehicleSpeedCRC & 0x01)
    raw_data = set_bits(raw_data, 63, 1, (vehicleSpeedCRC >> 1) & 0x01)

    # Convert raw_data to bytes
    encoded_msg = raw_data.to_bytes((raw_data.bit_length() + 7) // 8, byteorder='little')
    return encoded_msg

def log_data(tag, data, is_expected=False):
    timestamp = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S.%f")[:-3]
    print(f"{timestamp} - {tag}: {data}")

def parse_can_message(hex_string):
    # Ensure the hex string is long enough and contains only valid hex characters
    if len(hex_string) < 17 or any(c not in '0123456789abcdefABCDEF' for c in hex_string):
        raise ValueError("Invalid hex string received")

    # Extract CAN ID (assuming first 4 characters) and data (the rest of the string)
    can_id_str = hex_string[:4]
    can_data_str = hex_string[4:]
    can_id = int(can_id_str, 16)
    can_data = bytes.fromhex(can_data_str)
    return can_id, can_data

# Replace '/dev/ttyUSB0' with your actual serial port and configure other settings
ser = serial.Serial('/dev/ttyCH341USB0', baudrate=115200, timeout=0.1)

try:
    while True:
        if ser.isOpen():
            received_data = ser.read(ser.in_waiting)
            if received_data:
                received_hex = received_data.hex()
                log_data("Received HEX", received_hex)
                
                # Parse the received data
                try:
                    can_id, can_data = parse_can_message(received_hex)
                    log_data("CAN ID", f"0x{can_id:03X}")
                    
                    # Assuming the received data is the CAN message in bytes
                    decoded_message = decodeCANMessage(can_data)
                    
                    Expected_Wheel_Speed_FR = decoded_message.wheelSpeedFR
                    Expected_Wheel_Speed_FL = decoded_message.wheelSpeedFL
                    Expected_Vehicle_Speed  = decoded_message.vehicleSpeed
                    Expected_vehicleSpeedClock = decoded_message.vehicleSpeedClock
                    Expected_vehicleSpeedCRC   = decoded_message.vehicleSpeedCRC
                   
                    # Log actual decoded values
                    log_data("Expected_Wheel Speed FR", Expected_Wheel_Speed_FR)
                    log_data("Expected_Wheel Speed FL", Expected_Wheel_Speed_FL)
                    log_data("Expected_Vehicle Speed", Expected_Vehicle_Speed)
                    log_data("Vehicle Speed Clock", Expected_vehicleSpeedClock)
                    log_data("Vehicle Speed CRC", Expected_vehicleSpeedCRC)
                    
                    # Encode the modified message
                    encoded_data = encodeCANMessage(decoded_message)
                    log_data("Encoded Data", encoded_data.hex())
                    
                    # Prepend the CAN ID (0x123) to the encoded data
                    encoded_message_with_id = b'\x01\x26' + encoded_data  # 0x123 is 0x231 in little endian
                    log_data("Encoded Msg with ID", encoded_message_with_id.hex())
                    
                    # Send the encoded message with ID
                    #ser.write(encoded_message_with_id)
                    
                    if encoded_message_with_id:
                       ser.write(encoded_message_with_id)
                       
                       send_hex_data = b''  # Clear the data after sending to prevent re-sending
                except ValueError as e:
                    log_data("Error", str(e))

        time.sleep(0.1)
except KeyboardInterrupt:
    print("Interrupted by user")
finally:
    if ser.isOpen():
        ser.close()

