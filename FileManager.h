#pragma once
#include <fstream>
#include <string>
#include "AI.h"

class FileManager
{
public:
	const std::uint8_t END_CHAR = 254;

	FileManager(const std::string& filename);

	/*
	Reads data from the file for the given AI and automatically sets it
	@param ai The AI to read data for
	*/
	void readAIData(AI& ai);

	/*
	Writes data from the file for the given AI. A temporary copy file is created to avoid losing data
	@param ai The AI to write data for
	*/
	void writeAIData(AI& ai);

private:
	std::string filename;
};