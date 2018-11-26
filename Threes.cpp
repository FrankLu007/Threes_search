#include <cstdio>
#include <unordered_map>
#include <vector>
#include <cmath>
class Board
{
public:
	int board[6];
	Board() : board({0 ,0 ,0 ,0 ,0 ,0}) {}
	int getindex(const int next_tile, const int dir = 4)
	{
		int index = dir;
		for(int i = 0 ; i < 6 ; i++)
		{
			index <<= 4;
			index += board[i];
		}
		index <<= 2;
		index += next_tile;
		return index;
	}
	bool slide_up()
	{
		bool move = 0;
		for(int i = 3 ; i < 6 ; i++)
		{
			if(!board[i]) continue;
			if(!board[i-3]) {board[i-3] = board[i]; board[i] = 0; move = 1;}
			else if(board[i-3] + board[i] == 3) {board[i-3] = 3; board[i] = 0; move = 1;}
			else if(board[i] == board[i-3] && board[i] > 2) {board[i-3]++; board[i] = 0; move = 1;}
		}
		return move;
	}
	bool slide_left()
	{
		bool move = 0;
		for(int i = 1 ; i < 6 ; i++)
		{
			if(!board[i] || i == 3) continue;
			if(!board[i-1]) {board[i-1] = board[i]; board[i] = 0; move = 1;}
			else if(board[i-1] + board[i] == 3) {board[i-1] = 3; board[i] = 0; move = 1;}
			else if(board[i] == board[i-1] && board[i] > 2) {board[i-1]++; board[i] = 0; move = 1;}
		}
		return move;
	}
	bool slide(int dir)
	{
		bool move;
		if(!dir) return slide_up();
		if(dir == 1)
		{
			reflect();
			move = slide_left();
			reflect();
			return move;
		}
		if(dir == 2)
		{
			reverse();
			move = slide_up();
			reverse();
			return move;
		}
		return slide_left();
	}
	void reflect()
	{
		std::swap(board[0], board[2]);
		std::swap(board[3], board[5]);
	}
	void reverse()
	{
		std::swap(board[0], board[3]);
		std::swap(board[1], board[4]);
		std::swap(board[2], board[5]);
	}
};
class State
{
public:
	int min, max;
	float avg;
	State() {}
	State(int a, float b, int c) : min(a), avg(b), max(c) {}
};
std::unordered_map <int, State> state;
void P(Board board)
{
	printf("%d %d %d\n%d %d %d\n\n", board.board[0], board.board[1], board.board[2], board.board[3], board.board[4], board.board[5]);
}
int before_state(Board &board, bool bag[3], const int next_tile);
int test(Board board, const int next_tile , int dir = 4)
{
	if(state.find(board.getindex(next_tile, dir)) != state.end()) return board.getindex(next_tile, dir);
	board.reflect();
	if(dir == 1 || dir == 3) dir = 4 - dir;
	if(state.find(board.getindex(next_tile, dir)) != state.end()) return board.getindex(next_tile, dir);
	board.reverse();
	if(!dir || dir == 2) dir = 2 - dir;
	if(state.find(board.getindex(next_tile, dir)) != state.end()) return board.getindex(next_tile, dir);
	board.reflect();
	if(dir == 1 || dir == 3) dir = 4 - dir;
	if(state.find(board.getindex(next_tile, dir)) != state.end()) return board.getindex(next_tile, dir);
	return -1;
}
int after_state(Board &board, const int dir, const int next_tile, bool bag[3])
{
	//printf("a\n");
	//P(board);
	int index = test(board, next_tile, dir);
	if(index != -1) return index;
	Board tmp = board;
	int count = 0, min, max;
	float sum = 0;
	std::vector <int> v;
	min = 1000000;
	max = 0;
	v.clear();
	if(!dir) {v.push_back(3);v.push_back(4);v.push_back(5);}
	else if(dir == 1) {v.push_back(0);v.push_back(3);}
	else if(dir == 2) {v.push_back(0);v.push_back(1);v.push_back(2);}
	else {v.push_back(2);v.push_back(5);}
	bag[next_tile-1] = 0;
	for(int i = 0 ; i < v.size() ; i++)
	{
		if(tmp.board[v[i]]) continue;
		tmp.board[v[i]] = next_tile;
		if(!bag[0] && !bag[1] && !bag[2]) bag[0] = bag[1] = bag[2] = 1;
		for(int j = 0 ; j < 3 ; j++)
		{
			if(!bag[j]) continue;
			index = before_state(tmp, bag, j+1);
			max = std::max(max, state[index].max);
			min = std::min(min, state[index].min);
			sum += state[index].avg;
			count ++;
		}
		if(bag[0] && bag[1] && bag[2]) bag[0] = bag[1] = bag[2] = 0;
		tmp.board[v[i]] = 0;
	}
	bag[next_tile-1] = 1;
	index = board.getindex(next_tile, dir);
	state[index] = State(min, sum / count, max);
	return index;
}
int before_state(Board &board, bool bag[3], const int next_tile)
{
	//printf("b\n");
	//P(board);
	int index = test(board, next_tile), best = -1;
	if(index != -1) return index;
	Board tmp;
	float avg = 0;
	int count = 0, min, max;
	for(int i = 0 ; i < 4 ; i++)
	{
		tmp = board;
		if(!tmp.slide(i)) continue;
		index = after_state(tmp, i, next_tile, bag);
		if(best == -1 || state[index].avg > state[best].avg) best = index;
		count ++;
	}
	if(!count)
	{
		for(int i = 0 ; i < 6 ; i++) if(board.board[i] > 2) count += pow(3, board.board[i]-2);
		avg = max = min = count;
	}
	else
	{
		min = state[best].min;
		max = state[best].max;
		avg = state[best].avg;
	}
	index = board.getindex(next_tile);
	state[index] = State(min, avg, max); 
	return index;
}
int main()
{
	Board board;
	state.clear();
	bool bag[3] = {1, 1, 1};
	char c;
	int next_tile, index;
	for(int i = 0 ; i < 6 ; i++)
	{
		for(int j = 0 ; j < 3 ; j++)
		{
			board.board[i] = j + 1;
			bag[j] = 0;
			for(int k = 0 ; k < 3 ; k++) if(bag[k]) before_state(board, bag, k+1);
			bag[j] = 1;
			board.board[i] = 0;
		}
	}
	while(scanf(" %c", &c) != EOF)
	{
		for(int i = 0 ; i < 6 ; i++) scanf("%d", &board.board[i]);
		scanf(" +%d", &next_tile);
		printf("%c ", c);
		for(int i = 0 ; i < 6 ; i++)  printf("%d ", board.board[i]);
		printf("+%d = ", next_tile);
		for(int i = 0 ; i < 6 ; i++)
		{
			if(board.board[i] < 4) continue;
			board.board[i] = log2(board.board[i] / 3) + 3;
		}
		if(c == 'a')
		{
			bool y = 1;
			for(int i = 0 ; i < 4 ; i++)
			{
				index = test(board, next_tile, i);
				if(state.find(index) != state.end())
				{
					printf("%d %f %d\n", state[index].min, state[index].avg, state[index].max);
					y = 0;
					break;
				}
			}
			if(y) printf("-1\n");
		}
		else
		{
			index = test(board, next_tile);
			if(state.find(index) != state.end()) printf("%d %f %d\n", state[index].min, state[index].avg, state[index].max);
			else printf("-1\n");
		}
	}
	return 0;
}