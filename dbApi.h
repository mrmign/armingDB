
/********************************************************************/
/* Copyright (C) SSE-USTC, 2012                                     */
/*                                                                  */
/*  FILE NAME             :  api.h                                  */
/*  PRINCIPAL AUTHOR      :  LeeMing                                */
/*  SUBSYSTEM NAME        :  db                                     */
/*  MODULE NAME           :  db                                     */
/*  LANGUAGE              :  C                                      */
/*  TARGET ENVIRONMENT    :  Linux                                  */
/*  DATE OF FIRST RELEASE :  2012/11/22                             */
/*  DESCRIPTION           :  the interface of my db operation       */
/********************************************************************/

/*
 * Revision log:
 *
 * Created by LeeMing,2012/11/29
 *
 */


#ifndef API_H_
#define API_H_

typedef void* Database;
// typedef unsigned int key;

typedef struct myData
{
	int length;   /*the length of value*/
	char *value;
} Data;

/*
 * Create an DB
 * input	: None
 * output	: None
 * in/out	: None
 * return	: the database file
 */
Database createNewDB(char *dbName);

/*
 * Close  DB
 * input	: Database  File
 * output	: bool
 * in/out	: None
 * return	: SUCCESS(true)/FAILURE(false)
 */
int closeDB(Database db);
// bool closeDB(Database db);
/*
 * save key/value into database
 * input	: database name, data to be saved
 * output	: None
 * in/out	: None
 * return	: SUCCESS(0)/FAILURE(-1)
 */
int putKeyValue(Database db, int key, Data *tdata);

/*
 * get value by the key
 * input	: database name, key
 * output	: None
 * in/out	: tdata must be initialized. We will put tdata->key and tdata->value into db
 * return	: the value with the key
 */
int getValueByKey(Database db, int key, Data *result);

/*
 * delete one record by the key
 * input	: database name, key
 * output	: None
 * in/out	: None
 * return	: the deleted value
 */
int deleteValueByKey(Database db, int key);


/**********************************************/
/* Memory Database(no file) Operations
**********************************************/
/*
 * Create an Memory Database
 * input	: None
 * output	: None
 * in/out	: None
 * return	: if SUCCESS return (tDatabase *)Database handler
 *          : if FAILURE exit(-1)
 */
Database  create_MDB();
	
/*
 * Delete the Database
 * input	: tDatabase db
 * output	: None
 * in/out	: None
 * return	: SUCCESS(0)/exit(-1)
 */
int delete_MDB(Database mdb);


/*
 * Set key/value
 * input	: tKey key,tValue value - one key/value
 * output	: None
 * in/out	: None
 * return	: SUCCESS(0)/FAILURE(-1)
 */	
int putKeyValue_MDB(Database mdb,int key,Data value);

/*
 * get key/value
 * input	: tKey key
 * output	: None
 * in/out	: tValue *pvalue MUST BE initialized,it means pvalue->str is malloced,
            : and pvalue->len is the length of pvalue->str 
            : if return SUCCESS(0),value will stored in pvalue(str,len).
 * return	: SUCCESS(0)/FAILURE(-1)
 */
int getValueByKey_MDB(Database mdb,int key,Data *value);

/*
 * delete key/value
 * input	: tKey key
 * output	: None
 * in/out	: None
 * return	: SUCCESS(0)/FAILURE(-1)
 */
int deleteValueByKey_MDB(Database mdb,int key);

#endif /* API_H_ */
