// #include "storage/page.h"
// #include <cassert>
// #include <iostream>

#include "storage/disk_manager.h"
#include <iostream>
#include <cstring>


using namespace db;

int main() {
    // cout << "Start Main" << endl;
    // Page page;

    // assert(page.GetPageId() == -1);
    // assert(page.GetPinCount() == 0);
    // assert(!page.IsDirty());

    // page.Pin();
    // assert(page.GetPinCount() == 1);

    // page.Unpin();
    // assert(page.GetPinCount() == 0);

    // page.SetDirty(true);
    // assert(page.IsDirty());
    // cout << "End Main" << endl;

    DiskManager dm("test.db");

    page_id_t pid = dm.AllocatePage();

    char write_buf[PAGE_SIZE];
    std::memset(write_buf, 0, PAGE_SIZE);
    std::strcpy(write_buf, "Hello Disk Manager!");

    dm.WritePage(pid, write_buf);

    char read_buf[PAGE_SIZE];
    dm.ReadPage(pid, read_buf);

    std::cout << read_buf << std::endl; // expect Hello Disk Manager!
    return 0;
}
