#pragma once
#ifndef SQLITEO_H
#define SQLITEO_H  extern "C" _declspec(dllimport) 
#include <sqlite3.h>
#include <stdio.h>
#include <string>
#include <map>
#include <array>
#include "json/json.h"
#endif //SQLITEO_H

class SQLiteO
{
    /*定义私有属性*/
    private:

        /* err_message*/
        char* zErrMsg = 0;
        
        /* result data*/
        const char* data = "Callback function called";
        /* 表名 */
        const char* table_name;
        //std::string begin_transaction_clause_string = "";
        //std::string end_transaction_clause_string = "";
        std::string base_where_clause_string = "";
        std::string base_where_limit_clause_string = "";
        std::string base_where_orderby_clause_string = "";
        /* GROUP BY 子句放在 WHERE 子句之后，放在 ORDER BY 子句之前 */
        std::string base_where_groupby_clause_string = "";
        /* 与 SELECT 语句一起使用，来消除所有重复的记录，并只获取唯一一次记录 */
        std::string base_where_distinct_clause_string = "";
        std::string base_where_join_clause_string = "";
        bool in_array(const char* needle,const char *haystack[],int char_size) {
            int i, thisindex = -1;

            //int size = sizeof(haystack) / sizeof(haystack[0]);
            //std::cout << char_size << std::endl;
            for (i = 0; i < char_size; i++) {
                
                if (strcmp(needle, haystack[i]) == 0) {
                    thisindex = i; //返回元素所在位置
                    break;
                }
            }
            if (thisindex >= 0) {
                return true;
            }
            else {
                return false;
            }
        }
        /* 检查tablename */
        bool is_tablename() {
            typedef int (*cmpfunc)(void*, void*);

            const char* nums[4] = { ""," ","\0",NULL};
            int i, thisindex = -1;
            int size = sizeof(nums) / sizeof(nums[0]);
            for (i = 0; i < 10; i++) {
                if (nums[i] == this->table_name) {
                    thisindex = i; //返回元素所在位置
                    break;
                }
            }
            if (thisindex < 0) {
                return true;
            }
            else {
                std::cout << "No query form is specified 亲，叫个表吧，求你了！" << std::endl;
                return false;
            }
        }
        /// <summary>
        /// 原生sql查询
        /// </summary>
        /// <param name="sql"></param>
        /// <param name="callback"></param>
        /// <param name="_data"></param>
        /// <param name="errmsg"></param>
        /// <returns></returns>
        int query(const char* sql,                           /* SQL to be evaluated */
            int (*callback)(void*, int, char**, char**),  /* Callback function */
            void* _data,                                    /* 1st argument to callback */
            char** errmsg) {
            return sqlite3_exec(this->db, sql, callback, _data, errmsg);
        }


    /*定义私有属性*/
    protected:
        /* 数据库对象 */
		sqlite3* db = NULL;
        /// <summary>
        /// 检查传入map数据是否为空
        /// </summary>
        /// <param name="mapData"></param>
        /// <returns></returns>
        bool is_empty_map_datas(std::map<const char*, const char* > mapData) {
            if (mapData.empty()) {
                std::cout << "map is empty 这啥也没有啊，搞我呢？" << std::endl;
                return true;
            }
            else {
                return false;
            }
        }

        const char* operators[24] = {
            "==","=","!=","<>",">","<",">=","<=","!<","!>",
            "AND","BETWEEN","EXISTS","IN","NOT IN","LIKE",
            "GLOB","NOT","OR","IS NULL","IS","IS NOT","||","UNIQUE"
        };

    /*定公共有属性*/
	public: 
        /* 等号  */
        const char* EQUAL_SIGN_CHARACTER = "=";
        /* 空格  */
        const char* SPACE_CHARACTER = " ";
        /* 目录分隔符 : / */
        const char* DIRECTORY_SEPARATOR = "/";
        /* 目录分隔符 : / */
        const char* DS = "/";
        /* 单引号:' */
        const char* SINGLE_QUOTES_CHARACTER = "\'";
        /* 分号 : ; */
        const char* SEMICOLON_CHARACTER = ";";
        /* 换行 : \n */
        const char* LINE_BREAK_CHARACTER = "\n";
        /// <summary>
        /// 括号
        /// </summary>
        /// <returns>（</returns>
        const char* PARENTHESES_CHARACTER = "(";
        /* 构造函数 */
        SQLiteO(const char* database_name=NULL) {
            if (NULL != database_name) {
                sqlite3_open(database_name, &db);
                //this->open(database_name);
            }
        }
        /* 析构函数*/
        ~SQLiteO() {
            this->close();
            
        }
        /* 设置表名 */
        SQLiteO* table(const char* table_name);
		//void SQLiteCreateDeviceListTable(void);
        static SQLiteO* open(const char* db_name);

        /*
        * 创建数据表
        */
        int create_table(std::map<const char*, const char*>  create_columns);
        /* 检查数据表是否存在 */
        bool is_table();
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
        /// create function callback
        /// </summary>
        /// <param name="data"></param>
        /// <param name="argc"></param>
        /// <param name="argv"></param>
        /// <param name="azColName"></param>
        /// <returns>static int</returns>
        static int baseCallback(void* data, int argc, char** argv, char** azColName);

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
        /// update 回调
        /// </summary>
        /// <param name="NotUsed"></param>
        /// <param name="argc"></param>
        /// <param name="argv"></param>
        /// <param name="azColName"></param>
        /// <returns>static int</returns>
        static int updateCallback(void* data, int argc, char** argv, char** azColName);
        /// <summary>
        /// delete 回调
        /// </summary>
        /// <param name="NotUsed"></param>
        /// <param name="argc"></param>
        /// <param name="argv"></param>
        /// <param name="azColName"></param>
        /// <returns>static int</returns>
        static int deleteCallback(void* data, int argc, char** argv, char** azColName);
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
        Json::Value select(const char* columns = "*");
        /// <summary>
        /// 查询数量
        /// </summary>
        /// <param name="limit">数量</param>
        /// <returns></returns>
        SQLiteO* limit(int limit);
        /// <summary>
        /// 排序
        /// </summary>
        /// <param name="column">列</param>
        /// <param name="order">排序规则 默认DESC</param>
        /// <returns></returns>
        SQLiteO* orderBy(const char* column,const char* order="DESC");
        /// <summary>
        /// groupBy 用于与 SELECT 语句一起使用，来对相同的数据进行分组
        /// </summary>
        /// <param name="column">列</param>
        /// <param name="order">排序规则 默认DESC</param>
        /// <returns></returns>
        SQLiteO* groupBy(const char* column);
        /// <summary>
        /// groupBy 用于与 SELECT 语句一起使用，来对相同的数据进行分组
        /// </summary>
        /// <param name="column">列</param>
        /// <param name="order">排序规则 默认DESC</param>
        /// <returns></returns>
        SQLiteO* distinct();
        SQLiteO* join(const char* table,  const char* first, const char* op = NULL, const char* second = NULL, const char* type = "INNER");
        int count();
        /// <summary>
        /// 更新
        /// </summary>
        /// <returns>int</returns>
        int update(std::map<const char*, const char* > datas);
        /// <summary>
        /// 删除数据
        /// </summary>
        /// <returns></returns>
        int delete_data();
        /// <summary>
        /// 删除数据表
        /// </summary>
        /// <returns></returns>
        int delete_table(const char* table_name=NULL);
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
        SQLiteO* where(const char* column,const char* op = NULL,const char* value = NULL , const char* _boolean = "AND");
        /// <summary>
        /// 开始事务
        /// </summary>
        void begin_transaction() {
            
            this->query("BEGIN TRANSACTION;",0,0,0);
        }
        /// <summary>
        /// 结束事务
        /// </summary>
        void end_transaction() {
            this->query("END TRANSACTION;", 0, 0, 0);
        }
        /// <summary>
        /// 撤销事务
        /// </summary>
        void rollback_transaction() {
            this->query("ROLLBACK;", 0, 0, 0);
        }
        /* 关闭数据库链接 */
        void close() {
            sqlite3_close(db);
        }
};

