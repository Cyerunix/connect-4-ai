#include <iostream>
#include <string>
#include <conio.h>
#include "Board.h"
#include "AI.h"
#include "Random.h"
#include "FileManager.h"
#include <bitset>

int main(int argc, char* argv) {
	//Create a board
	Board board;

	//Initialize the random generation
	Random::init();

	std::cout << "Reading data, please wait...\n";

	//Create objects for both AI save files
	FileManager bot1Save("AI_Data/AI_1_data.txt");
	FileManager bot2Save("AI_Data/AI_2_data.txt");

	//Create the two AI objects
	AI AI1(Board::YELLOW_PIECE);
	AI AI2(Board::RED_PIECE);

	//Read all of the data for both AI objects
	bot1Save.readAIData(AI1);
	bot2Save.readAIData(AI2);
	
	//Initialize a variable to keep track of the number of moves each game takes
	std::uint8_t numMoves = 0;

	bool running = true;
	
	//Code for checking efficiency
	/*std::vector<std::chrono::milliseconds> times;

	std::chrono::milliseconds start, end;

	for (unsigned int it = 0; it < 100; it++) {
		start = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
		for (unsigned int x = 0; x < 100; x++) {
			while (!board.isFull()) {
				moves++;

				if (board.checkForWin(AI1.makeMove(board))) {
					AI1.learnFromGame(moves, true);
					AI2.learnFromGame(moves, false);

					moves = 0;

					board.clearBoard();

					break;
				}

				if (board.checkForWin(AI2.makeMove(board))) {
					AI1.learnFromGame(moves, false);
					AI2.learnFromGame(moves, true);

					moves = 0;

					board.clearBoard();

					break;
				}
			}

			if (board.isFull()) {
				AI1.endCurrentGame();
				AI2.endCurrentGame();

				moves = 0;

				board.clearBoard();
			}
		}
		end = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());

		times.emplace_back((end - start));
	}

	std::int64_t totalTime = 0;

	for (auto& time : times) {
		totalTime += time.count();
	}

	std::cout << "Times averaged out to " << totalTime / times.size() << " milliseconds";*/

	//Start UI
	while (running) {
		std::cout << "Welcome to the Connect 4 AI Program!\nWould you like to train the AI further (t) or play against the AI (p)?: ";
		std::string selection = std::string();

		while (selection == "") {
			std::getline(std::cin, selection);
		}

		switch (tolower(selection.at(0))) {
		case 't':
			std::cout << "\nThe AI is now training against itself... (press any key to stop): ";
			while (true) {
				if (_kbhit()) {
					_getche();
					running = false;

					std::cout << "\nSaving... please wait...";

					bot1Save.writeAIData(AI1);
					bot2Save.writeAIData(AI2);

					break;
				}

				while (!board.isFull()) {
					numMoves++;

					if (board.checkForWin(AI1.makeMove(board))) {
						AI1.learnFromGame(numMoves, true);
						AI2.learnFromGame(numMoves, false);

						numMoves = 0;

						board.clearBoard();

						break;
					}

					if (board.isFull()) {
						AI1.endCurrentGame();
						AI2.endCurrentGame();

						numMoves = 0;

						board.clearBoard();

						break;
					}

					if (board.checkForWin(AI2.makeMove(board))) {
						AI1.learnFromGame(numMoves, false);
						AI2.learnFromGame(numMoves, true);

						numMoves = 0;

						board.clearBoard();

						break;
					}
				}

				if (board.isFull()) {
					AI1.endCurrentGame();
					AI2.endCurrentGame();

					numMoves = 0;

					board.clearBoard();
				}
			}
			break;
		case 'p':
		{
			bool allFinished = false;

			while (!allFinished) {
				std::cout << "Would you like to play first (f) or second (s)?: ";
				selection = std::string();

				while (selection == "") {
					std::getline(std::cin, selection);
				}

				bool gameIsPlaying = true;

				switch (tolower(selection.at(0))) {
				case 'f':
					board.clearBoard();
					while (gameIsPlaying) {
						std::uint8_t col = 0;
						unsigned int input;
						
						//Human
						while (gameIsPlaying) {
							board.printBoard();
							std::cout << "Place your piece in one of the columns: ";
							std::cin >> input;
							col = input - 1;

							if (board.validMove(col)) {
								board.addPiece(col, Board::YELLOW_PIECE);

								if (board.checkForWin(col)) {
									board.printBoard();
									board.clearBoard();

									while (gameIsPlaying) {
										std::cout << "\nYou won! Would you like to play again? (y/n): ";

										selection = std::string();

										while (selection == "") {
											std::getline(std::cin, selection);
										}

										switch (tolower(selection.at(0))) {
										case 'y':
											gameIsPlaying = false;
											break;
										case 'n':
											gameIsPlaying = false;
											allFinished = true;
											break;
										default:
											std::cout << "\n\nInvalid response!\n\n";
											break;
										}
									}
								}

								break;
							}
							else {
								std::cout << "\n\nInvalid column!\n\n";
							}
						}

						//AI
						if (board.checkForWin(AI2.makeMove(board))) {
							board.printBoard();
							board.clearBoard();

							while (gameIsPlaying) {
								std::cout << "\nYou lost... Would you like to play again? (y/n): ";

								selection = std::string();

								while (selection == "") {
									std::getline(std::cin, selection);
								}

								switch (tolower(selection.at(0))) {
								case 'y':
									gameIsPlaying = false;
									break;
								case 'n':
									gameIsPlaying = false;
									allFinished = true;
									break;
								default:
									std::cout << "\n\nInvalid response!\n\n";
									break;
								}
							}
						}

						if (board.isFull()) {
							board.printBoard();
							board.clearBoard();

							while (gameIsPlaying) {
								std::cout << "\nYou tied. Would you like to play again? (y/n): ";

								selection = std::string();

								while (selection == "") {
									std::getline(std::cin, selection);
								}

								switch (tolower(selection.at(0))) {
								case 'y':
									gameIsPlaying = false;
									break;
								case 'n':
									gameIsPlaying = false;
									allFinished = true;
									break;
								default:
									std::cout << "\n\nInvalid response!\n\n";
									break;
								}
							}
						}
					}
					break;
				case 's':
					board.clearBoard();
					while (gameIsPlaying) {
						//AI
						if (board.checkForWin(AI1.makeMove(board))) {
							board.printBoard();
							board.clearBoard();

							while (gameIsPlaying) {
								std::cout << "\nYou lost... Would you like to play again? (y/n): ";

								selection = std::string();

								while (selection == "") {
									std::getline(std::cin, selection);
								}

								switch (tolower(selection.at(0))) {
								case 'y':
									gameIsPlaying = false;
									break;
								case 'n':
									gameIsPlaying = false;
									allFinished = true;
									break;
								default:
									std::cout << "\n\nInvalid response!\n\n";
									break;
								}
							}
						}

						std::uint8_t col = 0;
						unsigned int input;

						//Human
						while (gameIsPlaying) {
							board.printBoard();
							std::cout << "Place your piece in one of the columns: ";
							std::cin >> input;
							col = input - 1;

							if (board.validMove(col)) {
								board.addPiece(col, Board::RED_PIECE);

								if (board.checkForWin(col)) {
									board.printBoard();
									board.clearBoard();

									while (gameIsPlaying) {
										std::cout << "\nYou won! Would you like to play again? (y/n): ";

										selection = std::string();

										while (selection == "") {
											std::getline(std::cin, selection);
										}

										switch (tolower(selection.at(0))) {
										case 'y':
											gameIsPlaying = false;
											break;
										case 'n':
											gameIsPlaying = false;
											allFinished = true;
											break;
										default:
											std::cout << "\n\nInvalid response!\n\n";
											break;
										}
									}
								}

								break;
							}
							else {
								std::cout << "\n\nInvalid column!\n\n";
							}
						}

						if (board.isFull()) {
							board.printBoard();
							board.clearBoard();

							while (gameIsPlaying) {
								std::cout << "\nYou tied. Would you like to play again? (y/n): ";

								selection = std::string();

								while (selection == "") {
									std::getline(std::cin, selection);
								}

								switch (tolower(selection.at(0))) {
								case 'y':
									gameIsPlaying = false;
									break;
								case 'n':
									gameIsPlaying = false;
									allFinished = true;
									break;
								default:
									std::cout << "\n\nInvalid response!\n\n";
									break;
								}
							}
						}
					}
					break;
				default:
					std::cout << "\n\nInvalid response!\n\n";
					break;
				}
			}
		}
			break;
		default:
			std::cout << "\n\nInvalid response!\n\n";
			break;
		}
	}

	std::cout << "\n\nIt is now safe to exit";
}
