#pragma once

#include <cstdint>
#include <cstring>

namespace db {

constexpr uint32_t PAGE_SIZE = 4096;
using page_id_t = int32_t;

class Page {
public:
    Page();

    // Disable copy
    Page(const Page&) = delete;
    Page& operator=(const Page&) = delete;

    // Data access
    inline char* GetData() { return data_; }
    inline const char* GetData() const { return data_; }

    // Page metadata
    inline page_id_t GetPageId() const { return page_id_; }
    inline void SetPageId(page_id_t page_id) { page_id_ = page_id; }

    inline int GetPinCount() const { return pin_count_; }
    inline void Pin() { pin_count_++; }
    inline void Unpin() { if (pin_count_ > 0) pin_count_--; }

    inline bool IsDirty() const { return is_dirty_; }
    inline void SetDirty(bool dirty) { is_dirty_ = dirty; }

    // Reset page content
    void ResetMemory();

private:
    page_id_t page_id_;
    int pin_count_;
    bool is_dirty_;
    char data_[PAGE_SIZE];
};

} // namespace db
