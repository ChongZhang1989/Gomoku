#include "gomoku.h"
GomokuAgent::GomokuAgent(int d, int c, int t, int m, bool f)
{
	dimension = d;
	chain_len = c;
	time_limit = t;
	mode = m;
	first = f;
	board.assign(d, vector<char>(d, '.'));
}

/**
 * get action from user input
 */
Point GomokuAgent::input_action()
{
	int x, y;
	cin>>x>>y;
	return Point(x, y);
}

/**
 * random move
 */
Point GomokuAgent::random_action()
{
	bool flg = true;
	for (int i = 0; i < dimension; ++i) {
		for (int j = 0; j < dimension; ++j) {
			if (is_empty(board[i][j])) {
				flg = false;
				break;
			}
		}
	}
	if (flg) return Point(-1, -1);
	int x, y;
	srand(time(NULL));
	x = rand() % dimension;
	y = rand() % dimension;
	while (!is_empty(board[x][y])) {
		x = rand() % dimension;
		y = rand() % dimension;
	}
	return Point(x, y);
}

/**
 * judge whether this point is player1's chess piece
 */
bool GomokuAgent::is_first_player(char c)
{
	return c == 'O' ? true : false;
}

/**
 * judge whether this point is empty
 */
bool GomokuAgent::is_empty(char c)
{
	return c == '.' ? true : false;
}

/**
 * perform my move
 */
void GomokuAgent::my_move(Point &move)
{
	board[move.x][move.y] = first ? 'O' : 'X';
}

/**
 * perform op move
 */
void GomokuAgent::op_move(Point &move)
{
	board[move.x][move.y] = first ? 'X' : 'O';
}

Point GomokuAgent::my_action()
{
	Point ret;
	switch(mode) {
		case 1:
			ret = input_action();
			break;
		case 2:
			ret = random_action();
			break;
		case 3:
			ret = self_action();
			break;
	}
	my_move(ret);
	return ret;
}

void GomokuAgent::op_action(Point &p)
{
	op_move(p);
}