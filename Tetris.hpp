#ifndef TETRIS_H
#define TETRIS_H

#include <iostream>
#include <vector>
#include <utility>
#include <cstdlib>
#include <cmath>
#include <conio.h>
#include <map>
#include <algorithm>

enum States
{
	EMPTY,
	FILLED,
	TETROMINO
};

char getNonBlockingInput()
{
	if (_kbhit()) {
		return _getch();
	}
	return '\0';
}

enum Tetromino
{
	STRAIGHT,
	SQUARE,
	LEFTL,
	RIGHTL,
	LEFTS,
	RIGHTS,
	TSHAPE
};

enum Rotation
{
	RIGHT,
	UP,
	LEFT,
	DOWN
};

struct TetrominoState
{
private:
	const std::pair<Tetromino, Rotation> STR_R = std::make_pair(STRAIGHT, RIGHT);
	const std::pair<Tetromino, Rotation> STR_L = std::make_pair(STRAIGHT, LEFT);
	const std::pair<Tetromino, Rotation> STR_U = std::make_pair(STRAIGHT, UP);
	const std::pair<Tetromino, Rotation> STR_D = std::make_pair(STRAIGHT, DOWN);

	const std::pair<Tetromino, Rotation> SQR_R = std::make_pair(SQUARE, RIGHT);
	const std::pair<Tetromino, Rotation> SQR_L = std::make_pair(SQUARE, LEFT);
	const std::pair<Tetromino, Rotation> SQR_U = std::make_pair(SQUARE, UP);
	const std::pair<Tetromino, Rotation> SQR_D = std::make_pair(SQUARE, DOWN);

	const std::pair<Tetromino, Rotation> LL_R = std::make_pair(LEFTL, RIGHT);
	const std::pair<Tetromino, Rotation> LL_L = std::make_pair(LEFTL, LEFT);
	const std::pair<Tetromino, Rotation> LL_U = std::make_pair(LEFTL, UP);
	const std::pair<Tetromino, Rotation> LL_D = std::make_pair(LEFTL, DOWN);

	const std::pair<Tetromino, Rotation> RL_R = std::make_pair(RIGHTL, RIGHT);
	const std::pair<Tetromino, Rotation> RL_L = std::make_pair(RIGHTL, LEFT);
	const std::pair<Tetromino, Rotation> RL_U = std::make_pair(RIGHTL, UP);
	const std::pair<Tetromino, Rotation> RL_D = std::make_pair(RIGHTL, DOWN);

	const std::pair<Tetromino, Rotation> LS_R = std::make_pair(LEFTS, RIGHT);
	const std::pair<Tetromino, Rotation> LS_L = std::make_pair(LEFTS, LEFT);
	const std::pair<Tetromino, Rotation> LS_U = std::make_pair(LEFTS, UP);
	const std::pair<Tetromino, Rotation> LS_D = std::make_pair(LEFTS, DOWN);

	const std::pair<Tetromino, Rotation> RS_R = std::make_pair(RIGHTS, RIGHT);
	const std::pair<Tetromino, Rotation> RS_L = std::make_pair(RIGHTS, LEFT);
	const std::pair<Tetromino, Rotation> RS_U = std::make_pair(RIGHTS, UP);
	const std::pair<Tetromino, Rotation> RS_D = std::make_pair(RIGHTS, DOWN);

	const std::pair<Tetromino, Rotation> T_R = std::make_pair(TSHAPE, RIGHT);
	const std::pair<Tetromino, Rotation> T_L = std::make_pair(TSHAPE, LEFT);
	const std::pair<Tetromino, Rotation> T_U = std::make_pair(TSHAPE, UP);
	const std::pair<Tetromino, Rotation> T_D = std::make_pair(TSHAPE, DOWN);

	const std::pair<int, int> z_z = std::make_pair(0, 0);
	const std::pair<int, int> z_o = std::make_pair(0, 1);
	const std::pair<int, int> z_t = std::make_pair(0, 2);
	const std::pair<int, int> z_r = std::make_pair(0, 3);

	const std::pair<int, int> o_z = std::make_pair(1, 0);
	const std::pair<int, int> o_o = std::make_pair(1, 1);
	const std::pair<int, int> o_t = std::make_pair(1, 2);
	const std::pair<int, int> o_r = std::make_pair(1, 3);

	const std::pair<int, int> no_z = std::make_pair(-1, 0);
	const std::pair<int, int> no_o = std::make_pair(-1, 1);
	const std::pair<int, int> no_t = std::make_pair(-1, 2);
	const std::pair<int, int> no_r = std::make_pair(-1, 3);

	const std::pair<int, int> t_z = std::make_pair(2, 0);
	const std::pair<int, int> t_o = std::make_pair(2, 1);
	const std::pair<int, int> t_t = std::make_pair(2, 2);
	const std::pair<int, int> t_r = std::make_pair(2, 3);

	const std::pair<int, int> nt_z = std::make_pair(-2, 0);
	const std::pair<int, int> nt_o = std::make_pair(-2, 1);
	const std::pair<int, int> nt_t = std::make_pair(-2, 2);
	const std::pair<int, int> nt_r = std::make_pair(-2, 3);

	const std::pair<int, int> r_z = std::make_pair(3, 0);
	const std::pair<int, int> r_o = std::make_pair(3, 1);
	const std::pair<int, int> r_t = std::make_pair(3, 2);
	const std::pair<int, int> r_r = std::make_pair(3, 3);

	const std::pair<int, int> nr_z = std::make_pair(-3, 0);
	const std::pair<int, int> nr_o = std::make_pair(-3, 1);
	const std::pair<int, int> nr_t = std::make_pair(-3, 2);
	const std::pair<int, int> nr_r = std::make_pair(-3, 3);

public:
	std::map<std::pair<Tetromino, Rotation>, std::vector<std::pair<int, int>>> StateToCoords = 
	{ 
		{STR_R, {{z_z, z_o, z_t, z_r}} },
		{STR_L, {{z_z, z_o, z_t, z_r}} },
		{STR_U, {{z_z, o_z, t_z, r_z}} },
		{STR_D, {{z_z, o_z, t_z, r_z}} },

		{SQR_R, {{z_z, z_o, o_z, o_o}} },
		{SQR_L, {{z_z, z_o, o_z, o_o}} },
		{SQR_U, {{z_z, z_o, o_z, o_o}} },
		{SQR_D, {{z_z, z_o, o_z, o_o}} },

		{LL_R, {{z_z, z_o, o_z, t_z}} },
		{LL_L, {{z_z, z_o, no_o, nt_o}} },
		{LL_U, {{z_z, z_o, z_t, o_t}} },
		{LL_D, {{z_z, o_z, o_o, o_t}} },

		{RL_R, {{z_z, z_o, o_o, t_o}} },
		{RL_L, {{z_z, o_z, t_z, t_o}} },
		{RL_U, {{z_z, z_o, z_t, no_t}} },
		{RL_D, {{z_z, z_o, z_t, o_z}} },

		{LS_R, {{z_z, o_z, z_o, no_o}} },
		{LS_L, {{z_z, o_z, z_o, no_o}} },
		{LS_U, {{z_z, z_o, o_o, o_t}} },
		{LS_D, {{z_z, z_o, o_o, o_t}} },

		{RS_R, {{z_z, o_z, o_o, t_o}} },
		{RS_L, {{z_z, o_z, o_o, t_o}} },
		{RS_U, {{z_z, z_o, no_o, no_t}} },
		{RS_D, {{z_z, z_o, no_o, no_t}}},

		{T_R, {{z_z, z_o, z_t, o_o}} },
		{T_L, {{z_z, z_o, z_t, no_o}} },
		{T_U, {{z_z, no_z, o_z, z_o}} },
		{T_D, {{z_z, no_o, z_o, o_o}} }

	};

};


#endif