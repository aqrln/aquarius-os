#include <kernel.h>
#include <fs.h>
#include <initrd.h>
#include <string.h>
#include <rootfs.h>

fs_node_t *fs_root = 0;

u32 fs_read(fs_node_t *node, u32 offset, u32 length, u8 *buffer)
{
	return node->read != 0? node->read(node, offset, length, buffer) : 0;
}

u32 fs_write(fs_node_t *node, u32 offset, u32 length, u8 *buffer)
{
	return node->write != 0? node->write(node, offset, length, buffer) : 0;
}

u32 fs_open(fs_node_t *node, u32 mode)
{
	if (node->open == 0)
		return 0;
	else if (node->flags & FS_MOUNTPOINT || node->flags == FS_SYMLINK)
		return fs_open(node->ptr, mode);
	else
		return node->open(node, mode);
}

u32 fs_close(fs_node_t *node)
{
	return node->close != 0? node->close(node) : 0;
}

struct dirent *fs_readdir(fs_node_t *node, u32 index)
{
	if (node -> readdir != 0 && (node->flags & 7) == FS_DIRECTORY) {
		return node->readdir(node, index);
	} else {
		return 0;
	}
}

fs_node_t *fs_finddir(fs_node_t *node, char *name)
{
	if (node->flags == FS_SYMLINK || node->flags & FS_MOUNTPOINT) {
		if (node->ptr)
			return fs_finddir(node->ptr, name);
		else
			return 0;
	}
	if (node->finddir != 0 && (node->flags & 7) == FS_DIRECTORY) {
		return node->finddir(node, name);
	} else {
		return 0;
	}
}

static fs_node_t *resolve_path(char *name, fs_node_t *dir)
{
	while (*name == '/')
		name++;
	char *slash = strchr(name, '/');
	if (!slash) {
		return fs_finddir(dir, name);
	} else {
		char curr_name[FILENAME_MAX];
		strncpy(curr_name, name, (u32)slash - (u32)name);
		fs_node_t *new_dir = fs_finddir(dir, curr_name);
		if (!new_dir)
			return 0;
		else
			return resolve_path(slash, new_dir);
	}
}

fs_node_t *open(char *filename, u32 mode)
{
	fs_node_t *file = resolve_path(filename, fs_root);
	if (!file)
		return 0;
	if (!fs_open(file, mode))
		return 0;
	else
		return file;
}

u32 close(fs_node_t *node)
{
	return fs_close(node);
}

extern fs_node_t *ird_root;
void init_fs(u32 initrd_location)
{
	fs_node_t *initrd_root = init_initrd(initrd_location);
	ird_root = initrd_root;
	fs_root = init_rootfs(1, initrd_root);
}
