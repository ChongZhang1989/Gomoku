#ifndef Gomoku_H_
#define Gomoku_H_
#include <iostream>
#include <stdio.h>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <map>
#include <math.h>
#include <tr1/unordered_map>
using namespace std;

struct Point {
	int x, y;
	Point() {}
	Point(int x_, int y_) : x(x_), y(y_) {}
	bool operator < (const Point &b) const
	{
		return x == b.x ? y < b.y : x < b.x;
	}
};

/**
 * GomokuAgent
 * 1st Player : O
 * 2nd Player : X
 */
class GomokuAgent {
private:
	class PointHash {
	public:
		size_t operator () (const Point &a) const
		{
			return (a.x << 1) ^ (a.y);
		}
	};
	class PointEqual {
	public:
		bool operator () (const Point &a, const Point &b) const
		{
			return a.x == b.x && a.y == b.y;
		}
	};
	typedef tr1::unordered_map<Point, int, PointHash, PointEqual> PointMap;
	//typedef map<Point, int> PointMap;
	const static int MAX = 0x7fffffff;
	const static int MIN = -0x7fffffff;
	const static int max_level = 3;
	const static int direction[4][2];
	int dimension, chain_len, time_limit, mode;
	bool first;
	PointMap rec;
	vector<vector<char> >board;
	Point input_action();
	Point random_action();
	Point self_action();
	bool is_first_player(char c);
	bool is_empty(char c);
	void my_move(Point &move);
	void op_move(Point &move);
	bool visited(PointMap &pmap, int x, int y);
	int minimax(PointMap pmap, int alpha, int beta, bool max_layer, int level);
	int eval(PointMap &pmap);
	bool is_terminal_state(PointMap &pmap, int x0, int y0, bool flg);
public:
	GomokuAgent(int d, int c, int t, int m, bool f);
	Point my_action();
	void op_action(Point &p);
};

class GomokuEnvironment {
private:
	GomokuAgent *p1, *p2;
	int dimension;
	int chain_len;
	vector<vector<char> >board;
	bool is_terminate(Point p);
	bool is_draw();
	bool is_empty(char c);
	int move(bool first, Point &p, int &winner);
	int win(int &winner, bool first);
	void show_board();
public:
	GomokuEnvironment(int dimension, int chain_len, GomokuAgent &p1_, GomokuAgent &p2_);
	void run();
};

#endif
