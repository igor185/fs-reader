// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "FAT16.h"

int main(int argc, char **argv) {

    parse_fat16("../data/fat16.img");

    return EXIT_SUCCESS;
}