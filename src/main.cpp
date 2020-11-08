// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <cstring>
#include <iostream>
#include "FAT16.h"
#include "EXT2.h"

void print_help(){
    std::cout << "Usage: "
    << "-t|--type EXT|FAT16 -type of file system\n"
    << "-p|--path path - path to image\n"
    << "[-h|--help] - print this message" << std::endl;
    exit(EXIT_SUCCESS);
}

int main(int argc, char **argv) {
    std::string type, path;

//     type = "FAT16", path = "../data/fat16.img";
//     type = "EXT", path = "../data/ext2.img";


    for(int i = 0; i < argc; i++){
        if(strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "-help") == 0){
            print_help();
        }else if(strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "--type") == 0){
            if(i + 1 == argc)
                print_help();
            type = argv[++i];
        }else if(strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--path") == 0) {
            if (i + 1 == argc)
                print_help();
            path = argv[++i];
        }
    }

    if(type.empty() || path.empty())
        print_help();

    std::cout << "Parse: " << type << ". From: " << path << std::endl;
    if(type == "FAT16")
        parse_fat16(path);
    else if(type == "EXT")
        parse_ext(path);
    else
        print_help();

    return EXIT_SUCCESS;
}