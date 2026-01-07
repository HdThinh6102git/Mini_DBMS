// include/storage/table_heap.h
#pragma once
#include "storage/buffer_pool_manager.h"

namespace db {

class TableHeap {
public:
    explicit TableHeap(BufferPoolManager *bpm);

    bool InsertTuple(const char *tuple, size_t size);

private:
    BufferPoolManager *buffer_pool_manager_;
    page_id_t first_page_id_;
};

}
