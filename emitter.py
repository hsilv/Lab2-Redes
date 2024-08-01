import json
from random import random
import socket
import inquirer

#Escapes ANSI

STRONG_SKYBLUE = '\033[1;36m'
STRONG_RED = '\033[1;31m'
STRONG_GREEN = '\033[1;32m'
STRONG_YELLOW = '\033[1;33m'
STRONG_WHITE = '\033[1;37m'
STRONG_PURPLE = '\033[1;35m'
STRONG_CYAN = '\033[1;36m'
STRONG_BLUE = '\033[1;34m'
CLOSE = '\033[0m'

# CONFIGURACIÓN DE SOCKET
HOST = '127.0.0.1'
PORT = 65432


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
    numBits, numParityBits = hamming_combination(data)
    data = insert_parity_bits(data, numParityBits)
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

# CAPA DE ENLACE
def calcular_integridad(data: str, algorithm: str) -> str:
    if algorithm == 'Hamming':
        encoded = encode_hamming(data)
    elif algorithm == 'Cyclic Redundancy Check (CRC32)':
        encoded = append_crc32(data)
    else:
        encoded = "No encoding algorithm selected."
    return encoded

# CAPA DE PRESENTACIÓN
def codificar_mensaje(ascii_string: str) -> str:
    return ''.join(format(ord(char), '08b') for char in ascii_string)

# CAPA DE APLICACIÓN
def solicitar_mensaje():
    string = input(f"{STRONG_SKYBLUE}Ingrese un mensaje: {CLOSE}")
    
    errors = input(f"{STRONG_SKYBLUE}Ingrese la cantidad de errores a simular en formato (errores/bits): {CLOSE}")
    
    if errors:
        try:
            [quantity, bits] = errors.split('/')
            probability = int(quantity) / int(bits)
        except ValueError:
            print(f"{STRONG_RED}Error: Formato de errores inválido{CLOSE}")
            return solicitar_mensaje()
    else:
        probability = 0
    
    binary_string = codificar_mensaje(string)

    print()
    questions = [
        inquirer.List('algorithm',
                      message=f"{STRONG_WHITE}Seleccione el algoritmo a usar{CLOSE}",
                      choices=['Hamming', 'Cyclic Redundancy Check (CRC32)'],
                      ),
    ]
    answers = inquirer.prompt(questions)
    return binary_string, answers, probability

# CAPA DE INTERFERENCIA
def aplicar_ruido(binary_string: str, probability: float) -> str:
    noisy_string = ''
    for bit in binary_string:
        if random() < probability:
            noisy_string += '1' if bit == '0' else '0'
        else:
            noisy_string += bit
    return noisy_string

# CAPA DE TRANSMISIÓN
def enviar_informacion(host, port, data):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((host, port))
        s.sendall(data.encode('utf-8'))
        print("Datos enviados:", data)

def main():

    binary_string, answers, probability = solicitar_mensaje()
    encoded = calcular_integridad(binary_string, answers['algorithm'])
    noisy_string = aplicar_ruido(encoded, probability)
    print("Cadena Original: ", encoded)
    print("Cadena con ruido: ", noisy_string)
    print("Probabilidad de error: ", probability)
    
    data = {
        'data': noisy_string,
        'algorithm': answers['algorithm']
    }
    
    data = json.dumps(data)
    
    enviar_informacion(HOST, PORT, data)

if __name__ == "__main__":
    main()