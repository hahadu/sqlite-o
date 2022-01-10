# sqlite-operation
Sqlite database chain operation  

sqlite数据库的链式操作类

example:
```cpp
// connect database
 SQLiteO* db = new SQLiteO(DATABASE_NAME);
or
 SQLiteO* db = new SQLiteO();
 db->open(DATABASE_NAME);
```
//create
```cpp
db->table(DATABASE_DEVICE_LIST_TABLE_NAME)->create_table(std::map<const char*,const char*> createdMap)
```
//secect 
```cpp
 db->where("column_name","column_value")->select("*");
```
