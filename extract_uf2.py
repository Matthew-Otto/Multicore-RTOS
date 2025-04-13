import sys
import struct

# Define UF2 header size and magic number
UF2_HEADER_SIZE = 512
UF2_MAGIC_NUMBER = 0x0A324655  # Magic number to identify UF2 files

def read_uf2_header(file_path):
    with open(file_path, 'rb') as file:
        header = file.read(UF2_HEADER_SIZE)  # Read the first 512 bytes

        # Check if the magic number matches
        magic_number = struct.unpack('<I', header[:4])[0]
        if magic_number != UF2_MAGIC_NUMBER:
            print("Invalid UF2 file!")
            return

        # Extract data from the header
        file_size = struct.unpack('<I', header[4:8])[0]
        target_address = struct.unpack('<I', header[12:16])[0]
        flash_size = struct.unpack('<I', header[16:20])[0]
        
        # Print the extracted information
        print(f"Magic Number: {hex(magic_number)}")
        print(f"File Size: {file_size} bytes")
        print(f"Target Address: {hex(target_address)}")
        print(f"Flash Size: {flash_size} bytes")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python extract_uf2.py <input.uf2> <output.bin>")
        sys.exit(1)

    uf2_file = sys.argv[1]
    read_uf2_header(uf2_file)
