#include "storage/buffer_pool_manager.h"
#include <cassert>

namespace db {

BufferPoolManager::BufferPoolManager(size_t pool_size, DiskManager *disk_manager)
    : pool_size_(pool_size), disk_manager_(disk_manager), pages_(pool_size) {

    for (frame_id_t i = 0; i < static_cast<frame_id_t>(pool_size_); i++) {
        free_list_.push_back(i);
    }
}

BufferPoolManager::~BufferPoolManager() {
    FlushAllPages();
}

frame_id_t BufferPoolManager::GetVictimFrame() {
    if (!free_list_.empty()) {
        frame_id_t fid = free_list_.front();
        free_list_.pop_front();
        return fid;
    }

    for (auto it = lru_list_.begin(); it != lru_list_.end(); ++it) {
        frame_id_t fid = *it;
        if (pages_[fid].GetPinCount() == 0) {
            lru_list_.erase(it);
            return fid;
        }
    }

    return -1; // no frame available
}

Page *BufferPoolManager::FetchPage(page_id_t page_id) {
    // 1. Page already in buffer pool
    if (page_table_.count(page_id)) {
        frame_id_t fid = page_table_[page_id];
        Page &page = pages_[fid];
        page.Pin();
        lru_list_.remove(fid);
        return &page;
    }

    // 2. Need new frame
    frame_id_t victim = GetVictimFrame();
    if (victim == -1) {
        return nullptr;
    }

    Page &page = pages_[victim];

    // 3. Evict old page if needed
    if (page.GetPageId() != -1) {
        if (page.IsDirty()) {
            disk_manager_->WritePage(page.GetPageId(), page.GetData());
        }
        page_table_.erase(page.GetPageId());
    }

    // 4. Read new page
    disk_manager_->ReadPage(page_id, page.GetData());
    page.SetPageId(page_id);
    page.SetDirty(false);
    page.Pin();

    page_table_[page_id] = victim;
    return &page;
}

bool BufferPoolManager::UnpinPage(page_id_t page_id, bool is_dirty) {
    if (!page_table_.count(page_id)) {
        return false;
    }

    frame_id_t fid = page_table_[page_id];
    Page &page = pages_[fid];

    if (page.GetPinCount() <= 0) {
        return false;
    }

    page.Unpin();
    if (is_dirty) {
        page.SetDirty(true);
    }

    if (page.GetPinCount() == 0) {
        lru_list_.push_back(fid);
    }

    return true;
}

bool BufferPoolManager::FlushPage(page_id_t page_id) {
    if (!page_table_.count(page_id)) {
        return false;
    }

    frame_id_t fid = page_table_[page_id];
    Page &page = pages_[fid];

    disk_manager_->WritePage(page_id, page.GetData());
    page.SetDirty(false);
    return true;
}

void BufferPoolManager::FlushAllPages() {
    for (auto &pair : page_table_) {
        frame_id_t fid = pair.second;
        Page &page = pages_[fid];
        if (page.IsDirty()) {
            disk_manager_->WritePage(page.GetPageId(), page.GetData());
            page.SetDirty(false);
        }
    }
}

Page *BufferPoolManager::NewPage(page_id_t *page_id) {
    frame_id_t victim = GetVictimFrame();
    if (victim == -1) {
        return nullptr;
    }

    Page &page = pages_[victim];

    if (page.GetPageId() != -1) {
        if (page.IsDirty()) {
            disk_manager_->WritePage(page.GetPageId(), page.GetData());
        }
        page_table_.erase(page.GetPageId());
    }

    *page_id = disk_manager_->AllocatePage();

    page.ResetMemory();
    page.SetPageId(*page_id);
    page.SetDirty(false);
    page.Pin();

    page_table_[*page_id] = victim;
    return &page;
}

} // namespace db
