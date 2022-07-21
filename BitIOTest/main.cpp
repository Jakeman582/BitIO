#define CATCH_CONFIG_MAIN
#include <catch_amalgamated.hpp>

#include <iostream>

#include "BitWriter.h"
#include "BitReader.h"

std::string toString(int *buffer, int size)
{
	std::string arrayString;
	arrayString.append("[");
	for(int index = 0; index < size; index++)
	{
		arrayString.append(std::to_string(buffer[index]));
		if(index < size - 1)
		{
			arrayString.append(", ");
		}
	}
	arrayString.append("]");
	return arrayString;
}

TEST_CASE("BitWriter: Writing 64 bits to a file")
{
	BitWriter bitWriter;
	bitWriter << 
		0 << 0 << 0 << 0 << 0 << 0 << 0 << 1 << 
		0 << 0 << 1 << 0 << 0 << 0 << 1 << 1 << 
		0 << 1 << 0 << 0 << 0 << 1 << 0 << 1 << 
		0 << 1 << 1 << 0 << 0 << 1 << 1 << 1 << 
		1 << 0 << 0 << 0 << 1 << 0 << 0 << 1 << 
		1 << 0 << 1 << 0 << 1 << 0 << 1 << 1 << 
		1 << 1 << 0 << 0 << 1 << 1 << 0 << 1 << 
		1 << 1 << 1 << 0 << 1 << 1 << 1 << 1;
	bitWriter.flush("bits1.dat");

	BitReader bitReader;
	bitReader.load("bits1.dat");
	int bits[8 * BITS_PER_BYTE];
	for(int index = 0; index < 8 * BITS_PER_BYTE; index++)
	{
		bitReader >> bits[index];
	}

	SECTION("\"bits1.dat\": Testing to see if the correct number of bits are written")
	{
		REQUIRE(bitReader.getBitCount() == 64);
	}

	SECTION("\"bits1.dat\": Testing to see if the bits are read in correctly")
	{
		REQUIRE(toString(&bits[ 0], BITS_PER_BYTE) == "[0, 0, 0, 0, 0, 0, 0, 1]");
		REQUIRE(toString(&bits[ 8], BITS_PER_BYTE) == "[0, 0, 1, 0, 0, 0, 1, 1]");
		REQUIRE(toString(&bits[16], BITS_PER_BYTE) == "[0, 1, 0, 0, 0, 1, 0, 1]");
		REQUIRE(toString(&bits[24], BITS_PER_BYTE) == "[0, 1, 1, 0, 0, 1, 1, 1]");
		REQUIRE(toString(&bits[32], BITS_PER_BYTE) == "[1, 0, 0, 0, 1, 0, 0, 1]");
		REQUIRE(toString(&bits[40], BITS_PER_BYTE) == "[1, 0, 1, 0, 1, 0, 1, 1]");
		REQUIRE(toString(&bits[48], BITS_PER_BYTE) == "[1, 1, 0, 0, 1, 1, 0, 1]");
		REQUIRE(toString(&bits[56], BITS_PER_BYTE) == "[1, 1, 1, 0, 1, 1, 1, 1]");
	}
}

TEST_CASE("BitWriter: Writing an odd number of bits to a file")
{
	BitWriter bitWriter;
	bitWriter << 1 << 1 << 1 << 1 << 1;
	bitWriter.flush("bits2.dat");

	BitReader bitReader;
	bitReader.load("bits2.dat");

	int bits[8];
	for(int index = 0; index < 8; index++)
	{
		bitReader >> bits[index];
	}

	REQUIRE(toString(bits, 8) == "[1, 1, 1, 1, 1, 0, 0, 0]");
}