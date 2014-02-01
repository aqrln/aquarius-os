#ifndef _INITRD_H___
#define _INITRD_H___

typedef struct {
	u32 magic;
	char *name;
	u8* data;
	u32 size;
} ird_header_t;

// 'aqinitrd' in ascii -- 8 bytes
#define INITRD_SIGNATURE_1 0x6E697161
#define INITRD_SIGNATURE_2 0x64727469

#define INITRD_MAGIC 0x12345678

fs_node_t* init_initrd(u32 location);

struct dirent *initrd_readdir(fs_node_t *, u32);
struct fs_node *initrd_finddir(fs_node_t *, char *);
u32 initrd_read(fs_node_t *, u32, u32, u8 *);
u32 initrd_open(fs_node_t *, u32);
u32 initrd_close(fs_node_t *);

#endif
