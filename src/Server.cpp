//    Battleship game assignment for MSU CSCI 366
//    Copyright (C) 2020    Mike P. Wittie
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <https://www.gnu.org/licenses/>.


#include <map>
#include <cereal/types/map.hpp>
#include "common.hpp"
#include "Server.hpp"


/**
 * Calculate the length of a file (helper function)
 *
 * @param file - the file whose length we want to query
 * @return length of the file in bytes
 */
int get_file_length(ifstream *file){
}


void Server::initialize(unsigned int board_size,
                        string p1_setup_board,
                        string p2_setup_board) {

    if(board_size != BOARD_SIZE) {
        throw "Wrong board size.";
    }
    if(p1_setup_board != "player_1.setup_board.txt" and p2_setup_board != "player_2.setup_board.txt"){
        throw "Wrong name";
    }
    this->p1_setup_board = ifstream(p1_setup_board);
    this->p2_setup_board = ifstream(p2_setup_board);
    this->board_size = board_size;
}


int Server::evaluate_shot(unsigned int player, unsigned int x, unsigned int y) {
    string ships = SHIPS;
    if (x >= board_size or y >= board_size or x < 0 or y < 0){
        return OUT_OF_BOUNDS;
    }
    if (player < 1){
        throw "player number too low";
    }
    if (player > MAX_PLAYERS){
        throw "player number too high";
    }

    if (player == 1) {
        string line;
        for (int i = 0; i <= y; i++) {
            getline(p2_setup_board, line);
        }
        char loc = line.c_str()[x];
        if (loc == 'C' or loc == 'B' or loc == 'R' or loc == 'S' or loc == 'D') {
            return HIT;
        } else {
            return MISS;
        }
    }

    if (player == 2) {
        string line;
        for (int i = 0; i <= y; i++) {
            getline(p1_setup_board, line);
        }
        char loc = line.c_str()[x];
        if (loc == 'C' or loc == 'B' or loc == 'R' or loc == 'S' or loc == 'D') {
            return HIT;
        } else {
            return MISS;
        }
    }
}


int Server::process_shot(unsigned int player) {
    int x = 0;
    int y = 0;

    string filename_in = "player_" + std::to_string(player) + ".shot.json";
    ifstream file(filename_in);
    cereal::JSONInputArchive shot(file);
    shot(x, y);


    string filename_out = "player_";
    filename_out.append(to_string(player));
    filename_out.append(".result.json");

    int result = evaluate_shot(player, x, y);
    std::ofstream shotFile(filename_out);
    cereal::JSONOutputArchive archive( shotFile );
    archive( CEREAL_NVP(result) );
    remove(filename_in.c_str());

}