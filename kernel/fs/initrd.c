#include <kernel.h>
#include <fs.h>
#include <initrd.h>
#include <tty.h>
#include <panic.h>
#include <heap.h>
#include <mem.h>
#include <string.h>

static ird_header_t *initrd;
static u32 initrd_nfiles;

static fs_node_t initrd_root_node;
static fs_node_t *initrd_nodes;

static struct dirent initrd_dirent;

fs_node_t* init_initrd(u32 location)
{
	u32 *p_ird = (u32 *) location;
	assert(p_ird[0] == INITRD_SIGNATURE_1);
	assert(p_ird[1] == INITRD_SIGNATURE_2);
	assert(p_ird[2] > 0);
	initrd_nfiles = p_ird[2];
	initrd = (ird_header_t *) location;
	initrd++;

	for (u32 i = 0; i < initrd_nfiles; i++) {
		assert(initrd[i].magic == INITRD_MAGIC);
		initrd[i].name += location;
		initrd[i].data += location;
	}

	memset((u8*)&initrd_root_node, 0, sizeof(initrd_root_node));
	strcpy(initrd_root_node.name, "initrd");
	initrd_root_node.flags   = FS_DIRECTORY;
	initrd_root_node.readdir = &initrd_readdir;
	initrd_root_node.finddir = &initrd_finddir;

	initrd_nodes = (fs_node_t *) kmalloc(initrd_nfiles * sizeof (struct fs_node));
	for (u32 i = 0; i < initrd_nfiles; i++) {
		memset(&initrd_nodes[i], 0, sizeof(fs_node_t));
		strcpy(initrd_nodes[i].name, initrd[i].name);
		initrd_nodes[i].flags = FS_FILE;
		initrd_nodes[i].inode = i;
		initrd_nodes[i].size  = initrd[i].size;
		initrd_nodes[i].read  = &initrd_read;
		initrd_nodes[i].open  = &initrd_open;
		initrd_nodes[i].close = &initrd_close;
	}

	return &initrd_root_node;
}

struct dirent * initrd_readdir(fs_node_t *node, u32 index)
{
	if (node == &initrd_root_node) {
		if (index >= initrd_nfiles)
			return 0;
		memset(initrd_dirent.name, 0, FILENAME_MAX);
		strcpy(initrd_dirent.name, initrd_nodes[index].name);
		initrd_dirent.ino  = initrd_nodes[index].inode;
		return &initrd_dirent;
	} else {
		return 0;
	}
}

struct fs_node * initrd_finddir(fs_node_t *node, char *name)
{
	if (node == &initrd_root_node) {
		for (u32 i = 0; i < initrd_nfiles; i++) {
			if (!strcmp(initrd_nodes[i].name, name))
				return &initrd_nodes[i];
		}
	}
	return 0;
}

u32 initrd_open(fs_node_t *node, u32 mode)
{
	if (mode & O_WRITE)
		return 0;
	bool found = false;
	for (u32 i = 0; i < initrd_nfiles; i++) {
		if (&initrd_nodes[i] == node) {
			found = true;
			break;
		}
	}
	if (!found)
		return 0;
	else
		return (u32)node;
}

u32 initrd_close(fs_node_t *node)
{
	if (node == &initrd_root_node)
		return 0; // trololo
	else
		return 1; // hmmm...
}

u32 initrd_read(fs_node_t *node, u32 offset, u32 length, u8 *buffer)
{
	u32 index;
	for (index = 0; index < initrd_nfiles; index++) {
		if (&initrd_nodes[index] == node)
			break;
	}
	if (index >= initrd_nfiles)
		return 0; // didn't find the node
	u8 *src = initrd[index].data + offset;
	u32 maxsize = initrd[index].size - offset;
	u32 size = length <= maxsize? length : maxsize;
	memcpy(buffer, src, size);
	return size;
}
