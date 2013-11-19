#include "gomoku.h"

const int GomokuAgent::direction[4][2] = {{0, 1}, {1, 0}, {1, 1}, {1, -1}};

/*
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
				//int t = empty_side * ((int)(pow(10, my_pieces)) * my_consecutive - ((int)(pow(10, op_pieces))) * op_consecutive);
				int t = empty_side * ((int)(pow(10, my_consecutive)) - ((int)(pow(10, op_consecutive))));
				value += t;
			}
		}
	}
	return value;
}
*/

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
				//sum = (int)(pow(10, len));
				return first == flg ? MAX : MIN;
			} else if (empty_space1 && empty_space2) {
				if (len >= chain_len - 2 && first != flg)
					return MIN;
				sum = (int)(pow(10, len));
			} else {
				sum = (int) (pow(10, len)) / 30;
			}
			sum += empty_space1 * empty_space2;
			value += first == flg ? sum : -sum;
			/*
			if (len == chain_len) {
				value += first == flg ? 100000000 : -100000000;
			}
			if (empty_space1 && empty_space2 && len == chain_len - 1) {
				value += first == flg ? 10000000 : -10000000;
			}
			if (empty_space1 && empty_space2 && len == chain_len - 2) {
				value += first == flg ? 1000000 : -1000000;
			}
			sum += ((int)pow(10, len))* (int)(log(empty_space1 * empty_space2 + 1) * 10) / dup;
			value += first == flg ? sum : -sum;
			*/
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
		PointMap rec = pmap;
		for (int i = 0; i < dimension; ++i) {
			for (int j = 0; j < dimension; ++j) {
				if (board[i][j] == 'O') {
					rec[Point(i, j)] = true;
				} else if (board[i][j] == 'X'){
					rec[Point(i, j)] = false;
				}
			}
		}
		return eval(rec);
	}
	for (int i = 0; i < dimension; ++i) {
		for (int j = 0; j < dimension; ++j) {
			if (!visited(pmap, i, j)) {
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