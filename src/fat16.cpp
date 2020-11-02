// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <cstdio>
#include <iostream>
#include <fstream>
#include <cstdint>

#include "FAT16.h"

std::string parse_filename(const std::string& filename) {
    std::string res;

    for(size_t i = 0; i < 11; i++){
        if(filename[i] != ' ') {
            if(i == 8){
                res += ".";
            }
            res += filename[i];
        }
    }

    return res;
}

std::string get_time(uint16_t date, uint16_t time){
    tm time_st = {};
    time_st.tm_mday = date & 0x1F;
    time_st.tm_mon = ((date >> 5) & 0x0F) - 1;
    time_st.tm_year = ((date >> 9) & 0x7F) + 80;

    time_st.tm_sec = (time & 0x1F) * 2;
    time_st.tm_min = (time >> 5) & 0x3F;
    time_st.tm_hour = (time >> 11) & 0x1F;

    char buff[20];
    strftime (buff, sizeof(buff), "%F:%X", &time_st);

    return buff;
}

void parse_fat16(const std::string &path) {
    std::ifstream fs{"../data/fat16.img", std::ios::binary};


    fat_boot_sector_common bs_info{};
    fs.read((char *) &bs_info, sizeof(fat_boot_sector_common));

    printf("OEM name:                   %s\n", bs_info.oem_name);
    printf("Bytes per sector:           %d\n", bs_info.bytes_per_sector);
    printf("Sectors per cluster:        %d\n", bs_info.sectors_per_cluster);
    printf("Reserved sectors:           %d\n", bs_info.reserved_sectors);
    printf("FATS count:                 %d\n", bs_info.tables_count);
    printf("Max files number:           %d\n", bs_info.max_files_n);
    printf("Sectors per table:          %d\n", bs_info.sectors_per_table);
    printf("Number of sectors 2b:       %d\n", bs_info.all_sectors_2);
    printf("Number of sectors 4b:       %d\n", bs_info.all_sectors_4);
    printf("Media type:                 %s\n",
           bs_info.media_type == 0xf0 ? "removable disk" : (bs_info.media_type == 0xf8 ? "fixed disk" : "unknown"));
    printf("Size of each FAT(sectors):  %d\n", bs_info.sectors_per_track);
    printf("Heads in storage dev:       %d\n", bs_info.number_of_heads);
    printf("Sectors before start part:  %d\n", bs_info.hidden_sectors);

    fat_boot_sector_fat16 bs_info_16{};
    fs.read((char *) &bs_info_16, sizeof(fat_boot_sector_fat16));

    printf("Drive number:               %d\n", bs_info_16.drive_number);
    printf("Extended boot signature:    %s\n", bs_info_16.boot_signature == 0x29 ? "valid" : "invalid");
    printf("Volume serial number:       %d\n", bs_info_16.volume_id);
    printf("Volume serial label:        %s\n", bs_info_16.volume_label);
    printf("File system type level:     %s\n", bs_info_16.fs_type);
    printf("Signature:                  %s\n", bs_info_16.signature == 0xaa55 ? "valid" : "invalid");
    printf("Size of boot struct:        %lu\n", sizeof(bs_info) + sizeof(bs_info_16));

    // skip FAT and copies
    fs.ignore(bs_info.tables_count * bs_info.sectors_per_table * bs_info.bytes_per_sector);

    printf("\nFiles in root directory: \n\n");
    int i = 0, file_num = 0;
    fat_directory_entry_t root_dir{};
    while (i < bs_info.max_files_n) {
        fs.read((char *) &root_dir, sizeof(fat_directory_entry_t));
        if (root_dir.filename[0] != 0x00 && static_cast<unsigned char>(root_dir.filename[0]) != 0xe5) {
            printf("Name: %s\n", parse_filename(root_dir.filename).c_str());
            printf("Size: %s\n", (root_dir.attributes & 0x10) ? "directory" : std::to_string(root_dir.file_size).c_str());
            printf("Created: %s\n", get_time(root_dir.creat_date, root_dir.creat_time).c_str());
            printf("Modified: %s\n", get_time(root_dir.modified_date, root_dir.modified_time).c_str());
            printf("Attributes: \n");
            if (root_dir.attributes & 0x01 ) printf("\tRead-only\n");
            if (root_dir.attributes & 0x02 ) printf("\tHidden file\n");
            if (root_dir.attributes & 0x04 ) printf("\tSystem file\n");
            if (root_dir.attributes & 0x08 ) printf("\tVolume label\n");
            if (root_dir.attributes & 0x0f ) printf("\tLong file name\n");
            if (root_dir.attributes & 0x20 ) printf("\tArchive\n");


            printf("\n");
            file_num++;
        }
        i++;
    }

    printf("Total number of files: %d\n", file_num);
}