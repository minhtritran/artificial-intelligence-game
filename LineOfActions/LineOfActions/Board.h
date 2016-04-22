#pragma once

#include "stdafx.h"

using namespace std;

class Board {
private:
	const static int NUM_ROWS = 5;
	const static int NUM_COLUMNS = 5;
	int data[NUM_ROWS][NUM_COLUMNS]; //empty: 0, white piece: 1, black piece: 2 

public:
	Board();
	int getTile(const int row, const int col);
	void setTile(const int row, const int col, const int value);
	void display();
	bool playPiece(tuple<int, int, int> action);
};