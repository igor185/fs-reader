// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#ifndef READER_EXT2_H
#define READER_EXT2_H

#include <string>

# define OFFSET_SUPER_BLOCK 1024
# define MAGIC_SUPER_BLOCK 0xEF53

#define EXT2_VALID_FS    1    // Unmounted cleanly
#define EXT2_ERROR_FS    2  // Errors detected

#define EXT2_ERRORS_CONTINUE    1 // continue as if nothing happened
#define EXT2_ERRORS_RO    2 // remount read-only
#define EXT2_ERRORS_PANIC    3 // cause a kernel panic

#define EXT2_OS_LINUX    0    // Linux
#define EXT2_OS_HURD    1    // GNU HURD
#define EXT2_OS_MASIX    2    // MASIX
#define EXT2_OS_FREEBSD    3    // FreeBSD
#define EXT2_OS_LITES    4    // Lites

#define EXT2_GOOD_OLD_REV    0    // Revision 0
#define EXT2_DYNAMIC_REV    1    // Revision 1 with variable inode sizes, extended attributes, etc.

std::string get_date(uint16_t date);


void parse_ext(const std::string &path);

// https://www.nongnu.org/ext2-doc/ext2.html

struct s_super_block {
    uint32_t inodes_count; // 0-4 Count of inodes in the filesystem.
    uint32_t blocks_count; // 4-8 Count of blocks in the filesystem.
    uint32_t r_blocks_count; // 8-12 Count of the number of reserved blocks.
    uint32_t free_blocks_count; // 12-16 Count of the number of free blocks.
    uint32_t free_nodes_count; // 16-20 Count of the number of free inodes.
    uint32_t first_data_block; // 20-24 The first block which contains data.
    uint32_t log_block_size; // 24-28 Indicator of the block size.
    uint32_t log_frag_size; // 28-32 Indicator of the size of the fragments.
    uint32_t blocks_per_group; // 32-36 Count the number of blocks per group.
    uint32_t frags_per_group; // 36-40 Count of the number of fragments in each block group.
    uint32_t inodes_per_group; // 40-44 Count of the number of inodes in each block group.
    uint32_t mtime; // 44-48 The time that the filesystem was last mounted.
    uint32_t wtime; // 48-52 The time that the filesystem was last written to.
    uint16_t mnt_count; // 52-54 The number of times the file system has been mounted.
    uint16_t max_mnt_count; // 54-56 The number of times the file system can be mounted.
    uint16_t magic; // 56-58 Magic number indicating ex2fs.
    uint16_t state; // 58-60 Flags indicating the current state of the filesystem.
    uint16_t errors; // 60-62 Flags indicating the procedures for error reporting.
    uint16_t min_rev_level; // 62-64 Minor portion of version.
    uint32_t last_check; // 64-68 The time that the filesystem was last checked.
    uint32_t check_interval; // 68-72 The maximum time permissible between checks.
    uint32_t creator_os; // 72-76 Indicator of which OS created the filesystem.
    uint32_t rev_level; // 76-80 Major portion of version.
    uint16_t usr_id; // 80-82 User id.
    uint16_t grp_id; // 82-84 Group id.
}__attribute__((packed));

struct s_group_descriptor {
    uint32_t block_bitmap; // The address of the block containing.
    uint32_t inode_bitmap; // the block bitmap for this group.
    uint32_t inode_table; // The address of the block containing the inode bitmap for this group.
    uint16_t free_blocks_count; // The count of free blocks in this group.
    uint16_t free_inodes_count; // The count of free inodes in this group.
    uint16_t used_dirs_count; // The number inodes in this group which are directories.
    uint16_t pad; // Padding.
    uint32_t reserved[3]; // Padding;
}__attribute__((packed));

struct s_inode {
    uint16_t mode; // File mode.
    uint16_t uid; // Owner uid.
    uint32_t size; // Size in bytes.
    uint32_t atime; // Access time.
    uint32_t ctime; // Creation time.
    uint32_t mtime; // Modification time.
    uint32_t dtime; // Deletion time.
    uint16_t gid; // Group id.
    uint16_t links_count; // Links count.
    uint32_t blocks; // Blocks count.
    uint32_t flags; // File flags.
    uint32_t reserved1; // Os dependent.
    uint32_t block[15]; // Pointer to blocks.
    uint32_t version; // File version (for NFS).
    uint32_t file_acl; // File ACL.
    uint32_t dir_acl; // Directory ACL.
    uint32_t faddr; // Fragment address.
    uint8_t frag; // Fragment number.
    uint8_t fsize; // Fragment size.
    uint16_t pad1; // Padding.
    uint32_t reserved2[2]; // Padding.
}__attribute__ ((packed));

struct s_directory {
    uint32_t inode; // Address if inode.
    uint16_t rec_len; // Length of this record.
    uint8_t name_len; // Length of file name.
    uint8_t file_type;
    char name[255];
};

struct s_disk {
    void *fs;
    s_super_block *sblk;
    uint32_t block_size;
};

enum e_file_mode {
    S_IFMT = 0xF000, // Format mask.
    S_IFSOCK = 0xA000, // Socket.
    S_IFLNK = 0xC000, // Symbol link.
    S_IFREG = 0x8000, // Regular file.
    S_IFBLK = 0x6000, // Block device.
    S_IFDIR = 0x4000, // Directory.
    S_IFCHR = 0x2000, // Character device.
    S_IFIFO = 0x1000, // FIFO.

    S_ISUID = 0x0800, // SUID.
    S_ISGID = 0x0400, // SGID.
    S_ISVTX = 0x0200, // Sticky bit.

    S_IRWXU = 0x01C0, // User mask.
    S_IRUSR = 0x0100, // Read.
    S_IWUSR = 0x0080, // Write.
    S_IXUSR = 0x0040, // Execute.

    S_IRWXG = 0x0038, // Group mask.
    S_IRGRP = 0x0020, // Read.
    S_IWGRP = 0x0010, // Write.
    S_IXGRP = 0x0008, // Execute.

    S_IRWXO = 0x0007, // Other mask.
    S_IROTH = 0x0004, // Read.
    S_IWOTH = 0x0002, // Write.
    S_IXOTH = 0x0001, // Execute.
};

enum e_special_inodes {
    EXT2_BAD_INO = 1, // Bad blocks inode.
    EXT2_ROOT_INO = 2, // Root inodes.
    EXT2_ACL_IDX_INO = 3, // ACL inode.
    EXT2_ACL_DATA_INO = 4, // ACL inode.
    EXT2_BOOT_LOADER_INO = 5, // Boot loader inode.
    EXT2_UNDEL_DIR_INO = 6, // Undelete directory inode.
    EXT2_FIRST_INO = 11,// First non reserved inode.
};

enum FILE_TYPE {
    EXT2_FT_UNKNOWN = 0, //	Unknown File Type
    EXT2_FT_REG_FILE = 1,    //Regular File
    EXT2_FT_DIR = 2, //	Directory File
    EXT2_FT_CHRDEV = 3, //Character Device
    EXT2_FT_BLKDEV = 4, //	Block Device
    EXT2_FT_FIFO = 5,//	Buffer File
    EXT2_FT_SOCK = 6,//	Socket File
    EXT2_FT_SYMLINK = 7//	Symbolic Link
};


#endif //READER_EXT2_H
