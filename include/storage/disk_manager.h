// include/storage/disk_manager.h
#pragma once
#include <fstream>
#include <string>
#include "storage/page.h"

namespace db {

class DiskManager {
public:
    explicit DiskManager(const std::string &db_file);

    void ReadPage(page_id_t page_id, char *page_data);
    void WritePage(page_id_t page_id, const char *page_data);

    page_id_t AllocatePage();

private:
    std::fstream db_io_;
    std::string db_file_;
};

}
