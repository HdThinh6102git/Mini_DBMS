#pragma once

#include <fstream>
#include <string>
#include "storage/page.h"

namespace db {

class DiskManager {
public:
    explicit DiskManager(const std::string &db_file);
    ~DiskManager();

    void ReadPage(page_id_t page_id, char *page_data);
    void WritePage(page_id_t page_id, const char *page_data);

    page_id_t AllocatePage();
    void DeallocatePage(page_id_t page_id); // (chưa dùng, để trống cũng OK)

private:
    std::fstream db_io_;
    std::string db_file_;
    page_id_t next_page_id_;
};

} // namespace db
