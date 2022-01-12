#include <iostream>
#include "SQLiteO.h"
#include <stdio.h>
#include <map>
#include <string>
#include "json/json.h"
#include <algorithm>
SQLiteO* SQLiteO::table(const char* set_table_name) {
    table_name = set_table_name;
    return this;
}

SQLiteO* SQLiteO::open(const char* db_name) {
    
    sqlite3_open("data.db", &db);
    return this;
}

int SQLiteO::create_table(std::map<const char*, const char*> create_columns) {
    //判断表是否存在
    if (this->is_table()) {
        Json::Value tableList = this->where("name", this->table_name)->table("sqlite_master")->select();
        //Json::Value info = tableList[0];
        return true;
    }
    if (is_empty_map_datas(create_columns)) {
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

    std::string created = "CREATE TABLE IF NOT EXISTS ";
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

int SQLiteO::createCallback(void* data, int argc, char** argv, char** azColName) {
    int i;
    fprintf(stderr, "%s: ", (const char*)data);
    for (i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

int SQLiteO::updateCallback(void* data, int argc, char** argv, char** azColName) {
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
int SQLiteO::deleteCallback(void* data, int argc, char** argv, char** azColName) {
    int i;
    fprintf(stderr, "%s: ", (const char*)data);
    for (i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

int SQLiteO::insert(std::map<const char*, const char* > columns) {
    if (!this->is_tablename()) {
        return 0;
    }
    
    /// <summary>
    /// 判断不为空
    /// </summary>
    /// <param name="columns"></param>
    if (is_empty_map_datas(columns)) {
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
    //sqlite3_stmt* stmt;
    typedef struct sqlite3_stmt sqlite3_stmt;

    int result = sqlite3_exec(db, sql, insertCallback, (void*)data, &zErrMsg);
    //int result = 0;
    return result;
}
Json::Value select_result_data;

/* select 回调 */
int SQLiteO::selectCallback(void* data, int argc, char** argv, char** azColName) {
    int i;
    Json::Value info;
    for (i = 0; i < argc; i++) {
        info[azColName[i]] = argv[i] ? argv[i] : "NULL";
    }
    select_result_data.append(info);
    return 0;
}
Json::Value SQLiteO::select(const char* columns) {
    if (!this->is_tablename()) {

        return false;
    }
    
    std::string selected = "SELECT";
    selected += SPACE_CHARACTER;
    selected += base_where_distinct_clause_string;
    selected += SPACE_CHARACTER;
    selected += columns;
    selected += SPACE_CHARACTER;
    selected += "FROM";
    selected += SPACE_CHARACTER;
    selected += table_name;
    selected += SPACE_CHARACTER;
    selected += base_where_clause_string;
    selected += base_where_groupby_clause_string;
    selected += base_where_orderby_clause_string;
    selected += base_where_limit_clause_string;
    selected += base_where_join_clause_string;
    selected += "; ";
    char* sql = new char[strlen(selected.c_str()) + 1];
    strcpy_s(sql, strlen(selected.c_str()) + 1, selected.c_str());
    //子查询语句用完应该及时清理回收
    base_where_clause_string.clear();
    const char* result_data = "";
    int result = sqlite3_exec(db, sql, selectCallback, (void*)result_data, &zErrMsg);
    //std::cout << sql << std::endl;
    if (result == SQLITE_OK) {
        return select_result_data;
    }
    return false;
}
SQLiteO* SQLiteO::where(const char* column, const char* op, const char* value, const char* _boolean)
{
    //where方法应该只创建一个用于查询的where子语句
    std::string opString = op;
    std::transform(opString.begin(), opString.end(), opString.begin(), ::towupper);
    if (NULL == value) {
        if (!this->in_array(opString.c_str(), this->operators, sizeof(this->operators) / sizeof(this->operators[0]))) {
            value = op;
            op = "=";
        }
        else {
            char errorString[] = "query column requires a valid value 你找什么啊亲！";
            extern int errno;
            int errnum = errno;
            fprintf(stderr, errorString);
            exit(-1);
            return this;
        }
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
    base_where_clause_string += opString;
    base_where_clause_string += SPACE_CHARACTER;
    base_where_clause_string += SINGLE_QUOTES_CHARACTER;
    base_where_clause_string += value;
    base_where_clause_string += SINGLE_QUOTES_CHARACTER;
    base_where_clause_string += SPACE_CHARACTER;
    return this;
}
SQLiteO* SQLiteO::limit(int limit) {
    base_where_limit_clause_string = "LIMIT";
    base_where_limit_clause_string += SPACE_CHARACTER;
    base_where_limit_clause_string += std::to_string(limit);
    base_where_limit_clause_string += SPACE_CHARACTER;
    return this;
}
SQLiteO* SQLiteO::orderBy(const char* column,const char* order) {
    base_where_orderby_clause_string = "ORDER BY";
    base_where_orderby_clause_string += SPACE_CHARACTER;
    base_where_orderby_clause_string += column;
    base_where_orderby_clause_string += SPACE_CHARACTER;
    base_where_orderby_clause_string += order;
    base_where_orderby_clause_string += SPACE_CHARACTER;
    return this;
}
SQLiteO* SQLiteO::groupBy(const char* column) {
    base_where_groupby_clause_string = "GROUP BY";
    base_where_groupby_clause_string += SPACE_CHARACTER;
    base_where_groupby_clause_string += column;
    base_where_groupby_clause_string += SPACE_CHARACTER;
    return this;

}
SQLiteO* SQLiteO::distinct() {
    base_where_distinct_clause_string = "DISTINCT";
    base_where_distinct_clause_string += SPACE_CHARACTER;
    return this;
}
SQLiteO* SQLiteO::join(const char* table, const char* first, const char* op, const char* second, const char* type) {
    if (second == NULL) {
        second = op;
        op = "=";
    }
    base_where_join_clause_string += SPACE_CHARACTER;
    base_where_join_clause_string += type;
    base_where_join_clause_string += SPACE_CHARACTER;
    base_where_join_clause_string += "JOIN";
    base_where_join_clause_string += SPACE_CHARACTER;
    base_where_join_clause_string += table;
    base_where_join_clause_string += SPACE_CHARACTER;
    base_where_join_clause_string += "ON";
    base_where_join_clause_string += SPACE_CHARACTER;
    base_where_join_clause_string += first;
    base_where_join_clause_string += SPACE_CHARACTER;
    base_where_join_clause_string += op;
    base_where_join_clause_string += SPACE_CHARACTER;
    base_where_join_clause_string += second;
    base_where_join_clause_string += SPACE_CHARACTER;
    return this;
}
int SQLiteO::count() {
    Json::Value data = this->select("count(*)");
    const char* countStr = data[0]["count(*)"].asCString();
    return atoi(countStr);
}
bool SQLiteO::is_table()
{
    Json::Value data = this->where("name", this->table_name)->table("sqlite_master")->count();
    std::cout << data << std::endl;

    if (data.size() > 0) {
        return true;
    }
    return false;
}

int SQLiteO::update(std::map<const char*, const char* > datas) {
    if (!this->is_tablename()) {
        return 0;
    }
    if (is_empty_map_datas(datas)) {
        return false;
    }

    std::map<const char*, const char*>::iterator  iter;
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

    std::string update = "UPDATE ";
    update += table_name;
    update += SPACE_CHARACTER;
    update += "SET";
    update += SPACE_CHARACTER;
    update += update_string;
    update += SPACE_CHARACTER;
    update += base_where_clause_string;

    char* sql = new char[strlen(update.c_str()) + 1];
    strcpy_s(sql, strlen(update.c_str()) + 1, update.c_str());
    int result = sqlite3_exec(db, (const char*)sql, createCallback, (void*)data, &zErrMsg);
    delete sql;
    update.clear();
    base_where_clause_string.clear();
    return 0;
}

int SQLiteO::delete_data() {
    if (!this->is_tablename()) {
        return 0;
    }

    std::string delete_data = "DELETE FROM";
    delete_data += SPACE_CHARACTER;
    delete_data += table_name;
    delete_data += SPACE_CHARACTER;
    delete_data += base_where_clause_string;
    delete_data += SEMICOLON_CHARACTER;
    char* sql = new char[strlen(delete_data.c_str()) + 1];
    strcpy_s(sql, strlen(delete_data.c_str()) + 1, delete_data.c_str());
    delete_data.clear();

    int result = sqlite3_exec(db, sql, deleteCallback, (void*)data, &zErrMsg);
    return result;
}
int SQLiteO::delete_table(const char* table_name) {
    if ("" == table_name) {
        if (!this->is_tablename()) {
            return 0;
        }
        table_name = this->table_name;
    }

    std::string delete_table = "DROP TABLE";
    delete_table += SPACE_CHARACTER;
    delete_table += table_name;
    char* sql = new char[strlen(delete_table.c_str()) + 1];
    strcpy_s(sql, strlen(delete_table.c_str()) + 1, delete_table.c_str());
    delete_table.clear();
    int result = sqlite3_exec(db, sql, deleteCallback, (void*)data, &zErrMsg);
    return result;
}
