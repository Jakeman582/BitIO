#pragma once

#include <stdint.h>
#include <string>
#include <vector>

#define BITS_PER_BYTE 8

class BitWriter
{
private:
	int position;
	uint64_t count;
	std::vector<uint8_t> buffer;

public:
	BitWriter();
	BitWriter& operator<<(int bit);
	bool flush(std::string filename);
};