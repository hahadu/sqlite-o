#pragma once

#include <sqlite3.h>
#include <stdio.h>
#include <string>
#include <map>
class SQLite
{
    /*����˽������*/
    private:
        /* err_message*/
        char* zErrMsg = 0;
        
        /* result data*/
        const char* data = "Callback function called";

        /* ���� */
        const char* table_name;
        std::string base_where_clause_string = "";
        /* ���tablename */
        bool is_tablename() {
            return NULL != table_name;
        }

    /*����˽������*/
    protected:
        /* ���ݿ���� */
		sqlite3* db = NULL;

    /*������������*/
	public: 
        /* �ո�  */
        const char* SPACE_CHARACTER = " ";
        /* Ŀ¼�ָ��� : / */
        const char* DIRECTORY_SEPARATOR = "/";
        /* Ŀ¼�ָ��� : / */
        const char* DS = "/";
        /* ������:' */
        const char* SINGLE_QUOTES_CHARACTER = "'";
        /* ˫���� : " */
        const char* DOUBLE_QUOTES_CHARACTER = "\"";
        /// <summary>
        /// ����
        /// </summary>
        /// <returns>��</returns>
        const char* PARENTHESES_CHARACTER = "(";
        /* ���캯�� */
        SQLite(const char* database_name=NULL) {
            if (NULL != database_name) {
                this->open(database_name);
            }
        }
        /* ��������*/
        ~SQLite() {
            this->close();
            
        }
        /* ���ñ��� */
        SQLite* table(const char* table_name);
		//void SQLiteCreateDeviceListTable(void);
        SQLite* open(const char* db_name);
        /*
        * �������ݱ�
        */
        int create_table(const char* create_columns);
        /* ������ݱ��Ƿ���� */
        bool is_table();
        /// <summary>
        /// ����
        /// </summary>
        /// <returns>int</returns>
        int update();
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
        /// insert �ص�
        /// </summary>
        /// <param name="NotUsed"></param>
        /// <param name="argc"></param>
        /// <param name="argv"></param>
        /// <param name="azColName"></param>
        /// <returns>static int</returns>
        static int insertCallback(void* NotUsed, int argc, char** argv, char** azColName);

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
        int select(const char* columns = "*");
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
        SQLite* where(const char* column,const char* op = NULL,const char* value = NULL , const char* _boolean = "AND");
        /* �ر����ݿ����� */
        void close() {
            sqlite3_close(db);
        }
};

