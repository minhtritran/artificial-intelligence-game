#pragma once

#include "stdafx.h"

using namespace std;

class Board {
public:
	const static int TILE_EMPTY = 0;
	const static int TILE_WHITE = 1;
	const static int TILE_BLACK = 2;
	const static int DIR_UP = 0;
	const static int DIR_RIGHT = 1;
	const static int DIR_DOWN = 2;
	const static int DIR_LEFT = 3;
	const static int DIR_UP_RIGHT = 4;
	const static int DIR_DOWN_RIGHT = 5;
	const static int DIR_DOWN_LEFT = 6;
	const static int DIR_UP_LEFT = 7;
	const static int NOT_TERMINAL_STATE = -9999;
	const static int UTILITY_WHITE_WINS = -100;
	const static int UTILITY_BLACK_WINS = 100;
	const static int UTILITY_DRAW = 0;
	const static int UTILITY_FILLER_VALUE = -1;
	const static int NUM_ROWS = 5;
	const static int NUM_COLUMNS = 5;
	const static int NUM_DIRECTIONS = 8;

	Board();
	Board(int currentPlayerPieceValue, int data[][NUM_COLUMNS]);
	friend bool operator==(const Board& lhs, const Board& rhs);
	int getTile(const int row, const int col);
	int getCurrentPlayerPieceValue();
	vector<tuple<tuple<int, int, int>, int>> getValidActions();
	void setTile(const int row, const int col, const int value);
	void display();
	unique_ptr<Board> playPieceResult(const tuple<int, int, int>& action);
	int terminalTest();
private:
	int data[NUM_ROWS][NUM_COLUMNS];
	int currentPlayerPieceValue;
	vector<tuple<tuple<int, int, int>, int>> validActions;

	void initializeValidActions();
	int countLine(const tuple<int, int, int>& action);
	tuple<bool, string> checkIfEnemyPieceInWay(const int startRow, const int startCol, const int destRow, const int destCol);
	bool checkContiguousBody(const int pieceValue);
	tuple<int, int> findAnyOneLocationOfPiece(const int pieceValue);
};