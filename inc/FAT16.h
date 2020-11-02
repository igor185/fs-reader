// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#ifndef READER_FAT16_H
#define READER_FAT16_H

#include <string>

struct fat_boot_sector_common
{
    uint8_t jump_addr[3]; // 0-2 assembly code
    uint8_t oem_name[8]; // 3-10 OEM name
    uint16_t bytes_per_sector; // 11-12 likely 512, can be 1024, 2048 or 4096
    uint8_t sectors_per_cluster; // 13, in sectors
    uint16_t reserved_sectors; // 14-15 sectors
    uint8_t tables_count; // 16 FAT count
    uint16_t max_files_n; // 17-18 max files in dir (0 for FAT32)
    uint16_t all_sectors_2; // 19-20 number of sectors or 0 and this number in 32-35 bytes
    uint8_t media_type; // 21, defines the media type, 0xf0=removable disk, 0xf8=fixed disk
    uint16_t sectors_per_table; // 22-23 size of each FAT, 0 for FAT32
    uint16_t sectors_per_track; // 24-25 sectors per track in storage device
    uint16_t number_of_heads; // 26-27 number of heads in storage device
    uint32_t hidden_sectors; // 28-31 number of sectors before the start partition
    uint32_t all_sectors_4; // 32-35 number of sectors if 19-20 is zero, zero otherwise
}__attribute__((packed));

struct fat_boot_sector_fat16
{
    uint8_t drive_number; // 36 BIOS 13h
    uint8_t reserved; // 37 not used
    uint8_t boot_signature; // 38 signature to validate next 3 fields (0x29)
    uint32_t volume_id; // 39-42 volume serial number
    uint8_t volume_label[11]; // 43-53 volume label
    uint8_t fs_type[8]; // 54-61 FS type level
    uint8_t not_used[448]; // 62-509 not used (loader?)
    uint16_t signature; // 510-511 signature value (0xaa55)
}__attribute__((packed));


struct fat_directory_entry_t{
    char filename[11]; // 0 - 0x00 unallocated, 0xe5 deleted, 1-10 file name with ext
    uint8_t attributes; // 11 - attr -> 0x01 read-only, 0x02 hidden, 0x04 system, 0x08 volume label, 0x0f long file name, 0x10 directory, 0x20 archive
    uint8_t reserved; // 12 reserved
    uint8_t creation_time_ms; // 13 ms
    uint16_t creat_time; // 14-15 hours, minutes, seconds
    uint16_t creat_date; // 16-17 date
    uint16_t access_date; // 18-19 access date
    uint16_t first_cluster_addr_high; // 20-21 0 for FAT12/16 or 2 high-order bytes of address of first cluster
    uint16_t modified_time; // 22-23 modified time
    uint16_t modified_date; // 24-25 modified date
    uint16_t first_cluster_addr_low; // 26-27 low-order 2 bytes of address of first cluster
    uint32_t file_size; // 28-31 file size ( 0 if it directory )
}__attribute__((packed));

void parse_fat16(const std::string& path);
#endif //READER_FAT16_H
