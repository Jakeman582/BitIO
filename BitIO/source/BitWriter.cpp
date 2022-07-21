#include <stdint.h>
#include <string>
#include <vector>
#include <fstream>

#include "../headers/BitWriter.h"

BitWriter::BitWriter()
{
	this->count = 0ULL;
	this->position = 0;
}

BitWriter& BitWriter::operator<<(int bit)
{
	// Make space for more bits if we are trying to read the first bit of a 
	// byte
	if(this->position == 0)
	{
		this->buffer.push_back(0);
	}

	// Make sure we write bits to the correct byte in the buffer
	int index = (int) this->count / BITS_PER_BYTE;

	// For 1 bits, we need to do shifting and masking, for 0 bits, we just 
	// need to advance the position
	if(bit == 1)
	{
		this->buffer[index] |= (1 << (BITS_PER_BYTE - 1 - this->position));
	}

	// Keep track of how many bits have been written
	this->count++;
	this->position++;

	// We use 'uint8_t' variables for buffers which are 8 bits, and we can only 
	// modify bits 0-7 of each buffer
	if(this->position >= 8)
	{
		this->position = 0;
	}

	return *this;
}

bool BitWriter::flush(std::string filename)
{
	// Attempt to open the output file
	std::ofstream outputFile(filename, std::ios::binary);
	if(outputFile.fail())
	{
		return false;
	}

	// Write the 8 bytes of the 'counter' variable
	char *countBytes = (char*)&this->count;
	outputFile.write(countBytes, 8);

	// Write each byte of the buffer
	for(unsigned int index = 0; index < this->buffer.size(); index++)
	{
		outputFile << this->buffer[index];
	}

	// Close the file as necessary
	outputFile.close();

	// This BitWriter may be used to write to other files
	this->count = 0;
	this->position = 0;
	this->buffer.erase(this->buffer.begin(), this->buffer.end());

	return true;
}