#include "storage/page.h"
#include <cstring>

namespace db {

Page::Page()
    : page_id_(-1),
      pin_count_(0),
      is_dirty_(false) {
    ResetMemory();
}

void Page::ResetMemory() {
    std::memset(data_, 0, PAGE_SIZE);
}

} // namespace db
