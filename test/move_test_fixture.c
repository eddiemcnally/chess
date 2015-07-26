/*
 * move_test_fixture.c
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
#include "seatest.h"
#include "types.h"
#include "attack.h"
#include "fen.h"
#include "board.h"
#include "pieces.h"
#include "board_utils.h"
#include "move.h"

void test_generation_white_pawn_moves(void);
void test_generation_black_pawn_moves(void);
void test_generation_white_knight_pawn_moves(void);
void test_generation_black_knight_pawn_moves(void);


void test_generation_white_pawn_moves(void)
{

/*Move:1 > a2a3 (score:0)
Move:2 > a2a4 (score:0)
Move:3 > c2c3 (score:0)
Move:4 > c2c4 (score:0)
Move:5 > e2e3 (score:0)
Move:6 > e2e4 (score:0)
Move:7 > h3h4 (score:0)
Move:8 > b4b5 (score:0)
Move:9 > b4c5 (score:0)
Move:10 > d4d5 (score:0)
Move:11 > d4c5 (score:0)
Move:12 > d4e5 (score:0)
Move:13 > f5f6 (score:0)
Move:14 > f5e6 (score:0)
Move:15 > g7g8q (score:0)
Move:16 > g7g8r (score:0)
Move:17 > g7g8b (score:0)
Move:18 > g7g8n (score:0)
Move:19 > g7f8q (score:0)
Move:20 > g7f8r (score:0)
Move:21 > g7f8b (score:0)
Move:22 > g7f8n (score:0)
Move:23 > g7h8q (score:0)
Move:24 > g7h8r (score:0)
Move:25 > g7h8b (score:0)
Move:26 > g7h8n (score:0)
MoveList Total 26 Moves:
 */

	// setup
    char *test_fen =
	"rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1";

    struct board *brd = get_clean_board();

    consume_fen_notation(test_fen, brd);

	struct move_list * mvl = get_empty_move_list();
	generate_white_pawn_moves(brd, mvl);


	// test
	assert_true(mvl->move_count == 26);

	mv_bitmap mv = MOVE(a2, a3, NO_PIECE, NO_PIECE, 0);
	assert_true(is_move_in_list(mvl, mv));

	mv = MOVE(a2, a4, NO_PIECE, NO_PIECE, MFLAG_PAWN_START);
	//print_move_details(mv);
	assert_true(is_move_in_list(mvl, mv));

	mv = MOVE(c2, c3, NO_PIECE, NO_PIECE, 0);
	assert_true(is_move_in_list(mvl, mv));

	mv = MOVE(e2, e3, NO_PIECE, NO_PIECE, 0);
	assert_true(is_move_in_list(mvl, mv));

	mv = MOVE(e2, e4, NO_PIECE, NO_PIECE, MFLAG_PAWN_START);
	assert_true(is_move_in_list(mvl, mv));

	mv = MOVE(h3, h4, NO_PIECE, NO_PIECE, 0);
	assert_true(is_move_in_list(mvl, mv));

	mv = MOVE(b4, b5, NO_PIECE, NO_PIECE, 0);
	assert_true(is_move_in_list(mvl, mv));

	mv = MOVE(b4, c5, B_PAWN, NO_PIECE, 0);
	assert_true(is_move_in_list(mvl, mv));

	mv = MOVE(d4, d5, NO_PIECE, NO_PIECE, 0);
	assert_true(is_move_in_list(mvl, mv));

	mv = MOVE(d4, c5, B_PAWN, NO_PIECE, 0);
	assert_true(is_move_in_list(mvl, mv));

	mv = MOVE(d4, e5, B_PAWN, NO_PIECE, 0);
	assert_true(is_move_in_list(mvl, mv));

	mv = MOVE(f5, f6, NO_PIECE, NO_PIECE, 0);
	assert_true(is_move_in_list(mvl, mv));


	mv = MOVE(g7, g8, NO_PIECE, W_QUEEN, 0);
	assert_true(is_move_in_list(mvl, mv));

	mv = MOVE(g7, g8, NO_PIECE, W_KNIGHT, 0);
	assert_true(is_move_in_list(mvl, mv));

	mv = MOVE(g7, g8, NO_PIECE, W_BISHOP, 0);
	assert_true(is_move_in_list(mvl, mv));

	mv = MOVE(g7, g8, NO_PIECE, W_ROOK, 0);
	assert_true(is_move_in_list(mvl, mv));

	mv = MOVE(g7, f8, B_BISHOP, W_ROOK, 0);
	assert_true(is_move_in_list(mvl, mv));

	mv = MOVE(g7, f8, B_BISHOP, W_QUEEN, 0);
	assert_true(is_move_in_list(mvl, mv));

	mv = MOVE(g7, f8, B_BISHOP, W_BISHOP, 0);
	assert_true(is_move_in_list(mvl, mv));

	mv = MOVE(g7, f8, B_BISHOP, W_KNIGHT, 0);
	assert_true(is_move_in_list(mvl, mv));


/*

Move:23 > g7h8q (score:0)
Move:24 > g7h8r (score:0)
Move:25 > g7h8b (score:0)
Move:26 > g7h8n (score:0)

 */

	mv = MOVE(g7, h8, B_ROOK, W_KNIGHT, 0);
	assert_true(is_move_in_list(mvl, mv));

	mv = MOVE(g7, h8, B_ROOK, W_BISHOP, 0);
	assert_true(is_move_in_list(mvl, mv));

	mv = MOVE(g7, h8, B_ROOK, W_QUEEN, 0);
	assert_true(is_move_in_list(mvl, mv));

	mv = MOVE(g7, h8, B_ROOK, W_ROOK, 0);
	assert_true(is_move_in_list(mvl, mv));


}


void test_generation_black_pawn_moves(void)
{
	// setup
    char *test_fen =
	"rnbqkbnr/p1p1p3/3p3p/1p1p4/2P1Pp2/8/PP1P1PpP/RNBQKB1R b KQkq e3 0 1";

    struct board *brd = get_clean_board();

    consume_fen_notation(test_fen, brd);

	struct move_list * mvl = get_empty_move_list();
	generate_black_pawn_moves(brd, mvl);

	// test
	assert_true(mvl->move_count == 26);


	mv_bitmap mv = MOVE(g2, g1, NO_PIECE, B_ROOK, 0);
	assert_true(is_move_in_list(mvl, mv));

	mv = MOVE(g2, g1, NO_PIECE, B_QUEEN, 0);
	assert_true(is_move_in_list(mvl, mv));

	mv = MOVE(g2, g1, NO_PIECE, B_BISHOP, 0);
	assert_true(is_move_in_list(mvl, mv));

	mv = MOVE(g2, g1, NO_PIECE, B_KNIGHT, 0);
	assert_true(is_move_in_list(mvl, mv));


	mv = MOVE(g2, h1, W_ROOK, B_KNIGHT, 0);
	assert_true(is_move_in_list(mvl, mv));

	mv = MOVE(g2, h1, W_ROOK, B_BISHOP, 0);
	assert_true(is_move_in_list(mvl, mv));

	mv = MOVE(g2, h1, W_ROOK, B_ROOK, 0);
	assert_true(is_move_in_list(mvl, mv));

	mv = MOVE(g2, h1, W_ROOK, B_QUEEN, 0);
	assert_true(is_move_in_list(mvl, mv));


	mv = MOVE(g2, g1, NO_PIECE, B_KNIGHT, 0);
	assert_true(is_move_in_list(mvl, mv));

	mv = MOVE(g2, g1, NO_PIECE, B_BISHOP, 0);
	assert_true(is_move_in_list(mvl, mv));

	mv = MOVE(g2, g1, NO_PIECE, B_ROOK, 0);
	assert_true(is_move_in_list(mvl, mv));

	mv = MOVE(g2, g1, NO_PIECE, B_QUEEN, 0);
	assert_true(is_move_in_list(mvl, mv));

	mv = MOVE(f4, f3, NO_PIECE, NO_PIECE, 0);
	assert_true(is_move_in_list(mvl, mv));

	mv = MOVE(b5, b4, NO_PIECE, NO_PIECE, 0);
	assert_true(is_move_in_list(mvl, mv));

	mv = MOVE(d5, d4, NO_PIECE, NO_PIECE, 0);
	assert_true(is_move_in_list(mvl, mv));

}


void test_generation_white_knight_pawn_moves(void)
{
	// setup
    char *test_fen = "5k2/1n6/4n3/6N1/8/3N4/8/5K2 w - - 0 1";

    struct board *brd = get_clean_board();

    consume_fen_notation(test_fen, brd);

	struct move_list * mvl = get_empty_move_list();
	generate_knight_piece_moves(brd, mvl, WHITE);


	//printf("move count %d\n", mvl->move_count);
	//print_move_list_details(mvl);

	// test
	assert_true(mvl->move_count == 14);


	// check moves from d3
	mv_bitmap mv = MOVE(d3, f2, NO_PIECE, NO_PIECE, 0);
	assert_true(is_move_in_list(mvl, mv));
	mv = MOVE(d3, e1, NO_PIECE, NO_PIECE, 0);
	assert_true(is_move_in_list(mvl, mv));
	mv = MOVE(d3, c1, NO_PIECE, NO_PIECE, 0);
	assert_true(is_move_in_list(mvl, mv));
	mv = MOVE(d3, b2, NO_PIECE, NO_PIECE, 0);
	assert_true(is_move_in_list(mvl, mv));
	mv = MOVE(d3, b4, NO_PIECE, NO_PIECE, 0);
	assert_true(is_move_in_list(mvl, mv));
	mv = MOVE(d3, c5, NO_PIECE, NO_PIECE, 0);
	assert_true(is_move_in_list(mvl, mv));
	mv = MOVE(d3, e5, NO_PIECE, NO_PIECE, 0);
	assert_true(is_move_in_list(mvl, mv));
	mv = MOVE(d3, f4, NO_PIECE, NO_PIECE, 0);
	assert_true(is_move_in_list(mvl, mv));


	// check moves from g5
	mv = MOVE(g5, h3, NO_PIECE, NO_PIECE, 0);
	assert_true(is_move_in_list(mvl, mv));
	mv = MOVE(g5, f3, NO_PIECE, NO_PIECE, 0);
	assert_true(is_move_in_list(mvl, mv));
	mv = MOVE(g5, e4, NO_PIECE, NO_PIECE, 0);
	assert_true(is_move_in_list(mvl, mv));
	mv = MOVE(g5, f7, NO_PIECE, NO_PIECE, 0);
	assert_true(is_move_in_list(mvl, mv));
	mv = MOVE(g5, h7, NO_PIECE, NO_PIECE, 0);
	assert_true(is_move_in_list(mvl, mv));

	mv = MOVE(g5, e6, B_KNIGHT, NO_PIECE, 0);
	assert_true(is_move_in_list(mvl, mv));

}

void test_generation_black_knight_pawn_moves(void)
{
	// setup
    char *test_fen = "r1bqkb1r/pppppppp/2N5/3n1N2/8/1n6/PPPPPPPP/R1BQKB1R b KQkq - 0 1";

    struct board *brd = get_clean_board();

    consume_fen_notation(test_fen, brd);

	struct move_list * mvl = get_empty_move_list();
	generate_knight_piece_moves(brd, mvl, BLACK);


	//printf("move count %d\n", mvl->move_count);
	//print_move_list_details(mvl);

	// test
	assert_true(mvl->move_count == 12);

	// start on b3
	mv_bitmap mv = MOVE(b3, d2, W_PAWN, NO_PIECE, 0);
	assert_true(is_move_in_list(mvl, mv));
	mv = MOVE(b3, c1, W_BISHOP, NO_PIECE, 0);
	assert_true(is_move_in_list(mvl, mv));
	mv = MOVE(b3, a1, W_ROOK, NO_PIECE, 0);
	assert_true(is_move_in_list(mvl, mv));

	mv = MOVE(b3, d2, W_PAWN, NO_PIECE, 0);
	assert_true(is_move_in_list(mvl, mv));
	mv = MOVE(b3, c1, W_BISHOP, NO_PIECE, 0);
	assert_true(is_move_in_list(mvl, mv));
	mv = MOVE(b3, a1, W_ROOK, NO_PIECE, 0);
	assert_true(is_move_in_list(mvl, mv));


	// start on d5

	mv = MOVE(d5, f4, NO_PIECE, NO_PIECE, 0);
	assert_true(is_move_in_list(mvl, mv));
	mv = MOVE(d5, e3, NO_PIECE, NO_PIECE, 0);
	assert_true(is_move_in_list(mvl, mv));
	mv = MOVE(d5, c3, NO_PIECE, NO_PIECE, 0);
	assert_true(is_move_in_list(mvl, mv));
	mv = MOVE(d5, b4, NO_PIECE, NO_PIECE, 0);
	assert_true(is_move_in_list(mvl, mv));
	mv = MOVE(d5, b6, NO_PIECE, NO_PIECE, 0);
	assert_true(is_move_in_list(mvl, mv));
	mv = MOVE(d5, f6, NO_PIECE, NO_PIECE, 0);
	assert_true(is_move_in_list(mvl, mv));


}


void move_test_fixture(void)
{
    test_fixture_start();	// starts a fixture

    run_test(test_generation_white_pawn_moves);
    run_test(test_generation_black_pawn_moves);
	run_test(test_generation_white_knight_pawn_moves);
	run_test(test_generation_black_knight_pawn_moves);


    test_fixture_end();		// ends a fixture
}
