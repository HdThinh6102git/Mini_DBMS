#pragma once
#include "storage/page.h"
#include <cstdint>

namespace db {

/**
 * Slotted Page Layout (InnoDB-style)
 *
 * | Header | Free Space | Records | Slot Directory |
 */
class TablePage {
public:
    static constexpr uint32_t HEADER_SIZE = 16;

    static void Init(Page *page);

    static bool InsertRecord(Page *page,
                             const char *data,
                             uint16_t size,
                             uint16_t *slot_id);

    static const char *GetRecord(Page *page, uint16_t slot_id);

    static uint16_t GetRecordCount(Page *page);

private:
    static uint16_t GetFreeSpaceOffset(Page *page);
    static void SetFreeSpaceOffset(Page *page, uint16_t offset);

    static uint16_t GetSlotCount(Page *page);
    static void SetSlotCount(Page *page, uint16_t count);

    static uint16_t *GetSlotPtr(Page *page, uint16_t slot_id);
};

} // namespace db
