#pragma once

#ifndef VFS_H
#define VFS_H

#include "../types.h"
#include "../device/dev.h"
#include "fat.h"

// Forward declarations
/*typedef struct vfs_fs_type vfs_fs_type_t;
typedef struct vfs_superblock vfs_superblock_t;
typedef struct vfs_inode vfs_inode_t;
typedef struct vfs_dentry vfs_dentry_t;
typedef struct vfs_file vfs_file_t;
typedef struct vfs_mount vfs_mount_t;*/

typedef struct vfs_fs_type vfs_fs_type_t;

typedef struct vfs {
    vfs_fs_type_t* g_fs_types;
} vfs_t;

// Definitions
typedef struct vfs_fs_type {
	const char* name;
    struct dev_device* dev;

	u8 (*mount)(struct vfs_fs_type* fs_type, struct device* dev, struct vfs_superblock** out);
	void (*unmount)(struct vfs_superblock* sb);

	struct vfs_fs_type* next;
} vfs_fs_type_t;

// Operations
typedef struct vfs_file_ops {
    u32 (*read)(struct vfs_file* file, void* buffer, u64 size, ulong* pos);
    u32 (*write)(struct vfs_file* file, const void* buffer, u64 size, ulong* pos);
    u8 (*flush)(struct vfs_file* file);
    u8 (*release)(struct vfs_file* file);
} vfs_file_ops_t;

void vfs_init(vfs_t* vfs);  // Initialize the VFS

bool vfs_add_type(vfs_fs_type_t* vfs);

// Filesystem conversion types
bool FAT_convert_vfs(FAT_filesystem_t* fs, vfs_fs_type_t* vfs); // Defined in fat.c

#endif