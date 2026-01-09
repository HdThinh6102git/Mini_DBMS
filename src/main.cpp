// #include "storage/page.h"
// #include <cassert>
// #include <iostream>

// using namespace db;
// int main() {
//     std::cout << "Start Main" << std::endl;
//     Page page;

//     assert(page.GetPageId() == -1);
//     assert(page.GetPinCount() == 0);
//     assert(!page.IsDirty());

//     page.Pin();
//     assert(page.GetPinCount() == 1);

//     page.Unpin();
//     assert(page.GetPinCount() == 0);

//     page.SetDirty(true);
//     assert(page.IsDirty());
//     cout << "End Main" << endl;
//     return 0;
// }

// #include "storage/disk_manager.h"
// #include <iostream>
// #include <cstring>


// using namespace db;

// int main() {

//     DiskManager dm("test.db");

//     page_id_t pid = dm.AllocatePage();

//     char write_buf[PAGE_SIZE];
//     std::memset(write_buf, 0, PAGE_SIZE);
//     std::strcpy(write_buf, "Hello Disk Manager!");

//     dm.WritePage(pid, write_buf);

//     char read_buf[PAGE_SIZE];
//     dm.ReadPage(pid, read_buf);

//     std::cout << read_buf << std::endl; // expect Hello Disk Manager!
//     return 0;
// }

#include <iostream>
#include <cstring>
#include <cassert>

#include "storage/page.h"
#include "storage/disk_manager.h"
#include "storage/buffer_pool_manager.h"

using namespace std;
using namespace db;

int main() {
    cout << "===== Buffer Pool Manager Test =====" << endl;

    // 1. Create DiskManager
    DiskManager dm("test.db");

    // 2. Create BufferPoolManager with 2 frames
    BufferPoolManager bpm(2, &dm);

    // 3. Allocate new page
    page_id_t pid;
    Page *page = bpm.NewPage(&pid);
    assert(page != nullptr);

    cout << "New page id: " << pid << endl;

    // 4. Write data
    strcpy(page->GetData(), "Hello Buffer Pool");
    bpm.UnpinPage(pid, true); // mark dirty

    // 5. Fetch same page again
    Page *page2 = bpm.FetchPage(pid);
    assert(page2 != nullptr);

    cout << "Read from page: " << page2->GetData() << endl;

    bpm.UnpinPage(pid, false);

    // 6. Test eviction (pool size = 2)
    page_id_t pid2, pid3;

    Page *p2 = bpm.NewPage(&pid2);
    strcpy(p2->GetData(), "Page 2");
    bpm.UnpinPage(pid2, true);

    Page *p3 = bpm.NewPage(&pid3);
    strcpy(p3->GetData(), "Page 3");
    bpm.UnpinPage(pid3, true);

    // 7. Fetch first page again (should be read from disk)
    Page *page1_again = bpm.FetchPage(pid);
    cout << "Page 1 again: " << page1_again->GetData() << endl;
    bpm.UnpinPage(pid, false);

    // 8. Flush all pages
    bpm.FlushAllPages();

    cout << "===== Test Done =====" << endl;
    return 0;
}