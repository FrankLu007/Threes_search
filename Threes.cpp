#include <bits/stdc++.h>
class Board
{
public:
	int board[6]; // 6 tile
	int dir; // 4 direction
	bool state; // before : 0 | after : 1
	int getindex(int next_tile)
	{
		int index = 0;
		if(state) 
		{
			index = 4;
			index += next_tile;
			index <<= 2;
			index += dir;
		}
		for(int i = 0 ; i < 6 ; i++)
		{
			index <<= 4;
			index += board[i];
		}
	}
}
class State
{
public:
	int min, max;
	float avg;
	State() : avg(-1) {}
}state[1<<29]; // 1(before/after) + 2(next_tile) + 2(direction) + 4 * 6(tile)
void dfs(State *s)
{

}
int main()
{
	dfs()
	return 0;
}