#include <iostream>
#include <cstring>

// ===== storage engine =====
#include "storage/disk_manager.h"
#include "storage/buffer_pool_manager.h"
#include "storage/table_page.h"

using namespace db;

/* =====================================================
 * Fake schema: Table A(id, col1, col2)
 * ===================================================== */
struct Record {
    int32_t id;
    char col1[32];
    char col2[32];
};

static void PrintRecord(const Record &r) {
    std::cout << "Record { id=" << r.id
              << ", col1=" << r.col1
              << ", col2=" << r.col2
              << " }\n";
}

/* =====================================================
 * SQL Simulation Test
 * ===================================================== */
void RunTableSqlSimulation() {
    std::cout << "\n=== SQL SIMULATION TEST ===\n";

    /* -------------------------------------------------
     * INIT ENGINE
     * ------------------------------------------------- */
    DiskManager disk("test.db");
    BufferPoolManager bpm(2, &disk);

    /* -------------------------------------------------
     * CREATE TABLE A
     * ------------------------------------------------- */
    page_id_t table_pid;
    Page *page = bpm.NewPage(&table_pid);
    TablePage::Init(page);
    bpm.UnpinPage(table_pid, true);

    /* -------------------------------------------------
     * INSERT INTO A VALUES (1, 'Alice', 'CS')
     * ------------------------------------------------- */
    {
        Page *p = bpm.FetchPage(table_pid);

        Record r{};
        r.id = 1;
        strcpy(r.col1, "Alice");
        strcpy(r.col2, "CS");

        uint16_t slot;
        TablePage::InsertRecord(
            p,
            reinterpret_cast<char *>(&r),
            sizeof(Record),
            &slot
        );

        std::cout << "[SQL] INSERT (1, Alice, CS)\n";
        bpm.UnpinPage(table_pid, true);
    }

    /* -------------------------------------------------
     * INSERT INTO A VALUES (2, 'Bob', 'CS')
     * ------------------------------------------------- */
    {
        Page *p = bpm.FetchPage(table_pid);

        Record r{};
        r.id = 2;
        strcpy(r.col1, "Bob");
        strcpy(r.col2, "CS");

        uint16_t slot;
        TablePage::InsertRecord(
            p,
            reinterpret_cast<char *>(&r),
            sizeof(Record),
            &slot
        );

        std::cout << "[SQL] INSERT (2, Bob, CS)\n";
        bpm.UnpinPage(table_pid, true);
    }

    /* -------------------------------------------------
     * SELECT * FROM A WHERE id = 1
     * ------------------------------------------------- */
    {
        Page *p = bpm.FetchPage(table_pid);
        uint16_t cnt = TablePage::GetRecordCount(p);

        std::cout << "[SQL] SELECT * FROM A WHERE id=1\n";
        for (uint16_t i = 0; i < cnt; i++) {
            const Record *r =
                reinterpret_cast<const Record *>(
                    TablePage::GetRecord(p, i));
            if (r->id == 1) {
                PrintRecord(*r);
            }
        }

        bpm.UnpinPage(table_pid, false);
    }

    /* -------------------------------------------------
     * UPDATE A SET col2='Math' WHERE id=1
     * ------------------------------------------------- */
    {
        Page *p = bpm.FetchPage(table_pid);
        uint16_t cnt = TablePage::GetRecordCount(p);

        for (uint16_t i = 0; i < cnt; i++) {
            Record *r =
                reinterpret_cast<Record *>(
                    const_cast<char *>(
                        TablePage::GetRecord(p, i)));
            if (r->id == 1) {
                strcpy(r->col2, "Math");
                std::cout << "[SQL] UPDATE id=1 SET col2=Math\n";
            }
        }

        bpm.UnpinPage(table_pid, true);
    }

    /* -------------------------------------------------
     * SELECT * FROM A WHERE id = 1
     * ------------------------------------------------- */
    {
        Page *p = bpm.FetchPage(table_pid);
        uint16_t cnt = TablePage::GetRecordCount(p);

        std::cout << "[SQL] SELECT * FROM A WHERE id=1\n";
        for (uint16_t i = 0; i < cnt; i++) {
            const Record *r =
                reinterpret_cast<const Record *>(
                    TablePage::GetRecord(p, i));
            if (r->id == 1) {
                PrintRecord(*r);
            }
        }

        bpm.UnpinPage(table_pid, false);
    }

    /* -------------------------------------------------
     * SELECT * FROM A
     * ------------------------------------------------- */
    {
        Page *p = bpm.FetchPage(table_pid);
        uint16_t cnt = TablePage::GetRecordCount(p);

        std::cout << "[SQL] SELECT * FROM A\n";
        for (uint16_t i = 0; i < cnt; i++) {
            const Record *r =
                reinterpret_cast<const Record *>(
                    TablePage::GetRecord(p, i));
            PrintRecord(*r);
        }

        bpm.UnpinPage(table_pid, false);
    }

    bpm.FlushAllPages();
}
