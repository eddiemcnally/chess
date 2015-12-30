/*
 * performance_tests.c
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "seatest.h"
#include "types.h"
#include "attack.h"
#include "assert.h"
#include "init.h"
#include "fen.h"
#include "board.h"
#include "pieces.h"
#include "board_utils.h"
#include "makemove.h"
#include "utils.h"
#include "move_gen.h"
#include "move_gen_utils.h"


void perf_test(int depth, struct board *brd);
void divide_perft(int depth, struct board *brd);
uint32_t divide(int depth, struct board *brd);
void test_move_gen_depth(void);
void perft(int depth, struct board *brd);
void bug_check(void);

// struct representing a line in the perftsuite.epd file
typedef struct EPD {
	char *fen;
	uint64_t depth1;
	uint64_t depth2;
	uint64_t depth3;
	uint64_t depth4;
	uint64_t depth5;
	uint64_t depth6;
} epd;



//=================================================================
//
// This array is generated from perftsuite.epd, which can be found
// on the net. It gives the number of legal moves at each depth for a 
// given position
// 
// see : 
//		http://chessprogramming.wikispaces.com/Perft
//		http://chessprogramming.wikispaces.com/Extended+Position+Description
//
// The included file "perftsuit.epd" was downloaded from the following
// link :
//		http://code.haskell.org/ChessLibrary/perftsuite.epd
//



#define NUM_EPD	126
struct EPD test_positions[NUM_EPD] = {
	{"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 20, 400,
	 8902, 197281, 4865609, 119060324},
	{"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1",
	 48, 2039, 97862, 4085603, 193690690, 8031647685},
	{"4k3/8/8/8/8/8/8/4K2R w K - 0 1", 15, 66, 1197, 7059, 133987, 764643},
	{"4k3/8/8/8/8/8/8/R3K3 w Q - 0 1", 16, 71, 1287, 7626, 145232, 846648},
	{"4k2r/8/8/8/8/8/8/4K3 w k - 0 1", 5, 75, 459, 8290, 47635, 899442},
	{"r3k3/8/8/8/8/8/8/4K3 w q - 0 1", 5, 80, 493, 8897, 52710, 1001523},
	{"4k3/8/8/8/8/8/8/R3K2R w KQ - 0 1", 26, 112, 3189, 17945, 532933,
	 2788982},
	{"r3k2r/8/8/8/8/8/8/4K3 w kq - 0 1", 5, 130, 782, 22180, 118882,
	 3517770},
	{"8/8/8/8/8/8/6k1/4K2R w K - 0 1", 12, 38, 564, 2219, 37735, 185867},
	{"8/8/8/8/8/8/1k6/R3K3 w Q - 0 1", 15, 65, 1018, 4573, 80619, 413018},
	{"4k2r/6K1/8/8/8/8/8/8 w k - 0 1", 3, 32, 134, 2073, 10485, 179869},
	{"r3k3/1K6/8/8/8/8/8/8 w q - 0 1", 4, 49, 243, 3991, 20780, 367724},
	{"r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1", 26, 568, 13744, 314346,
	 7594526, 179862938},
	{"r3k2r/8/8/8/8/8/8/1R2K2R w Kkq - 0 1", 25, 567, 14095, 328965,
	 8153719, 195629489},
	{"r3k2r/8/8/8/8/8/8/2R1K2R w Kkq - 0 1", 25, 548, 13502, 312835,
	 7736373, 184411439},
	{"r3k2r/8/8/8/8/8/8/R3K1R1 w Qkq - 0 1", 25, 547, 13579, 316214,
	 7878456, 189224276},
	{"1r2k2r/8/8/8/8/8/8/R3K2R w KQk - 0 1", 26, 583, 14252, 334705,
	 8198901, 198328929},
	{"2r1k2r/8/8/8/8/8/8/R3K2R w KQk - 0 1", 25, 560, 13592, 317324,
	 7710115, 185959088},
	{"r3k1r1/8/8/8/8/8/8/R3K2R w KQq - 0 1", 25, 560, 13607, 320792,
	 7848606, 190755813},
	{"4k3/8/8/8/8/8/8/4K2R b K - 0 1", 5, 75, 459, 8290, 47635, 899442},
	{"4k3/8/8/8/8/8/8/R3K3 b Q - 0 1", 5, 80, 493, 8897, 52710, 1001523},
	{"4k2r/8/8/8/8/8/8/4K3 b k - 0 1", 15, 66, 1197, 7059, 133987, 764643},
	{"r3k3/8/8/8/8/8/8/4K3 b q - 0 1", 16, 71, 1287, 7626, 145232, 846648},
	{"4k3/8/8/8/8/8/8/R3K2R b KQ - 0 1", 5, 130, 782, 22180, 118882,
	 3517770},
	{"r3k2r/8/8/8/8/8/8/4K3 b kq - 0 1", 26, 112, 3189, 17945, 532933,
	 2788982},
	{"8/8/8/8/8/8/6k1/4K2R b K - 0 1", 3, 32, 134, 2073, 10485, 179869},
	{"8/8/8/8/8/8/1k6/R3K3 b Q - 0 1", 4, 49, 243, 3991, 20780, 367724},
	{"4k2r/6K1/8/8/8/8/8/8 b k - 0 1", 12, 38, 564, 2219, 37735, 185867},
	{"r3k3/1K6/8/8/8/8/8/8 b q - 0 1", 15, 65, 1018, 4573, 80619, 413018},
	{"r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 1", 26, 568, 13744, 314346,
	 7594526, 179862938},
	{"r3k2r/8/8/8/8/8/8/1R2K2R b Kkq - 0 1", 26, 583, 14252, 334705,
	 8198901, 198328929},
	{"r3k2r/8/8/8/8/8/8/2R1K2R b Kkq - 0 1", 25, 560, 13592, 317324,
	 7710115, 185959088},
	{"r3k2r/8/8/8/8/8/8/R3K1R1 b Qkq - 0 1", 25, 560, 13607, 320792,
	 7848606, 190755813},
	{"1r2k2r/8/8/8/8/8/8/R3K2R b KQk - 0 1", 25, 567, 14095, 328965,
	 8153719, 195629489},
	{"2r1k2r/8/8/8/8/8/8/R3K2R b KQk - 0 1", 25, 548, 13502, 312835,
	 7736373, 184411439},
	{"r3k1r1/8/8/8/8/8/8/R3K2R b KQq - 0 1", 25, 547, 13579, 316214,
	 7878456, 189224276},
	{"8/1n4N1/2k5/8/8/5K2/1N4n1/8 w - - 0 1", 14, 195, 2760, 38675, 570726,
	 8107539},
	{"8/1k6/8/5N2/8/4n3/8/2K5 w - - 0 1", 11, 156, 1636, 20534, 223507,
	 2594412},
	{"8/8/4k3/3Nn3/3nN3/4K3/8/8 w - - 0 1", 19, 289, 4442, 73584, 1198299,
	 19870403},
	{"K7/8/2n5/1n6/8/8/8/k6N w - - 0 1", 3, 51, 345, 5301, 38348, 588695},
	{"k7/8/2N5/1N6/8/8/8/K6n w - - 0 1", 17, 54, 835, 5910, 92250, 688780},
	{"8/1n4N1/2k5/8/8/5K2/1N4n1/8 b - - 0 1", 15, 193, 2816, 40039, 582642,
	 8503277},
	{"8/1k6/8/5N2/8/4n3/8/2K5 b - - 0 1", 16, 180, 2290, 24640, 288141,
	 3147566},
	{"8/8/3K4/3Nn3/3nN3/4k3/8/8 b - - 0 1", 4, 68, 1118, 16199, 281190,
	 4405103},
	{"K7/8/2n5/1n6/8/8/8/k6N b - - 0 1", 17, 54, 835, 5910, 92250, 688780},
	{"k7/8/2N5/1N6/8/8/8/K6n b - - 0 1", 3, 51, 345, 5301, 38348, 588695},
	{"B6b/8/8/8/2K5/4k3/8/b6B w - - 0 1", 17, 278, 4607, 76778, 1320507,
	 22823890},
	{"8/8/1B6/7b/7k/8/2B1b3/7K w - - 0 1", 21, 316, 5744, 93338, 1713368,
	 28861171},
	{"k7/B7/1B6/1B6/8/8/8/K6b w - - 0 1", 21, 144, 3242, 32955, 787524,
	 7881673},
	{"K7/b7/1b6/1b6/8/8/8/k6B w - - 0 1", 7, 143, 1416, 31787, 310862,
	 7382896},
	{"B6b/8/8/8/2K5/5k2/8/b6B b - - 0 1", 6, 106, 1829, 31151, 530585,
	 9250746},
	{"8/8/1B6/7b/7k/8/2B1b3/7K b - - 0 1", 17, 309, 5133, 93603, 1591064,
	 29027891},
	{"k7/B7/1B6/1B6/8/8/8/K6b b - - 0 1", 7, 143, 1416, 31787, 310862,
	 7382896},
	{"K7/b7/1b6/1b6/8/8/8/k6B b - - 0 1", 21, 144, 3242, 32955, 787524,
	 7881673},
	{"7k/RR6/8/8/8/8/rr6/7K w - - 0 1", 19, 275, 5300, 104342, 2161211,
	 44956585},
	{"R6r/8/8/2K5/5k2/8/8/r6R w - - 0 1", 36, 1027, 29215, 771461,
	 20506480,
	 525169084},
	{"7k/RR6/8/8/8/8/rr6/7K b - - 0 1", 19, 275, 5300, 104342, 2161211,
	 44956585},
	{"R6r/8/8/2K5/5k2/8/8/r6R b - - 0 1", 36, 1027, 29227, 771368,
	 20521342,
	 524966748},
	{"6kq/8/8/8/8/8/8/7K w - - 0 1", 2, 36, 143, 3637, 14893, 391507},
	{"6KQ/8/8/8/8/8/8/7k b - - 0 1", 2, 36, 143, 3637, 14893, 391507},
	{"K7/8/8/3Q4/4q3/8/8/7k w - - 0 1", 6, 35, 495, 8349, 166741, 3370175},
	{"6qk/8/8/8/8/8/8/7K b - - 0 1", 22, 43, 1015, 4167, 105749, 419369},
	{"6KQ/8/8/8/8/8/8/7k b - - 0 1", 2, 36, 143, 3637, 14893, 391507},
	{"K7/8/8/3Q4/4q3/8/8/7k b - - 0 1", 6, 35, 495, 8349, 166741, 3370175},
	{"8/8/8/8/8/K7/P7/k7 w - - 0 1", 3, 7, 43, 199, 1347, 6249},
	{"8/8/8/8/8/7K/7P/7k w - - 0 1", 3, 7, 43, 199, 1347, 6249},
	{"K7/p7/k7/8/8/8/8/8 w - - 0 1", 1, 3, 12, 80, 342, 2343},
	{"7K/7p/7k/8/8/8/8/8 w - - 0 1", 1, 3, 12, 80, 342, 2343},
	{"8/2k1p3/3pP3/3P2K1/8/8/8/8 w - - 0 1", 7, 35, 210, 1091, 7028,
	 34834},
	{"8/8/8/8/8/K7/P7/k7 b - - 0 1", 1, 3, 12, 80, 342, 2343},
	{"8/8/8/8/8/7K/7P/7k b - - 0 1", 1, 3, 12, 80, 342, 2343},
	{"K7/p7/k7/8/8/8/8/8 b - - 0 1", 3, 7, 43, 199, 1347, 6249},
	{"7K/7p/7k/8/8/8/8/8 b - - 0 1", 3, 7, 43, 199, 1347, 6249},
	{"8/2k1p3/3pP3/3P2K1/8/8/8/8 b - - 0 1", 5, 35, 182, 1091, 5408,
	 34822},
	{"8/8/8/8/8/4k3/4P3/4K3 w - - 0 1", 2, 8, 44, 282, 1814, 11848},
	{"4k3/4p3/4K3/8/8/8/8/8 b - - 0 1", 2, 8, 44, 282, 1814, 11848},
	{"8/8/7k/7p/7P/7K/8/8 w - - 0 1", 3, 9, 57, 360, 1969, 10724},
	{"8/8/k7/p7/P7/K7/8/8 w - - 0 1", 3, 9, 57, 360, 1969, 10724},
	{"8/8/3k4/3p4/3P4/3K4/8/8 w - - 0 1", 5, 25, 180, 1294, 8296, 53138},
	{"8/3k4/3p4/8/3P4/3K4/8/8 w - - 0 1", 8, 61, 483, 3213, 23599, 157093},
	{"8/8/3k4/3p4/8/3P4/3K4/8 w - - 0 1", 8, 61, 411, 3213, 21637, 158065},
	{"k7/8/3p4/8/3P4/8/8/7K w - - 0 1", 4, 15, 90, 534, 3450, 20960},
	{"8/8/7k/7p/7P/7K/8/8 b - - 0 1", 3, 9, 57, 360, 1969, 10724},
	{"8/8/k7/p7/P7/K7/8/8 b - - 0 1", 3, 9, 57, 360, 1969, 10724},
	{"8/8/3k4/3p4/3P4/3K4/8/8 b - - 0 1", 5, 25, 180, 1294, 8296, 53138},
	{"8/3k4/3p4/8/3P4/3K4/8/8 b - - 0 1", 8, 61, 411, 3213, 21637, 158065},
	{"8/8/3k4/3p4/8/3P4/3K4/8 b - - 0 1", 8, 61, 483, 3213, 23599, 157093},
	{"k7/8/3p4/8/3P4/8/8/7K b - - 0 1", 4, 15, 89, 537, 3309, 21104},
	{"7k/3p4/8/8/3P4/8/8/K7 w - - 0 1", 4, 19, 117, 720, 4661, 32191},
	{"7k/8/8/3p4/8/8/3P4/K7 w - - 0 1", 5, 19, 116, 716, 4786, 30980},
	{"k7/8/8/7p/6P1/8/8/K7 w - - 0 1", 5, 22, 139, 877, 6112, 41874},
	{"k7/8/7p/8/8/6P1/8/K7 w - - 0 1", 4, 16, 101, 637, 4354, 29679},
	{"k7/8/8/6p1/7P/8/8/K7 w - - 0 1", 5, 22, 139, 877, 6112, 41874},
	{"k7/8/6p1/8/8/7P/8/K7 w - - 0 1", 4, 16, 101, 637, 4354, 29679},
	{"k7/8/8/3p4/4p3/8/8/7K w - - 0 1", 3, 15, 84, 573, 3013, 22886},
	{"k7/8/3p4/8/8/4P3/8/7K w - - 0 1", 4, 16, 101, 637, 4271, 28662},
	{"7k/3p4/8/8/3P4/8/8/K7 b - - 0 1", 5, 19, 117, 720, 5014, 32167},
	{"7k/8/8/3p4/8/8/3P4/K7 b - - 0 1", 4, 19, 117, 712, 4658, 30749},
	{"k7/8/8/7p/6P1/8/8/K7 b - - 0 1", 5, 22, 139, 877, 6112, 41874},
	{"k7/8/7p/8/8/6P1/8/K7 b - - 0 1", 4, 16, 101, 637, 4354, 29679},
	{"k7/8/8/6p1/7P/8/8/K7 b - - 0 1", 5, 22, 139, 877, 6112, 41874},
	{"k7/8/6p1/8/8/7P/8/K7 b - - 0 1", 4, 16, 101, 637, 4354, 29679},
	{"k7/8/8/3p4/4p3/8/8/7K b - - 0 1", 5, 15, 102, 569, 4337, 22579},
	{"k7/8/3p4/8/8/4P3/8/7K b - - 0 1", 4, 16, 101, 637, 4271, 28662},
	{"7k/8/8/p7/1P6/8/8/7K w - - 0 1", 5, 22, 139, 877, 6112, 41874},
	{"7k/8/p7/8/8/1P6/8/7K w - - 0 1", 4, 16, 101, 637, 4354, 29679},
	{"7k/8/8/1p6/P7/8/8/7K w - - 0 1", 5, 22, 139, 877, 6112, 41874},
	{"7k/8/1p6/8/8/P7/8/7K w - - 0 1", 4, 16, 101, 637, 4354, 29679},
	{"k7/7p/8/8/8/8/6P1/K7 w - - 0 1", 5, 25, 161, 1035, 7574, 55338},
	{"k7/6p1/8/8/8/8/7P/K7 w - - 0 1", 5, 25, 161, 1035, 7574, 55338},
	{"3k4/3pp3/8/8/8/8/3PP3/3K4 w - - 0 1", 7, 49, 378, 2902, 24122,
	 199002},
	{"7k/8/8/p7/1P6/8/8/7K b - - 0 1", 5, 22, 139, 877, 6112, 41874},
	{"7k/8/p7/8/8/1P6/8/7K b - - 0 1", 4, 16, 101, 637, 4354, 29679},
	{"7k/8/8/1p6/P7/8/8/7K b - - 0 1", 5, 22, 139, 877, 6112, 41874},
	{"7k/8/1p6/8/8/P7/8/7K b - - 0 1", 4, 16, 101, 637, 4354, 29679},
	{"k7/7p/8/8/8/8/6P1/K7 b - - 0 1", 5, 25, 161, 1035, 7574, 55338},
	{"k7/6p1/8/8/8/8/7P/K7 b - - 0 1", 5, 25, 161, 1035, 7574, 55338},
	{"3k4/3pp3/8/8/8/8/3PP3/3K4 b - - 0 1", 7, 49, 378, 2902, 24122,
	 199002},
	{"8/Pk6/8/8/8/8/6Kp/8 w - - 0 1", 11, 97, 887, 8048, 90606, 1030499},
	{"n1n5/1Pk5/8/8/8/8/5Kp1/5N1N w - - 0 1", 24, 421, 7421, 124608,
	 2193768, 37665329},
	{"8/PPPk4/8/8/8/8/4Kppp/8 w - - 0 1", 18, 270, 4699, 79355, 1533145,
	 28859283},
	{"n1n5/PPPk4/8/8/8/8/4Kppp/5N1N w - - 0 1", 24, 496, 9483, 182838,
	 3605103, 71179139},
	{"8/Pk6/8/8/8/8/6Kp/8 b - - 0 1", 11, 97, 887, 8048, 90606, 1030499},
	{"n1n5/1Pk5/8/8/8/8/5Kp1/5N1N b - - 0 1", 24, 421, 7421, 124608,
	 2193768, 37665329},
	{"8/PPPk4/8/8/8/8/4Kppp/8 b - - 0 1", 18, 270, 4699, 79355, 1533145,
	 28859283},
	{"n1n5/PPPk4/8/8/8/8/4Kppp/5N1N b - - 0 1", 24, 496, 9483, 182838,
	 3605103, 71179139},
};

uint64_t leafNodes = 0;

void test_move_gen_depth()
{


	uint64_t total_move_time = 0;
	uint64_t start_time, elapsed;

	int depth = 5;
	
	uint64_t total_nodes = 0;

	for (int i = 0; i < NUM_EPD; i++) {
	
		struct EPD e = test_positions[i];

		printf("Analysing FEN to depth %d : '%s'\n", depth, e.fen);
		struct board *brd = init_game(e.fen);

		start_time = get_time_in_millis();

		////////////
		leafNodes = 0;
		perf_test(depth, brd);
		elapsed = get_elapsed_time_in_millis(start_time);

		assert_true(leafNodes == e.depth5);
		total_nodes += leafNodes;
		free(brd);
		//////////

		total_move_time += elapsed;
	}

	double moves_per_sec = ((double)total_nodes / ((double)total_move_time / 1000));
	printf("Total node count : %ju\n", total_nodes);
	printf("#moves/sec : %f\n", moves_per_sec);
	printf("Total elapsed time (ms) %ju\n", total_move_time);

}

void perf_test(int depth, struct board *brd)
{

	leafNodes = 0;
	uint64_t start_time, elapsed;

	struct move_list mv_list = {
		.moves = {0},
		.move_count = 0
	};

	start_time = get_time_in_millis();

	generate_all_moves(brd, &mv_list);

	mv_bitmap mv;
	for (uint32_t mv_num = 0; mv_num < mv_list.move_count; ++mv_num) {
		mv = mv_list.moves[mv_num];
		if (!make_move(brd, mv)) {
			continue;
		}
		perft(depth - 1, brd);
		take_move(brd);
	}
	elapsed = get_elapsed_time_in_millis(start_time);
	
	double nps = ((double)leafNodes / ((double)elapsed / 1000));
	
	printf("Test Complete : %ju nodes visited, elapsed (ms) %d, nodes/sec %f\n\n\n", leafNodes, (int)elapsed, nps);

	return;
}

void perft(int depth, struct board *brd)
{

	if (depth == 0) {
		leafNodes++;
		return;
	}


	struct move_list mv_list = {
		.moves = {0},
		.move_count = 0
	};

	generate_all_moves(brd, &mv_list);

	mv_bitmap mv;
	for (uint32_t mv_num = 0; mv_num < mv_list.move_count; ++mv_num) {
		mv = mv_list.moves[mv_num];
		if (make_move(brd, mv)) {
			perft(depth - 1, brd);
			take_move(brd);
		}
	}
	return;

}

///////////////////

void divide_perft(int depth, struct board *brd)
{

	//ASSERT_BOARD_OK(brd);

	//uint32_t move_cnt = 0;

	struct move_list mv_list = {
		.moves = {0},
		.move_count = 0
	};

	generate_all_moves(brd, &mv_list);

	mv_bitmap mv;
	for (uint32_t mv_num = 0; mv_num < mv_list.move_count; ++mv_num) {
		mv = mv_list.moves[mv_num];

		if (make_move(brd, mv)) {
			//move_cnt = divide((depth - 1), brd, mv);
			divide((depth - 1), brd);
			take_move(brd);

			//printf("%s %d\n", print_move(mv), move_cnt);
		} else {
			printf("Invalid move %s\n", print_move(mv));
		}

	}

	printf("\nTest Complete :%ju nodes visited\n", leafNodes);

	return;
}

uint32_t divide(int depth, struct board * brd)
{

	//ASSERT_BOARD_OK(brd);

	uint32_t nodes = 0;

	if (depth <= 0) {
		return 1;
	}

	struct move_list mv_list = {
		.moves = {0},
		.move_count = 0
	};

	generate_all_moves(brd, &mv_list);

	mv_bitmap mv;
	for (uint32_t mv_num = 0; mv_num < mv_list.move_count; ++mv_num) {
		mv = mv_list.moves[mv_num];
		if (make_move(brd, mv)) {
			nodes += divide((depth - 1), brd);
			take_move(brd);
		}
	}
	return nodes;
}

void bug_check(void)
{
	struct board *brd =
	    init_game
	    ("r3k2r/p1ppqpb1/bn2pnp1/3PN3/Pp2P3/2N2Q1p/1PPBBPPP/R3K2R b KQkq a3 0 1");

	leafNodes = 0;
	divide_perft((uint8_t) 3, brd);

	assert_true(leafNodes == 193690690);
}

void perf_test_fixture(void)
{
	test_fixture_start();	// starts a fixture

	run_test(test_move_gen_depth);

	//run_test(bug_check);

	test_fixture_end();	// ends a fixture
}
