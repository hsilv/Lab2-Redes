import argparse


def hamming_combination(data: str) -> tuple:
    n = len(data)
    r = 0

    while (2 ** r) < (n + r + 1):
        r += 1

    return (n + r, r)

def insert_parity_bits(data: str, numParityBits: int) -> str:
    data = list(data)
    m = len(data)
    result = []
    parities = [0] * numParityBits

    j = 0
    k = 0
    for i in range(1, m + numParityBits + 1):
        if i == 2 ** j:
            result.append('0')
            j += 1
        else:
            result.append(data[k])
            k += 1

    for i in range(numParityBits):
        parity = 0
        for j in range(2 ** i - 1, len(result), 2 ** (i + 1)):
            for k in range(2 ** i):
                if j + k >= len(result):
                    break
                parity ^= result[j + k] == '1'
        parities[i] = parity
        result[2 ** i - 1] = str(int(parity))

    return ''.join(result)

def encode_hamming(data: str):
    # print(f"Original: {data}")
    numBits, numParityBits = hamming_combination(data)
    # print(f"Using Hamming({numBits}, {numBits - numParityBits})")
    data = insert_parity_bits(data, numParityBits)
    # print(f"Encoded: {data}")
    return data

def binary_string_to_bytes(binary_string: str) -> bytes:
    byte_array = bytearray()
    for i in range(0, len(binary_string), 8):
        byte = binary_string[i:i+8]
        byte_array.append(int(byte, 2))
    return bytes(byte_array)

def crc32(data: bytes) -> int:
    crc = 0xFFFFFFFF
    for byte in data:
        crc ^= byte
        for _ in range(8):
            if crc & 1:
                crc = (crc >> 1) ^ 0xEDB88320
            else:
                crc >>= 1
    return crc ^ 0xFFFFFFFF

def bytes_to_binary_string(data: bytes) -> str:
    return ''.join(format(byte, '08b') for byte in data)

def append_crc32(binary_string: str) -> str:
    data_bytes = binary_string_to_bytes(binary_string)
    checksum = crc32(data_bytes)
    checksum_binary = format(checksum, '032b')
    return binary_string + checksum_binary

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Encode data using different algorithms.")
    parser.add_argument("data", type=str, help="The binary data to encode")
    parser.add_argument("-Ha", "--hamming", action="store_true", help="Use Hamming code")
    parser.add_argument("-CRC", "--cyclic-redundancy-check", action="store_true", help="Use Cyclic Redundancy Check")
    args = parser.parse_args()

    data = args.data

    if args.hamming:
        encoded = encode_hamming(data)
    elif args.cyclic_redundancy_check:
        encoded = append_crc32(data)
    else:
        encoded = "No encoding algorithm selected."

    print(encoded)