#ifndef ___FS_H
#define ___FS_H

#define FILENAME_MAX 128

#define FS_FILE        1
#define FS_DIRECTORY   2
#define FS_CHARDEVICE  3
#define FS_BLOCKDEVICE 4
#define FS_PIPE        5
#define FS_SYMLINK     6
#define FS_MOUNTPOINT  8

typedef struct fs_node fs_node_t;

typedef u32 (*read_function_t)(fs_node_t *, u32 offset, u32 length, u8 *buffer);
typedef u32 (*write_function_t)(fs_node_t *, u32 offset, u32 length, u8 *buffer);
typedef u32 (*open_function_t)(fs_node_t *, u32 mode);
typedef u32 (*close_function_t)(fs_node_t *);
typedef struct dirent * (*readdir_function_t)(fs_node_t *, u32 index);
typedef struct fs_node *  (*finddir_function_t)(fs_node_t *, char *name);

struct dirent {
	char name[FILENAME_MAX];
	u32  ino;
};

struct fs_node {
	char name[FILENAME_MAX];
	u32  flags;
	u32  inode;
	u32  impl;            // an implementation-defined number
	u32  size;

	struct fs_node *ptr;  // used by links and mountpoints

	read_function_t    read;
	write_function_t   write;
	open_function_t    open;
	close_function_t   close;
	readdir_function_t readdir;
	finddir_function_t finddir;
};

extern fs_node_t *fs_root;

u32 fs_read(fs_node_t *, u32, u32, u8*);
u32 fs_write(fs_node_t *, u32, u32, u8*);
u32 fs_open(fs_node_t*, u32 mode);
u32 fs_close(fs_node_t*);
struct dirent * fs_readdir(fs_node_t *, u32);
fs_node_t * fs_finddir(fs_node_t *, char *);

#define O_READ  1
#define O_WRITE 2
#define O_RDWR  3

fs_node_t *open(char *, u32);
u32 close(fs_node_t*);

void init_fs(u32 initrd_location);

#endif
