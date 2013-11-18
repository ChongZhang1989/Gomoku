#include "gomoku.h"

const int GomokuAgent::direction[4][2] = {{0, 1}, {1, 0}, {1, 1}, {1, -1}};

int GomokuAgent::eval(PointMap &pmap)
{
	vector<char>s(chain_len + 1);
	int value = 0;
	int cnt = 0;
	for (int i = 0; i < dimension; ++i) {
		for (int j = 0; j < dimension; ++j) {
			for (int k = 0; k < 4; ++k) {
				int x = i, y = j;
				cnt = 0;
				int my_pieces = 0;
				int op_pieces = 0;
				int empty = 0;
				int flg = 0;
				while (cnt < chain_len + 1) {
					if (x < 0 || y < 0 || x >= dimension || y >= dimension) {
						flg = 1;
						break;
					}
					if (pmap.count(Point(x, y))) {
						if (pmap[Point(x, y)] == first) {
							++my_pieces;
							s[cnt] = first ? 'O' : 'X';
						} else {
							++op_pieces;
							s[cnt] = first ? 'X' : 'O';
						}
					}
					else if (is_empty(board[x][y])) {
						s[cnt] = board[x][y];
						++empty;
					}
					else if (board[x][y] == (first ? 'O' : 'X')) {
						s[cnt] = board[x][y];
						++my_pieces;
					}
					else {
						s[cnt] = board[x][y];
						++op_pieces;
					}
					++cnt;
					x += direction[k][0];
					y += direction[k][1];
				}
				if (flg) continue;
				int my_consecutive = 0;
				int op_consecutive = 0;
				int sum = 0;
				for (int l = 0; l < chain_len + 1; ++l) {
					if (s[l] == (first ? 'O' : 'X'))
						sum++;
					else
						sum = 0;
					my_consecutive = max(sum, my_consecutive);
				}
				sum = 0;
				for (int l = 0; l < chain_len + 1; ++l) {
					if (s[l] == (first ? 'X' : 'O'))
						sum++;
					else
						sum = 0;
					op_consecutive = max(sum, op_consecutive);
				}
				int empty_side = 1;
				if (is_empty(s[0])) empty_side++;
				if (is_empty(s[chain_len])) empty_side++;
				value += empty_side * ((int)(pow(10, my_pieces)) * my_consecutive - ((int)(pow(10, op_pieces))) * op_consecutive);
			}
		}
	}
	return value;
}

bool GomokuAgent::visited(PointMap &pmap, int x, int y)
{
	if (!is_empty(board[x][y])) return true;
	if (pmap.count(Point(x, y))) return true;
	return false;
}

int GomokuAgent::minimax(PointMap pmap, int alpha, int beta, bool max_layer, int level)
{
	if (level == max_level) {
		return eval(pmap);
	}
	int tmp = max_layer ? MIN : MAX;
	for (int i = 0; i < dimension; ++i) {
		for (int j = 0; j < dimension; ++j) {
			if (!visited(pmap, i, j)) {
				pmap[Point(i, j)] = max_layer ? first : !first;
				int ret = minimax(pmap, alpha, beta, !max_layer, level + 1);
				if (max_layer) {
					alpha = max(ret, alpha);
				} else {
					beta = min(beta, ret);
					if (alpha > beta) return beta;
				}
				pmap.erase(Point(i, j));
			}
		}
	}
	return max_layer ? alpha : beta;
}

Point GomokuAgent::self_action()
{
	PointMap pmap;
	int alpha = MIN;
	int beta = MAX;
	Point p(-1, -1);
	for (int i = 0; i < dimension; ++i) {
		for (int j = 0; j < dimension; ++j) {
			if (!is_empty(board[i][j])) continue;
			pmap[Point(i, j)] = first;
			int ret = minimax(pmap, alpha, beta, false, 1);
			printf("(%d %d) : %d\n", i, j, ret);
			if (ret > alpha) {
				alpha = ret;
				p.x = i;
				p.y = j;
			}
			pmap.erase(Point(i, j));
		}
	}
	cout<<p.x<<" "<<p.y<<endl;
	return p;
}