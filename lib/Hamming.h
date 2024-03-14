#ifndef UNTITLED1_HAMMING_H
#define UNTITLED1_HAMMING_H

#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>
#include <bitset>
#include <algorithm>
#include <cstdio>
#include <utility>

struct File {
    File(uint32_t file_bytes, uint8_t file_modulo, uint16_t file_empty_bits, uint32_t file_start_pos, uint16_t file_name_size, uint8_t file_name_modulo, std::string file_name) {
        size = file_bytes;
        modulo_size = file_modulo;
        empty_bits = file_empty_bits;
        start_pos = file_start_pos;
        name_size = file_name_size;
        modulo_name = file_name_modulo;
        name = file_name;
    }
    uint32_t size;
    uint8_t modulo_size;
    uint16_t empty_bits;
    uint32_t start_pos;
    uint16_t name_size;
    uint8_t modulo_name;
    std::string name;
};

bool is_exp_of_2(uint32_t n);

uint64_t FastPow(uint64_t number, uint8_t degree);

uint8_t GetParityBit(const uint64_t& len);

std::pair<std::string,uint16_t> Encode(const std::string& binary_message, const uint32_t& message_len);

std::string FixError(std::string& encoded_message, const uint32_t& message_len);

std::string RemoveParityBits(const std::string& encoded_message, const uint32_t& message_len);

std::string BinStrFromChar(char ch);

std::string BinStrFromFile(const std::string& filename);

std::string BinStrFromStr(const std::string& primitive_message);

std::string StrFromBinStr(const std::string& binary_message);

std::string BinStrFromUint32_t(uint32_t value);

std::string BinStrFromUint16_t(uint16_t value);

std::string BinStrFromUint8_t(uint8_t value);

uint32_t Uint32_tFromBinStr(const std::string& binary_message);

uint16_t Uint16_tFromBinStr(const std::string& binary_message);

uint8_t Uint8_tFromBinStr(const std::string& binary_message);

char CharFromBinStr(const std::string& binary_message);

void ExtractFileFromArchive(const std::string& archive_name, const std::string& output_file);

void WriteBinStrToFile(const std::string& filename, const uint32_t& start_pos, const std::string& message);

void WriteHeaderToFile(const std::string& archive_name, uint32_t& start_pos, const std::vector<File>& header, const uint32_t& message_len, const uint32_t& delta);

void CreateArchive(const std::string& archive_name, const uint32_t& message_len);

uint32_t SeeArchive(const std::string& archive_name,uint32_t& len, uint32_t& delta);

void AddFileToArchive(const std::string& archive_name, const std::vector<std::string>& files);

void AddRawFileToArchive(const std::string& archive_name, uint32_t& previous_finish, const std::string& file_for_writing, std::vector<File>& header, const uint32_t& message_len, uint32_t& delta);

void HeaderFromArchive(const std::string& archive_name, std::vector<File>& header, const uint32_t& delta);

File FileInfoFromArchive(std::ifstream& fin, uint32_t& header_size);

void DeleteFileFromArchive(const std::string& archive_name, const std::string& file_for_deleting);

void SeeHeaderList(const std::string& archive_name);

void Merge(const std::string& archive_name1, const std::string& archive_name2, const std::string& destination_archive, const uint32_t& message_len);

#endif //UNTITLED1_HAMMING_H
