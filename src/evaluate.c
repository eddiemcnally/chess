/*
 * evaluate.c
 * 
 * Copyright (C) 2015 Eddie McNally <emcn@gmx.com>
 *
 * kestrel is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * kestrel is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "types.h"
#include "evaluate.h"
#include "pieces.h"
#include "move.h"


static I32 eval_piece(const struct board *brd, enum piece pce, const I8 *pt);



/*****************************************************
 * Piece tables 
 * 
 * Initial values selected from the following link:
 * 		http://chessprogramming.wikispaces.com/Simplified+evaluation+function#Piece-Square%20Tables
 * 
 * These tables are from White's POV and [0] is a1
 */

static const I8 PAWN_PT[NUM_SQUARES] = {
	0,  0,  0,  0,	0,  0,  0,  0,
	5, 10, 10,-20,-20, 10, 10,  5,
	5, -5,-10,	0,	0,-10, -5, 5,
	0,  0,  0, 20, 20,  0,  0,  0,
	5,  5, 10, 25, 25, 10,  5,  5,
   10, 10, 20, 30, 30, 20, 10, 10,
   50, 50, 50, 50, 50, 50, 50, 50,
    0,  0,  0,  0,  0,  0,  0,  0
};


static const I8 KNIGHT_PT[NUM_SQUARES] ={
	-50,-40,-30,-30,-30,-30,-40,-50,
	-40,-20,  0,  5,  5,  0,-20,-40,
	-30,  5, 10, 15, 15, 10,  5,-30,
	-30,  0, 15, 20, 20, 15,  0,-30,
	-30,  5, 15, 20, 20, 15,  5,-30,
	-30,  0, 10, 15, 15, 10,  0,-30,
	-40,-20,  0,  0,  0,  0,-20,-40,
	-50,-40,-30,-30,-30,-30,-40,-50
};


static const I8 BISHOP_PT[NUM_SQUARES] ={
	-20,-10,-10,-10,-10,-10,-10,-20,	
	-10,  5,  0,  0,  0,  0,  5,-10,
	-10, 10, 10, 10, 10, 10, 10,-10,
	-10,  0, 10, 10, 10, 10,  0,-10,
	-10,  5,  5, 10, 10,  5,  5,-10,
	-10,  0,  5, 10, 10,  5,  0,-10,
	-10,  0,  0,  0,  0,  0,  0,-10,
	-20,-10,-10,-10,-10,-10,-10,-20
};


static const I8 ROOK_PT[NUM_SQUARES] ={
	0,  0,  0,  5,  5,  0,  0,  0,	
   -5,  0,  0,  0,  0,  0,  0, -5,
   -5,  0,  0,  0,  0,  0,  0, -5,
   -5,  0,  0,  0,  0,  0,  0, -5,
   -5,  0,  0,  0,  0,  0,  0, -5,
   -5,  0,  0,  0,  0,  0,  0, -5,
	5, 10, 10, 10, 10, 10, 10,  5,
	0,  0,  0,  0,  0,  0,  0,  0	
};



static const I8 QUEEN_PT[NUM_SQUARES] ={
	-20,-10,-10, -5, -5,-10,-10,-20,	
	-10,  0,  5,  0,  0,  0,  0,-10,
	-10,  5,  5,  5,  5,  5,  0,-10,
	  0,  0,  5,  5,  5,  5,  0, -5,
	 -5,  0,  5,  5,  5,  5,  0, -5,
	-10,  0,  5,  5,  5,  5,  0,-10,
	-10,  0,  0,  0,  0,  0,  0,-10,
	-20,-10,-10, -5, -5,-10,-10,-20
};


static const I8 KING_MID_GAME_PT [NUM_SQUARES] ={
	 20, 30, 10,  0,  0, 10, 30, 20,
	 20, 20,  0,  0,  0,  0, 20, 20,
	-10,-20,-20,-20,-20,-20,-20,-10,
	-20,-30,-30,-40,-40,-30,-30,-20,
	-30,-40,-40,-50,-50,-40,-40,-30,
	-30,-40,-40,-50,-50,-40,-40,-30,
	-30,-40,-40,-50,-50,-40,-40,-30,
	-30,-40,-40,-50,-50,-40,-40,-30
};


static const I8 KING_END_GAME_PT[NUM_SQUARES] ={
	-50,-30,-30,-30,-30,-30,-30,-50,
	-30,-30,  0,  0,  0,  0,-30,-30,
	-30,-10, 20, 30, 30, 20,-10,-30,
	-30,-10, 30, 40, 40, 30,-10,-30,
	-30,-10, 30, 40, 40, 30,-10,-30,
	-30,-10, 20, 30, 30, 20,-10,-30,
	-30,-20,-10,  0,  0,-10,-20,-30,
	-50,-40,-30,-20,-20,-30,-40,-50
};


// a mirror array to allow for looking up the 
// piece tables for BLACK

static const I8 MIRROR_PT[NUM_SQUARES] ={
	56,	57,	58,	59,	60,	61,	62,	63,
	48,	49,	50,	51,	52,	53,	54,	55,
	40,	41,	42,	43,	44,	45,	46,	47,
	32,	33,	34,	35,	36,	37,	38,	39,
	24,	25,	26,	27,	28,	29,	30,	31,
	16,	17,	18,	19,	20,	21,	22,	23,
	8,	9,	10,	11,	12,	13,	14,	15,	
	0,	1,	2,	3,	4,	5,	6,	7
};

#define MIRROR_SQUARE(sq)	(MIRROR_PT[sq])

/*
 * 
 * name: evaluate_position
 * @param
 * @return >0 for white, <0 for black
 * 
 */
I32 evaluate_position(const struct board *brd)
{

	// initially based on material
	I32 score = (I32)brd->material[WHITE] - (I32)brd->material[BLACK];

	score += eval_piece(brd, W_PAWN, PAWN_PT);
	score += eval_piece(brd, B_PAWN, PAWN_PT);

	score += eval_piece(brd, W_BISHOP, BISHOP_PT);
	score += eval_piece(brd, B_BISHOP, BISHOP_PT);

	score += eval_piece(brd, W_KNIGHT, KNIGHT_PT);
	score += eval_piece(brd, B_KNIGHT, KNIGHT_PT);

	score += eval_piece(brd, W_ROOK, ROOK_PT);
	score += eval_piece(brd, B_ROOK, ROOK_PT);

	score += eval_piece(brd, W_QUEEN, QUEEN_PT);
	score += eval_piece(brd, B_QUEEN, QUEEN_PT);

	score += eval_piece(brd, W_KING, KING_MID_GAME_PT);
	score += eval_piece(brd, B_KING, KING_MID_GAME_PT);

	if (brd->side_to_move == WHITE) {
		return score;
	} else {
		return -score;
	}
}

inline static I32 eval_piece(const struct board *brd, enum piece pce, const I8 * pt)
{
	bool is_black = IS_BLACK(pce);
	I32 score = 0;

	U64 bb = brd->bitboards[pce];
	if (is_black) {
		while (bb != 0) {
			enum square sq = pop_1st_bit(&bb);
			score += pt[MIRROR_SQUARE(sq)];
		}
	} else {
		while (bb != 0) {
			enum square sq = pop_1st_bit(&bb);
			score += pt[sq];
		}
	}
	return score;
}
