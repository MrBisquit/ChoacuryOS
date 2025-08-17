/* Virtual File System Driver */

#include "virtualfs.h"
#include "../../memory/kmalloc.h"

void vfs_init(vfs_t* vfs) {
    vfs = (vfs_t*)kmalloc(sizeof(vfs_t*));
}