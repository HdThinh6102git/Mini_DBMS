
### Directory Responsibilities

- **include/**
  - Contains public header files.
  - Defines interfaces, data structures, and abstractions.
  - No implementation logic is placed here.

- **src/**
  - Contains implementation files corresponding to headers in `include/`.

- **data/**
  - Stores persistent database files managed by the DBMS at runtime.

- **tests/**
  - Contains unit and integration tests for individual components.

---

### Source Code Tree

```text
mini_dbms/
├── CMakeLists.txt
├── README.md
├── bin/
├── data/
│   └── test.db
├── include/
│   ├── common/
│   │   ├── config.h
│   │   ├── types.h
│   │   └── macros.h
│   │
│   ├── storage/
│   │   ├── disk_manager.h
│   │   ├── buffer_pool_manager.h
│   │   ├── page.h
│   │   ├── table_heap.h
│   │   └── table_page.h
│   │
│   └── util/
│       ├── logger.h
│       └── file.h
│
├── src/
│   ├── storage/
│   │   ├── disk_manager.cpp
│   │   ├── buffer_pool_manager.cpp
│   │   ├── page.cpp
│   │   ├── table_heap.cpp
│   │   └── table_page.cpp
│   │
│   └── main.cpp
└── tests/
```


---

## II. Why DBMS Uses This Structure

Database management systems commonly adopt this structure because it fits the
requirements of **low-level, performance-critical system software**.

### 1. Header / Source Separation

- Faster compilation times
- Clear and stable interfaces
- Explicit separation between API and implementation

### 2. Subsystem-Oriented Organization

Code is organized by functional subsystems, such as:

- **storage**   – disk management, pages, buffer pool
- **index**     – index structures (e.g. B+Tree)
- **execution** – query execution engine
- **optimizer** – query planner and cost-based optimization

This closely reflects real-world DBMS internal architecture.

### 3. Extensibility

- New index engines can be added without modifying storage code
- Query execution logic can evolve independently
- Storage remains a stable foundation for the system

### 4. Compatibility with Build Systems (CMake)

- Clear dependency boundaries between modules
- Scales well as the project grows
- Matches industrial C++ engine build workflows

---

This structure is not specific to MySQL.
It represents a **general-purpose C++ engine architecture** widely used in
database engines, storage systems, compilers, and other system-level software.


Cách build 
Bước 1: Cài MinGW-w64
Tải về từ link https://winlibs.com/#download-release
GCC 15.2.0 (LATEST) tải Zip  archive
Giải nén => Copy folder mingw64 bỏ vào ổ đĩa C
Set up System Variables 
Vào tìm Environment Variables 
Tìm Path ở System Variables => Edit Window => New => Dán C:\mingw64\bin
ĐÓNG HOÀN TOÀN: VS Code, Git Bash

cd ..
rm -rf build
mkdir build
cd build
cmake .. -G "MinGW Makefiles"
mingw32-make


Vì sao sửa main.cpp mà phải build lại?
C++ là compiled language

File .cpp → compile → .obj

Nhiều .obj → link → .exe

Khi bạn sửa bất kỳ .cpp nào, file .exe cũ KHÔNG tự thay đổi.
Không cần rebuild everything
Chỉ cần chạy lại make: mingw32-make
make rất thông minh:
  - Chỉ recompile file bị sửa (main.cpp)
  - Không build lại mấy file khác (page.cpp, disk_manager.cpp…)
Vì vậy chạy mingw32-make là rất nhanh.

Nếu sau này bạn build với Release mode: thì assert sẽ bị tắt hoàn toàn assert chỉ dùng cho debug / development
cmake .. -DCMAKE_BUILD_TYPE=Release

C++: Compiled (native)
Java: Compiled → Bytecode
C#: Compiled → IL
JavaScript: Interpreted / JIT

C++ – compiled language
  - .cpp → compiler → .obj
  - .obj → linker → .exe
.exe là mã máy
Sửa 1 dòng → .exe cũ KHÔNG đổi
Phải build lại

Đổi lại:
  - Rất nhanh
  - Kiểm soát memory
  - Phù hợp DBMS, OS, engine

Java – compiled nhưng không native
  - .java → javac → .class (bytecode)
  - .class → JVM → machine code (JIT)
Java cũng phải compile lại
Nhưng:
  - Không link kiểu C++
  - Không tạo .exe
JVM làm JIT (Just-In-Time)
Nếu không javac lại → chạy code cũ

C# – rất giống Java (managed language)
  - .cs → csc → .exe/.dll (IL)
  - .exe → CLR → machine code (JIT)
Sửa .cs → phải build lại
Nhưng:
  - .exe KHÔNG phải mã máy
  - Là Intermediate Language (IL)
Visual Studio tự build giúp bạn, nên bạn ít thấy bước này.

JavaScript – KHÁC HOÀN TOÀN
Pipeline (Node.js / Browser):
  - .js → V8 → machine code (JIT)
Không compile trước
Không build
Không .exe
Sửa file → chạy lại → code mới chạy ngay

Vì sao JS không cần build lại?
  - Không tạo binary
  - Code được đọc trực tiếp khi chạy
  - Engine (V8) compile tại runtime

Vì sao DBMS dùng C++ chứ không JS?
  - Memory control
  - Cache locality
  - Predictable perf
  - IO thấp tầng
  - JIT overhead

JIT là gì?
JIT (Just-In-Time compilation) là: Compile code sang mã máy ngay lúc chương trình đang chạy.

Trước JIT là gì?
AOT – Ahead-Of-Time compilation
source code → compiler → machine code → run
  - Compile trước khi chạy
  - .exe = mã máy thật
  - Chạy lên là CPU chạy trực tiếp

JIT hoạt động như thế nào?
  source → bytecode / IL → (runtime) → JIT → machine code
  1. Code không compile thẳng ra mã máy
  2. Chỉ compile ra code trung gian
  3. Khi chương trình chạy:
    - Runtime quan sát code nào chạy nhiều
    - Compile chỗ đó sang mã máy
  4. Cache mã máy đó để chạy nhanh lần sau
Compile lúc đang chạy = JIT

Java có JIT như thế nào?
  - .java → javac → .class (bytecode)
  - .class → JVM
  - JVM → JIT → machine code
Ban đầu chạy chậm
Chạy lâu → JVM tối ưu dần
Gọi là HotSpot
Vì vậy Java: Chạy càng lâu → càng nhanh (Chức năng nào chạy nhiều thì sẽ nhanh dần từ lúc mới bắt đầu và giữ ổn định ở tốc độ đó nhờ JVM)

C# có JIT như thế nào?
  - .cs → csc → IL
  - IL → CLR
  - CLR → JIT → machine code
Mỗi method compile khi được gọi lần đầu
Sau đó cache
.exe của C# chưa phải mã máy.

JavaScript cũng có JIT (nhưng khác)
  - .js → V8 engine
  - V8 → parse → bytecode
  - V8 → JIT → machine code
Không cần bước compile trước
Mọi thứ diễn ra lúc chạy
V8 có nhiều tầng JIT (Ignition, TurboFan)
JS hiện đại rất nhiều JIT, không phải “script ngu” như xưa.

Vì sao DBMS ghét JIT?
DBMS cần:
  - Performance ổn định
  - Latency predictable
  - Cache locality chặt chẽ
  - Không GC pause
  - Không warm-up
JIT: 
  - Chạy chậm lúc đầu
  - GC có thể pause
  - Tối ưu thay đổi theo workload

MySQL, PostgreSQL, SQLite → C/C++
Oracle Java DB rất hiếm (Tại sao ?????)

Nhưng JIT có ưu điểm gì?
Rất mạnh cho:
  - Web server
  - App business
  - Long-running service
  - Hot path rõ ràng

Vì JIT tối ưu theo hành vi thực tế, C++ chỉ tối ưu theo dự đoán.
# FLOW TEST BUFFER POOL

B1: Init Disk Manager
- Hàm Init() của Disk Manager sẽ mở file test.db nếu tồn tại hoặc tạo mới file nếu chưa có

B2: Init Buffer Pool Manager
- Buffer pool có 2 frame => tối đa 2 page trong RAM
- Buffer Pool Manager được gắn với DiskManager

Trạng thái ban đầu:
Buffer Pool:
  Frame 0: empty
  Frame 1: empty
Page Table: empty
LRU List: empty
Free List: [0, 1]

B3: Create Page
page_id_t pid;
Page *page = bpm.NewPage(&pid);

Luồng xử lý:
1. Lấy frame từ free_list → frame 0
2. Gọi DiskManager::AllocatePage() → pid = 0
3. Reset memory page
4. Set page_id = 0
5. pin_count = 1
6. Ghi mapping vào page_table

Trạng thái:
Frame 0: page_id=0, pin=1, dirty=false
Frame 1: empty
Page Table: {0 -> 0}
Free List: [1]
LRU List: []

B4: Write Page
strcpy(page->GetData(), "Hello Buffer Pool");
bpm.UnpinPage(pid, true);

Hành vi:
1. Ghi dữ liệu trong RAM
2. Disk CHƯA bị động tới

Unpin(true):
- pin_count: 1 → 0
- dirty = true
- Page được đưa vào LRU

Trạng thái:
Frame 0: page_id=0, pin=0, dirty=true
LRU List: [0]

B5: Fetch lại page vừa tạo
Page *page2 = bpm.FetchPage(pid);

Luồng FetchPage():
1. Page 0 đã có trong page_table
2. Không đọc disk
3. pin_count: 0 → 1
4. Remove khỏi LRU

Trạng thái:
Frame 0: page_id=0, pin=1, dirty=true
LRU List: []

B6: Đọc dữ liệu
cout << page2->GetData() << endl;
bpm.UnpinPage(pid, false);

Hành vi:
1. Đọc string từ RAM
2. Unpin(false)
   - pin_count: 1 → 0
   - dirty giữ nguyên (true)

B7: Tạo page thứ 2
Page *p2 = bpm.NewPage(&pid2);

Luồng:
1. Lấy frame 1 từ free_list
2. Allocate page_id = 1
3. pin = 1

Sau khi unpin:
Frame 0: page_id=0, dirty=true
Frame 1: page_id=1, dirty=true
LRU List: [0, 1]

B8: Tạo page thứ 3 → BẮT BUỘC EVICT
Page *p3 = bpm.NewPage(&pid3);

Buffer pool size = 2, không còn free frame

Eviction process:
1. Lấy frame đầu LRU → frame 0
2. Page 0 dirty → flush xuống disk
3. Xóa mapping page 0
4. Reuse frame 0
5. Allocate page_id = 2

Trạng thái:
Frame 0: page_id=2, dirty=true
Frame 1: page_id=1, dirty=true
Disk: page 0 đã được ghi

B9: Fetch lại page 0 (đã bị evict)
Page *page1_again = bpm.FetchPage(pid);

Luồng:
1. Page 0 không có trong buffer pool
2. Evict page 1 (dirty → flush)
3. Read page 0 từ disk
4. Pin page 0

Ý nghĩa:
- Chứng minh disk ↔ buffer pool hoạt động đúng
- Data "Hello Buffer Pool" được đọc lại từ disk

B10: Flush toàn bộ page
bpm.FlushAllPages();

Hành vi:
1. Ghi tất cả page dirty trong buffer pool xuống disk
2. Đảm bảo disk = RAM

Buffer Pool trong InnoDB làm 3 việc chính:
1. Cache page đọc từ disk
2. Giữ page đang được modify (dirty pages)
3. Chứa page mới được tạo (chưa tồn tại trên disk)
