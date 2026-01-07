// include/storage/buffer_pool_manager.h
#pragma once
#include <unordered_map>
#include <list>
#include "storage/page.h"
#include "storage/disk_manager.h"

namespace db {

class BufferPoolManager {
public:
    BufferPoolManager(size_t pool_size, DiskManager *disk_manager);

    Page* FetchPage(page_id_t page_id);
    bool UnpinPage(page_id_t page_id, bool is_dirty);
    Page* NewPage(page_id_t *page_id);

private:
    size_t pool_size_;
    DiskManager *disk_manager_;

    std::unordered_map<page_id_t, Page*> page_table_;
    std::list<Page*> free_list_;
    std::list<Page*> lru_list_;
};

}
