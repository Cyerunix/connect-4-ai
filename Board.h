#pragma once
#include <iostream>
#include <array>

class Board
{
public:
	static const std::int8_t NO_PIECE = -1;
	static const std::int8_t RED_PIECE = 0;
	static const std::int8_t YELLOW_PIECE = 1;

	static const std::uint8_t NUM_ROWS = 4;
	static const std::uint8_t NUM_COLS = 5;

	typedef std::array<std::array<std::int8_t, NUM_COLS>, NUM_ROWS> BoardType;

	/*
	Initializes the board as completely empty
	*/
	Board();
	
	~Board() {}

	/*
	Returns the board as a 2D array
	@return std::array<std::array<std::int8_t, 7>, 6> The board
	*/
	inline const Board::BoardType getBoard() const { return board; }

	/*
	Gets the piece at the given position
	@param row The row to get the piece from
	@parma col The column to get the piece from
	@return std::int8_t One of the constants corresponding to a piece (NO_PIECE is returned if the position is out of bounds)
	*/
	inline const std::int8_t getPiece(const std::uint8_t& row, const std::uint8_t& col) const {
		if (positionInBounds(row, col)) {
			return board.at(row).at(col);
		}

		//If the position is out of bounds, it may as well just be no piece
		return NO_PIECE;
	}

	/*
	Adds a piece to the board in the given column if possible
	type must either be RED_PIECE or YELLOW_PIECE
	@param col The column in which to add the piece
	@param type The type of piece to add (either RED_PIECE or YELLOW_PIECE)
	@return bool true if successful or false otherwise
	*/
	inline const bool addPiece(const std::uint8_t& col, const int& type) {
		//Make sure the column is in bounds
		if (!colInBounds(col)) {
			return false;
		}

		//Check if the topmost space in the column is filled
		if (colIsFull(col)) {
			//No space to add the piece
			return false;
		}

		//Loop through the column to find the spot where the piece can be added
		for (std::uint8_t row = 1; row < board.size(); row++) {
			if (getPiece(row, col) != NO_PIECE) {
				board.at(row - 1).at(col) = type;
				return true;
			}
		}

		//The very last row is the space that is open
		board.at(board.size() - 1).at(col) = type;
		return true;
	}

	/*
	Checks if the given row is in bounds
	@param row The row to check
	@return bool true if the row is in bounds and false otherwise
	*/
	inline const bool rowInBounds(const std::uint8_t& row) const { return row >= 0 && row < NUM_ROWS; }

	/*
	Checks if the given column is in bounds
	@param col The column to check
	@return bool true if the column is in bounds and false otherwise
	*/
	inline const bool colInBounds(const std::uint8_t& col) const { return col >= 0 && col < NUM_COLS; }

	/*
	Returns true if the given position is in bounds on the board or false otherwise
	@param row The row to check
	@param col The column to check
	*/
	inline const bool positionInBounds(const std::uint8_t& row, const std::uint8_t& col) const { return rowInBounds(row) && colInBounds(col); }

	/*
	Returns true if the given column is in bounds and the column is not filled or false otherwise
	@param col The column to check
	@return bool true if the conditions are met or false otherwise
	*/
	inline const bool validMove(const std::uint8_t& col) const { return colInBounds(col) && !colIsFull(col); }

	/*
	Prints the board to the console
	*/
	void printBoard() const;

	/*
	Returns true if there are four pieces in a row on the board or false otherwise
	@param col The column where the last piece was placed
	*/
	inline const bool checkForWin(const std::uint8_t& col) const {
		unsigned int piecesInARow = 1;
		int type;
		std::uint8_t row = UINT8_MAX;

		//Find the row in which the last piece was placed
		for (std::uint8_t r = 0; r < board.size(); r++) {
			if (getPiece(r, col) != NO_PIECE) {
				type = getPiece(r, col);
				row = r;
				break;
			}
		}

		if (row == UINT8_MAX) {
			//The column must be out of bounds
			return false;
		}

		//Check for pieces below the current
		if (row <= 2) { //The current row must be at least up to 2 for there to be 4 in a vertical row
			for (std::uint8_t r = row + 1; r < board.size(); r++) {
				if (getPiece(r, col) == type) {
					piecesInARow++;
				}
				else {
					break;
				}
			}
		}

		if (piecesInARow >= 4) {
			return true;
		}

		//Reset the piece counter
		piecesInARow = 1;

		//Check for pieces to the left and right of the current
		//Start with the left
		if (col > 0) {
			for (std::uint8_t c = col - 1; c < UINT8_MAX; c--) {
				if (getPiece(row, c) == type) {
					piecesInARow++;
				}
				else {
					break;
				}
			}
		}

		//Now check the right
		for (std::uint8_t c = col + 1; c < board.at(0).size(); c++) {
			if (getPiece(row, c) == type) {
				piecesInARow++;
			}
			else {
				break;
			}
		}

		if (piecesInARow >= 4) {
			return true;
		}

		//Reset the piece counter
		piecesInARow = 1;

		//Check the diagonals starting with up and to the left and down and to the right (\)
		//Start with up and to the left
		int offset = 1;
		while (getPiece(row - offset, col - offset) == type) {
			piecesInARow++;
			offset++;
		}

		//Now do down and to the right
		offset = 1;
		while (getPiece(row + offset, col + offset) == type) {
			piecesInARow++;
			offset++;
		}

		if (piecesInARow >= 4) {
			return true;
		}

		//Reset the piece counter
		piecesInARow = 1;

		//Check the diagonal going from up and to the right to down and to the left (/)
		//Start with going up and to the right
		offset = 1;
		while (getPiece(row - offset, col + offset) == type) {
			piecesInARow++;
			offset++;
		}

		//Now go down and to the left
		offset = 1;
		while (getPiece(row + offset, col - offset) == type) {
			piecesInARow++;
			offset++;
		}

		if (piecesInARow >= 4) {
			return true;
		}

		//No matches of four were found
		return false;
	}

	/*
	Returns true if the board is completely filled or false otherwise
	@return bool true if the board is completely full or false otherwise
	*/
	inline const bool isFull() const {
		for (std::uint8_t col = 0; col < NUM_COLS; col++) {
			if (!colIsFull(col)) {
				return false;
			}
		}

		return true;
	}

	/*
	Returns true if the given column is full or false otherwise
	@param col The column to check
	@return bool true if the given column is full or false otherwise
	*/
	inline const bool colIsFull(const std::uint8_t& col) const {
		//Make sure the column is in bounds
		if (colInBounds(col)) {
			return getPiece(0, col) != NO_PIECE;
		}

		//The column may as well be considered full since it does not exist
		return true;
	}

	/*
	Replaces all of the pieces on the board with NO_PIECE
	*/
	inline void clearBoard() {
		for (std::uint8_t row = 0; row < NUM_ROWS; row++) {
			for (std::uint8_t col = 0; col < NUM_COLS; col++) {
				board.at(row).at(col) = NO_PIECE;
			}
		}
	}

private:
	BoardType board;
};