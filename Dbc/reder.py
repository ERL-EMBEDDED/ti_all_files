import cantools

def read_dbc(file_path):
    try:
        # Load the DBC file
        db = cantools.database.Database()
        db.add_dbc_file(file_path)

        # Print messages and their attributes
        for message in db.messages:
            print("Message Name:", message.name)
            print("  ID (hex):", hex(message.frame_id))
            print("  Length:", message.length)
            print("  Signals:")
            for signal in message.signals:
                print("    Name:", signal.name)
                print("    Start Bit:", signal.start)
                print("    Length:", signal.length)
                print("    Byte Order:", signal.byte_order)
                print("    Signed:", signal.is_signed)
                print("    Scale:", signal.scale)
                print("    Offset:", signal.offset)
                print("    Minimum:", signal.minimum)
                print("    Maximum:", signal.maximum)
                print("    Unit:", signal.unit)
                print("    Choices:", signal.choices)
                print()

    except Exception as e:
        print("Error:", e)

# Provide the path to your .dbc file
dbc_file_path = "/home/ti/Dbc/Batch3_202108.dbc"
read_dbc(dbc_file_path)

