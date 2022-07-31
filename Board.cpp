#include "Board.h"

Board::Board()
{
	for (std::uint8_t row = 0; row < board.size(); row++) {
		for (std::uint8_t col = 0; col < board.at(row).size(); col++) {
			board.at(row).at(col) = NO_PIECE;
		}
	}
}

void Board::printBoard() const
{
	//Print the column numbers
	for (size_t x = 1; x <= board.size() + 1; x++) {
		std::cout << x << " ";
	}
	std::cout << "\n\n";

	//Print all the pieces on the board
	for (auto& row : board) {
		for (auto& elem : row) {
			switch (elem) {
			case NO_PIECE:
				std::cout << "- ";
				break;
			case RED_PIECE:
				//Print a square
				std::cout << static_cast<unsigned char>(254) << " ";
				break;
			case YELLOW_PIECE:
				std::cout << "O ";
				break;
			}
		}

		std::cout << "\n";
	}
}