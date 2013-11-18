#include "gomoku.h"

const int GomokuAgent::turn[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

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
				while (cnt < chain_len) {
					if (x < 0 || y < 0 || x >= dimension || y >= dimension) {
						break;
					}
					if (is_empty(board[x][y]))
						++empty;
					else if (board[x][y] == (first ? 'O' : 'X'))
						++my_pieces;
					else
						++op_pieces;
					s[cnt] = board[x][y];
					++cnt;
					x += direction[k][0];
					y += direction[k][1];
				}
				// int my_consecutive = 0;
// 				int op_consecutive = 0;
// 				for (int l = 0; l < chain_len + 1; ++l) {
// 					
// 				}
				if (my_pieces == 5) value += FIVE;
				if (my_pieces == 4) value += FOUR;
				if (my_pieces == 3) value += THREE;
				if (my_pieces == 2) value += TWO;
				if (my_pieces == 1) value += ONE;
				
				if (op_pieces == 5) value -= FIVE;
				if (op_pieces == 4) value -= FOUR;
				if (op_pieces == 3) value -= THREE;
				if (op_pieces == 2) value -= TWO;
				if (op_pieces == 1) value -= ONE;
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
				int ret = minimax(pmap, alpha, beta, !max_layer, level + 1);
				if (ret > beta || ret < alpha) return ret;
				else tmp = max_layer ? max(tmp, ret) : min(tmp, ret);
				pmap.erase(Point(i, j));
			}
		}
	}
	return tmp;
	/*
	int startx = (dimension - 1) / 2;
	int starty = (dimension - 1) / 2;
	int x, y;
	int len = dimension % 2 == 0 ? 2 : 1;
	for (; len <= dimension; len += 2) {
		x = --startx;
		y = --starty;
		int d = 0;
		for (int i = 0; i < len; ++i) {
			if (!visited(pmap, x, y)) {
				pmap[Point(x, y)] = first;
				int ret = minimax(pmap, alpha, beta, !max_layer, level + 1);
				if (ret > beta || ret < alpha) return ret;
				else tmp = max_layer ? max(tmp, ret) : min(tmp, ret);
				pmap.erase(Point(x, y));
			}
			x += turn[d][0];
			y += turn[d][1];
		}
		d = 1;
		for (int i = 0; i < len - 1; ++i) {
			x += turn[d][0];
			y += turn[d][1];
			if (!visited(pmap, x, y)) {
				pmap[Point(x, y)] = first;
				int ret = minimax(pmap, alpha, beta, !max_layer, level + 1);
				if (ret > beta || ret < alpha) return ret;
				else tmp = max_layer ? max(tmp, ret) : min(tmp, ret);
				pmap.erase(Point(x, y));
			}
		}
		d = 2;
		for (int i = 0; i < len - 1; ++i) {
			x += turn[d][0];
			y += turn[d][1];
			if (!visited(pmap, x, y)) {
				pmap[Point(x, y)] = first;
				int ret = minimax(pmap, alpha, beta, !max_layer, level + 1);
				if (ret > beta || ret < alpha) return ret;
				else tmp = max_layer ? max(tmp, ret) : min(tmp, ret);
				pmap.erase(Point(x, y));
			}
		}
		d = 3;
		for (int i = 0; i < len - 2; ++i) {
			x += turn[d][0];
			y += turn[d][1];
			if (!visited(pmap, x, y)) {
				pmap[Point(x, y)] = first;
				int ret = minimax(pmap, alpha, beta, !max_layer, level + 1);
				if (ret > beta || ret < alpha) return ret;
				else tmp = max_layer ? max(tmp, ret) : min(tmp, ret);
				pmap.erase(Point(x, y));
			}
		}
	}
	return tmp;
	*/
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
			if (ret < alpha) {
				break;
			} else {
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