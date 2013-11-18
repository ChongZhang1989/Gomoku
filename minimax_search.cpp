#include "gomoku.h"

const int GomokuAgent::turn[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

int GomokuAgent::eval(PointMap &pmap)
{
	return 0;
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
}

Point GomokuAgent::self_action()
{
	PointMap pmap;
	for (int i = 0; i < dimension; ++i) {
		for (int j = 0; j < dimension; ++j) {
			if (!is_empty(board[i][j])) continue;
			pmap[Point(i, j)] = first;
			minimax(pmap, MIN, MAX, false, 1);
			pmap.erase(Point(i, j));
		}
	}
}