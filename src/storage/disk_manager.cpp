#include "storage/disk_manager.h"
#include <cassert>
#include <iostream>

namespace db {

DiskManager::DiskManager(const std::string &db_file)
    : db_file_(db_file), next_page_id_(0) {

    // Open or create file
    db_io_.open(db_file_, std::ios::binary | std::ios::in | std::ios::out);

    if (!db_io_.is_open()) {
        // File does not exist → create
        db_io_.clear();
        db_io_.open(db_file_, std::ios::binary | std::ios::out);
        db_io_.close();

        // Reopen read/write
        db_io_.open(db_file_, std::ios::binary | std::ios::in | std::ios::out);
    }

    assert(db_io_.is_open());

    // Determine file size → next_page_id
    db_io_.seekg(0, std::ios::end);
    size_t file_size = db_io_.tellg();
    next_page_id_ = static_cast<page_id_t>(file_size / PAGE_SIZE);
}

DiskManager::~DiskManager() {
    if (db_io_.is_open()) {
        db_io_.close();
    }
}

void DiskManager::ReadPage(page_id_t page_id, char *page_data) {
    size_t offset = static_cast<size_t>(page_id) * PAGE_SIZE;

    db_io_.seekg(0, std::ios::end);
    size_t file_size = db_io_.tellg();

    if (offset >= file_size) {
        // Page chưa tồn tại → trả về page rỗng
        std::memset(page_data, 0, PAGE_SIZE);
        return;
    }

    db_io_.seekg(offset);
    db_io_.read(page_data, PAGE_SIZE);
}

void DiskManager::WritePage(page_id_t page_id, const char *page_data) {
    size_t offset = static_cast<size_t>(page_id) * PAGE_SIZE;

    db_io_.seekp(offset);
    db_io_.write(page_data, PAGE_SIZE);
    db_io_.flush();
}

page_id_t DiskManager::AllocatePage() {
    return next_page_id_++;
}

void DiskManager::DeallocatePage(page_id_t page_id) {
    // Mini DBMS: chưa cần implement
}

} // namespace db
