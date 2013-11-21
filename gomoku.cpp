#include "gomoku.h"
int main(int argc, char **argv)
{
	int dimension, chain_len, mode;
	double time_limit;
	bool first;
	puts("Please input n:");
	cin>>dimension;
	puts("Please input m:");
	cin>>chain_len;
	puts("Please input time limit:");
	cin>>time_limit;
	puts("Please input mode:(1: input, 2: random, 3: self)");
	cin>>mode;
	puts("Please input who goes first: (1: myself, 0: opponent)");
	cin>>first;
	switch(mode) {
		case 1: {
			GomokuAgent p1(dimension, chain_len, time_limit, 3, first);
			GomokuAgent p2(dimension, chain_len, time_limit, 1, !first);
			GomokuEnvironment env(dimension, chain_len, first ? p1 : p2, first ? p2 : p1);
			env.run();
			break;
		}
		case 2: {
			GomokuAgent p1(dimension, chain_len, time_limit, 3, first);
			GomokuAgent p2(dimension, chain_len, time_limit, 2, !first);
			GomokuEnvironment env(dimension, chain_len, first ? p1 : p2, first ? p2 : p1);
			env.run();
			break;
		}
		case 3: {
			GomokuAgent p1(dimension, chain_len, time_limit, 3, first);
			GomokuAgent p2(dimension, chain_len, time_limit, 3, !first);
			GomokuEnvironment env(dimension, chain_len, first ? p1 : p2, first ? p2 : p1);
			env.run();
			break;
		}
	}
	return 0;
}