#ifndef ___FILL_FS_NODE_H
#define ___FILL_FS_NODE_H

#include <string.h>

__inline__ static void fill_fs_node (
		fs_node_t   node,
		char      * name,
		u32         flags,
		u32         inode,
		u32         impl,
		u32         size,
		fs_node_t * ptr,
		read_function_t    read,
		write_function_t   write,
		open_function_t    open,
		close_function_t   close,
		readdir_function_t readdir,
		finddir_function_t finddir
	)
{
	strcpy(node.name, name);
	u32 len = strlen(name);
	if (len >= FILENAME_MAX) {
		warning("vfs: filename is too long");
	}
	node.name[len] = 0;

	node.flags   = flags;
	node.inode   = inode;
	node.impl    = impl;
	node.size    = size;
	node.ptr     = ptr;
	node.read    = read;
	node.write   = write;
	node.open    = open;
	node.close   = close;
	node.readdir = readdir;
	node.finddir = finddir;
}

__inline__ static void fill_fs_pnode (
		fs_node_t * node,
		char      * name,
		u32         flags,
		u32         inode,
		u32         impl,
		u32         size,
		fs_node_t * ptr,
		read_function_t    read,
		write_function_t   write,
		open_function_t    open,
		close_function_t   close,
		readdir_function_t readdir,
		finddir_function_t finddir
	)
{
	strcpy(node->name, name);
	u32 len = strlen(name);
	if (len >= FILENAME_MAX) {
		warning("vfs: filename is too long");
	}
	node->name[len] = 0;

	node->flags   = flags;
	node->inode   = inode;
	node->impl    = impl;
	node->size    = size;
	node->ptr     = ptr;
	node->read    = read;
	node->write   = write;
	node->open    = open;
	node->close   = close;
	node->readdir = readdir;
	node->finddir = finddir;
}

#endif
