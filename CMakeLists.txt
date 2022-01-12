project(sqlite-o)

set(APP_SRC src/SQLiteO.h)
#静态库，不需要可执行文件
#add_executable(${PROJECT_NAME} src/SQLiteO.h)
add_library(${PROJECT_NAME} STATIC src/SQLiteO.h) # 生成静态库
add_library(${PROJECT_NAME} SHARED src/SQLiteO.h) # 生成动态库或共享库
#print message
message(${PROJECT_SOURCE_DIR})