import serial
import datetime
import time
import json
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

# Example: Find a specific message by its ID
message_id_hex = "0x28A"  # Replace with the ID you are looking for
specific_message = find_message_by_id(db.messages, message_id_hex)

if specific_message:
    _config_wheelSpeedFR_start_bit, _config_wheelSpeedFR_length, _config_wheelSpeedFR_Scale = get_signal_details(specific_message, 'wheelSpeedFR')
    _config_wheelSpeedFL_start_bit, _config_wheelSpeedFL_length, _config_wheelSpeedFL_Scale = get_signal_details(specific_message, 'wheelSpeedFL')
    _config_vehicleSpeed_start_bit, _config_vehicleSpeed_length, _config_vehicleSpeed_Scale = get_signal_details(specific_message, 'vehicleSpeed')
    _config_vehicleSpeedClock_start_bit, _config_vehicleSpeedClock_length, _ = get_signal_details(specific_message, 'vehicleSpeedClock')
else:
    raise ValueError(f"Message with ID {message_id_hex} not found or missing signals.")

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

def log_data(tag, data):
    timestamp = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S.%f")[:-3]
    print(f"{timestamp} - {tag}: {data}")

# Replace '/dev/ttyUSB0' with your actual serial port and configure other settings
ser = serial.Serial('/dev/ttyCH341USB0', baudrate=115200, timeout=0.1)

try:
    while True:
        if ser.isOpen():
            received_data = ser.read(ser.in_waiting)
            if received_data:
                received_hex = received_data.hex()
                log_data("Received HEX", received_hex)
                
                # Assuming the received data is the CAN message in bytes
                decoded_message = decodeCANMessage(received_data)
                
                log_data("Wheel Speed FR", decoded_message.wheelSpeedFR)
                log_data("Wheel Speed FL", decoded_message.wheelSpeedFL)
                log_data("Vehicle Speed", decoded_message.vehicleSpeed)
                log_data("Vehicle Speed Clock", decoded_message.vehicleSpeedClock)
                log_data("Vehicle Speed CRC", decoded_message.vehicleSpeedCRC)

        time.sleep(0.1)
except KeyboardInterrupt:
    print("Interrupted by user")
finally:
    if ser.isOpen():
        ser.close()

