#include "storage/page.h"
#include <cassert>

using namespace db;

int main() {
    Page page;

    assert(page.GetPageId() == -1);
    assert(page.GetPinCount() == 0);
    assert(!page.IsDirty());

    page.Pin();
    assert(page.GetPinCount() == 1);

    page.Unpin();
    assert(page.GetPinCount() == 0);

    page.SetDirty(true);
    assert(page.IsDirty());

    return 0;
}
