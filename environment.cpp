#include "gomoku.h"
GomokuEnvironment::GomokuEnvironment(int dimension_, int chain_len_, GomokuAgent &p1_, GomokuAgent &p2_)
{
	p1 = &p1_;
	p2 = &p2_;
	dimension = dimension_;
	chain_len = chain_len_;
}

void GomokuEnvironment::run()
{
	Point p;
	board.assign(dimension, vector<char>(dimension, '.'));
	int winner = 0; // 1: player1, 2: player2, 0: draw
	int ans;
	while (1) {
		p = p1->my_action();
		if (!move(true, p, winner)) break;
		if (win(winner, true)) break;
		p2->op_action(p);
		show_board();
		p = p2->my_action();
		if (!move(false, p, winner)) break;
		if (win(winner, false)) break;
		p1->op_action(p);
		show_board();
	}
	show_board();
	cout<<winner<<endl;
}

bool GomokuEnvironment::is_terminate(Point p)
{
	return p.x < 0 || p.y < 0 ? true : false;
}

void GomokuEnvironment::show_board()
{
	printf(" ");
	for (int i = 0; i < dimension; ++i) {
		printf(" %2d", i);
	}
	puts("");
	for (int i = 0; i < dimension; ++i) {
		printf("%2d", i);
		for (int j = 0; j < dimension; ++j) {
			printf(" %c ", board[i][j]);
		}
		puts("");
	}
	puts("");
}

bool GomokuEnvironment::is_draw()
{
	for (int i = 0; i < dimension; ++i) {
		for (int j = 0; j < dimension; ++j) {
			if (is_empty(board[i][j])) {
				return false;
			}
		}
	}
	return true;
}

/**
 * make action
 * return value:
 * 0: termination
 * 1: normal move
 */
int GomokuEnvironment::move(bool first, Point &p, int &winner)
{
	if (is_terminate(p)) {
		if (is_draw())
			winner = 0;
		else
			winner = first ? 2 : 1;
		return 0;
	}
	if (!is_empty(board[p.x][p.y])) {
		winner = first ? 2 : 1;
		return 0;
	}
	board[p.x][p.y] = first ? 'O' : 'X';
	return 1;
}

/**
 * judge whether this point is empty
 */
bool GomokuEnvironment::is_empty(char c)
{
	return c == '.' ? true : false;
}

int GomokuEnvironment::win(int &winner, bool first)
{
	int direction[8][2] = {{-1, 0}, {1, 0}, {0, 1}, {0, -1}, {-1, -1}, {1, 1}, {-1, 1}, {1, -1}};
	int cnt = 0;
	for (int i = 0; i < dimension; ++i) {
		for (int j = 0; j < dimension; ++j) {
			for (int k = 0; k < 8; ++k) {
				int x = i, y = j;
				cnt = 0;
				while (cnt < chain_len) {
					if (x < 0 || y < 0 || x >= dimension || y >= dimension || 
						board[x][y] != (first ? 'O' : 'X')) {
						break;	
					}
					++cnt;
					x += direction[k][0];
					y += direction[k][1];
				}
				if (cnt == chain_len) {
					winner = first ? 1 : 2;
					return 1;
				}
			}
		}
	}
	return 0;
}