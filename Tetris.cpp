#include "Tetris.hpp"
#include <chrono>
#include <thread>
#include <iostream>
#include <fstream>

struct Grid
{
private:
	std::vector<Tetromino> que = { SQUARE, LEFTL, RIGHTL, LEFTS, RIGHTS, TSHAPE };

	TetrominoState state;

	const int columns;
	const int rows;

	std::vector<States> grid;

	int tetromino_x = 5;
	int tetromino_y = 21;

	Tetromino piece = STRAIGHT;
	Rotation rot = RIGHT;
	std::vector<std::pair<int, int>> Block;

	const int index(const int x, const int y)
	{
		return (y * columns) + x;
	}

	bool clear_row(int row)
	{
		for (int i = 0; i < columns; i++)
		{
			if (grid[index(i, row)] == EMPTY || grid[index(i, row)] == TETROMINO)
			{
				return false;
			}
		}
		for (int i = row; i < rows; i++)
		{
			for (int j = 0; j < columns; j++)
			{
				if (i + 1 >= rows)
				{
					grid[index(j, i)] = EMPTY;

				}
				else
				{
					grid[index(j, i)] = grid[index(j, i + 1)];
				}
			}
		}
		score += 10;
		return true;
	}

	const void print()
	{
		for (int i = rows - 4; i >= 0; i--)
		{
			for (int j = 0; j < columns; j++)
			{
				if (grid[index(j, i)] == EMPTY)
				{
					std::cout << " ";
				}
				else if (grid[index(j, i)] == FILLED)
				{
					std::cout << "#";

				}
				else if (grid[index(j, i)] == TETROMINO)
				{
					std::cout << "@";

				}
			}
			std::cout << "|";
			std::cout << "\n";
		}
		std::cout << "-----------\n";
		std::cout << score;
	}

	bool DetectCollision()
	{
		for (int i = 0; i < Block.size(); i++)
		{
			int dx = Block[i].first;
			int dy = Block[i].second;
			if (grid[index(tetromino_x + dx, tetromino_y + dy - 1)] == FILLED)
			{
				return true;
			}
		}
		return false;
	}

	bool DetectRight()
	{
		for (int i = 0; i < Block.size(); i++)
		{
			int dx = Block[i].first;
			if (tetromino_x + dx < columns - 1 && grid[index(tetromino_x + dx + 1, tetromino_y)] != FILLED)
			{
				continue;
			}
			else
			{
				return false;
			}
		}
		return true;
	}

	bool DetectLeft()
	{
		for (int i = 0; i < Block.size(); i++)
		{
			int dx = Block[i].first;
			if (tetromino_x + dx > 0 && grid[index(tetromino_x + dx - 1, tetromino_y)] != FILLED)
			{
				continue;
			}
			else
			{
				return false;
			}
		}
		return true;
	}

	bool ValidLeftRotate()
	{
		std::vector<std::pair<int, int>> WishBlock;

		Rotation wishRot;
		switch (rot)
		{
		case RIGHT:
			wishRot = UP;
			break;
		case UP:
			wishRot = LEFT;
			break;
		case LEFT:
			wishRot = DOWN;
			break;
		case DOWN:
			wishRot = RIGHT;
			break;
		default:
			break;
		}

		std::pair<Tetromino, Rotation> wishkey = std::make_pair(piece, wishRot);

		if (state.StateToCoords.find(wishkey) != state.StateToCoords.end())
		{
			WishBlock = state.StateToCoords[wishkey];
		}

		for (int i = 0; i < WishBlock.size(); i++)
		{
			int dx = WishBlock[i].first;
			int dy = WishBlock[i].second;
			if (tetromino_x + dx < 0 || tetromino_x + dx >= columns || grid[index(tetromino_x + dx, tetromino_y + dy)] == FILLED)
			{
				return false;
			}
		}

		return true;
	}

	bool ValidRightRotate()
	{
		std::vector<std::pair<int, int>> WishBlock;

		Rotation wishRot;
		switch (rot)
		{
		case RIGHT:
			wishRot = DOWN;
			break;
		case UP:
			wishRot = RIGHT;
			break;
		case LEFT:
			wishRot = UP;
			break;
		case DOWN:
			wishRot = LEFT;
			break;
		default:
			break;
		}

		std::pair<Tetromino, Rotation> wishkey = std::make_pair(piece, wishRot);

		if (state.StateToCoords.find(wishkey) != state.StateToCoords.end())
		{
			WishBlock = state.StateToCoords[wishkey];
		}

		for (int i = 0; i < WishBlock.size(); i++)
		{
			int dx = WishBlock[i].first;
			int dy = WishBlock[i].second;
			if (tetromino_x + dx < 0 || tetromino_x + dx >= columns || grid[index(tetromino_x + dx, tetromino_y + dy)] == FILLED)
			{
				return false;
			}
		}

		return true;

	}


	void tetromino()
	{
		std::pair<Tetromino, Rotation> key = std::make_pair(piece, rot);

		if (state.StateToCoords.find(key) != state.StateToCoords.end())
		{
			Block = state.StateToCoords[key];
		}

		static int timer = 0;
		char input = getNonBlockingInput();
		if (input == 'a' || input == 'A')
		{
			if (DetectLeft())
			{
				tetromino_x--;
			}
		}
		else if (input == 'd' || input == 'D')
		{
			if (DetectRight())
			{
				tetromino_x++;
			}
		}
		else if (input == 's' || input == 'S')
		{
			if (tetromino_y > 1 && grid[index(tetromino_x, tetromino_y - 2)] != FILLED)
			{
				tetromino_y--;
			}
		}
		else if (input == 'q' || input == 'Q')
		{
			if (ValidLeftRotate())
			{
				switch (rot)
				{
				case RIGHT:
					rot = UP;
					break;
				case UP:
					rot = LEFT;
					break;
				case LEFT:
					rot = DOWN;
					break;
				case DOWN:
					rot = RIGHT;
					break;
				default:
					break;
				}
			}
		}
		else if (input == 'e' || input == 'E')
		{
			if (ValidRightRotate())
			{
				switch (rot)
				{
				case RIGHT:
					rot = DOWN;
					break;
				case UP:
					rot = RIGHT;
					break;
				case LEFT:
					rot = UP;
					break;
				case DOWN:
					rot = LEFT;
					break;
				default:
					break;
				}
			}
		}
		for (int i = rows - 1; i >= 0; i--)
		{
			for (int j = 0; j < columns; j++)
			{
				if (grid[index(j, i)] == TETROMINO)
				{
					grid[index(j, i)] = EMPTY;
				}
			}
		}

		if (tetromino_y > 0 && timer > 20)
		{
			tetromino_y--;
			timer = 0;
		}
		if (tetromino_y <= 0 || DetectCollision())
		{
			for (int i = 0; i < Block.size(); i++)
			{
				int dx = Block[i].first;
				int dy = Block[i].second;
				grid[index(tetromino_x + dx, tetromino_y + dy)] = FILLED;
			}
			que.push_back(piece);
			piece = que[0];
			que.erase(que.begin());
			rot = RIGHT;
			tetromino_x = 5;
			tetromino_y = 21;
		}

		for (int i = 0; i < Block.size(); i++)
		{
			int dx = Block[i].first;
			int dy = Block[i].second;
			grid[index(tetromino_x + dx, tetromino_y + dy)] = TETROMINO;
		}

		timer++;
	}
public:

	int score = 0;

	Grid(int _rows, int _columns)
		:columns(_columns),
		rows(_rows)
	{
		grid.resize(columns * rows);

		for (int i = 0; i < grid.size(); i++)
		{
			grid[i] = EMPTY;
		}
	}

	bool update()
	{
		tetromino();

		int i = 0;
		while (i < rows)
		{
			if (!clear_row(i))
			{
				i++;
			}
		}

		print();

		for (int j = 0; j < columns; j++)
		{
			if (grid[index(j, 20)] == FILLED)
			{
				return false;
			}
		}

		return true;
	}
};

int main()
{
	std::string getch;

	Grid grid(25,10);

	bool play = true;

	while (play)
	{
		system("CLS");
		play = grid.update();
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	std::cout << "\n" << "Your Score was: " << grid.score;

	std::cout << "\n" << "Input your name: ";

	std::cin >> getch;

	return 0;
}
