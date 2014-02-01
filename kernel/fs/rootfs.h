#ifndef ______ROOTFS_H
#define ______ROOTFS_H

fs_node_t *init_rootfs(u32, ...);

fs_node_t *rootfs_finddir(fs_node_t *, char *);
struct dirent *rootfs_readdir(fs_node_t *, u32);

#endif
