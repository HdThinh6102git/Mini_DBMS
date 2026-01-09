#pragma once

#include <unordered_map>
#include <list>
#include <vector>

#include "storage/page.h"
#include "storage/disk_manager.h"

namespace db {

using frame_id_t = int32_t;

class BufferPoolManager {
public:
    BufferPoolManager(size_t pool_size, DiskManager *disk_manager);
    ~BufferPoolManager();

    Page *FetchPage(page_id_t page_id);
    bool UnpinPage(page_id_t page_id, bool is_dirty);
    bool FlushPage(page_id_t page_id);
    void FlushAllPages();

    Page *NewPage(page_id_t *page_id);

private:
    size_t pool_size_;
    DiskManager *disk_manager_;

    std::vector<Page> pages_;  // frame array
    std::unordered_map<page_id_t, frame_id_t> page_table_;

    std::list<frame_id_t> free_list_;
    std::list<frame_id_t> lru_list_;

    frame_id_t GetVictimFrame();
};

} // namespace db
