#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <cstdint>
#include <cmath>
using namespace std;

#include <iostream>
#include <string>
#include <vector>

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

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        cerr << "Usage: " << argv[0] << " <data> <-Ha|-CRC>" << endl;
        return 1;
    }

    string data = argv[1];
    string option = argv[2];

    if (option == "-Ha")
    {
        string decoded_data = decode_hamming(data);
    }
    else if (option == "-CRC")
    {
        string decoded_data = decode_crc32(data);
    }
    else
    {
        cerr << "Unknown option: " << option << endl;
        return 1;
    }

    return 0;
}