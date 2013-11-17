#include "gomoku.h"
GomokuAgent::GomokuAgent(int d, int c, int t, int m, bool f)
{
	dimension = d;
	chain_len = c;
	time_limit = t;
	mode = m;
	first = f;
	board.assign(n, vector<char>(n, 0));
}

Point GomokuAgent::random_action()
{
	
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
	return ret;
}