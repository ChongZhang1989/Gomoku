#include "gomoku.h"
int main(int argc, char **argv)
{
	int dimension, chain_len, time_limit, mode;
	bool first;
	//cin>>dimension>>chain_len>>time_limit>>mode;
	cin>>dimension>>chain_len;
	GomokuAgent p1(dimension, chain_len, 0, 2, true);
	GomokuAgent p2(dimension, chain_len, 0, 1, false);
	GomokuEnvironment env(dimension, chain_len, p1, p2);
	env.run();
	return 0;
}