#include "stdafx.h"

using namespace std;

//Set up initial 5x5 board
Board::Board() {
	//Make every tile empty
	for (int row = 0; row < NUM_ROWS; row++) {
		for (int col = 0; col < NUM_COLUMNS; col++) {
			data[row][col] = TILE_EMPTY;
		}
	}
	//Set up white pieces
	for (int row = 1; row < NUM_COLUMNS - 1; row++) {
		data[row][0] = TILE_WHITE;
	}
	for (int row = 1; row < NUM_COLUMNS - 1; row++) {
		data[row][NUM_COLUMNS - 1] = TILE_WHITE;
	}
	
	//Set up black pieces
	for (int col = 1; col < NUM_ROWS - 1; col++) {
		data[0][col] = TILE_BLACK;
	}
	for (int col = 1; col < NUM_ROWS - 1; col++) {
		data[NUM_ROWS - 1][col] = TILE_BLACK;
	}
}

//Get tile value at grid position
int Board::getTile(const int row, const int col) {
	if (row < 0 || row >= NUM_ROWS || col < 0 || col >= NUM_COLUMNS) {
		throw invalid_argument("Get Tile out of range");
	}
	return data[row][col];
}

//Set tile value at grid position
void Board::setTile(const int row, const int col, const int value) {
	if (row < 0 || row >= NUM_ROWS || col < 0 || col >= NUM_COLUMNS) {
		throw invalid_argument("Set Tile out of range");
	}
	if (value < 0 || value > 2) {
		throw invalid_argument("Set Tile value can only be 0, 1, or 2.");
	}
	data[row][col] = value;
}

//Display the current board with formatting
void Board::display() {
	cout << "Current board:" << endl;
	for (int row = 0; row < NUM_ROWS; row++) {
		cout << "\t";
		for (int col = 0; col < NUM_COLUMNS; col++) {
			if (data[row][col] == TILE_EMPTY) {
				cout << "[ ]";
			}
			else if (data[row][col] == TILE_WHITE) {
				cout << "[W]";
			}
			else if (data[row][col] == TILE_BLACK) {
				cout << "[B]";
			}
		}
		cout << endl;
	}
}

//Attempt to do action on board as either black or white
//Returns true if it's a valid move, false otherwise
bool Board::playPiece(const tuple<int, int, int>& action, const bool playAsBlack) {
	int row = get<0>(action);
	int col = get<1>(action);
	int direction = get<2>(action);
	int pieceValue = playAsBlack ? TILE_BLACK : TILE_WHITE;
	if (data[row][col] != pieceValue) {
		cout << "Illegal move: You did not select your own piece." << endl;
		return false;
	}
	int numTilesOnLine = countLine(action);
	int destRow, destCol, destValue;
	switch (direction) {
		case DIR_UP:
			destRow = row - numTilesOnLine;
			destCol = col;
			break;
		case DIR_RIGHT:
			destRow = row;
			destCol = col + numTilesOnLine;
			break;
		case DIR_DOWN:
			destRow = row + numTilesOnLine;
			destCol = col;
			break;
		case DIR_LEFT:
			destRow = row;
			destCol = col - numTilesOnLine;
			break;
		case DIR_UP_RIGHT:
			destRow = row - numTilesOnLine;
			destCol = col + numTilesOnLine;
			break;
		case DIR_DOWN_RIGHT:
			destRow = row + numTilesOnLine;
			destCol = col + numTilesOnLine;
			break;
		case DIR_DOWN_LEFT:
			destRow = row + numTilesOnLine;
			destCol = col - numTilesOnLine;
			break;
		case DIR_UP_LEFT:
			destRow = row - numTilesOnLine;
			destCol = col - numTilesOnLine;
			break;
	}

	//Check if destination is outside of board bounds
	try {
		destValue = getTile(destRow, destCol);
	}
	catch (const invalid_argument& e) {
		cout << "Illegal move: Outside of board bounds." << endl;
		return false;
	}
	//Check if the destination tile contains your own piece
	if (destValue == pieceValue) {
		cout << "Illegal move: Your own piece is at the destination tile." << endl;
		return false;
	}

	//Check if enemy piece is in the way
	tuple<bool, string> result = checkIfEnemyPieceInWay(row, col, destRow, destCol, pieceValue);
	if (!get<0>(result)) {
		cout << get<1>(result) << endl;
		return false;
	}
	
	//Move piece
	try {
		setTile(row, col, TILE_EMPTY);
		setTile(destRow, destCol, pieceValue);
	}
	catch (const invalid_argument& e) {
		cerr << "Exception: " << e.what() << endl;
	}

	return true;
}

//Count number of tiles on the given line (specified by row, column, and direction)
int Board::countLine(const tuple<int, int, int>& action) {
	int row = get<0>(action);
	int col = get<1>(action);
	int direction = get<2>(action);
	int count = 0;
	switch (direction) {
		case DIR_UP:
		case DIR_DOWN:
			for (int i = 0; i < NUM_ROWS; i++) {
				if (data[i][col] != 0) count++;
			}
			break;
		case DIR_RIGHT:
		case DIR_LEFT:
			for (int i = 0; i < NUM_COLUMNS; i++) {
				if (data[row][i] != 0) count++;
			}
			break;
		case DIR_UP_RIGHT:
		case DIR_DOWN_LEFT: {
			//count tiles (including self) in the down left diagonal direction
			int tmp_row = row;
			int tmp_col = col;
			while (tmp_row < NUM_ROWS && tmp_col >= 0) {
				if (data[tmp_row][tmp_col] != 0) count++;
				tmp_row++;
				tmp_col--;
			}
			//count tiles (excluding self) in the up right diagonal direction
			tmp_row = row - 1;
			tmp_col = col + 1;
			while (tmp_row >= 0 && tmp_col < NUM_COLUMNS) {
				if (data[tmp_row][tmp_col] != 0) count++;
				tmp_row--;
				tmp_col++;
			}
			break;
		}
		case DIR_UP_LEFT:
		case DIR_DOWN_RIGHT: {
			//count tiles (including self) in the down right diagonal direction
			int tmp_row = row;
			int tmp_col = col;
			while (tmp_row < NUM_ROWS && tmp_col < NUM_COLUMNS) {
				if (data[tmp_row][tmp_col] != 0) count++;
				tmp_row++;
				tmp_col++;
			}
			//count tiles (excluding self) in the up left diagonal direction
			tmp_row = row - 1;
			tmp_col = col - 1;
			while (tmp_row >= 0 && tmp_col >= 0) {
				if (data[tmp_row][tmp_col] != 0) count++;
				tmp_row--;
				tmp_col--;
			}
			break;
		}
	}
	return count;
}

//Check if an enemy piece is in the way
//Returns a tuple that returns the result of the check and the error message
tuple<bool, string> Board::checkIfEnemyPieceInWay(const int startRow, const int startCol, const int destRow, const int destCol, const int pieceValue) {
	bool valid = true;
	string errorMessage;
	int minRow = min(startRow, destRow);
	int maxRow = max(startRow, destRow);
	int minCol = min(startCol, destCol);
	int maxCol = max(startCol, destCol);

	//same row
	if (minRow == maxRow) {
		for (int col = minCol + 1; col < maxCol; col++) {
			if (data[minRow][col] != TILE_EMPTY && data[minRow][col] != pieceValue) valid = false;
		}
	}
	//same column
	else if (minCol == maxCol) {
		for (int row = minRow + 1; row < maxRow; row++) {
			if (data[row][minCol] != TILE_EMPTY && data[row][minCol] != pieceValue) valid = false;
		}
	}
	//positive diagonal
	else if ((startRow == minRow && startCol == maxCol) || (startRow == maxRow && startCol == minCol)) {
		int row, col;
		for (row = minRow + 1, col = maxCol - 1; row < maxRow && col > minCol; col--, row++) {
			if (data[row][col] != TILE_EMPTY && data[row][col] != pieceValue) valid = false;
		}
	}
	//negative diagonal
	else {
		int row, col;
		for (row = minRow + 1, col = minCol + 1; row < maxRow && col < maxCol; row++, col++) {
			if (data[row][col] != TILE_EMPTY && data[row][col] != pieceValue) valid = false;
		}
	}

	if (!valid) errorMessage = "Illegal move: Can not jump over enemy piece.";
	return make_tuple(valid, errorMessage);
}