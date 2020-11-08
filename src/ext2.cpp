// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <fstream>
#include <iostream>
#include <cstring>
#include <chrono>
#include <iomanip>

#include "EXT2.h"

std::string get_date(uint32_t date_unix) {
    std::time_t temp = date_unix;
    std::tm *t = std::gmtime(&temp);
    std::stringstream ss;
    ss << std::put_time(t, "%F:%X");
    return ss.str();
}

std::string get_state(uint16_t state) {
    if (state == EXT2_VALID_FS)
        return "Unmounted cleanly";

    if (state == EXT2_ERROR_FS)
        return "Errors detected";

    return "Unknown";
}

std::string get_errors(uint16_t errors) {
    if (errors == EXT2_ERRORS_CONTINUE)
        return "Continue as if nothing happened";

    if (errors == EXT2_ERRORS_RO)
        return "remount read-only";

    if (errors == EXT2_ERRORS_PANIC)
        return "cause a kernel panic";

    return "Unknown";
}

std::string get_creator_os(uint32_t creator_os) {
    switch (creator_os) {
        case EXT2_OS_LINUX:
            return "Linux";
        case EXT2_OS_HURD:
            return "GNU HURD";
        case EXT2_OS_MASIX:
            return "MASIX";
        case EXT2_OS_FREEBSD:
            return "FreeBSD";
        case EXT2_OS_LITES:
            return "Lites";
        default:
            return std::to_string(creator_os);
    }
}

void print_access(uint16_t mode) {
    printf("Access:                            %c%c%c%c%c%c%c%c%c\n",
           mode & S_IRUSR ? 'r' : '-',
           mode & S_IWUSR ? 'w' : '-',
           mode & S_IXUSR ? 'e' : '-',
           mode & S_IRGRP ? 'r' : '-',
           mode & S_IWGRP ? 'w' : '-',
           mode & S_IXGRP ? 'e' : '-',
           mode & S_IROTH ? 'r' : '-',
           mode & S_IWOTH ? 'w' : '-',
           mode & S_IXOTH ? 'e' : '-');
}

std::string get_type(uint16_t type) {
    if (type & S_IFSOCK)
        return "Socket";
    if (type & S_IFLNK)
        return "Symbol link";
    if (type & S_IFBLK)
        return "Block device";
    if (type & S_IFDIR)
        return "Directory";
    if (type & S_IFCHR)
        return "Character device";
    if (type & S_IFIFO)
        return "FIFO";
    if (type & S_IFREG)
        return "Regular file";
    return "Unknown";

}

std::string get_file_type(uint8_t file_type) {
    switch (file_type) {
        case EXT2_FT_DIR:
            return "Directory";
        case EXT2_FT_CHRDEV:
            return "Character Device";
        case EXT2_FT_BLKDEV:
            return "Block Device";
        case EXT2_FT_FIFO:
            return "Buffer File";
        case EXT2_FT_SOCK:
            return "Socket File";
        case EXT2_FT_SYMLINK:
            return "Symbolic Link";
        case EXT2_FT_REG_FILE:
            return "Regular File";
        case EXT2_FT_UNKNOWN:
        default:
            return "Unknown";
    }
}


void read_super_block(s_super_block *block, const std::string &data, off_t offset) {
    memcpy(block, data.c_str() + offset, sizeof(s_super_block));
}

void read_group(s_group_descriptor *group, const std::string &data, off_t offset) {
    memcpy(group, data.c_str() + offset, sizeof(s_group_descriptor));
}

void read_inode(s_inode *inode, const std::string &data, off_t offset) {
    memcpy(inode, data.c_str() + offset, sizeof(s_inode));
}

void read_dirs(s_directory *dir, const std::string &data, off_t &offset, uint16_t &rec_len) {
    memcpy(&dir->inode, data.c_str() + offset, 4);
    offset += 4;

    memcpy(&dir->rec_len, data.c_str() + offset, 2);
    offset += 2;

    memcpy(&dir->name_len, data.c_str() + offset, 1);
    offset += 1;

    memcpy(&dir->file_type, data.c_str() + offset, 1);
    offset += 1;

    memcpy(&dir->name, data.c_str() + offset, dir->name_len);
    offset += dir->name_len;
    dir->name[dir->name_len] = '\0';

    offset += dir->rec_len - 4 - 2 - 1 - 1 - dir->name_len;
    rec_len = dir->rec_len;
}

void read_inode_value(s_inode inode, const std::string &data, unsigned int block_size) {
    size_t i = 0;
    off_t offset;
    s_directory dir{};

    while (inode.block[i] != 0 && i <= 11) { // TODO add for other blocks (should be link to another tables)
        offset = block_size * inode.block[i];

        uint16_t rec_len = 0;

        while (rec_len != block_size) {

            read_dirs(&dir, data, offset, rec_len);

            printf("Inode:                             %d\n", dir.inode);
            printf("Record length:                     %d\n", dir.rec_len);
            printf("Name length:                       %d\n", dir.name_len);
            printf("File type:                         %s\n", get_file_type(dir.file_type).c_str());
            printf("Name:                              %s\n\n", dir.name);
        }
        i++;
    }
}


void parse_ext(const std::string &path) {
    std::ifstream infile{path, std::ios::binary};

    std::string data((std::istreambuf_iterator<char>(infile)),
                     std::istreambuf_iterator<char>());


    s_super_block sp_block{};

    read_super_block(&sp_block, data, OFFSET_SUPER_BLOCK);

    unsigned int block_size = 1024u << sp_block.log_block_size;

    printf("Super block info: \n\n");
    printf("Inode count:                       %d\n", sp_block.inodes_count);
    printf("Blocks count:                      %d\n", sp_block.blocks_count);
    printf("Reserved blocks count:             %d\n", sp_block.r_blocks_count);
    printf("Free blocks:                       %d\n", sp_block.free_blocks_count);
    printf("Free nodes:                        %d\n", sp_block.free_nodes_count);
    printf("First data block:                  %d\n", sp_block.first_data_block);
    printf("Block size:                        %d\n", block_size);
    printf("Blocks per group:                  %d\n", sp_block.blocks_per_group);
    printf("Inodes per group:                  %d\n", sp_block.inodes_per_group);
    printf("Was mounted:                       %s\n", get_date(sp_block.mtime).c_str());
    printf("Last write:                        %s\n", get_date(sp_block.wtime).c_str());
    printf("Last check:                        %s\n", get_date(sp_block.last_check).c_str());
    printf("Mounted count:                     %d\n", sp_block.mnt_count);
    printf("Max mounted count:                 %d\n", sp_block.max_mnt_count);
    printf("Magic number:                      %s\n", sp_block.magic == MAGIC_SUPER_BLOCK ? "valid" : "invalid");
    printf("State:                             %s\n", get_state(sp_block.state).c_str());
    printf("Errors:                            %s\n", get_errors(sp_block.errors).c_str());
    printf("OS created FS:                     %s\n", get_creator_os(sp_block.creator_os).c_str());
    printf("Revision:                          %d\n", sp_block.rev_level);
    printf("User id:                           %d\n", sp_block.usr_id);
    printf("Group id:                          %d\n", sp_block.grp_id);


    s_group_descriptor first_group{};
    read_group(&first_group, data, block_size);

    printf("\n\nFirst group descriptor info: \n\n");
    printf("Block bitmap index:                %d\n", first_group.block_bitmap);
    printf("Inode bitmap index:                %d\n", first_group.inode_bitmap);
    printf("Inode table index:                 %d\n", first_group.inode_table);
    printf("Free blocks:                       %d\n", first_group.free_blocks_count);
    printf("Free inodes:                       %d\n", first_group.free_inodes_count);
    printf("Amount dirs in dir:                %d\n", first_group.used_dirs_count);

    s_inode root_inode{};
    read_inode(&root_inode, data, (first_group.inode_table) * block_size + sizeof(s_inode));

    printf("\n\nRoot inode info: \n\n");

    print_access(root_inode.mode);
    printf("Type:                              %s\n", get_type(root_inode.mode).c_str());
    printf("Size:                              %d\n", root_inode.size);
    printf("Inode:                             %d\n", root_inode.size);
    printf("Access time:                       %s\n", get_date(root_inode.atime).c_str());
    printf("Create time:                       %s\n", get_date(root_inode.ctime).c_str());
    printf("Modified time:                     %s\n", get_date(root_inode.mtime).c_str());
    printf("Links count:                       %d\n", root_inode.links_count);
    printf("Amount of blocks:                  %d\n", root_inode.blocks);
    printf("Block number of first block:       %d\n", root_inode.block[0]);


    printf("\n\nFiles in root dir: \n\n");
    read_inode_value(root_inode, data, block_size);
}


