#include <stdint.h>
#include <string>
#include <vector>
#include <fstream>
#include <cmath>

#include "../headers/BitReader.h"

BitReader::BitReader()
{
	this->position = 0ULL;
	this->count = 0ULL;
}

BitReader& BitReader::operator>>(int& bit)
{
	// If the last bit has been extracted, just return 0
	if(this->position >= this->count)
	{
		bit = 0;
		return *this;
	}

	// Extract the necessary bit
	int index = (int) this->position / BITS_PER_BYTE;
	int bitNumber = (int) this->position % BITS_PER_BYTE;
	bit = !(!(this->buffer[index] & (1 << (BITS_PER_BYTE - 1 - bitNumber))));

	// Get ready to extract the next bit
	this->position++;

	return *this;
}

bool BitReader::load(std::string filename)
{
	// This reader may be used to load data from multiple files
	this->count = 0;
	this->position = 0;
	this->buffer.erase(this->buffer.begin(), this->buffer.end());

	// make sure the file opens correctly
	std::ifstream inputFile(filename, std::ios::binary);
	if(inputFile.fail())
	{
		return false;
	}

	// Extract the number of bits from the file
	char countBuffer[sizeof(uint64_t)];
	inputFile.read(countBuffer, sizeof(uint64_t));
	this->count = (uint64_t)*countBuffer;

	// Extract each byte from the input file
	uint64_t byteCount = (uint64_t) std::ceil((this->count / BITS_PER_BYTE) + 1);
	for(uint64_t index = 0; index < byteCount; index++)
	{
		char *byte = new char;
		inputFile.read(byte, 1);
		this->buffer.push_back(*byte);
	}

	// Close the file as necessary
	inputFile.close();

	return true;
}

uint64_t BitReader::getBitCount()
{
	return this->count;
}