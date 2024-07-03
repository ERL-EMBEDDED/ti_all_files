import serial
import datetime
import time
import cantools
import sys

# Load the configuration
with open('config.txt', 'r') as f:
    exec(f.read())

class DecodedCANMessage:
    def __init__(self):
        self.wheelSpeedFR = 0.0
        self.wheelSpeedFL = 0.0
        self.vehicleSpeed = 0.0
        self.vehicleSpeedClock = 0
        self.vehicleSpeedCRC = 0

def extract_bits(data, start, length):
    mask = (1 << length) - 1
    return (data >> start) & mask

def set_bits(data, start, length, value):
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
db = cantools.database.load_file(dbc_file_path)

# Example: Find a specific message by its ID
specific_message = find_message_by_id(db.messages, message_id_hex)

if specific_message:
    signal_names = ['WheelSpeedFR', 'WheelSpeedFL', 'VehicleSpeed', 'VehicleSpeedClock']
    signal_details = {}
    for name in signal_names:
        start_bit, length, scale = get_signal_details(specific_message, name)
        signal_details[name] = (start_bit, length, scale)
        if start_bit is None:
            raise ValueError(f"Signal '{name}' not found in message {message_id_hex}")

    _config_wheelSpeedFR_start_bit, _config_wheelSpeedFR_length, _config_wheelSpeedFR_Scale = signal_details['WheelSpeedFR']
    _config_wheelSpeedFL_start_bit, _config_wheelSpeedFL_length, _config_wheelSpeedFL_Scale = signal_details['WheelSpeedFL']
    _config_vehicleSpeed_start_bit, _config_vehicleSpeed_length, _config_vehicleSpeed_Scale = signal_details['VehicleSpeed']
    _config_vehicleSpeedClock_start_bit, _config_vehicleSpeedClock_length, _ = signal_details['VehicleSpeedClock']

else:
    raise ValueError(f"Message with ID {message_id_hex} not found.")

def decodeCANMessage(msg):
    decoded = DecodedCANMessage()
    
    raw_data = int.from_bytes(msg, byteorder='little')

    decoded.wheelSpeedFR = float(extract_bits(raw_data, _config_wheelSpeedFR_start_bit, _config_wheelSpeedFR_length) * _config_wheelSpeedFR_Scale)
    decoded.wheelSpeedFL = float(extract_bits(raw_data, _config_wheelSpeedFL_start_bit, _config_wheelSpeedFL_length) * _config_wheelSpeedFL_Scale)
    decoded.vehicleSpeed = float(extract_bits(raw_data, _config_vehicleSpeed_start_bit, _config_vehicleSpeed_length) * _config_vehicleSpeed_Scale)
    decoded.vehicleSpeedClock = int(extract_bits(raw_data, _config_vehicleSpeedClock_start_bit, _config_vehicleSpeedClock_length))

    decoded.vehicleSpeedCRC = int(extract_bits(raw_data, 1, 6) << 2)
    decoded.vehicleSpeedCRC |= (extract_bits(raw_data, 0, 1) & 0x01)
    decoded.vehicleSpeedCRC |= (extract_bits(raw_data, 63, 1) & 0x01) << 1

    return decoded

def encodeCANMessage(decoded):
    raw_data = 0
    Actual_speed = decoded.vehicleSpeed
    
    if decoded.vehicleSpeed > 0:
        Actual_speed = decoded.vehicleSpeed - 1.0
    
    wheelSpeedFR_encoded = int((decoded.wheelSpeedFR) / _config_wheelSpeedFR_Scale)
    wheelSpeedFL_encoded = int((decoded.wheelSpeedFL) / _config_wheelSpeedFL_Scale)
    vehicleSpeed_encoded = int((Actual_speed) / _config_vehicleSpeed_Scale)
    vehicleSpeedClock_encoded = int(decoded.vehicleSpeedClock)

    raw_data = set_bits(raw_data, _config_wheelSpeedFR_start_bit, _config_wheelSpeedFR_length, wheelSpeedFR_encoded)
    raw_data = set_bits(raw_data, _config_wheelSpeedFL_start_bit, _config_wheelSpeedFL_length, wheelSpeedFL_encoded)
    raw_data = set_bits(raw_data, _config_vehicleSpeed_start_bit, _config_vehicleSpeed_length, vehicleSpeed_encoded)
    raw_data = set_bits(raw_data, _config_vehicleSpeedClock_start_bit, _config_vehicleSpeedClock_length, vehicleSpeedClock_encoded)

    raw_data = set_bits(raw_data, 1, 6, extract_bits(decoded.vehicleSpeedCRC, 2, 6))
    raw_data = set_bits(raw_data, 0, 1, extract_bits(decoded.vehicleSpeedCRC, 0, 1))
    raw_data = set_bits(raw_data, 63, 1, extract_bits(decoded.vehicleSpeedCRC, 1, 1))

    encoded = raw_data.to_bytes((raw_data.bit_length() + 7) // 8, byteorder='little')
    return encoded

if __name__ == "__main__":
    if '--display' in sys.argv:
        # Display the decoded values
        if Expected_Wheel_Speed_FR is not None:
            print(f"Expected Wheel Speed FR: {Expected_Wheel_Speed_FR}")
            print(f"Expected Wheel Speed FL: {Expected_Wheel_Speed_FL}")
            print(f"Expected Vehicle Speed: {Expected_Vehicle_Speed}")
            print(f"Vehicle Speed Clock: {Expected_vehicleSpeedClock}")
            print(f"Vehicle Speed CRC: {Expected_vehicleSpeedCRC}")
        else:
            print("No values to display. Run the CAN processing first.")
    else:
        try:
            with serial.Serial(serial_port, 115200, timeout=1) as ser:
                while True:
                    ser.write(b'\x08\x28\x00\x00\x00\x00\x00\x00')
                    response = ser.read(8)

                    if response:
                        decoded = decodeCANMessage(response)
                        decoded.vehicleSpeed += 1.0

                        global Expected_Wheel_Speed_FR
                        global Expected_Wheel_Speed_FL
                        global Expected_Vehicle_Speed
                        global Expected_vehicleSpeedClock
                        global Expected_vehicleSpeedCRC
                        
                        Expected_Wheel_Speed_FR = decoded.wheelSpeedFR
                        Expected_Wheel_Speed_FL = decoded.wheelSpeedFL
                        Expected_Vehicle_Speed = decoded.vehicleSpeed
                        Expected_vehicleSpeedClock = decoded.vehicleSpeedClock
                        Expected_vehicleSpeedCRC = decoded.vehicleSpeedCRC

                        print(f"Expected Wheel Speed FR: {Expected_Wheel_Speed_FR}")
                        print(f"Expected Wheel Speed FL: {Expected_Wheel_Speed_FL}")
                        print(f"Expected Vehicle Speed: {Expected_Vehicle_Speed}")
                        print(f"Vehicle Speed Clock: {Expected_vehicleSpeedClock}")
                        print(f"Vehicle Speed CRC: {Expected_vehicleSpeedCRC}")

                        time.sleep(0.5)
        except Exception as e:
            print(f"An error occurred: {e}")
