
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
typedef unsigned int key;

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
Database createNewDB(const char *dbName);

/*
 * Close  DB
 * input	: Database  File
 * output	: bool
 * in/out	: None
 * return	: SUCCESS(true)/FAILURE(false)
 */

bool closeDB(Database db);
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


#endif /* API_H_ */
