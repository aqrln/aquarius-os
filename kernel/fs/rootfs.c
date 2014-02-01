#include <kernel.h>
#include <fs.h>
#include <rootfs.h>
#include <heap.h>
#include <tty.h>
#include <mem.h>
#include <string.h>

static fs_node_t *rootfs_nodes;
static u32 rootfs_nfiles;
static fs_node_t rootfs_node;
static struct dirent rootfs_dirent;

fs_node_t *init_rootfs(u32 nNodes, ...)
{
	rootfs_nfiles = nNodes;
	rootfs_nodes = (fs_node_t *) kmalloc(rootfs_nfiles * sizeof (fs_node_t));

	va_list args;
	va_start(args, nNodes);
	for (u32 i = 0; i < nNodes; i++) {
		fs_node_t *node = va_arg(args, fs_node_t *);
		memcpy((u8*)&rootfs_nodes[i], (u8*)node, sizeof(fs_node_t));
		rootfs_nodes[i].ptr = node;
		rootfs_nodes[i].inode = i;
		rootfs_nodes[i].flags = FS_MOUNTPOINT | FS_DIRECTORY;
	}
	va_end(args);

	memset((u8 *) &rootfs_node, 0, sizeof(fs_node_t));
	rootfs_node.flags = FS_DIRECTORY;
	rootfs_node.readdir = &rootfs_readdir;
	rootfs_node.finddir = &rootfs_finddir;

	return &rootfs_node;
}

struct dirent *rootfs_readdir(fs_node_t *node, u32 index)
{
	if (node == &rootfs_node && index < rootfs_nfiles) {
		strcpy(rootfs_dirent.name, rootfs_nodes[index].name);
		rootfs_dirent.ino = rootfs_nodes[index].inode;
		return &rootfs_dirent;
	}
	return 0;
}

fs_node_t *rootfs_finddir(fs_node_t *node, char *name)
{
	if (node == &rootfs_node) {
		u32 i;
		for (i = 0; i < rootfs_nfiles; i++) {
			if (!strcmp(rootfs_nodes[i].name, name))
				return &rootfs_nodes[i];
		}
	}
	return 0;
}
