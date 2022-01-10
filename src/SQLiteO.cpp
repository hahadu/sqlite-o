#include <iostream>
#include "SQLiteO.h"
#include <stdio.h>
#include <map>

SQLiteO* SQLiteO::table(const char* set_table_name) {
    table_name = set_table_name;
    return this;
}

SQLiteO* SQLiteO::open(const char* db_name) {
    sqlite3_open("data.db", &db);
    return this;

}

int SQLiteO::create_table(std::map<const char*, const char*> create_columns) {

    if (create_columns.empty()) {
        std::cout << "这啥也没有啊，搞我呢？" << std::endl;
        return false;
    }
    std::map<const char*, const char*>::iterator   iter;
    std::string columns_string = "";
    
    for (iter = create_columns.begin(); iter != create_columns.end(); iter++) {
        columns_string += iter->first;
        columns_string += SPACE_CHARACTER;
        columns_string += iter->second;
        columns_string += ",";
    }

    //移除最后一个","
    columns_string.pop_back();

    std::string created = "CREATE TABLE ";
    created += table_name;
    created += " ( ";
    created += columns_string;
    created += " ); ";
    char* sql = new char[strlen(created.c_str()) + 1];
    strcpy_s(sql, strlen(created.c_str()) + 1, created.c_str());

    int result = sqlite3_exec(db, (const char*)sql, createCallback, (void*)data, &zErrMsg);
    delete sql;
    //int result = 0;
    return result;

}
/* select 回调 */
int SQLiteO::selectCallback(void* data, int argc, char** argv, char** azColName) {
    int i;
    fprintf(stderr, "%s: ", (const char*)data);
    for (i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}
int SQLiteO::createCallback(void* data, int argc, char** argv, char** azColName) {
    int i;
    fprintf(stderr, "%s: ", (const char*)data);
    for (i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}
/* insert 回调 */
int SQLiteO::insertCallback(void* NotUsed, int argc, char** argv, char** azColName) {
    int i;
    for (i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}


int SQLiteO::insert(std::map<const char*, const char* > columns) {
    if (!this->is_tablename()) {
        std::cout << "No query form is specified" << std::endl;
        return 0;
    }

    /// <summary>
    /// 判断不为空
    /// </summary>
    /// <param name="columns"></param>
    if (columns.empty()) {
        std::cout << "这啥也没有啊，搞我呢？" << std::endl;
        return false;
    }
    std::map<const char*, const char*>::iterator   iter;
    std::string columns_string = "";
    std::string values_string = "";

    for (iter = columns.begin(); iter != columns.end(); iter++) {
        columns_string += iter->first;
        columns_string += ",";
        values_string += SINGLE_QUOTES_CHARACTER;
        values_string += iter->second;
        values_string += SINGLE_QUOTES_CHARACTER;
        values_string += ",";
    }

    //移除最后一个","
    columns_string.pop_back();
    values_string.pop_back();

    /// <summary>
    /// 封装insert语句
    /// </summary>
    /// <param name="columns"></param>
    /// <returns></returns>
    std::string insert = "INSERT INTO ";
    insert += table_name;
    insert += " ( ";
    insert += columns_string;
    insert += " ) VALUES ( ";
    insert += values_string;
    insert += " ); ";
    char* sql = new char[strlen(insert.c_str()) + 1];
    strcpy_s(sql, strlen(insert.c_str()) + 1, insert.c_str());

    int result = sqlite3_exec(db, sql, insertCallback, (void*)data, &zErrMsg);
    //int result = 0;
    return result;
}
int SQLiteO::select(const char* columns) {
    if (!this->is_tablename()) {
        std::cout << "No query form is specified" << std::endl;
        return 0;
    }
    
    std::string selected = "SELECT ";
    selected += columns;
    selected += " FROM ";
    selected += table_name;
    selected += SPACE_CHARACTER;
    selected += base_where_clause_string;
    selected += "; ";
    char* sql = new char[strlen(selected.c_str()) + 1];
    strcpy_s(sql, strlen(selected.c_str()) + 1, selected.c_str());
    //子查询语句用完应该及时清理回收
    base_where_clause_string.clear();
   // std::cout << sql << std::endl;
    int result = sqlite3_exec(db, sql, selectCallback, (void*)data, &zErrMsg);
    return result;
}


SQLiteO* SQLiteO::where(const char* column, const char* op, const char* value, const char* _boolean)
{
    //where方法应该只创建一个用于查询的where子语句
    //std::
    if (NULL == value) {
        value = op;
        op = "=";
    }
    if (base_where_clause_string.length() > 0) {
        base_where_clause_string += _boolean;
        base_where_clause_string += SPACE_CHARACTER;
    }
    else {
        base_where_clause_string += "WHERE";
        base_where_clause_string += SPACE_CHARACTER;
    }
    base_where_clause_string += column;
    base_where_clause_string += SPACE_CHARACTER;
    base_where_clause_string += op;
    base_where_clause_string += SPACE_CHARACTER;
    base_where_clause_string += SINGLE_QUOTES_CHARACTER;
    base_where_clause_string += value;
    base_where_clause_string += SINGLE_QUOTES_CHARACTER;
    base_where_clause_string += SPACE_CHARACTER;
    return this;
}

bool SQLiteO::is_table()
{

    return false;
}

int SQLiteO::update(std::map<const char*, const char* > datas) {
    if (!this->is_tablename()) {
        std::cout << "No query form is specified" << std::endl;
        return 0;
    }
    // "UPDATE device_list SET device_serial_num = 'ARMJ9X1529W01916' WHERE ID = 1;
    if (datas.empty()) {
        std::cout << "这啥也没有啊，搞我呢？" << std::endl;
        return false;
    }
    std::map<const char*, const char*>::iterator   iter;
    std::string update_string = "";

    for (iter = datas.begin(); iter != datas.end(); iter++) {
        update_string += iter->first;
        update_string += SPACE_CHARACTER;
        update_string += EQUAL_SIGN_CHARACTER;
        update_string += SPACE_CHARACTER;
        update_string += SINGLE_QUOTES_CHARACTER;
        update_string += iter->second;
        update_string += SINGLE_QUOTES_CHARACTER;
        update_string += ",";
    }

    //移除最后一个","
    update_string.pop_back();
    std::cout << update_string << std::endl;
    std::string update = "UPDATE ";
    update += table_name;
    update += SPACE_CHARACTER;
    update += "SET";
    update += SPACE_CHARACTER;
    update += update_string;
    update += SPACE_CHARACTER;
    update += base_where_clause_string;

    std::cout << update << std::endl;
    char* sql = new char[strlen(update.c_str()) + 1];
    strcpy_s(sql, strlen(update.c_str()) + 1, update.c_str());

    int result = sqlite3_exec(db, (const char*)sql, createCallback, (void*)data, &zErrMsg);
    delete sql;
    update.clear();
    base_where_clause_string.clear();
    return 0;
}

int SQLiteO::delete_data() {
    return 0;
}
int SQLiteO::delete_table() {
    return 0;
}