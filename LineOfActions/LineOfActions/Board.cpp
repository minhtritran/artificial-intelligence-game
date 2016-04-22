#include "stdafx.h"

using namespace std;

//Set up initial 5x5 board
Board::Board() {
	//Make every tile empty
	for (int row = 0; row < NUM_ROWS; row++) {
		for (int col = 0; col < NUM_COLUMNS; col++) {
			data[row][col] = 0;
		}
	}
	//Set up white pieces
	for (int row = 1; row < NUM_COLUMNS - 1; row++) {
		data[row][0] = 1;
	}
	for (int row = 1; row < NUM_COLUMNS - 1; row++) {
		data[row][NUM_COLUMNS - 1] = 1;
	}
	
	//Set up black pieces
	for (int col = 1; col < NUM_ROWS - 1; col++) {
		data[0][col] = 2;
	}
	for (int col = 1; col < NUM_ROWS - 1; col++) {
		data[NUM_ROWS - 1][col] = 2;
	}
}

//Get tile value at grid position
int Board::getTile(const int row, const int col) {
	if (row < 0 || row > NUM_ROWS || col < 0 || col > NUM_COLUMNS) {
		throw invalid_argument("getTile out of range");
	}
	return data[row][col];
}

//Set tile value at grid position
void Board::setTile(const int row, const int col, const int value) {
	if (row < 0 || row > NUM_ROWS || col < 0 || col > NUM_COLUMNS) {
		throw invalid_argument("getTile out of range");
	}
	if (value < 0 || value > 2) {
		throw invalid_argument("setTile value can only be 0, 1, or 2.");
	}
	data[row][col] = value;
}

//Display the current board with formatting
void Board::display() {
	cout << "Current board:" << endl;
	for (int row = 0; row < NUM_ROWS; row++) {
		cout << "\t";
		for (int col = 0; col < NUM_COLUMNS; col++) {
			if (data[row][col] == 0) {
				cout << "[ ]";
			}
			else if (data[row][col] == 1) {
				cout << "[W]";
			}
			else if (data[row][col] == 2) {
				cout << "[B]";
			}
		}
		cout << endl;
	}
}

bool Board::playPiece(tuple<int, int, int> action) {
	
	return true;
}