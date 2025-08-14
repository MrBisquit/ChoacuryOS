#pragma once

#ifndef DEV_H
#define DEV_H

#include "../types.h"

// Forward declarations
/*typedef enum dev_device_type dev_device_type_t;
typedef struct dev_block_ops dev_block_ops_t;
typedef struct dev_char_ops dev_char_ops_t;
typedef struct dev_stream_ops dev_stream_ops_t;
typedef struct dev_device dev_device_t;*/

// Device types (For VFS)
// Devices must be registered here before they can be used in the VFS
typedef enum dev_device_type {
    DEV_BLOCK,      // Disk
    DEV_CHAR,       // Serial, terminal, etc
    DEV_STREAM      // Network
} dev_device_type_t;

typedef struct dev_block_ops {
	int (*open)(void* priv);
	int (*close)(void* priv);
	u32 (*read)(void* priv, void* buf, u32 sz, s64 off);
	u32 (*write)(void* priv, const void* buf, u32 sz, s64 off);
	int (*ioctl)(void* priv, ulong cmd, ulong arg);
} dev_block_ops_t;

typedef struct dev_char_ops {
	u32 (*read)(void* priv, void* buf, u32 sz);
	u32 (*write)(void* priv, const void* buf, u32 sz);
	int (*ioctl)(void* priv, ulong cmd, ulong arg);
} dev_char_ops_t;

typedef struct dev_stream_ops {
	u32 (*read)(void* priv, void* buf, u32 sz);
	u32 (*write)(void* priv, const void* buf, u32 sz);
	int (*flush)(void* priv);
	int (*close)(void* priv);
} dev_stream_ops_t;

typedef struct dev_device {
	const char* scheme;
	u64 num;
	enum dev_device_type type;
	void* private_data;
    uptr ptr; // Pointer to origin
              // All DEV_BLOCK devices will be registered as storage_device_t

	union {
		struct dev_block_ops* block;
		struct dev_char_ops* ch;
		struct dev_stream_ops* stream;
	} ops;

	struct dev_device* next;
} dev_device_t;

//static struct dev_device_t** device_list;

#endif