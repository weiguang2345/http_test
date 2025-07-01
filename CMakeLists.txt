cmake_minimum_required(VERSION 3.10)
project(MiniKV C)

set(CMAKE_C_STANDARD 99)

# 包含头文件目录
include_directories(include)

# 主程序构建
add_executable(minikv
    src/main.c
    src/parser.c
    src/engine.c
    src/storage.c
    src/http_server.c
    src/api_handler.c
)

# 构建测试程序
add_executable(test_storage
    test/test_storage.c
    src/storage.c
)

# 链接 Check 测试库（不依赖 find_package）
target_link_libraries(test_storage check subunit m pthread)

# 可选：启用测试集成（ctest）
enable_testing()
add_test(NAME StorageTest COMMAND test_storage)
