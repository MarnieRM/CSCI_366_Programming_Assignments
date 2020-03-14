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

#include "common.hpp"
#include "Client.hpp"

Client::~Client() {
}


void Client::initialize(unsigned int player, unsigned int board_size){
    vector<vector<int>> vect(board_size);
    for (int i = 0; i < board_size; i++) {
        vect[i].resize(board_size);
    }

    this->player = player;
    this->board_size = board_size;

    string filename = "player_";
    filename.append(to_string(player));
    filename.append(".action_board.json");
    this->board_name = filename;
    std::ofstream file(filename);
    cereal::JSONOutputArchive archive( file );
    archive(cereal::make_nvp("board", vect));
}


void Client::fire(unsigned int x, unsigned int y) {
    string filename = "player_";
    filename.append(to_string(player));
    filename.append(".shot.json");


    std::ofstream shotFile(filename);
    cereal::JSONOutputArchive archive( shotFile );
    archive( CEREAL_NVP(x), CEREAL_NVP(y) );

}


bool Client::result_available() {
    ifstream file("player_" + std::to_string(player) + ".result.json");
    return file.good();
}


int Client::get_result() {
    int r = 0;
    string filename_in = "player_" + std::to_string(player) + ".result.json";
    ifstream file(filename_in);
    cereal::JSONInputArchive result(file);
    result(r);
    remove(filename_in.c_str());
    if (r == HIT or r == MISS or r == OUT_OF_BOUNDS){
        return r;
    }
    else {
        throw "invalid result";
    }
}



void Client::update_action_board(int result, unsigned int x, unsigned int y) {
    if (result == OUT_OF_BOUNDS) {
        return;
    }

    vector<vector<int>> vect(board_size);
    string filename = "player_" + std::to_string(player) + ".action_board.json";
    ifstream in(filename);
    cereal::JSONInputArchive action_board_in(in);
    action_board_in(vect);

    vect[y][x] = result;
    ofstream out(filename);
    cereal::JSONOutputArchive action_board_out(out);
    action_board_out(cereal::make_nvp("board", vect));
}


string Client::render_action_board(){
}