#include "gomoku.h"
int main(int argc, char **argv)
{
	int dimension, chain_len, time_limit, mode;
	bool first;
	//cin>>dimension>>chain_len>>time_limit>>mode;
	cin>>dimension>>chain_len;
	//1: input, 2: random, 3: self
	GomokuAgent p1(dimension, chain_len, 100, 3, true);
	GomokuAgent p2(dimension, chain_len, 100, 3, false);
	GomokuEnvironment env(dimension, chain_len, p1, p2);
	env.run();
	return 0;
}