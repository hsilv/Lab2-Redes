#include <iostream>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <vector>
#include <bitset>
#include <cstdint>
#include <cmath>
#include <jsoncpp/json/json.h>
using namespace std;

string decode_hamming(const string &data)
{
    int n = data.length();
    int r = 0;

    while ((1 << r) < (n + 1))
    {
        r++;
    }

    vector<int> error_positions;
    for (int i = 0; i < r; i++)
    {
        int parity = 0;
        for (int j = (1 << i) - 1; j < n; j += (1 << (i + 1)))
        {
            for (int k = 0; k < (1 << i); k++)
            {
                if (j + k >= n)
                    break;
                parity ^= (data[j + k] == '1');
            }
        }
        if (parity != 0)
        {
            error_positions.push_back(1 << i);
        }
    }

    int error_position = 0;
    for (int pos : error_positions)
    {
        error_position += pos;
    }

    string corrected_data = data;
    if (error_position != 0)
    {
        cout << "Error detected at position: " << error_position << endl;
        corrected_data[error_position - 1] = (corrected_data[error_position - 1] == '0') ? '1' : '0';
        cout << "Error corrected at position: " << error_position << endl;
    }

    string data_bits;
    int j = 0;
    for (int i = 1; i <= n; i++)
    {
        if (i != (1 << j))
        {
            data_bits += corrected_data[i - 1];
        }
        else
        {
            j++;
        }
    }

    if (error_positions.size() == 0)
    {
        cout << "No errors detected. Original message: " << data_bits << endl;
    }
    else if (error_positions.size() == 1 && corrected_data == data)
    {
        cout << "Errors detected but unable to correct. Message discarded." << endl;
        return "";
    }
    else if (error_positions.size() > 1)
    {
        cout << "Multiple errors detected at positions: ";
        for (int pos : error_positions)
        {
            cout << pos << " ";
        }
        cout << endl;

        if (corrected_data == data)
        {
            cout << "Unable to correct errors. Message discarded." << endl;
            return "";
        }
        else
        {
            cout << "Errors detected and corrected. Corrected message: " << data_bits << endl;
            return data_bits;
        }
        return "";
    }
    else
    {
        cout << "Errors detected and corrected. Corrected message: " << data_bits << endl;
    }

    return data_bits;
}

vector<uint8_t> binaryStringToBytes(const string &binaryString)
{
    vector<uint8_t> bytes;
    for (size_t i = 0; i < binaryString.size(); i += 8)
    {
        bitset<8> byte(binaryString.substr(i, 8));
        bytes.push_back(static_cast<uint8_t>(byte.to_ulong()));
    }
    return bytes;
}

uint32_t crc32(const vector<uint8_t> &data)
{
    uint32_t crc = 0xFFFFFFFF;
    for (size_t i = 0; i < data.size(); ++i)
    {
        crc ^= data[i];
        for (int j = 0; j < 8; ++j)
        {
            if (crc & 1)
            {
                crc = (crc >> 1) ^ 0xEDB88320;
            }
            else
            {
                crc >>= 1;
            }
        }
    }
    return crc ^ 0xFFFFFFFF;
}

string decode_crc32(const string &data)
{
    if (data.size() < 32)
    {
        cerr << "Data too short to contain CRC-32 checksum." << endl;
        return "";
    }

    string data_bits = data.substr(0, data.size() - 32);
    string checksum_bits = data.substr(data.size() - 32);

    vector<uint8_t> data_bytes = binaryStringToBytes(data_bits);
    uint32_t calculated_checksum = crc32(data_bytes);
    uint32_t received_checksum = bitset<32>(checksum_bits).to_ulong();

    if (calculated_checksum == received_checksum)
    {
        cout << "No errors detected. Original message: " << data_bits << endl;
        return data_bits;
    }
    else
    {
        cerr << "CRC-32 checksum mismatch. Data may be corrupted. Message discarded." << endl;
        return "";
    }
}

string binary_to_ascii(const string &data)
{
    string ascii = "";
    for (size_t i = 0; i < data.size(); i += 8)
    {
        bitset<8> byte(data.substr(i, 8));
        ascii += static_cast<char>(byte.to_ulong());
    }
    return ascii;
}

int main()
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        cerr << "Error al crear el socket" << endl;
        return 1;
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        cerr << "Error en setsockopt" << endl;
        close(server_fd);
        return 1;
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(65432);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        cerr << "Error en bind" << endl;
        close(server_fd);
        return 1;
    }
    if (listen(server_fd, 3) < 0)
    {
        cerr << "Error en listen" << endl;
        close(server_fd);
        return 1;
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
    {
        cerr << "Error en accept" << endl;
        close(server_fd);
        return 1;
    }

    int valread = read(new_socket, buffer, 1024);
    if (valread < 0)
    {
        cerr << "Error al leer del socket" << endl;
        close(new_socket);
        close(server_fd);
        return 1;
    }

    string received_data(buffer, valread);
    Json::Value root;
    Json::CharReaderBuilder reader;
    string errs;

    istringstream sstr(received_data);
    if (!Json::parseFromStream(reader, sstr, &root, &errs))
    {
        cerr << "Error al parsear JSON: " << errs << endl;
        close(new_socket);
        close(server_fd);
        return 1;
    }

    string data = root["data"].asString();
    string option = root["algorithm"].asString();

    if (option == "Hamming")
    {
        string decoded_data = decode_hamming(data);
        decoded_data = binary_to_ascii(decoded_data);
        if (decoded_data.empty())
        {
            close(new_socket);
            close(server_fd);
            return 1;
        }
        cout << "Datos decodificados (Hamming): " << decoded_data << endl;
    }
    else if (option == "Cyclic Redundancy Check (CRC32)")
    {
        string decoded_data = decode_crc32(data);
        decoded_data = binary_to_ascii(decoded_data);
        cout << "Datos decodificados (CRC): " << decoded_data << endl;
    }
    else
    {
        cerr << "Opción desconocida: " << option << endl;
        close(new_socket);
        close(server_fd);
        return 1;
    }

    close(new_socket);
    close(server_fd);
    return 0;
}