/*
 * board_utils.c
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
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "types.h"
#include "hashkeys.h"
#include "board.h"
#include "pieces.h"
#include "board_utils.h"


//typedef unsigned int piece_t;



/* Pretty-prints the board
 * 
 * name: print_board
 * @param: a board
 * @return : void
 * 
 */

// char arrays to suport printing
static const char ranks[] 	= "12345678";
static const char files[] 	= "abcdefgh";

/**
 * Thanks again to Bluefever Software for this code
 */
void print_board(board_container_t * the_board)
{

	//int sq,piece;
	
	printf("\nGame Board:\n\n");
	
	for(int rank = RANK_8; rank >= RANK_1; rank--) {
		printf("%d  ",rank+1);	// enum is zero-based
		for(int file = FILE_A; file <= FILE_H; file++) {
			square_t sq = GET_SQUARE(rank, file);
			piece_id_t pce = get_piece_at_square(the_board, sq);
			if (pce != NO_PIECE){
				char c = get_piece_label(pce);
				printf("%3c", c);
			} else {
				printf("  -");
			}
		}
		printf("\n");
	}
	
	printf("\n   ");
	for(int file = FILE_A; file <= FILE_H; file++) {
		printf("%3c",'a'+file);	
	}
	printf("\n\n");
	char side;
	if (the_board->side_to_move == WHITE){
		side = 'w';
	} else {
		side = 'b';
	}
	printf("side:\t%c\n", side);
	
	if (the_board->en_passant == NO_SQUARE){
		printf("enPas:\t-\n");
	}
	else{
		int rank = GET_RANK(the_board->en_passant);
		int file = GET_FILE(the_board->en_passant);
		printf("enPas:\t%c%c\n", files[file], ranks[rank]);
	}

	printf("castle:\t%c%c%c%c\n",
			(the_board->castle_perm & WKCA) ? 'K' : '-',
			(the_board->castle_perm & WQCA) ? 'Q' : '-',
			(the_board->castle_perm & BKCA) ? 'k' : '-',
			(the_board->castle_perm & BQCA) ? 'q' : '-'	
			);
	printf("PosKey:\t0x%016llx\n",the_board->position_key);

	printf("\n\n");
	
}


/*
 * Validates the contents of a board struct.
 * 
 * name: ASSERT_BOARD_OK
 * @param
 * @return
 * 
 */

bool ASSERT_BOARD_OK(board_container_t * brd){

	// check bit boards
	board_t conflated = 0;

	for (int i = 0; i < NUM_PIECE_TYPES; i++){
		conflated |= brd->piece_boards[i];	
	}
	assert(conflated == brd->board);

	// check where Kings are
	for(square_t sq = 0; sq < NUM_SQUARES; sq++){
		piece_id_t pce = get_piece_at_square(brd, sq);
		if (pce != NO_PIECE){
			if (pce == W_KING){
				assert(sq == brd->king_squares[WHITE]);
			} else if (pce == B_KING){
				assert(sq == brd->king_squares[BLACK]);
			}
		}		
	}
	
	// check number of pieces on board
	// -------------------------------
	U8 pce_num[NUM_PIECE_TYPES] = {0};
	for(square_t sq = 0; sq < NUM_SQUARES; sq++){
		piece_id_t pce = get_piece_at_square(brd, sq);
		if (pce != NO_PIECE){
			pce_num[pce]++;
		}		
	}
	for(int i = 0; i < NUM_PIECE_TYPES; i++){
		assert(pce_num[i] == brd->pce_num[i]);
	}
	

	// check on big, major and minor piece count
	U8 big_pieces[NUM_COLOURS] = {0}; 
	U8 major_pieces[NUM_COLOURS] = {0};
	U8 minor_pieces[NUM_COLOURS] = {0};
	for(square_t sq = 0; sq < NUM_SQUARES; sq++){
		piece_id_t pce = get_piece_at_square(brd, sq);
		if (pce != NO_PIECE){
			colour_t col = get_colour(pce);
			if (IS_BIG_PIECE(pce)){
				big_pieces[col] += 1;
			}
			if (IS_MAJOR_PIECE(pce)){
				major_pieces[col] += 1;
			}
			if (IS_MINOR_PIECE(pce)){
				minor_pieces[col] += 1;
			}
		}		
	}
	assert(big_pieces[WHITE] == brd->big_pieces[WHITE]);
	assert(big_pieces[BLACK] == brd->big_pieces[BLACK]);
	assert(major_pieces[WHITE] == brd->major_pieces[WHITE]);
	assert(major_pieces[BLACK] == brd->major_pieces[BLACK]);
	assert(minor_pieces[WHITE] == brd->minor_pieces[WHITE]);
	assert(minor_pieces[BLACK] == brd->minor_pieces[BLACK]);
	
	

	// calc and verify the material count
	U8 material[NUM_COLOURS] = {0};
	for(square_t sq = 0; sq < NUM_SQUARES; sq++){
		piece_id_t pce = get_piece_at_square(brd, sq);
		if (pce != NO_PIECE){
			colour_t col = get_colour(pce);
			material[col] += piece_values[pce];
		}		
	}
	assert(material[WHITE] == brd->material[WHITE]);
	assert(material[BLACK] == brd->material[BLACK]);
	
	
	// check on position key
	assert(brd->position_key == get_position_hashkey(brd));
	
	
	return true;

}

