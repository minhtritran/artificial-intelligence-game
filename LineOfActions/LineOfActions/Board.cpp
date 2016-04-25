#include "stdafx.h"

using namespace std;

//Set up initial board
Board::Board() {
	////Make every tile empty
	//for (int row = 0; row < NUM_ROWS; row++) {
	//	for (int col = 0; col < NUM_COLUMNS; col++) {
	//		data[row][col] = TILE_EMPTY;
	//	}
	//}
	////Set up white pieces
	//for (int row = 1; row < NUM_COLUMNS - 1; row++) {
	//	data[row][0] = TILE_WHITE;
	//}
	//for (int row = 1; row < NUM_COLUMNS - 1; row++) {
	//	data[row][NUM_COLUMNS - 1] = TILE_WHITE;
	//}
	//
	////Set up black pieces
	//for (int col = 1; col < NUM_ROWS - 1; col++) {
	//	data[0][col] = TILE_BLACK;
	//}
	//for (int col = 1; col < NUM_ROWS - 1; col++) {
	//	data[NUM_ROWS - 1][col] = TILE_BLACK;
	//}

	data[0][0] = TILE_BLACK;
	data[0][3] = TILE_BLACK;
	data[2][1] = TILE_WHITE;
	data[0][2] = TILE_WHITE;
	data[0][4] = TILE_BLACK;
	data[4][4] = TILE_WHITE;

	currentPlayerPieceValue = TILE_BLACK; //Black starts first
	initializeValidActions();
}

//Initialize board with data
Board::Board(int currentPlayerPieceValue, int data[][NUM_COLUMNS]) : currentPlayerPieceValue(currentPlayerPieceValue) {
	for (int row = 0; row < NUM_ROWS; row++) {
		for (int col = 0; col < NUM_COLUMNS; col++) {
			this->data[row][col] = data[row][col];
		}
	}
}

bool operator==(const Board & lhs, const Board & rhs)
{
	for (int row = 0; row < Board::NUM_ROWS; row++) {
		for (int col = 0; col < Board::NUM_COLUMNS; col++) {
			if (lhs.data[row][col] != rhs.data[row][col]) return false;
		}
	}
	if (lhs.currentPlayerPieceValue != rhs.currentPlayerPieceValue) return false;

	return true;
}

//Get tile value at grid position
int Board::getTile(const int row, const int col) {
	if (row < 0 || row >= NUM_ROWS || col < 0 || col >= NUM_COLUMNS) {
		throw invalid_argument("Get Tile out of range");
	}
	return data[row][col];
}

//Get current player piece value
int Board::getCurrentPlayerPieceValue() {
	return currentPlayerPieceValue;
}

vector<tuple<tuple<int, int, int>, int>> Board::getValidActions() {
	return validActions;
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

//Do action on a new copy of the board and return it
//Returns a null board if action is invalid
unique_ptr<Board> Board::playPieceResult(const tuple<int, int, int>& action) {
	int row = get<0>(action);
	int col = get<1>(action);
	int direction = get<2>(action);

	//Return a null board if action is not valid
	bool validAction = false;
	for (auto it = validActions.begin(); it != validActions.end(); it++) {
		tuple<int, int, int> currentAction = get<0>(*it);
		if (currentAction == action) {
			validAction = true;
			break;
		}
	}
	if (!validAction) {
		unique_ptr<Board> nullReturn(nullptr);
		return nullReturn;
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
	
	int nextPlayerPieceValue = currentPlayerPieceValue == TILE_BLACK ? TILE_WHITE : TILE_BLACK;
	unique_ptr<Board> newBoard(new Board(nextPlayerPieceValue, data));

	//Move piece
	try {
		newBoard->setTile(row, col, TILE_EMPTY);
		newBoard->setTile(destRow, destCol, currentPlayerPieceValue);
	}
	catch (const invalid_argument& e) {
		cerr << "Exception: " << e.what() << endl;
	}

	newBoard->initializeValidActions();

	return newBoard;
}

//Check to see if board is in a terminal state
//If it is, return the appropriate utility value
int Board::terminalTest() {
	bool blackContiguous = checkContiguousBody(TILE_BLACK);
	bool whiteContiguous = checkContiguousBody(TILE_WHITE);

	//if both players have contiguous bodies, then the player who made the move wins
	if (blackContiguous && whiteContiguous) {
		if (currentPlayerPieceValue == TILE_BLACK) return UTILITY_WHITE_WINS;
		else return UTILITY_BLACK_WINS;
	}
	else if (blackContiguous) return UTILITY_BLACK_WINS;
	else if (whiteContiguous) return  UTILITY_WHITE_WINS;
	else if (validActions.size() == 0) return UTILITY_DRAW;
	else return NOT_TERMINAL_STATE;
}

//Initialize the vector of valid actions based on the current board and the current player
void Board::initializeValidActions() {
	for (int row = 0; row < NUM_ROWS; row++) {
		for (int col = 0; col < NUM_COLUMNS; col++) {
			if (data[row][col] == currentPlayerPieceValue) {
				for (int direction = 0; direction < NUM_DIRECTIONS; direction++) {
					tuple<int, int, int> currentAction = make_tuple(row, col, direction);
					int numTilesOnLine = countLine(currentAction);
					int destRow = -1;
					int destCol = -1;
					int destValue = -1;
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
					if (destRow < 0 || destRow >= NUM_ROWS || destCol < 0 || destCol >= NUM_COLUMNS) {
						continue;
					}

					//Check if the destination tile contains your own piece
					if (destValue == currentPlayerPieceValue) {
						continue;
					}

					//Check if enemy piece is in the way
					tuple<bool, string> result = checkIfEnemyPieceInWay(row, col, destRow, destCol);
					if (!get<0>(result)) {
						continue;
					}
					
					validActions.push_back(make_tuple(currentAction, UTILITY_FILLER_VALUE));
				}
			}
		}
	}
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
tuple<bool, string> Board::checkIfEnemyPieceInWay(const int startRow, const int startCol, const int destRow, const int destCol) {
	bool valid = true;
	string errorMessage;
	int minRow = min(startRow, destRow);
	int maxRow = max(startRow, destRow);
	int minCol = min(startCol, destCol);
	int maxCol = max(startCol, destCol);

	//same row
	if (minRow == maxRow) {
		for (int col = minCol + 1; col < maxCol; col++) {
			if (data[minRow][col] != TILE_EMPTY && data[minRow][col] != currentPlayerPieceValue) valid = false;
		}
	}
	//same column
	else if (minCol == maxCol) {
		for (int row = minRow + 1; row < maxRow; row++) {
			if (data[row][minCol] != TILE_EMPTY && data[row][minCol] != currentPlayerPieceValue) valid = false;
		}
	}
	//positive diagonal
	else if ((startRow == minRow && startCol == maxCol) || (startRow == maxRow && startCol == minCol)) {
		int row, col;
		for (row = minRow + 1, col = maxCol - 1; row < maxRow && col > minCol; col--, row++) {
			if (data[row][col] != TILE_EMPTY && data[row][col] != currentPlayerPieceValue) valid = false;
		}
	}
	//negative diagonal
	else {
		int row, col;
		for (row = minRow + 1, col = minCol + 1; row < maxRow && col < maxCol; row++, col++) {
			if (data[row][col] != TILE_EMPTY && data[row][col] != currentPlayerPieceValue) valid = false;
		}
	}

	if (!valid) errorMessage = "Illegal move: Can not jump over enemy piece.";
	return make_tuple(valid, errorMessage);
}

//Check the if every piece of the given value is in one contiguous body using BFS explore
bool Board::checkContiguousBody(const int pieceValue) {
	tuple<int, int> firstPiece = findAnyOneLocationOfPiece(pieceValue);

	//a body made of no pieces is contiguous
	if (get<0>(firstPiece) == -1 && get<1>(firstPiece) == -1) return true;

	queue<tuple<int, int>> bfsQueue;
	vector<tuple<int, int>> bfsVisited;
	bfsQueue.push(firstPiece);

	tuple<int, int> currentPiece;
	int row, col;
	while (!bfsQueue.empty()) {
		currentPiece = bfsQueue.front();
		bfsQueue.pop();
		
		//already visited
		if (find(bfsVisited.begin(), bfsVisited.end(), currentPiece) != bfsVisited.end()) continue;
		
		bfsVisited.push_back(currentPiece);

		//Add neighboring same-value pieces to queue
		row = get<0>(currentPiece);
		col = get<1>(currentPiece);
		if (row - 1 >= 0 && col - 1 >= 0 && data[row - 1][col - 1] == pieceValue) bfsQueue.push(make_tuple(row - 1, col - 1)); //up left
		if (row - 1 >= 0 && data[row - 1][col] == pieceValue) bfsQueue.push(make_tuple(row - 1, col)); //up
		if (row - 1 >= 0 && col + 1 < NUM_COLUMNS && data[row - 1][col + 1] == pieceValue) bfsQueue.push(make_tuple(row - 1, col + 1)); //up right
		if (col + 1 < NUM_COLUMNS && data[row][col + 1] == pieceValue) bfsQueue.push(make_tuple(row, col + 1)); //right
		if (row + 1 < NUM_ROWS && col + 1 < NUM_COLUMNS && data[row + 1][col + 1] == pieceValue) bfsQueue.push(make_tuple(row + 1, col + 1)); //down right
		if (row + 1 < NUM_ROWS && data[row + 1][col] == pieceValue) bfsQueue.push(make_tuple(row + 1, col)); //down
		if (row + 1 < NUM_ROWS && col - 1 >= 0 && data[row + 1][col - 1] == pieceValue) bfsQueue.push(make_tuple(row + 1, col - 1)); //down left
		if (col - 1 >= 0 && data[row][col - 1] == pieceValue) bfsQueue.push(make_tuple(row, col - 1)); //left
	}
	
	//get total number of pieces on board with the given value
	int numPieces = 0;
	for (int row = 0; row < NUM_ROWS; row++) {
		for (int col = 0; col < NUM_COLUMNS; col++) {
			if (data[row][col] == pieceValue) numPieces++;
		}
	}

	//if all pieces were visited, that means they are in one contiguous body
	if (numPieces == bfsVisited.size()) return true;
	else return false;
}

//Find any piece of the given value and return its location 
tuple<int, int> Board::findAnyOneLocationOfPiece(const int pieceValue) {
	for (int row = 0; row < NUM_ROWS; row++) {
		for (int col = 0; col < NUM_COLUMNS; col++) {
			if (data[row][col] == pieceValue) {
				return make_tuple(row, col);
			}
		}
	}
	return make_tuple(-1, -1);
}