/*
	Written by Thomas Upfold for fun on the 1st of June
	Last Updated: 2/6/12
	Notes: 
		-Could potential add polygon functionality using adjacncy lists
		-carveMaze could potential be better
		-the 4 bools in the tile struct could potential be changed with an array and #defines for the directions
		Currently, the 4 bools makes it easier to understand, but for 6+ sided tiles it might be tedious
*/

#include <cstdlib>
#include <ncurses/ncurses.h>
#include <algorithm>
using namespace std;

struct tile {
	bool up;
	bool down;
	bool left;
	bool right;
	bool visited;
};

void write(char s, int x, int y);
void write(int s, int y, int x); 
void write(string s, int y, int x);
void printMaze(int size);
void carveMaze(int x, int y, int size);
void printTile(int x, int y);
void initMaze(int size);
void clearMaze(int size);
void shuffle(char* dir, int size);

//int size = 15;
tile* maze[100][100];

int main(int argc, char *argv[]) {
	/* Basic args stuff */
	int size = 10;
	if (argc > 1) {
		size = atoi(argv[1]);
		if (size > 24) {
			size = 24;
			write("Size has been restricted", 0,50);
			getch();
		}
	}
	/* initialisation functions*/
	initMaze(size);
	initscr();
	noecho();	
	initMaze(size);
	
	/*Maze is printed here to keep the printTile function only printing spaces*/
	printMaze(size);
	carveMaze(0,0,size);
	
	/*memory clears*/
	clearMaze(size);
	endwin();
	
	return 0;
}
//Sets each array location in the maze
void initMaze(int size) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			maze[j][i] = new tile;
			maze[j][i]->up = true;
			maze[j][i]->down = true;
			maze[j][i]->left = true;
			maze[j][i]->right = true;
			maze[j][i]->visited = false;
		}
	}
}
//Frees pointers using in maze
void clearMaze(int size) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			delete maze[j][i];
		}
	}
}
//Basic bounts check function
bool inBounds(int x, int y, char dir, int size) {
	switch(dir) {
		case 'U':
			return y > 0;
		case 'D':
			return y+1 < size;
		case 'L':
			return x > 0;
		case 'R':
			return x+1 < size;
	}
	return false;
}
/*This function takes location as parameters, picks a direction at random and calls 
	this function on that new location, when it returns, it sees if it can go in any more directions.
	See this page for a better explaination: 
	http://weblog.jamisbuck.org/2011/2/7/maze-generation-algorithm-recap */	
void carveMaze(int x, int y, int size) {
	char dir[] = {'U','D','L','R'};
	std::random_shuffle(dir, dir+4);
	tile *curTile = maze[x][y];
	curTile->visited = true;
	printTile(x,y);
	for (int i = 0; i < 4; i++) {
		if (inBounds(x,y,dir[i],size)) {
			switch(dir[i]) {
				case 'U': 
					if(maze[x][y-1]->visited == false) {
						curTile->up = false;
						maze[x][y-1]->down = false;
						carveMaze(x,y-1,size);
					}
					break;
				case 'D':
					if(maze[x][y+1]->visited == false) {
						curTile->down = false;
						maze[x][y+1]->up = false;
						carveMaze(x,y+1,size);
					}
					break;
				case 'L':
					if(maze[x-1][y]->visited == false) {
						curTile->left = false;
						maze[x-1][y]->right = false;
						carveMaze(x-1,y,size);
					}
					break;
				case 'R':
					if(maze[x+1][y]->visited == false) {
						curTile->right = false;
						maze[x+1][y]->left = false;
						carveMaze(x+1,y,size);
					}
					break;
			}
		}
	}
}
//Used in conjunction with carveTile to 'animate' the carve process. It updates surrounding tiles
void printTile(int x, int y) {
	tile *cur = maze[x][y];
	int offsetX = 1 + (2*x); 
	if(!cur->up) 		write(' ', y, offsetX);
	if(!cur->down)	write(' ', y+1, offsetX);
	if(!cur->right)  	write(' ', y+1, offsetX+1);
	if(!cur->left)		write(' ', y+1, offsetX-1);
	move(y+1,offsetX);
	getch();
}
void printMaze(int size) {
	int curX = 1, curY = 1;
	for (int i = 0; i < size; i++) {
		curY = 1;
		for (int j = 0; j < size; j++) {
			tile *cur = maze[i][j];
			if(cur->up) 		write('_', curY-1, curX); else write(' ', curY-1, curX);
			if(cur->down) 	write('_',curY, curX); else write(' ',curY, curX);
			if(cur->right)  	write('|',curY,curX+1); else write(' ',curY,curX+1);
			if(cur->left) 		write('|', curY,curX-1);else write(' ', curY,curX-1);
			curY++;
		}
		curX+=2;
	}
	refresh();
}
//Some additional functions I use with ncurses
void write(char s, int y, int x) {
	move(y, x);
	addch(s);
}
void write(string s, int y, int x) {
	move(y,x);
	for (int i = 0; i < s.length();i++) {
		addch(s[i]);
	}
}	

	
	
	
	
