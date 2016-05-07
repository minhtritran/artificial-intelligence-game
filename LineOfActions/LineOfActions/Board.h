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
	const static int UTILITY_MIN = -100;
	const static int UTILITY_MAX = 100;
	const static int UTILITY_DRAW = 0;
	const static int NUM_DIRECTIONS = 8;
	static bool playerIsBlack;
	Board(int boardSize);
	Board(int boardSize, int currentPlayerPieceValue, int** data);
	~Board();
	friend bool operator==(const Board& lhs, const Board& rhs);
	int getTile(const int row, const int col);
	int getNumRows();
	int getNumColumns();
	int getCurrentPlayerPieceValue();
	vector<tuple<int, int, int>> getValidActions();
	void setTile(const int row, const int col, const int value);
	void display();
	unique_ptr<Board> playPieceResult(const tuple<int, int, int>& action);
	int terminalTest();
	int evaluate();
private:
	int numRows;
	int numColumns;
	int** data;
	//int data[numRows][numColumns];
	int currentPlayerPieceValue;
	vector<tuple<int, int, int>> validActions;

	void initializeValidActions();
	int countLine(const tuple<int, int, int>& action);
	tuple<bool, string> checkIfEnemyPieceInWay(const int startRow, const int startCol, const int destRow, const int destCol);
	bool checkContiguousBody(const int pieceValue);
	vector<tuple<int, int>> bfsExplore(tuple<int, int> startTile, int pieceValue);
	tuple<int, int> findAnyOneLocationOfPiece(const int pieceValue);
};