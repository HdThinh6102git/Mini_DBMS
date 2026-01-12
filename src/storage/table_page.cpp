#include "storage/table_page.h"
#include <cstring>

namespace db {

/**
 * Page Header Layout (16 bytes)
 * 0–1   : free_space_offset
 * 2–3   : slot_count
 */
void TablePage::Init(Page *page) {
    char *data = page->GetData();
    std::memset(data, 0, PAGE_SIZE);

    SetFreeSpaceOffset(page, HEADER_SIZE);
    SetSlotCount(page, 0);
}

bool TablePage::InsertRecord(Page *page,
                             const char *data,
                             uint16_t size,
                             uint16_t *slot_id) {
    uint16_t free_offset = GetFreeSpaceOffset(page);
    uint16_t slot_count = GetSlotCount(page);

    uint16_t slot_dir_start =
        PAGE_SIZE - (slot_count + 1) * sizeof(uint16_t);

    if (free_offset + size > slot_dir_start) {
        return false; // no space
    }

    // Copy record
    std::memcpy(page->GetData() + free_offset, data, size);

    // Write slot
    uint16_t *slot = reinterpret_cast<uint16_t *>(
        page->GetData() + PAGE_SIZE - (slot_count + 1) * sizeof(uint16_t));
    *slot = free_offset;

    SetFreeSpaceOffset(page, free_offset + size);
    SetSlotCount(page, slot_count + 1);

    *slot_id = slot_count;
    return true;
}

const char *TablePage::GetRecord(Page *page, uint16_t slot_id) {
    uint16_t slot_count = GetSlotCount(page);
    if (slot_id >= slot_count) return nullptr;

    uint16_t *slot = GetSlotPtr(page, slot_id);
    return page->GetData() + *slot;
}

uint16_t TablePage::GetRecordCount(Page *page) {
    return GetSlotCount(page);
}

/* ---------- private helpers ---------- */

uint16_t TablePage::GetFreeSpaceOffset(Page *page) {
    return *reinterpret_cast<uint16_t *>(page->GetData());
}

void TablePage::SetFreeSpaceOffset(Page *page, uint16_t offset) {
    *reinterpret_cast<uint16_t *>(page->GetData()) = offset;
}

uint16_t TablePage::GetSlotCount(Page *page) {
    return *reinterpret_cast<uint16_t *>(page->GetData() + 2);
}

void TablePage::SetSlotCount(Page *page, uint16_t count) {
    *reinterpret_cast<uint16_t *>(page->GetData() + 2) = count;
}

uint16_t *TablePage::GetSlotPtr(Page *page, uint16_t slot_id) {
    uint16_t slot_count = GetSlotCount(page);
    return reinterpret_cast<uint16_t *>(
        page->GetData() + PAGE_SIZE -
        (slot_count - slot_id) * sizeof(uint16_t));
}

} // namespace db
