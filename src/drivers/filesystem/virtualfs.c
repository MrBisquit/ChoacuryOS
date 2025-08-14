/* Virtual File System Driver */

#include "virtualfs.h"
#include "../../memory/kmalloc.h"

void vfs_init(vfs_t* vfs) {
    vfs = (vfs_t*)kmalloc(sizeof(vfs_t*));
}

bool FAT_register_vfs(FAT_filesystem_t* fs, vfs_fs_type_t* vfs) {

}