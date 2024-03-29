// LineOfActions.cpp : Defines the entry point for the console application.
// Minhtri Tran 2016

#include "stdafx.h"

using namespace std;

const int DEPTH_CUTOFF = 4;

int maxDepth = 0;
int numNodesGenerated = 0;
int numTimesEvalCalledMax = 0;
int numTimesEvalCalledMin = 0;
int numTimesPruneMax = 0;
int numTimesPruneMin = 0;

int getBoardSize();
bool getPlayerChoice();
tuple<int, int, int> getAction(shared_ptr<Board> currentBoard);
tuple<int, int, int> alphaBetaSearch(shared_ptr<Board> board);
int maxValue(shared_ptr<Board> board, int alpha, int beta, int depth);
int minValue(shared_ptr<Board> board, int alpha, int beta, int depth);

int main()
{
	int boardSize = getBoardSize();
	Board::playerIsBlack = getPlayerChoice();
	int currentPlayerPieceValue = Board::playerIsBlack ? Board::TILE_BLACK : Board::TILE_WHITE;
	shared_ptr<Board> currentBoard(new Board(boardSize));
	int gameState = currentBoard->terminalTest();

	//Game loop
	while (gameState == Board::NOT_TERMINAL_STATE) {
		currentBoard->display();
		unique_ptr<Board> nextBoard(nullptr);

		//Player's turn
		if ((currentBoard->getCurrentPlayerPieceValue() == Board::TILE_BLACK && Board::playerIsBlack) ||
			(currentBoard->getCurrentPlayerPieceValue() == Board::TILE_WHITE && !Board::playerIsBlack))
		{
			while (!nextBoard) {
				tuple<int, int, int> action = getAction(currentBoard);
				nextBoard = move(currentBoard->playPieceResult(action));
				if (!nextBoard) {
					cout << "Illegal move." << endl;
				}
			}
		}
		//AI's turn
		else {
			//TO DO
			cout << "Doot doot doot, AI deciding on a move." << endl;
			tuple<int, int, int> action = alphaBetaSearch(currentBoard);
			nextBoard = move(currentBoard->playPieceResult(action));
		}

		gameState = nextBoard->terminalTest();
		currentBoard = move(nextBoard);
	}

	//Show winning/losing board along with a statement
	currentBoard->display();
	if (gameState == Board::UTILITY_MIN) cout << "You won! Congraluations!" << endl;
	else if (gameState == Board::UTILITY_MAX) cout << "You lost..." << endl;
	else if (gameState == Board::UTILITY_DRAW) cout << "No one has won.  Draw." << endl;

	return 0;
}

//Aset player which board size to play on
int getBoardSize() {
	int boardSize;
	cout << "Which board size do you want to play on? (5/6): ";
	do {
		cin >> boardSize;
		if (boardSize == 5 || boardSize == 6) {
			return boardSize;
		}
		else cout << "Invalid input. Try again (5/6): ";
		cin.clear();
		cin.ignore(10000, '\n');
		
	} while (true);
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
tuple<int, int, int> getAction(shared_ptr<Board> currentBoard) {
	int row, col, direction;
	bool valid = false;
	while (!valid) {
		valid = true;
		row = -1;
		col = -1;
		direction = -1;
		string direction_str;
		cout << "Enter row and column of piece and direction for it to move (0-" << currentBoard->getNumRows() - 1 << " 0-" << currentBoard->getNumColumns() - 1;
		cout << " u/r/d/l/ur/dr/dl/ul): ";
		cin >> row >> col >> direction_str;
		//check if input for row and column is valid
		if (row < 0 || col < 0 || row > currentBoard->getNumRows() - 1 || col > currentBoard->getNumColumns() - 1) {
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

//Entrance point for the alpha-beta pruning AI algorithm
//Uses a heuristic function to approximate the best action to take if the search tree is too deep
tuple<int, int, int> alphaBetaSearch(shared_ptr<Board> board) {
	maxDepth = 0;
	numNodesGenerated = 1;
	numTimesEvalCalledMax = 0;
	numTimesEvalCalledMin = 0;
	numTimesPruneMax = 0;
	numTimesPruneMin = 0;

	tuple<int, int, int> bestAction;
	int v = INT_MIN;
	int alpha = INT_MIN;
	int depth = 0;
	vector<tuple<int, int, int>> actions = board->getValidActions();
	for (auto it = actions.begin(); it != actions.end(); it++) {
		v = max(v, minValue(move(board->playPieceResult(*it)), alpha, INT_MAX, depth + 1));
		if (v > alpha) {
			alpha = v;
			bestAction = *it;
		}
	}

	cout << "Max depth: " << maxDepth << endl;
	cout << "Total # of nodes generated: " << numNodesGenerated << endl;
	cout << "# of times evaluate() was called in MAX-VALUE function: " << numTimesEvalCalledMax << endl;
	cout << "# of times evaluate() was called in MIN-VALUE function: " << numTimesEvalCalledMin << endl;
	cout << "# of times pruning occured in MAX-VALUE function: " << numTimesPruneMax << endl;
	cout << "# of times pruning occured in MIN-VALUE function: " << numTimesPruneMin << endl;

	return bestAction;
}

//Max-value function used for the alpha-beta pruning algorithm
int maxValue(shared_ptr<Board> board, int alpha, int beta, int depth) {
	numNodesGenerated++;
	maxDepth = max(depth, maxDepth);

	int utility = board->terminalTest();
	if (utility != Board::NOT_TERMINAL_STATE) {
		return utility;
	}

	if (depth >= DEPTH_CUTOFF) {
		numTimesEvalCalledMax++;
		return board->evaluate();
	}

	int v = INT_MIN;
	vector<tuple<int, int, int>> actions = board->getValidActions();
	for (auto it = actions.begin(); it != actions.end(); it++) {
		v = max(v, minValue(move(board->playPieceResult(*it)), alpha, beta, depth + 1));
		if (v >= beta) {
			numTimesPruneMax++;
			return v;
		}
		alpha = max(alpha, v);
	}
	return v;
}

//Min-value function used for the alpha-beta pruning algorithm
int minValue(shared_ptr<Board> board, int alpha, int beta, int depth) {
	numNodesGenerated++;
	maxDepth = max(depth, maxDepth);

	int utility = board->terminalTest();
	if (utility != Board::NOT_TERMINAL_STATE) {
		return utility;
	}

	if (depth >= DEPTH_CUTOFF) {
		numTimesEvalCalledMin++;
		return board->evaluate();
	}

	int v = INT_MAX;
	vector<tuple<int, int, int>> actions = board->getValidActions();
	for (auto it = actions.begin(); it != actions.end(); it++) {
		v = min(v, maxValue(move(board->playPieceResult(*it)), alpha, beta, depth + 1));
		if (v <= alpha) {
			numTimesPruneMin++;
			return v;
		}
		beta = min(beta, v);
	}

	return v;
}