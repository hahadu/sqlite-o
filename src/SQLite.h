#pragma once

#include <sqlite3.h>
#include <stdio.h>
#include <string>
#include <map>
class SQLite
{
    /*定义私有属性*/
    private:
        /* err_message*/
        char* zErrMsg = 0;
        
        /* result data*/
        const char* data = "Callback function called";

        /* 表名 */
        const char* table_name;
        std::string base_where_clause_string = "";
        /* 检查tablename */
        bool is_tablename() {
            return NULL != table_name;
        }

    /*定义私有属性*/
    protected:
        /* 数据库对象 */
		sqlite3* db = NULL;

    /*定公共有属性*/
	public: 
        /* 空格  */
        const char* SPACE_CHARACTER = " ";
        /* 目录分隔符 : / */
        const char* DIRECTORY_SEPARATOR = "/";
        /* 目录分隔符 : / */
        const char* DS = "/";
        /* 单引号:' */
        const char* SINGLE_QUOTES_CHARACTER = "'";
        /* 双引号 : " */
        const char* DOUBLE_QUOTES_CHARACTER = "\"";
        /// <summary>
        /// 括号
        /// </summary>
        /// <returns>（</returns>
        const char* PARENTHESES_CHARACTER = "(";
        /* 构造函数 */
        SQLite(const char* database_name=NULL) {
            if (NULL != database_name) {
                this->open(database_name);
            }
        }
        /* 析构函数*/
        ~SQLite() {
            this->close();
            
        }
        /* 设置表名 */
        SQLite* table(const char* table_name);
		//void SQLiteCreateDeviceListTable(void);
        SQLite* open(const char* db_name);
        /*
        * 创建数据表
        */
        int create_table(const char* create_columns);
        /* 检查数据表是否存在 */
        bool is_table();
        /// <summary>
        /// 更新
        /// </summary>
        /// <returns>int</returns>
        int update();
        /// <summary>
        /// select 回调
        /// </summary>
        /// <param name="data"></param>
        /// <param name="argc"></param>
        /// <param name="argv"></param>
        /// <param name="azColName"></param>
        /// <returns>static int</returns>
        static int selectCallback(void* data, int argc, char** argv, char** azColName);

        /// <summary>
        /// create function callback
        /// </summary>
        /// <param name="data"></param>
        /// <param name="argc"></param>
        /// <param name="argv"></param>
        /// <param name="azColName"></param>
        /// <returns>static int</returns>
        static int createCallback(void* data, int argc, char** argv, char** azColName);

        /// <summary>
        /// insert 回调
        /// </summary>
        /// <param name="NotUsed"></param>
        /// <param name="argc"></param>
        /// <param name="argv"></param>
        /// <param name="azColName"></param>
        /// <returns>static int</returns>
        static int insertCallback(void* NotUsed, int argc, char** argv, char** azColName);

        /// <summary>
        /// inster操作
        /// </summary>
        /// <param name="columns">std::map需要更新的列</param>
        /// <returns>int</returns>
        int insert(std::map<const char*, const char* > columns);

        /// <summary>
        /// select操作
        /// </summary>
        /// <param name="columns">需要查询的列，默认*</param>
        /// <returns>int</returns>
        int select(const char* columns = "*");
        /*
        * 查询单列结果值
        */
        //int column(const char* column,const char* type = "utf8");

        /*
        * 设置查询条件
        * const char* column 查询的列
        * const char* op = NULL 查询运算符 >、<、=、LIKE、NOT
        * const char* value = NULL 查询值
        * const char* _boolean 
        */
        SQLite* where(const char* column,const char* op = NULL,const char* value = NULL , const char* _boolean = "AND");
        /* 关闭数据库链接 */
        void close() {
            sqlite3_close(db);
        }
};

