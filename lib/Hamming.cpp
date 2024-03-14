#include "Hamming.h"

bool is_exp_of_2(uint32_t n) {
    return ( n & (n - 1) ) == 0;
}

uint64_t FastPow(uint64_t number, uint8_t degree) {
    uint64_t result = 1;
    while (degree) {
        if (degree % 2 == 0) {
            degree /= 2;
            number *= number;
        } else {
            degree--;
            result *= number;
        }
    }
    return result;
}

uint8_t GetParityBit(const uint64_t& len) {
    uint8_t number = 0;
    while (FastPow(2,number) < number + len + 1) {
        number++;
    }
    return number;
}

std::pair<std::string,uint16_t> Encode(const std::string& binary_message, const uint32_t& message_len) {
    uint8_t r = GetParityBit(message_len);
    std::string output;
    uint32_t byte_count = binary_message.size() / message_len;
    uint16_t modulo = binary_message.size() % message_len;
    uint16_t empty_bits = 0;
    uint32_t ii = 0;
    while (ii < byte_count) {
        int j = 0;
        std::string help_string = binary_message.substr(ii * message_len,message_len);
        std::string encode_me (message_len + r, '0');
        for (int i = 0; i < message_len + r; i++) {
            if (!is_exp_of_2(i+1)) {
                if (help_string[j] != '0' && help_string[j] != '1') {
                    std::cout << "Error! cant Encode message which contains non 0/1 char" << '\n';
                    return std::make_pair("",0);
                }
                encode_me[i] = help_string[j];
                j++;
            }
        }
        for (uint8_t current_parity_bit = 0; current_parity_bit < r; current_parity_bit++) {
            uint32_t parity_bit_index = (1 << current_parity_bit);
            uint8_t bit_sum = 0;
            for (uint32_t position = parity_bit_index - 1; position < message_len + r; position += 2 * parity_bit_index) {
                for (uint32_t current_bit = position; (current_bit < position + parity_bit_index) && (current_bit < message_len + r); current_bit++) {
                    bit_sum = bit_sum ^ uint8_t(encode_me[current_bit] - '0');
                }
            }
            encode_me[parity_bit_index - 1] = static_cast<char>(bit_sum + 48);
        }
        output += encode_me;
        ii++;
    }
    if (modulo > 0) {
        empty_bits = message_len - modulo;
        int j = 0;
        std::string help_string = binary_message.substr(ii * message_len,modulo);
        help_string += std::string(empty_bits,'0');
        std::string encode_me (message_len + r, '0');
        for (int i = 0; i < message_len + r; i++) {
            if (!is_exp_of_2(i+1)) {
                if (help_string[j] != '0' && help_string[j] != '1') {
                    std::cout << "Error! cant Encode message which contains non 0/1 char" << '\n';
                    return std::make_pair("",0);
                }
                encode_me[i] = help_string[j];
                j++;
            }
        }
        for (uint8_t current_parity_bit = 0; current_parity_bit < r; current_parity_bit++) {
            uint32_t parity_bit_index = (1 << current_parity_bit);
            uint8_t bit_sum = 0;
            for (uint32_t position = parity_bit_index - 1; position < message_len + r; position += 2 * parity_bit_index) {
                for (uint32_t current_bit = position; (current_bit < position + parity_bit_index) && (current_bit < message_len + r); current_bit++) {
                    bit_sum = bit_sum ^ uint8_t(encode_me[current_bit] - '0');
                }
            }
            encode_me[parity_bit_index - 1] = static_cast<char>(bit_sum + 48);
        }
        output += encode_me;
    }
    return std::make_pair(output,empty_bits);
}

std::string FixError(std::string& encoded_message, const uint32_t& message_len) {
    uint8_t r = GetParityBit(message_len);
    uint32_t number_of_partitions = encoded_message.size() / (message_len + r);
    if (encoded_message.size() % (message_len + r) != 0) {
        std::cout << "Error! cant fix error in this message" << '\n';
        return "";
    }
    for (uint32_t ii = 0; ii < number_of_partitions; ii++) {
        uint32_t start_pos = ii * (message_len + r);
        uint32_t mistake = 0;
        for (uint32_t i = start_pos; i < start_pos + message_len + r; i++) {
            if (encoded_message[i] != '1' && encoded_message[i] != '0') {
                std::cout << "Error! cant fix message which contains non 0/1 char" << '\n';
                return "";
            }
            if (encoded_message[i] == '1') {
                mistake ^= (i + 1 - start_pos);
            }
        }
        if (mistake > 0) {
            encoded_message[start_pos + mistake - 1] ^= '0' ^ '1';
        }
    }
    return encoded_message;
}

std::string RemoveParityBits(const std::string& encoded_message, const uint32_t& message_len) {
    std::string output;
    uint8_t r = GetParityBit(message_len);
    uint32_t number_of_partitions = encoded_message.size() / (message_len + r);
    for (uint32_t ii = 0; ii < number_of_partitions; ii++) {
        uint32_t start_pos = ii * (message_len + r);
        for (uint32_t i = start_pos; i < start_pos + message_len + r; i++) {
            if (!is_exp_of_2((i + 1 - start_pos))) {
                output += encoded_message[i];
            }
        }
    }
    return output;
}

std::string BinStrFromChar(char ch) {
    std::string output(8,'0');
    if (ch < 0) {
        output[0] = '1';
        ch = char(ch + 128);
    }
    uint8_t count = 1;
    while (ch > 0) {
        if (ch % 2 == 1) {
            output[8 - count] = '1';
        }
        ch /= 2;
        count++;
    }
    return output;
}

std::string BinStrFromFile(const std::string& filename) {
    std::ifstream fin(filename, std::ios::binary);
    if (!fin) {
        std::cout << "Error! file " << filename << " cannot be opened" << '\n';
        return "$~$~$";
    }
    char ch;
    std::string output;
    while (fin.get(ch)) {
        output += BinStrFromChar(ch);
    }
    return output;
}

std::string BinStrFromStr(const std::string& primitive_message) {
    std::string output;
    for (char ch : primitive_message) {
        output += BinStrFromChar(ch);
    }
    return output;
}

std::string StrFromBinStr(const std::string& binary_message) {
    if (binary_message.size() % 8 != 0) {
        std::cout << "Error! bin str len must be divisible by 8" << '\n';
    }
    std::string output;
    for (int i = 0; i < binary_message.size(); i+=8) {
        output += CharFromBinStr(binary_message.substr(i,8));
    }
    return output;
}

std::string BinStrFromUint32_t(uint32_t value) {
    std::string output(32,'0');
    uint8_t counter = 1;
    while (value > 0) {
        if (value % 2 == 1) {
            output[32 - counter] = '1';
        }
        value /= 2;
        counter++;
    }
    return output;
}

std::string BinStrFromUint16_t(uint16_t value) {
    std::string output(16,'0');
    uint8_t counter = 1;
    while (value > 0) {
        if (value % 2 == 1) {
            output[16 - counter] = '1';
        }
        value /= 2;
        counter++;
    }
    return output;
}

std::string BinStrFromUint8_t(uint8_t value) {
    std::string output(8,'0');
    uint8_t counter = 1;
    while (value > 0) {
        if (value % 2 == 1) {
            output[8 - counter] = '1';
        }
        value /= 2;
        counter++;
    }
    return output;
}

uint32_t Uint32_tFromBinStr(const std::string& binary_message) {
    if (binary_message.size() != 32) {
        std::cout << "Error! word must have len == 32" << '\n';
        return 0;
    }
    uint32_t value = 0;
    for (uint8_t i = 0; i < 32; i++) {
        if (binary_message[i] == '1') {
            value += 1 << (32 - i - 1);
        }
    }
    return value;
}

uint16_t Uint16_tFromBinStr(const std::string& binary_message) {
    if (binary_message.size() != 16) {
        std::cout << "Error! word must have len == 16" << '\n';
        return 0;
    }
    uint16_t value = 0;
    for (uint8_t i = 0; i < 16; i++) {
        if (binary_message[i] == '1') {
            value += 1 << (16 - i - 1);
        }
    }
    return value;
}

uint8_t Uint8_tFromBinStr(const std::string& binary_message) {
    if (binary_message.size() != 8) {
        std::cout << "Error! word must have len == 8" << '\n';
        return 0;
    }
    uint8_t value = 0;
    for (uint8_t i = 0; i < 8; i++) {
        if (binary_message[i] == '1') {
            value += 1 << (8 - i - 1);
        }
    }
    return value;
}

char CharFromBinStr(const std::string& binary_message) {
    return static_cast<char>(Uint8_tFromBinStr(binary_message));
}

void ExtractFileFromArchive(const std::string& archive_name, const std::string& file_for_extracting) {
    uint32_t len = 0;
    uint32_t delta = 0;
    SeeArchive(archive_name,len,delta);
    if (len == 0 && delta == 0) {
        return;
    }
    std::vector<File> header;
    HeaderFromArchive(archive_name,header,delta);
    if (header.empty()) {
        return;
    }
    bool file_contains_in_archive = false;
    uint32_t byte;
    uint8_t modulo;
    uint16_t empty_bits;
    uint32_t start_pos;
    for (const File& current_file : header) {
        if (file_for_extracting == current_file.name) {
            byte = current_file.size;
            modulo = current_file.modulo_size;
            empty_bits = current_file.empty_bits;
            start_pos = current_file.start_pos;
            file_contains_in_archive = true;
            break;
        }
    }
    if (!file_contains_in_archive) {
        std::cout << "Error! " << file_for_extracting << " does not belongs to " << archive_name << '\n';
        return;
    }
    std::ifstream fin(archive_name, std::ios::binary);
    fin.seekg(start_pos, std::ios::beg);
    std::string encoded_data;
    char ch;
    for (uint32_t i = 0; i < byte; i++) {
        if (!fin.get(ch)) {
            std::cout << "Error! cant extract file from archive" << '\n';
        }
        encoded_data += BinStrFromChar(ch);
    }
    if (modulo > 0) {
        if (!fin.get(ch)) {
            std::cout << "Error! cant extract file from archive" << '\n';
        }
        encoded_data += BinStrFromChar(ch).substr(8 - modulo, modulo);
    }
    fin.close();
    FixError(encoded_data,len);
    std::string binary_data = RemoveParityBits(encoded_data,len);
    binary_data.resize(binary_data.size() - empty_bits);
    std::ofstream fout(file_for_extracting, std::ios::binary | std::ios::trunc);
    fout.close();
    WriteBinStrToFile(file_for_extracting,0,binary_data);
}

void WriteBinStrToFile(const std::string& filename, const uint32_t& start_pos, const std::string& message) {
    std::fstream file (filename, std::ios::binary | std::ios::out | std::ios::in);
    if (!file) {
        std::cout << '\n' << "Error! file for Putting must exist" << '\n';
        return;
    }
    file.seekp(start_pos,std::ios::beg);
    uint32_t byte_count = message.size() / 8;
    uint8_t modulo = message.size() % 8;
    uint32_t i = 0;
    while (i < byte_count) {
        std::bitset<8> bitset(message.substr(i * 8, 8));
        uint8_t value = bitset.to_ulong();
        file.write((char*)&value,sizeof(value));
        i++;
    }
    if (modulo > 0) {
        std::bitset<8> bitset(message.substr(i * 8, modulo));
        uint8_t value = bitset.to_ulong();
        file.write((char*)&value,sizeof(value));
    }
    file.close();
}

void WriteHeaderToFile(const std::string& archive_name, uint32_t& start_pos, const std::vector<File>& header, const uint32_t& message_len, const uint32_t& delta) {
    for (const File& current_file : header) {
        std::string encoded_size = Encode(BinStrFromUint32_t(current_file.size),32).first;
        std::string encoded_modulo = Encode(BinStrFromUint8_t(current_file.modulo_size),8).first;
        std::string encoded_empty_bits = Encode(BinStrFromUint16_t(current_file.empty_bits),16).first;
        std::string encoded_start_pos = Encode(BinStrFromUint32_t(current_file.start_pos),32).first;
        std::string encoded_name_size = Encode(BinStrFromUint16_t(current_file.name_size),16).first;
        std::string encoded_name_modulo = Encode(BinStrFromUint8_t(current_file.modulo_name),8).first;
        std::string binary_name = BinStrFromStr(current_file.name);
        std::string encoded_name = Encode(binary_name,binary_name.size()).first;
        WriteBinStrToFile(archive_name,start_pos,encoded_size);
        start_pos += 5;
        WriteBinStrToFile(archive_name,start_pos,encoded_modulo);
        start_pos += 2;
        WriteBinStrToFile(archive_name,start_pos,encoded_empty_bits);
        start_pos += 3;
        WriteBinStrToFile(archive_name,start_pos,encoded_start_pos);
        start_pos += 5;
        WriteBinStrToFile(archive_name,start_pos,encoded_name_size);
        start_pos += 3;
        WriteBinStrToFile(archive_name,start_pos,encoded_name_modulo);
        start_pos += 2;
        WriteBinStrToFile(archive_name,start_pos,encoded_name);
        start_pos += current_file.name_size + (7 + current_file.modulo_name) / 8;
    }
    std::string encoded_len = Encode(BinStrFromUint32_t(message_len),32).first;
    std::string encoded_delta = Encode(BinStrFromUint32_t(delta),32).first;
    WriteBinStrToFile(archive_name,start_pos,encoded_len);
    start_pos += 5;
    WriteBinStrToFile(archive_name,start_pos,encoded_delta);
    start_pos += 5;
}

void CreateArchive(const std::string& archive_name, const uint32_t& message_len) {
    std::ifstream fin;
    fin.open(archive_name, std::ios::binary);
    if (fin.is_open()) {
        std::cout << "Error! " << archive_name << " already exists!" << '\n';
        return;
    }
    fin.close();
    std::ofstream fout(archive_name, std::ios::binary);
    fout.close();
    uint32_t delta = 10;
    std::string encoded_len = Encode(BinStrFromUint32_t(message_len),32).first;
    std::string encoded_delta = Encode(BinStrFromUint32_t(delta),32).first;
    WriteBinStrToFile(archive_name, 0, encoded_len);
    WriteBinStrToFile(archive_name, 5, encoded_delta);
}

uint32_t SeeArchive(const std::string& archive_name,uint32_t& len, uint32_t& delta) {
    std::ifstream fin(archive_name, std::ios::binary);
    if (!fin) {
        std::cout << "Error! archive file does not exist or readable not a archive" << '\n';
        return 0;
    }
    fin.seekg(-10, std::ios::end);
    uint32_t file_size_minus_10 = fin.tellg();
    char ch;
    std::string encoded_len;
    for (uint8_t i = 0; i < 5; i++) {
        if (!fin.get(ch)) {
            std::cout << "Error! cant read len from archive" << '\n';
        }
        encoded_len += BinStrFromChar(ch);
    }
    encoded_len = encoded_len.substr(0,32) + encoded_len.substr(40 - 6,6);
    FixError(encoded_len,32);
    len = Uint32_tFromBinStr(RemoveParityBits(encoded_len,32));
    std::string encoded_delta;
    for (uint8_t i = 0; i < 5; i++) {
        if (!fin.get(ch)) {
            std::cout << "Error! cant read delta from archive" << '\n';
        }
        encoded_delta += BinStrFromChar(ch);
    }
    encoded_delta = encoded_delta.substr(0,32) + encoded_delta.substr(40 - 6,6);
    FixError(encoded_delta,32);
    delta = Uint32_tFromBinStr(RemoveParityBits(encoded_delta,32));
    fin.close();
    return file_size_minus_10;
}

void AddFileToArchive(const std::string& archive_name, const std::vector<std::string>& files) {
    if (files.empty()) {
        return;
    }
    uint32_t len = 0;
    uint32_t delta = 0;
    SeeArchive(archive_name,len,delta);
    if (len == 0 && delta == 0) {
        return;
    }
    uint32_t previous_finish = 0;
    std::vector<File> header;
    uint32_t new_delta = delta;
    if (delta == 10) {
        previous_finish = 0;
        for (const std::string& current_file : files) {
            AddRawFileToArchive(archive_name,previous_finish,current_file,header,len,new_delta);
        }
    } else {
        HeaderFromArchive(archive_name,header,delta);
        File last_file = header.back();
        std::vector<std::string> file_names;
        for (const File& file : header) {
            file_names.push_back(file.name);
        }
        previous_finish = last_file.start_pos + last_file.size + (7 + last_file.modulo_size) / 8;
        for (const std::string& current_file : files) {
            if (std::find(file_names.begin(), file_names.end(), current_file) != file_names.end()) {
                std::cout << "Error! " << current_file << " already in " << archive_name << '\n';
                continue;
            }
            AddRawFileToArchive(archive_name,previous_finish,current_file,header,len,new_delta);
        }
    }
    WriteHeaderToFile(archive_name, previous_finish, header, len, new_delta);
}

void AddRawFileToArchive(const std::string& archive_name, uint32_t& previous_finish, const std::string& file_for_writing, std::vector<File>& header, const uint32_t& message_len, uint32_t& delta) {
    std::string data = BinStrFromFile(file_for_writing);
    if (data == "$~$~$") {
        return;
    }
    std::pair<std::string,uint16_t> info = Encode(data,message_len);
    std::string encoded_data = info.first;
    uint32_t bytes = encoded_data.size() / 8;
    uint8_t modulo = encoded_data.size() % 8;
    uint16_t empty_bits = info.second;
    uint32_t start_pos = previous_finish;
    std::string binary_name = BinStrFromStr(file_for_writing);
    std::string encoded_name = Encode(binary_name,binary_name.size()).first;
    uint16_t name_bytes = encoded_name.size() / 8;
    uint8_t modulo_name = encoded_name.size() % 8;
    header.emplace_back(bytes,modulo,empty_bits,start_pos,name_bytes,modulo_name,file_for_writing);
    WriteBinStrToFile(archive_name,start_pos,encoded_data);
    previous_finish += bytes + (7 + modulo) / 8;
    delta += 5 + 2 + 3 + 5 + 3 + 2 + name_bytes + (7 + modulo_name) / 8;
}

File FileInfoFromArchive(std::ifstream& fin, uint32_t& header_size) {
    char ch;
    std::string current_data;
    File regular_file {0,0,0,0,0,0,""};
    for (int i = 0; i < 5; i++) {
        header_size--;
        if (!fin.get(ch)) {
            std::cout << "Error! can't read file size" << '\n';
            return regular_file;
        }
        current_data += BinStrFromChar(ch);
    }
    current_data = current_data.substr(0,32) + current_data.substr(40 - 6,6);
    FixError(current_data,32);
    regular_file.size = Uint32_tFromBinStr(RemoveParityBits(current_data,32));
    current_data = "";
    for (int i = 0; i < 2; i++) {
        header_size--;
        if (!fin.get(ch)) {
            std::cout << "Error! can't read file size modulo" << '\n';
            return regular_file;
        }
        current_data += BinStrFromChar(ch);
    }
    current_data = current_data.substr(0,8) + current_data.substr(16 - 4,4);
    FixError(current_data,8);
    regular_file.modulo_size = Uint8_tFromBinStr(RemoveParityBits(current_data,8));
    current_data = "";
    for (int i = 0; i < 3; i++) {
        header_size--;
        if (!fin.get(ch)) {
            std::cout << "Error! can't read file empty bits" << '\n';
            return regular_file;
        }
        current_data += BinStrFromChar(ch);
    }
    current_data = current_data.substr(0,16) + current_data.substr(24 - 5,5);
    FixError(current_data,16);
    regular_file.empty_bits = Uint16_tFromBinStr(RemoveParityBits(current_data,16));
    current_data = "";
    for (int i = 0; i < 5; i++) {
        header_size--;
        if (!fin.get(ch)) {
            std::cout << "Error! can't read file start position" << '\n';
            return regular_file;
        }
        current_data += BinStrFromChar(ch);
    }
    current_data = current_data.substr(0,32) + current_data.substr(40 - 6,6);
    FixError(current_data,32);
    regular_file.start_pos = Uint32_tFromBinStr(RemoveParityBits(current_data,32));
    current_data = "";
    for (int i = 0; i < 3; i++) {
        header_size--;
        if (!fin.get(ch)) {
            std::cout << "Error! can't read file name len" << '\n';
            return regular_file;
        }
        current_data += BinStrFromChar(ch);
    }
    current_data = current_data.substr(0,16) + current_data.substr(24 - 5,5);
    FixError(current_data,16);
    regular_file.name_size = Uint16_tFromBinStr(RemoveParityBits(current_data,16));
    current_data = "";
    for (int i = 0; i < 2; i++) {
        header_size--;
        if (!fin.get(ch)) {
            std::cout << "Error! can't read file name modulo" << '\n';
            return regular_file;
        }
        current_data += BinStrFromChar(ch);
    }
    current_data = current_data.substr(0,8) + current_data.substr(16 - 4,4);
    FixError(current_data,8);
    regular_file.modulo_name = Uint8_tFromBinStr(RemoveParityBits(current_data,8));
    current_data = "";
    for (int i = 0; i < regular_file.name_size; i++) {
        header_size--;
        if (!fin.get(ch)) {
            std::cout << "Error! cant read file name" << '\n';
            return regular_file;
        }
        current_data += BinStrFromChar(ch);
    }
    if (regular_file.modulo_name > 0) {
        header_size--;
        if (!fin.get(ch)) {
            std::cout << "Error! cant read file name" << '\n';
            return regular_file;
        }
        current_data += BinStrFromChar(ch).substr(8 - regular_file.modulo_name, regular_file.modulo_name);
    }
    uint8_t r = 1;
    while (FastPow(2,r) < current_data.size() + 1) {
        r++;
    }
    uint32_t argument_for_name = current_data.size() - r;
    FixError(current_data,argument_for_name);
    regular_file.name = StrFromBinStr(RemoveParityBits(current_data,argument_for_name));
    return regular_file;
}

void HeaderFromArchive(const std::string& archive_name, std::vector<File>& header, const uint32_t& delta) {
    std::ifstream fin(archive_name, std::ios::binary);
    fin.seekg(-int(delta),std::ios::end);
    if (!fin) {
        std::cout << "Error! delta incorrect" << '\n';
        return;
    }
    uint32_t header_size = delta - 10;

    while (header_size > 0) {
        header.push_back(FileInfoFromArchive(fin,header_size));
    }
    fin.close();
}

void DeleteFileFromArchive(const std::string& archive_name, const std::string& file_for_deleting) {
    std::ifstream fin(archive_name, std::ios::binary);
    uint32_t len = 0;
    uint32_t delta = 0;
    uint32_t pos2 = SeeArchive(archive_name,len,delta);
    if (len == 0 && delta == 0) {
        return;
    }
    fin.seekg(-int(delta),std::ios::end);
    uint32_t header_size = delta - 10;
    uint32_t pos1 = fin.tellg();
    bool founded = false;
    std::vector<File> header;
    File current_file{0,0,0,0,0,0,""};
    while (header_size > 0) {
        current_file = FileInfoFromArchive(fin,header_size);
        if (current_file.name == file_for_deleting) {
            pos2 = fin.tellg();
            founded = true;
            break;
        } else {
            pos1 = fin.tellg();
        }
    }
    while (header_size > 0) {
        File file_after_deleted_file = FileInfoFromArchive(fin,header_size);
        file_after_deleted_file.start_pos -= (current_file.size + (7 + current_file.modulo_size) / 8);
        header.push_back(file_after_deleted_file);
    }
    delta -= (pos2 - pos1);
    if (!founded) {
        std::cout << "Error! " << file_for_deleting << " does not belongs to " << archive_name << '\n';
        return;
    }
    std::string temporary_archive_name = archive_name + '1';
    std::ofstream fout(temporary_archive_name, std::ios::binary);
    const uint32_t first_position = current_file.start_pos;
    const uint32_t second_position = first_position + current_file.size + (7 + current_file.modulo_size) / 8;
    const uint32_t third_position = pos1;
    char ch;
    fin.seekg(0,std::ios::beg);
    for (uint32_t i = 0; i < first_position; i++) {
        if (!fin.get(ch)) {
            std::cout << "Error! by some reasons cant delete file from archive" << '\n';
            return;
        }
        fout.write((char*)&ch,sizeof(ch));
    }
    fin.seekg(second_position,std::ios::beg);
    for (uint32_t i = second_position; i < third_position; i++) {
        if (!fin.get(ch)) {
            std::cout << "Error! by some reasons cant delete file from archive" << '\n';
            return;
        }
        fout.write((char*)&ch,sizeof(ch));
    }
    uint32_t new_header_start_pos = first_position + third_position - second_position;
    fout.close();
    WriteHeaderToFile(temporary_archive_name,new_header_start_pos,header,len,delta);
    std::remove(archive_name.c_str());
    std::rename(temporary_archive_name.c_str(),archive_name.c_str());
    fin.close();
}

void SeeHeaderList(const std::string& archive_name) {
    uint32_t len = 0;
    uint32_t delta = 0;
    SeeArchive(archive_name,len,delta);
    if (len == 0 && delta == 0) {
        return;
    }
    std::vector<File> header;
    HeaderFromArchive(archive_name,header,delta);
    if (header.empty()) {
        std::cout << archive_name << " is empty" << '\n';
    } else {
        std::cout << archive_name << " have these files: ";
        for (const File& current_file : header) {
            std::cout << current_file.name << ' ';
        }
        std::cout << '\n';
    }
}

void Merge(const std::string& archive_name1, const std::string& archive_name2, const std::string& destination_archive, const uint32_t& message_len) {
    uint32_t delta = 0;

    std::vector<File> header1;
    uint32_t len1;
    uint32_t delta1;
    SeeArchive(archive_name1,len1,delta1);
    std::ifstream fin1(archive_name1, std::ios::binary);
    fin1.seekg(-int(delta1),std::ios::end);
    uint32_t size1 = fin1.tellg();
    HeaderFromArchive(archive_name1,header1,delta1);

    for (const File& current_file : header1) {
        fin1.seekg(current_file.start_pos, std::ios::beg);
        std::string encoded_data;
        char ch;
        for (uint32_t i = 0; i < current_file.size; i++) {
            if (!fin1.get(ch)) {
                std::cout << "Error! cant extract file from archive" << '\n';
            }
            encoded_data += BinStrFromChar(ch);
        }
        if (current_file.modulo_size > 0) {
            if (!fin1.get(ch)) {
                std::cout << "Error! cant extract file from archive" << '\n';
            }
            encoded_data += BinStrFromChar(ch).substr(8 - current_file.modulo_size, current_file.modulo_size);
        }
        FixError(encoded_data,len1);
        std::string binary_data = RemoveParityBits(encoded_data,len1);
        binary_data.resize(binary_data.size() - current_file.empty_bits);
        WriteBinStrToFile(destination_archive,delta,binary_data);
        delta += current_file.size + (7 + current_file.modulo_size) / 8;
    }
    fin1.close();

    std::vector<File> header2;
    uint32_t len2;
    uint32_t delta2;
    SeeArchive(archive_name2,len2,delta2);
    std::ifstream fin2(archive_name2, std::ios::binary);
    fin2.seekg(-int(delta2),std::ios::end);
    uint32_t size2 = fin2.tellg();
    HeaderFromArchive(archive_name2,header2,delta2);

    for (const File& current_file : header2) {
        fin2.seekg(current_file.start_pos, std::ios::beg);
        std::string encoded_data;
        char ch;
        for (uint32_t i = 0; i < current_file.size; i++) {
            if (!fin2.get(ch)) {
                std::cout << "Error! cant extract file from archive" << '\n';
            }
            encoded_data += BinStrFromChar(ch);
        }
        if (current_file.modulo_size > 0) {
            if (!fin2.get(ch)) {
                std::cout << "Error! cant extract file from archive" << '\n';
            }
            encoded_data += BinStrFromChar(ch).substr(8 - current_file.modulo_size, current_file.modulo_size);
        }
        FixError(encoded_data,len1);
        std::string binary_data = RemoveParityBits(encoded_data,len1);
        binary_data.resize(binary_data.size() - current_file.empty_bits);
        WriteBinStrToFile(destination_archive,delta,binary_data);
        delta += current_file.size + (7 + current_file.modulo_size) / 8;
    }
    fin2.close();

    for (File& current_file : header2) {
        current_file.start_pos += size1;
        header1.push_back(current_file);
    }

    WriteHeaderToFile(destination_archive,delta,header1,message_len,delta1 + delta2);
}

