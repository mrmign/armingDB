/********************************************************************/
/* Copyright (C) SSE-USTC, 2012                                     */
/*                                                                  */
/*  FILE NAME             :  api.c                                  */
/*  PRINCIPAL AUTHOR      :  LeeMing                                */
/*  SUBSYSTEM NAME        :  db                                     */
/*  MODULE NAME           :  db                                     */
/*  LANGUAGE              :  C                                      */
/*  TARGET ENVIRONMENT    :  Linux                                  */
/*  DATE OF FIRST RELEASE :  2012/11/29                             */
/*  DESCRIPTION           :  the implementation to wrapped api of db*/
/********************************************************************/

/*
 * Revision log:
 *
 * Created by LeeMing,2012/11/29
 *
 */
#include <tcutil.h>
#include <tchdb.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "dbApi.h"

Database createNewDB(const char *dbName)
{
	TCHDB *tdb = tchdbnew();

	if (!tchdbopen(tdb, dbName, HDBOWRITER | HDBOCREAT))
	{
		int ecode = tchdbecode(tdb);
		fprintf(stderr, "close error: %s\n", tchdberrmsg(ecode));
		exit(-1);
	}
	return (Database) tdb;
}

/*
 * close DB
 */ 
bool closeDB(Database db)
{
	if (tchdbclose(db))
		return true;
	else
	{
		int ecode = tchdbecode(db);
		fprintf(stderr, "close error: %s\n", tchdberrmsg(ecode));
		return false;
	}
}

/*
 * set value
 */
int putKeyValue(Database db, int key, Data *tdata)
{
	int ecode;
	if (tchdbput(db, &key, sizeof(int), tdata->value,
			tdata->length))
		return 0;
	else
	{
		ecode = tchdbecode(db);
		fprintf(stderr, "insert error: %s\n", tchdberrmsg(ecode));
		return -1;
	}
}
/*
 * get value with the key
 */
int getValue(Database db, int key, Data *result)
{
	int ecode;
	int count;
	count = tchdbget3(db, &key, sizeof(int), result->value, 1024);
	if (!count)
	{
		ecode = tchdbecode(db);
		fprintf(stderr, "get error: %s\n", tchdberrmsg(ecode));
		return -1;
	}
	else
	{
		result->length = count;
		result->value[count] = '\0';
		return 0;
		
	}

}

/*
 * delete one record
 */
int deleteValueByKey(Database db, int key)
{
	int ecode;

	if (!tchdbout(db, &key, sizeof(key)))
	{
		ecode = tchdbecode(db);
		fprintf(stderr, "delete error: %s\n", tchdberrmsg(ecode));
		return -1;
	}

	return 0;
}
