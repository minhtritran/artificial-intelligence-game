// LineOfActions.cpp : Defines the entry point for the console application.
// Minhtri Tran 2016

#include "stdafx.h"

using namespace std;

bool getPlayerChoice();
tuple<int, int, int> getAction();
tuple<int, int, int> alphaBetaSearch(shared_ptr<Board> board);
int maxValue(shared_ptr<Board> board, int alpha, int beta, int depth);
int minValue(shared_ptr<Board> board, int alpha, int beta, int depth);

int main()
{
	bool playerIsBlack = getPlayerChoice();
	int currentPlayerPieceValue = playerIsBlack ? Board::TILE_BLACK : Board::TILE_WHITE;
	shared_ptr<Board> currentBoard(new Board());
	int gameState = currentBoard->terminalTest();;

	//Game loop
	while (gameState == Board::NOT_TERMINAL_STATE) {
		currentBoard->display();
		unique_ptr<Board> nextBoard(nullptr);

		//Player's turn
		if ((currentBoard->getCurrentPlayerPieceValue() == Board::TILE_BLACK && playerIsBlack) || 
			(currentBoard->getCurrentPlayerPieceValue() == Board::TILE_WHITE && !playerIsBlack))
		{
			while (!nextBoard) {
				tuple<int, int, int> action = getAction();
				nextBoard = move(currentBoard->playPieceResult(action));
				if (!nextBoard) {
					cout << "Illegal move." << endl;
				}
			}
		}
		//AI's turn
		else {
			//TO DO
			cout << "Doot doot doot, AI makes a move." << endl;
			tuple<int, int, int> action = alphaBetaSearch(currentBoard);
			nextBoard = move(currentBoard->playPieceResult(action));
		}

		gameState = nextBoard->terminalTest();
		currentBoard = move(nextBoard);
	}

	//Show winning/losing board along with a statement
	currentBoard->display();
	if (gameState == Board::UTILITY_BLACK_WINS && playerIsBlack) cout << "You (Black) win! Congraluations!" << endl;
	else if (gameState == Board::UTILITY_BLACK_WINS && !playerIsBlack) cout << "You (White) lost..." << endl;
	else if (gameState == Board::UTILITY_WHITE_WINS && playerIsBlack) cout << "You (Black) lost..." << endl;
	else if (gameState == Board::UTILITY_WHITE_WINS && !playerIsBlack) cout << "You (White) win! Congraluations!" << endl;
	else if (gameState == Board::UTILITY_DRAW) cout << "No one has won.  Draw." << endl;

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

tuple<int, int, int> alphaBetaSearch(shared_ptr<Board> board) {
	vector<tuple<tuple<int, int, int>, int>> actions = board->getValidActions();
	int v = maxValue(move(board), Board::UTILITY_WHITE_WINS, Board::UTILITY_BLACK_WINS, 0);
	/*for (auto it = actions.begin(); it != actions.end(); it++) {
		if (get<1>(*it) == v) return get<0>(*it);
	}*/
	return get<0>(*actions.begin());
}

int maxValue(shared_ptr<Board> board, int alpha, int beta, int depth) {
	cout << depth << endl;
	int utility = board->terminalTest();
	if (utility != Board::NOT_TERMINAL_STATE) {
		return utility;
	}

	//if cutoff test is true, then return eval(board)
	int v = INT_MIN;
	vector<tuple<tuple<int, int, int>, int>> actions = board->getValidActions();
	for (auto it = actions.begin(); it != actions.end(); it++) {
		v = max(v, minValue(move(board->playPieceResult(get<0>(*it))), alpha, beta, depth + 1));
		if (v >= beta) return v;
		alpha = max(alpha, v);
	}
	return v;
}

int minValue(shared_ptr<Board> board, int alpha, int beta, int depth) {
	cout << depth << endl;
	int utility = board->terminalTest();
	if (utility != Board::NOT_TERMINAL_STATE) {
		return utility;
	}

	int v = INT_MAX;
	vector<tuple<tuple<int, int, int>, int>> actions = board->getValidActions();
	for (auto it = actions.begin(); it != actions.end(); it++) {
		v = min(v, maxValue(move(board->playPieceResult(get<0>(*it))), alpha, beta, depth + 1));
		if (v <= alpha) return v;
		beta = min(beta, v);
	}
	return v;
}