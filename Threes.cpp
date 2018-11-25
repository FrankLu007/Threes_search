#include <bits/stdc++.h>
class Board
{
public:
	int board[6]; // 6 tile
	int dir; // 4 direction
	bool state; // before : 0 | after : 1
	Board() : board({0 ,0 ,0 ,0 ,0 ,0}), state(1) {}
	int getindex(int next_tile = 0)
	{
		int index = 0;
		if(state) 
		{
			index = 4;
			index += next_tile;
		}
		for(int i = 0 ; i < 6 ; i++)
		{
			index <<= 4;
			index += board[i];
		}
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
		for(int i = 2 ; i < 6 ; i++)
		{
			if(!board[i] || i == 3) continue;
			if(!board[i-1]) {board[i-1] = board[i]; board[i] = 0; move = 1;}
			else if(board[i-1] + board[i] == 3) {board[i-1] = 3; board[i] = 0; move = 1;}
			else if(board[i] == board[i-1] && board[i] > 2) {board[i-1]++; board[i] = 0; move = 1;}
		}
		return move;
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
}
class State
{
public:
	int min, max;
	float avg;
	State() : avg(-1) {}
}state[1<<27]; // 1(before/after) + 2(next_tile) + 4 * 6(tile)
int after_state(Board &board, int dir, int tile, bool bag[3])
{
	Board tmp = board;
	if(state[board.getindex(tile)].avg != -1) return board.getindex(tile);
	tmp.reflect();
	if(state[tmp.getindex(tile)].avg != -1) return tmp.getindex(tile);
	tmp.reverse();
	if(state[tmp.getindex(tile)].avg != -1) return tmp.getindex(tile);
	tmp.reflect();
	if(state[tmp.getindex(tile)].avg != -1) return tmp.getindex(tile);
	State &s = state[board.getindex()];
	float sum  = 0;
	int count = 0, index;
	s.min = 1000000;
	s.max = 0;
	tmp.state = 0;
	if(!dir) int loc[] = {3, 4, 5};
	else if(dir == 1) int loc[] = {0, 3};
	else if(dir == 2) int loc[] = {0, 1, 2};
	else int loc[] = {2, 5};
	for(int i = 0 ; i < (sizeof(loc) >> 2) ; i++)
	{
		if(tmp.board[loc[i]]) continue;
		for(int j = 0 ; j < 3 ; j++)
		{
			if(!bag[j]) continue;
			tmp.board[loc[i]] = j;
			bag[j] = 0;
			index = before_state(tmp, bag);
			s.max = std::max(s.max, state[index].max);
			s.min = std::min(s.min, state[index].min);
			count ++;
			sum += state[index].avg;
			tmp.board[loc[i]] = 0;
			bag[j] = 1;
		}
	}
	s.avg = sum / count;
	return board.getindex();
}
int before_state(Board &board, bool bag[3])
{
	Board tmp = board;
	if(state[board.getindex()].avg != -1) return board.getindex();
	tmp.reflect();
	if(state[tmp.getindex()].avg != -1) return tmp.getindex();
	tmp.reverse();
	if(state[tmp.getindex()].avg != -1) return tmp.getindex();
	tmp.reflect();
	if(state[tmp.getindex()].avg != -1) return tmp.getindex();
	tmp = board;

}
int main()
{
	after_state(Board(), 0, {1, 1, 1});
	after_state(Board(), 1, {1, 1, 1});
	after_state(Board(), 2, {1, 1, 1});
	after_state(Board(), 3, {1, 1, 1});
	return 0;
}