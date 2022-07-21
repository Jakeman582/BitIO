#pragma once

#include <stdint.h>
#include <string>
#include <vector>

#define BITS_PER_BYTE 8

class BitReader
{
private:
	uint64_t position;
	uint64_t count;
	std::vector<uint8_t> buffer;

public:
	BitReader();
	BitReader& operator>>(int& bit);
	bool load(std::string filename);
	uint64_t getBitCount();
};