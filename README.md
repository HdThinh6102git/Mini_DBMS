
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