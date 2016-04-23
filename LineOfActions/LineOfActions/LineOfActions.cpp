// LineOfActions.cpp : Defines the entry point for the console application.
// Minhtri Tran 2016

#include "stdafx.h"

using namespace std;

bool getPlayerChoice();
tuple<int, int, int> getAction();

int main()
{
	bool playerIsBlack = getPlayerChoice();
	unique_ptr<Board> board(new Board());

	while (true) {
		board->display();
		bool validAction = false;
		while (!validAction) {
			tuple<int, int, int> action = getAction();
			validAction = board->playPiece(action, playerIsBlack);
		}
	}

	return 0;
}

//Ask player whether he wants to be black or white
bool getPlayerChoice() {
	string playerChoice;
	cout << "Which color do you want to play as? (b/w): ";
	do {
		cin >> playerChoice;
		if (playerChoice.compare("b") == 0 || playerChoice.compare("B") == 0) {
			cout << "You're player Black." << endl;
			return true;
		}
		else if (playerChoice.compare("w") == 0 || playerChoice.compare("W") == 0) {
			cout << "You're player White." << endl;
			return false;
		}
		else cout << "Invalid input. Try again (b/w): ";
	} while (true);
	
}

//Ask player for his action, and on which piece
tuple<int, int, int> getAction() {
	int row, col, direction;
	bool valid = false;
	while (!valid) {
		valid = true;
		row = -1;
		col = -1;
		direction = -1;
		string direction_str;
		cout << "Enter row and column of piece and direction for it to move (0-" << Board::NUM_ROWS - 1 << " 0-" << Board::NUM_COLUMNS - 1;
		cout << " u/r/d/l/ur/dr/dl/ul): ";
		cin >> row >> col >> direction_str;
		//check if input for row and column is valid
		if (row < 0 || col < 0 || row > Board::NUM_ROWS - 1 || col > Board::NUM_COLUMNS - 1) {
			cout << "Invalid input for row/column, please try again." << endl;
			valid = false;
		}
		else {
			//cout << "Enter direction of movement (u/r/d/l/ur/dr/dl/ul): ";
			//cin >> direction_str;
			//store direction as an int
			if (direction_str.compare("u") == 0 || direction_str.compare("U") == 0) direction = Board::DIR_UP;
			else if (direction_str.compare("r") == 0 || direction_str.compare("R") == 0) direction = Board::DIR_RIGHT;
			else if (direction_str.compare("d") == 0 || direction_str.compare("D") == 0) direction = Board::DIR_DOWN;
			else if (direction_str.compare("l") == 0 || direction_str.compare("L") == 0) direction = Board::DIR_LEFT;
			else if (direction_str.compare("ur") == 0 || direction_str.compare("UR") == 0) direction = Board::DIR_UP_RIGHT;
			else if (direction_str.compare("dr") == 0 || direction_str.compare("DR") == 0) direction = Board::DIR_DOWN_RIGHT;
			else if (direction_str.compare("dl") == 0 || direction_str.compare("DL") == 0) direction = Board::DIR_DOWN_LEFT;
			else if (direction_str.compare("ul") == 0 || direction_str.compare("UL") == 0) direction = Board::DIR_UP_LEFT;
			//check if input for direction is valid
			if (direction == -1) {
				cout << "Invalid input for direction, please try again." << endl;
				valid = false;
			}
		}
		cin.clear();
		cin.ignore(10000, '\n');
	}
	return make_tuple(row, col, direction);
}