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
    /*����˽������*/
    private:

        /* err_message*/
        char* zErrMsg = 0;
        
        /* result data*/
        const char* data = "Callback function called";
        /* ���� */
        const char* table_name;
        //std::string begin_transaction_clause_string = "";
        //std::string end_transaction_clause_string = "";
        std::string base_where_clause_string = "";
        std::string base_where_limit_clause_string = "";
        std::string base_where_orderby_clause_string = "";
        /* GROUP BY �Ӿ���� WHERE �Ӿ�֮�󣬷��� ORDER BY �Ӿ�֮ǰ */
        std::string base_where_groupby_clause_string = "";
        /* �� SELECT ���һ��ʹ�ã������������ظ��ļ�¼����ֻ��ȡΨһһ�μ�¼ */
        std::string base_where_distinct_clause_string = "";
        std::string base_where_join_clause_string = "";
        bool in_array(const char* needle,const char *haystack[],int char_size) {
            int i, thisindex = -1;

            //int size = sizeof(haystack) / sizeof(haystack[0]);
            //std::cout << char_size << std::endl;
            for (i = 0; i < char_size; i++) {
                
                if (strcmp(needle, haystack[i]) == 0) {
                    thisindex = i; //����Ԫ������λ��
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
        /* ���tablename */
        bool is_tablename() {
            typedef int (*cmpfunc)(void*, void*);

            const char* nums[4] = { ""," ","\0",NULL};
            int i, thisindex = -1;
            int size = sizeof(nums) / sizeof(nums[0]);
            for (i = 0; i < 10; i++) {
                if (nums[i] == this->table_name) {
                    thisindex = i; //����Ԫ������λ��
                    break;
                }
            }
            if (thisindex < 0) {
                return true;
            }
            else {
                std::cout << "No query form is specified �ף��и���ɣ������ˣ�" << std::endl;
                return false;
            }
        }
        /// <summary>
        /// ԭ��sql��ѯ
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


    /*����˽������*/
    protected:
        /* ���ݿ���� */
		sqlite3* db = NULL;
        /// <summary>
        /// ��鴫��map�����Ƿ�Ϊ��
        /// </summary>
        /// <param name="mapData"></param>
        /// <returns></returns>
        bool is_empty_map_datas(std::map<const char*, const char* > mapData) {
            if (mapData.empty()) {
                std::cout << "map is empty ��ɶҲû�а��������أ�" << std::endl;
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

    /*������������*/
	public: 
        /* �Ⱥ�  */
        const char* EQUAL_SIGN_CHARACTER = "=";
        /* �ո�  */
        const char* SPACE_CHARACTER = " ";
        /* Ŀ¼�ָ��� : / */
        const char* DIRECTORY_SEPARATOR = "/";
        /* Ŀ¼�ָ��� : / */
        const char* DS = "/";
        /* ������:' */
        const char* SINGLE_QUOTES_CHARACTER = "\'";
        /* �ֺ� : ; */
        const char* SEMICOLON_CHARACTER = ";";
        /* ���� : \n */
        const char* LINE_BREAK_CHARACTER = "\n";
        /// <summary>
        /// ����
        /// </summary>
        /// <returns>��</returns>
        const char* PARENTHESES_CHARACTER = "(";
        /* ���캯�� */
        SQLiteO(const char* database_name=NULL) {
            if (NULL != database_name) {
                sqlite3_open(database_name, &db);
                //this->open(database_name);
            }
        }
        /* ��������*/
        ~SQLiteO() {
            this->close();
            
        }
        /* ���ñ��� */
        SQLiteO* table(const char* table_name);
		//void SQLiteCreateDeviceListTable(void);
        static SQLiteO* open(const char* db_name);

        /*
        * �������ݱ�
        */
        int create_table(std::map<const char*, const char*>  create_columns);
        /* ������ݱ��Ƿ���� */
        bool is_table();
        /// <summary>
        /// select �ص�
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
        /// insert �ص�
        /// </summary>
        /// <param name="NotUsed"></param>
        /// <param name="argc"></param>
        /// <param name="argv"></param>
        /// <param name="azColName"></param>
        /// <returns>static int</returns>
        static int insertCallback(void* NotUsed, int argc, char** argv, char** azColName);
        /// <summary>
        /// update �ص�
        /// </summary>
        /// <param name="NotUsed"></param>
        /// <param name="argc"></param>
        /// <param name="argv"></param>
        /// <param name="azColName"></param>
        /// <returns>static int</returns>
        static int updateCallback(void* data, int argc, char** argv, char** azColName);
        /// <summary>
        /// delete �ص�
        /// </summary>
        /// <param name="NotUsed"></param>
        /// <param name="argc"></param>
        /// <param name="argv"></param>
        /// <param name="azColName"></param>
        /// <returns>static int</returns>
        static int deleteCallback(void* data, int argc, char** argv, char** azColName);
        /// <summary>
        /// inster����
        /// </summary>
        /// <param name="columns">std::map��Ҫ���µ���</param>
        /// <returns>int</returns>
        int insert(std::map<const char*, const char* > columns);

        /// <summary>
        /// select����
        /// </summary>
        /// <param name="columns">��Ҫ��ѯ���У�Ĭ��*</param>
        /// <returns>int</returns>
        Json::Value select(const char* columns = "*");
        /// <summary>
        /// ��ѯ����
        /// </summary>
        /// <param name="limit">����</param>
        /// <returns></returns>
        SQLiteO* limit(int limit);
        /// <summary>
        /// ����
        /// </summary>
        /// <param name="column">��</param>
        /// <param name="order">������� Ĭ��DESC</param>
        /// <returns></returns>
        SQLiteO* orderBy(const char* column,const char* order="DESC");
        /// <summary>
        /// groupBy ������ SELECT ���һ��ʹ�ã�������ͬ�����ݽ��з���
        /// </summary>
        /// <param name="column">��</param>
        /// <param name="order">������� Ĭ��DESC</param>
        /// <returns></returns>
        SQLiteO* groupBy(const char* column);
        /// <summary>
        /// groupBy ������ SELECT ���һ��ʹ�ã�������ͬ�����ݽ��з���
        /// </summary>
        /// <param name="column">��</param>
        /// <param name="order">������� Ĭ��DESC</param>
        /// <returns></returns>
        SQLiteO* distinct();
        SQLiteO* join(const char* table,  const char* first, const char* op = NULL, const char* second = NULL, const char* type = "INNER");
        int count();
        /// <summary>
        /// ����
        /// </summary>
        /// <returns>int</returns>
        int update(std::map<const char*, const char* > datas);
        /// <summary>
        /// ɾ������
        /// </summary>
        /// <returns></returns>
        int delete_data();
        /// <summary>
        /// ɾ�����ݱ�
        /// </summary>
        /// <returns></returns>
        int delete_table(const char* table_name=NULL);
        /*
        * ��ѯ���н��ֵ
        */
        //int column(const char* column,const char* type = "utf8");

        /*
        * ���ò�ѯ����
        * const char* column ��ѯ����
        * const char* op = NULL ��ѯ����� >��<��=��LIKE��NOT
        * const char* value = NULL ��ѯֵ
        * const char* _boolean 
        */
        SQLiteO* where(const char* column,const char* op = NULL,const char* value = NULL , const char* _boolean = "AND");
        /// <summary>
        /// ��ʼ����
        /// </summary>
        void begin_transaction() {
            
            this->query("BEGIN TRANSACTION;",0,0,0);
        }
        /// <summary>
        /// ��������
        /// </summary>
        void end_transaction() {
            this->query("END TRANSACTION;", 0, 0, 0);
        }
        /// <summary>
        /// ��������
        /// </summary>
        void rollback_transaction() {
            this->query("ROLLBACK;", 0, 0, 0);
        }
        /* �ر����ݿ����� */
        void close() {
            sqlite3_close(db);
        }
};

