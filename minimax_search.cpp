#include "gomoku.h"

const int GomokuAgent::direction[4][2] = {{0, 1}, {1, 0}, {1, 1}, {1, -1}};

long long GomokuAgent::eval(PointMap &pmap)
{
	long long value = 0;
	for (PointMap::iterator it = pmap.begin(); it != pmap.end(); ++it) {
		Point p = it->first;
		bool flg = it->second;
		for (int i = 0; i < 4; ++i) {	
			long long sum = 0;
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
				if (board[x][y] != (flg ? 'X' : 'O'))
					break;
			}
			len += max(abs(x - p.x), abs(y - p.y));
			for (; x < dimension && x >= 0 && y < dimension && y >= 0; x += direction[i][0], y += direction[i][1]) {
				PointMap::iterator tmp = pmap.find(Point(x, y));
				if (tmp != pmap.end() && tmp->second != flg) {
					break;
				}
				if (is_empty(board[x][y])) empty_space1++;
				else if (board[x][y] == (flg ? 'X' : 'O')) empty_space1 += 1;
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
				if (board[x][y] != (flg ? 'X' : 'O'))
					break;
			}
			len += max(abs(x - p.x + direction[i][0]), abs(y - p.y + direction[i][1]));
			for (; x < dimension && x >= 0 && y < dimension && y >= 0; x -= direction[i][0], y -= direction[i][1]) {
				PointMap::iterator tmp = pmap.find(Point(x, y));
				if (tmp != pmap.end() && tmp->second != flg) {
					break;
				}
				if (is_empty(board[x][y])) empty_space2++;
				else if (board[x][y] == (flg ? 'X' : 'O')) empty_space2 += 1;
				else break;
			}
			//Not enough space, useless move
			if (len + empty_space1 + empty_space2 < chain_len) {
				continue;
			}
			
			if (len >= chain_len) {
				//sum = (int)(pow(100.0, len));
				return first == flg ? MAX : MIN; //need to be modified
			} else if (empty_space1 && empty_space2) {
				if (len >= chain_len - 1) {
					return first == flg ? MAX / 10 : MIN / 10;
				}
				sum = (long long)(pow(100.0, len));
			} else {
				sum = (long long) (pow(100.0, len)) / 300;
			}
			
			/*
			if (empty_space1 && empty_space2) {
				sum = (long long)(pow(100.0, len));
			} else {
				sum = (long long) (pow(100.0, len)) / 300;
			}
			*/
			sum += empty_space1 + empty_space2;
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
		int x = x0 + direction[i][0], y = y0 + direction[i][1];
		int len = 0;
		for (; x < dimension && x >= 0 && y < dimension && y >= 0; x += direction[i][0], y += direction[i][1]) {
			PointMap::iterator tmp = pmap.find(Point(x, y));
			if (tmp != pmap.end() && tmp->second == flg)
				continue;
			if (board[x][y] != (flg ? 'X' : 'O'))
				break;
		}
		len += max(abs(x - x0), abs(y - y0));
		x = x0 - direction[i][0];
		y = y0 - direction[i][1];
		for (; x < dimension && x >= 0 && y < dimension && y >= 0; x -= direction[i][0], y -= direction[i][1]) {
			PointMap::iterator tmp = pmap.find(Point(x, y));
			if (tmp != pmap.end() && tmp->second == flg)
				continue;
			if (board[x][y] != (flg ? 'X' : 'O'))
				break;
		}
		len += max(abs(x0 - direction[i][0] - x), abs(y0 - direction[i][1] - y));
		if (len >= chain_len) {
			return true;
		}
	}
	return false;
}

bool GomokuAgent::is_prune(PointMap &pmap, int x0, int y0)
{
	int level = 1;
	for (int i = x0 - level; i <= x0 + level; ++i) {
		for (int j = y0 - level; j <= y0 + level; ++j) {
			if (i < 0 || j < 0 || i >= dimension || j >= dimension) continue;
			if (!is_empty(board[i][j])) return false;
			if (pmap.count(Point(i, j))) return false;
		}
	}
	return true;
}

long long GomokuAgent::minimax(PointMap pmap, long long alpha, long long beta, bool max_layer, int level)
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
				if (is_prune(pmap, i, j)) continue;
				pmap[Point(i, j)] = max_layer ? first : !first;
				if (is_terminal_state(pmap, i, j, max_layer ? first : !first)) {
					return max_layer ? MAX : MIN;
				}
				long long ret = minimax(pmap, alpha, beta, !max_layer, level + 1);
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
	int t1 = clock();
	PointMap pmap;
	long long alpha = MIN - 1;
	long long beta = MAX + 1;
	Point p(-1, -1);
	for (int i = 0; i < dimension; ++i) {
		for (int j = 0; j < dimension; ++j) {
			double runtime = (clock() - t1) * 1.0 / CLOCKS_PER_SEC;
			if (time_limit - runtime < threshold)
				return p;
			if (!is_empty(board[i][j])) continue;
			pmap[Point(i, j)] = first;
			if (is_terminal_state(pmap, i, j, first)) {
				return Point(i, j);
			}
			long long ret = minimax(pmap, alpha, beta, false, 1);
			//printf("(%d %d) : %lld\n", i, j, ret);
			if (ret > alpha) {
				alpha = ret;
				p.x = i;
				p.y = j;
			}
			pmap.erase(Point(i, j));
		}
	}
	double runtime = (clock() - t1) * 1.0 / CLOCKS_PER_SEC;
	cout<<"Runtime: "<<runtime<<"s"<<endl;
	cout<<p.x<<" "<<p.y<<endl;
	return p;
}