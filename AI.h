#include <map>
#include <array>
#include <vector>
#include <typeinfo>
#include "Board.h"
#include "Random.h"

#pragma once
class AI
{
public:
	static const std::uint8_t PRIORITY_INIT_VALUE = 10;
	static const std::uint8_t SPEED_PRIORITY_MODIFIER = 20;
	static const std::uint8_t MAX_PRIORITY_VALUE = 250;

	/*
	Initializes the piece this AI will be playing with
	@param pieceToUse The piece that the AI will be playing with (either RED_PIECE or YELLOW_PIECE)
	*/
	AI(const std::int8_t& pieceToUse);

	/*
	Initializes the piece this AI will be playing with and recalls all previously learned data
	@param pieceToUse The piece that the AI will be playing with (either RED_PIECE or YELLOW_PIECE)
	@param data The data to recall
	*/
	AI(const std::int8_t& pieceToUse, const std::map<Board::BoardType, std::map<std::uint8_t, std::uint8_t>>& data);

	~AI();

	/*
	Chooses a move to make based on the board
	The move will be made on the board directly by the function
	@param board The current board
	@return std::uint8_t The column the AI placed its piece in
	*/
	inline const std::uint8_t makeMove(Board& board) {
		//First, make sure the map has a key equal to this board
		if (movePriorities.find(board.getBoard()) == movePriorities.end()) {
			//Initialize the priority list
			initPriorities(board);
		}

		//Save this board in the list
		boardsFoundInGame.emplace_back(board.getBoard());

		//Sum all of the priority values of the moves that can be made at this point
		std::uint16_t sum = 0;
		for (auto& priority : movePriorities.at(board.getBoard())) {
			auto currentPriorityVal = priority.second;

			sum += static_cast<std::uint16_t>(currentPriorityVal);
		}

		//Generate a random number between 1 and the total sum of the priority values
		const std::uint16_t columnChoice = Random::nextInt(static_cast<std::uint16_t>(1), sum);

		//Convert this random number into a column
		/*First, we need to declare a variable to hold the current sum of priorities as we move through the map to ensure we pick
		the correct column*/
		std::uint16_t currentPriority = 0;

		//Get the selected move
		for (auto& priority : movePriorities.at(board.getBoard())) {
			const auto indexOfChosenMove = priority.first;
			const auto currentPriorityVal = priority.second;

			currentPriority += currentPriorityVal;

			if (currentPriority >= columnChoice) {
				//The column choice is between the last priority value and the current priority value, so this is the column we choose
				board.addPiece(indexOfChosenMove, pieceBeingUsed);
				//Save this move in the list
				indicesOfMoves.emplace_back(indexOfChosenMove);
				return indexOfChosenMove;
			}
		}

		//An error occurred, as no move was selected
		return -1;
	}

	/*
	Modifies the priority values of all of the moves used during this game based upon whether the AI won or not
	*/
	inline void learnFromGame(const std::uint8_t& turnsTaken, const bool& won) {
		//Calculate the value to be added to/subtracted from the priority values
		const std::uint16_t valueModifier = std::max(SPEED_PRIORITY_MODIFIER / turnsTaken, 1);

		if (won) {
			//We are adding to the priority values
			//Loop through every priority value except the very last
			for (auto x = 0; x < boardsFoundInGame.size() - 1; x++) {
				//Define a pointer to the value being modified for clarity
				std::uint8_t* priorityValueBeingModified = &movePriorities.at(boardsFoundInGame.at(x)).at(indicesOfMoves.at(x));

				if (MAX_PRIORITY_VALUE - valueModifier < *priorityValueBeingModified) {
					*priorityValueBeingModified = MAX_PRIORITY_VALUE;
				}
				else {
					*priorityValueBeingModified += valueModifier;
				}
			}

			//Define this expression as the last move priority list for clarity
			auto *lastMovePriorityList = &movePriorities.at(boardsFoundInGame.at(boardsFoundInGame.size() - 1));

			//All moves other than the move chosen potentially miss out on winning the game, so set their priority values to 0
			for (auto it = lastMovePriorityList->begin(); it != lastMovePriorityList->end(); ++it) {
				//Use the iterator to set the priority value to 0
				it->second = 0;
			}

			//Set the priority value of the winning move to the maximum
			lastMovePriorityList->at(indicesOfMoves.at(indicesOfMoves.size() - 1)) = MAX_PRIORITY_VALUE;

			//Clear the game storage vectors
			boardsFoundInGame.clear();
			boardsFoundInGame.shrink_to_fit();
			indicesOfMoves.clear();
			indicesOfMoves.shrink_to_fit();
		}
		else {
			//We are subtracting from the priority values
			//Loop through every priority value except the very last
			for (auto x = 0; x < boardsFoundInGame.size() - 1; x++) {
				//Define a pointer to the value being modified for clarity
				std::uint8_t* priorityValueBeingModified = &movePriorities.at(boardsFoundInGame.at(x)).at(indicesOfMoves.at(x));

				if (valueModifier >= *priorityValueBeingModified) {
					*priorityValueBeingModified = 1;
				}
				else {
					*priorityValueBeingModified -= valueModifier;
				}
			}

			//The last move caused a loss, so we set that priority value to 0
			auto* lastMovePriorityValue = &movePriorities.at(boardsFoundInGame.at(boardsFoundInGame.size() - 1)).at(indicesOfMoves.at(indicesOfMoves.size() - 1));
			lastMovePriorityValue = 0;

			//Now, we need to check if every single move on the final board of the game causes a loss
			for (auto x = boardsFoundInGame.size() - 1; x >= 1; x--) {
				bool allZeros = true;

				//Define a pointer to the priority list of the last move for clarity
				auto *lastMovePriorityList = &movePriorities.at(boardsFoundInGame.at(x));

				for (auto it = lastMovePriorityList->begin(); it != lastMovePriorityList->end(); ++it) {
					if (it->second != 0) {
						allZeros = false;
						break;
					}
				}

				if (allZeros) {
					//Set the priority of the move that caused us to arrive at the board that guarantees a loss to 0
					movePriorities.at(boardsFoundInGame.at(x - 1)).at(indicesOfMoves.at(x - 1)) = 0;

					//Erase the board from memory
					movePriorities.erase(boardsFoundInGame.at(x));
				}
				else {
					break;
				}
			}

			//Clear the game storage vectors
			boardsFoundInGame.clear();
			boardsFoundInGame.shrink_to_fit();
			indicesOfMoves.clear();
			indicesOfMoves.shrink_to_fit();
		}
	}

	/*
	Ends the current game without teaching the AI anything
	*/
	inline void endCurrentGame() {
		//Clear the game storage vectors
		boardsFoundInGame.clear();
		boardsFoundInGame.shrink_to_fit();
		indicesOfMoves.clear();
		indicesOfMoves.shrink_to_fit();
	}

	/*
	Takes learned data in the proper format and remembers it
	@param data The data to be remembered
	*/
	void rememberData(const std::map<Board::BoardType, std::map<std::uint8_t, std::uint8_t>>& data);

	/*
	Returns the learned data
	@return std::map<std::array<std::array<std::int8_t, 7>, 6>, std::map<std::uint8_t, std::uint32_t>> The data
	*/
	const std::map<Board::BoardType, std::map<std::uint8_t, std::uint8_t>> getData() const;

private:
	//This maps boards with certain combinations of pieces on the board to a mapping of columns to move priorities
	std::map<Board::BoardType, std::map<std::uint8_t, std::uint8_t>> movePriorities;

	//Collectively, these two vectors store all of the board statuses and the index of the move chosen for this game
	std::vector<Board::BoardType> boardsFoundInGame;
	std::vector<std::uint8_t> indicesOfMoves;

	//Stores the piece this AI is playing with
	std::int8_t pieceBeingUsed;

	/*
	Initializes a new portion of the movePriorities map if the given board situation has never been seen by the AI before
	@param board The unknown board
	*/
	void initPriorities(Board& board);
};