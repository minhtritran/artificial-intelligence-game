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
	const static int NUM_ROWS = 5;
	const static int NUM_COLUMNS = 5;

	Board();
	int getTile(const int row, const int col);
	void setTile(const int row, const int col, const int value);
	void display();
	bool playPiece(const tuple<int, int, int>& action, const bool playAsBlack);
private:
	int data[NUM_ROWS][NUM_COLUMNS];

	int countLine(const tuple<int, int, int>& action);
	tuple<bool, string> checkIfEnemyPieceInWay(const int startRow, const int startCol, const int destRow, const int destCol, const int pieceValue);
};