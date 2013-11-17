#ifndef gomoku_H_
#define gomoku_H_
#include <iostream>
#include <stdio.h>
#include <vector>

struct Point {
	int x, y;
	Point() {}
	Point(int x_, int y_) : x(x_), y(_y) {}
};

class Environment {
private:
public:
};

/**
 * action of board;
 */
class GomokuAgent {
private:
	int dimension, chain_len, time_limit, mode;
	bool first;
	vector<vector<char> >board;
	Point op_action();
	Point input_action();
	Point random_action();
	Point self_action();
public:
	GomokuAgent(int d, int c, int t, int m, bool f);
	Point my_action();
};

#endif
