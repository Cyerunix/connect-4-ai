#include "AI.h"

AI::AI(const std::int8_t& pieceToUse)
	: pieceBeingUsed(pieceToUse)
{}

AI::AI(const std::int8_t & pieceToUse, const std::map<std::array<std::array<std::int8_t, Board::NUM_COLS>, Board::NUM_ROWS>, std::map<std::uint8_t, std::uint8_t>>& data)
	: pieceBeingUsed(pieceToUse)
{
	rememberData(data);
}

AI::~AI()
{
	movePriorities.clear();
	boardsFoundInGame.clear();
	indicesOfMoves.clear();
}

void AI::rememberData(const std::map<std::array<std::array<std::int8_t, Board::NUM_COLS>, Board::NUM_ROWS>, std::map<std::uint8_t, std::uint8_t>>& data)
{
	movePriorities = data;
}

const std::map<std::array<std::array<std::int8_t, Board::NUM_COLS>, Board::NUM_ROWS>, std::map<std::uint8_t, std::uint8_t>> AI::getData() const
{
	return movePriorities;
}

void AI::initPriorities(Board& board)
{
	//Create a map to hold all of the generated priorities
	std::map<std::uint8_t, std::uint8_t> generatedPriorities;

	//Generate the priorities
	for (std::uint8_t col = 0; col < Board::NUM_COLS; col++) {
		//If the column is full, this move is impossible
		if (!board.colIsFull(col)) {
			generatedPriorities.emplace(col, PRIORITY_INIT_VALUE);
		}
	}

	//Map the priorities to the current board
	movePriorities.emplace(board.getBoard(), generatedPriorities);
}
