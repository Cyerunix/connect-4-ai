#include "FileManager.h"

FileManager::FileManager(const std::string& filename)
{
	this->filename = filename;
}

void FileManager::readAIData(AI & ai)
{
	std::map<Board::BoardType, std::map<std::uint8_t, std::uint8_t>> data;
	char currentChar = -2;
	
	//Open the input file
	std::ifstream input;
	input.open(filename, std::ifstream::in | std::ifstream::binary);

	//Make sure the file opened properly
	if (!input.is_open()) {
		std::cout << "\nERROR: Could not open file " << filename;
		return;
	}

	bool reading = true;
	while (!input.eof()) {
		//Read the board that is mapped to the priority data
		Board::BoardType currentBoard;
		for (size_t x = 0; x < currentBoard.size(); x++) {
			//Store the current row being read
			std::array<std::int8_t, Board::NUM_COLS> currentRow;

			//Get all of the values in this row
			for (size_t y = 0; y < currentRow.size(); y++) {
				input.get(currentChar);
				currentRow.at(y) = static_cast<std::uint8_t>(currentChar);

				if (input.eof()) {
					reading = false;
					break;
				}
			}

			//At this row to the board
			currentBoard.at(x) = currentRow;
		}

		if (!reading) {
			break;
		}

		//Read the priority data that is mapped to the board
		std::map<std::uint8_t, std::uint8_t> currentPriority;
		while (true) {
			std::uint8_t currentCol;
			std::uint8_t currentPriorityNum;
			input.get(currentChar);
			currentCol = static_cast<std::uint8_t>(currentChar);

			if (currentCol == END_CHAR) {
				break;
			}

			input.get(currentChar);
			currentPriorityNum = static_cast<std::uint8_t>(currentChar);

			if (currentPriorityNum == END_CHAR) {
				break;
			}

			currentPriority.emplace(currentCol, currentPriorityNum);
		}

		data.emplace(currentBoard, currentPriority);
	}

	ai.rememberData(data);

	input.close();
}

void FileManager::writeAIData(AI & ai)
{
	//Create and open a temporary output file
	std::ofstream output;

	output.open("temp.txt", std::ofstream::out | std::ofstream::trunc | std::ofstream::binary);

	//Make sure the file opened properly
	if (!output.is_open()) {
		//The file likely did not open because it does not exist, so we will make it instead
		output.open("temp.txt", std::ofstream::out | std::ofstream::binary);

		if (!output.is_open()) {
			//Something is wrong
			std::cout << "\nERROR: Could not open file " << filename;
			return;
		}
	}

	//Create a variable to hold all of the data we are going to write
	std::map<Board::BoardType, std::map<std::uint8_t, std::uint8_t>> dataToWrite = ai.getData();

	//Loop through all of the pairs of boards and priorities within the map
	for (auto& boardsAndPrioritiesPair : dataToWrite) {
		//Write the current board to the file
		for (auto& row : boardsAndPrioritiesPair.first) {
			for (auto& elem : row) {
				output << elem;
			}
		}

		//Write the priority pair list to the file
		for (auto& columnAndPriorityValuePair : boardsAndPrioritiesPair.second) {
			output << columnAndPriorityValuePair.first << columnAndPriorityValuePair.second;
		}

		//Write the end character so we'll know to stop here when reading
		output << END_CHAR;
	}

	//Flush and close the file
	output.flush();
	output.close();

	//Delete the original file
	std::remove(filename.c_str());
	//Rename the temporary file to the original file's name
	std::rename("temp.txt", filename.c_str());
}