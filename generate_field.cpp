#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>


void hot_top__cold_rest(std::ofstream &out_file, const unsigned int &height, const unsigned int &width){
    int i, j;

    for (j = 0; j < width; j++){
        out_file << 100 << " ";
    }
    out_file << std::endl;
    for (i = 1; i < height; i++) {
        for (j = 0; j < width; j++) {
            out_file << 0 << " ";
        }
        out_file << std::endl;
    }
}


void cold_edges__hot_middle(std::ofstream &out_file, const unsigned int &height, const unsigned int &width){
    int i, j;

    for (i = 0; i < height / 3; i++) {
        for (j = 0; j < width; j++) {
            out_file << 0 << " ";
        }
        out_file << std::endl;
    }
    for (i = 1; i < height / 3; i++) {
        for (j = 0; j < width / 3; j++) {
            out_file << 0 << " ";
        }
        for (j = 0; j < width / 3; j++) {
            out_file << 100 << " ";
        }
        for (j = 0; j < (width / 3) + (width % 3); j++) {
            out_file << 0 << " ";
        }
        out_file << std::endl;
    }
    for (i = 0; i < (height / 3) + (height % 3); i++) {
        for (j = 0; j < width; j++) {
            out_file << 0 << " ";
        }
        out_file << std::endl;
    }
}


void hot_rigth_top__cold_rest(std::ofstream &out_file, const unsigned int &height, const unsigned int &width){
    int i, j;
    for (j = 0; j < width - 1; j++){
        out_file << 0 << " ";
    }
    out_file << 100 << std::endl;

    for (i = 1; i < height; i++){
        for (j = 0; j < width; j++){
            out_file << 0 << " ";
        }
        out_file << std::endl;
    }
}

void hot_top__middle_right_left__cold_bottom_middle(std::ofstream &out_file, const unsigned int &height, const unsigned int &width){
    int i, j;

    for (j = 0; j < width; j++){
        out_file << 100 << " ";
    }
    out_file << std::endl;
    for (i = 1; i < height; i++) {
        out_file << 50 << " ";
        for (j = 1; j < width - 1; j++) {
            out_file << 0 << " ";
        }
        out_file << 50 << " ";
        out_file << std::endl;
    }
}


int main(int argc, char* argv[]){
    std::string out_filename = "field.txt";
    unsigned int height = 1000;
    unsigned int width = 1000;

    if (argc > 2){
        std::istringstream iss1( argv[1] );
        unsigned int val;
        if (iss1 >> val){
            height = val;
        } else{
            std::cerr << "Invalid first argument. Should be an unsigned integer -- height" << std::endl;
            return -1;
        }

        std::istringstream iss2( argv[2] );
        if (iss2 >> val){
            width = val;
        } else{
            std::cout << argv[2];
            std::cerr << "Invalid second argument. Should be an unsigned integer -- width" << std::endl;
            return -2;
        }

        if (argc > 3){
            out_filename = argv[3];
        } else{
            std::cout << "Out filename: " << out_filename << std::endl;
        }

    } else{
        std::cout << "Height: " << height << "\nWidth: " << width << "\nOut filename: " << out_filename << std::endl;
    }

    std::ofstream out_file(out_filename);
    if (out_file.good()) {
        hot_top__cold_rest(out_file, height, width);
    } else{
        std::cerr << "Something wrong with out file." << std::endl;
        return -3;
    }

    out_file.close();
    return 0;
}