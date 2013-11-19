#include "gomoku.h"

const int GomokuAgent::direction[4][2] = {{0, 1}, {1, 0}, {1, 1}, {1, -1}};

int GomokuAgent::eval(PointMap &pmap)
{
	int value = 0;
	for (PointMap::iterator it = pmap.begin(); it != pmap.end(); ++it) {
		Point p = it->first;
		bool flg = it->second;
		for (int i = 0; i < 4; ++i) {	
			int sum = 0;
			int x = p.x, y = p.y;
			int len = 0;
			int dup = 1;
			int empty_space1 = 0;
			int empty_space2 = 0;
			for (; x < dimension && x >= 0 && y < dimension && y >= 0; x += direction[i][0], y += direction[i][1]) {
				PointMap::iterator tmp = pmap.find(Point(x, y));
				if (tmp != pmap.end() && tmp->second == flg) {
					dup++;
					continue;
				}
				if (board[x][y] != (flg ? 'O' : 'X'))
					break;
			}
			len += max(abs(x - p.x), abs(y - p.y));
			for (; x < dimension && x >= 0 && y < dimension && y >= 0; x += direction[i][0], y += direction[i][1]) {
				PointMap::iterator tmp = pmap.find(Point(x, y));
				if (tmp != pmap.end() && tmp->second != flg) {
					break;
				}
				if (is_empty(board[x][y])) empty_space1++;
				else if (board[x][y] == (flg ? 'O' : 'X')) empty_space1 += 1;
				else break;
			}
			x = p.x - direction[i][0];
			y = p.y - direction[i][1];
			for (; x < dimension && x >= 0 && y < dimension && y >= 0; x -= direction[i][0], y -= direction[i][1]) {
				PointMap::iterator tmp = pmap.find(Point(x, y));
				if (tmp != pmap.end() && tmp->second == flg) {
					dup++;
					continue;
				}
				if (board[x][y] != (flg ? 'O' : 'X'))
					break;
			}
			len += max(abs(x - p.x + direction[i][0]), abs(y - p.y + direction[i][1]));
			for (; x < dimension && x >= 0 && y < dimension && y >= 0; x -= direction[i][0], y -= direction[i][1]) {
				PointMap::iterator tmp = pmap.find(Point(x, y));
				if (tmp != pmap.end() && tmp->second != flg) {
					break;
				}
				if (is_empty(board[x][y])) empty_space2++;
				else if (board[x][y] == (flg ? 'O' : 'X')) empty_space2 += 1;
				else break;
			}
			//Not enough space, useless move
			if (len + empty_space1 + empty_space2 < chain_len) {
				continue;
			}
			
			if (len >= chain_len) {
				sum = (int)(pow(10.0, len));
				//return first == flg ? MAX : MIN; //need to be modified
			} else if (empty_space1 && empty_space2) {
				//if (len >= chain_len - 2 && first != flg)
				//	return MIN;
				sum = (int)(pow(10.0, len));
			} else {
				sum = (int) (pow(10.0, len)) / 30;
			}
			sum += empty_space1 * empty_space2;
			value += first == flg ? sum : -sum;
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

bool GomokuAgent::is_terminal_state(PointMap &pmap, int x0, int y0, bool flg)
{
	for (int i = 0; i < 4; ++i) {
		int x = x0, y = y0;
		int len = 0;
		for (; x < dimension && x >= 0 && y < dimension && y >= 0; x += direction[i][0], y += direction[i][1]) {
			PointMap::iterator tmp = pmap.find(Point(x, y));
			if (tmp != pmap.end() && tmp->second == flg)
				continue;
			if (board[x][y] != (flg ? 'O' : 'X'))
				break;
		}
		len += max(abs(x - x0), abs(y - y0));
		x = x0 - direction[i][0];
		y = y0 - direction[i][1];
		for (; x < dimension && x >= 0 && y < dimension && y >= 0; x -= direction[i][0], y -= direction[i][1]) {
			PointMap::iterator tmp = pmap.find(Point(x, y));
			if (tmp != pmap.end() && tmp->second == flg)
				continue;
			if (board[x][y] != (flg ? 'O' : 'X'))
				break;
		}
		len += max(abs(x - x0 + direction[i][0]), abs(y - y0 + direction[i][1]));
		if (len >= chain_len) return true;
	}
	return false;
}

int GomokuAgent::minimax(PointMap pmap, int alpha, int beta, bool max_layer, int level)
{
	if (level == max_level) {
		for (PointMap::iterator it = pmap.begin(); it != pmap.end(); ++it)
			rec[it->first] = it->second;
		int ans = eval(rec);
		for (PointMap::iterator it = pmap.begin(); it != pmap.end(); ++it)
			rec.erase(it->first);
		return ans;
	}
	for (int i = 0; i < dimension; ++i) {
		for (int j = 0; j < dimension; ++j) {
			if (!visited(pmap, i, j)) {
				if (is_terminal_state(pmap, i, j, max_layer ? first : !first)) {
					return max_layer ? MAX : MIN;
				}
				pmap[Point(i, j)] = max_layer ? first : !first;
				int ret = minimax(pmap, alpha, beta, !max_layer, level + 1);
				if (max_layer) {
					alpha = max(ret, alpha);
					if (alpha >= beta) return alpha;
				} else {
					beta = min(beta, ret);
					if (alpha >= beta) return beta;
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
	int alpha = MIN - 1;
	int beta = MAX;
	Point p(-1, -1);
	for (int i = 0; i < dimension; ++i) {
		for (int j = 0; j < dimension; ++j) {
			if (!is_empty(board[i][j])) continue;
			pmap[Point(i, j)] = first;
			int ret = minimax(pmap, alpha, beta, false, 1);
			//printf("(%d %d) : %d\n", i, j, ret);
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