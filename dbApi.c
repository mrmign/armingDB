/********************************************************************/
/* Copyright (C) SSE-USTC, 2012                                     */
/*                                                                  */
/*  FILE NAME             :  dbApi.c                                */
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
#include <string.h>
#include "dbApi.h"
#include "debug.h"
#include <pthread.h>

TCMDB *all_opened_db = NULL;
pthread_mutex_t dbmutex;
typedef struct opened_db
{
    TCHDB *hdb;
    int counter;
}mOpenedDB;
Database createNewDB(char *dbName)
{
    debug;
    int code;
    TCHDB *hdb;
    if (all_opened_db == NULL )
    {
        all_opened_db = tcmdbnew();
        pthread_mutex_init(&dbmutex, NULL);
    }
    else
    {
        int vsize = -1;
        pthread_mutex_lock(&dbmutex);
        mOpenedDB *opendb = (mOpenedDB *)tcmdbget(all_opened_db, (void *)dbName, strlen(dbName), &vsize);
        if(opendb != NULL)
        {
            hdb = opendb->hdb;
            opendb->counter ++;
            tcmdbput(all_opened_db, (void *)dbName, strlen(dbName), (void *)opendb, vsize);
            pthread_mutex_unlock(&dbmutex);
            free (opendb);
            return (Database)hdb;
        }
        pthread_mutex_unlock(&dbmutex);
    }

    hdb = tchdbnew();

    //set mutex exclusion
    tchdbsetmutex(hdb);

    if (!tchdbopen(hdb, dbName, HDBOWRITER | HDBOCREAT | HDBONOLCK))
    {
        int ecode = tchdbecode(hdb);
        fprintf(stderr, "close error: %s\n", tchdberrmsg(ecode));
        exit(-1);
    }
    mOpenedDB db;
    db.hdb = hdb;
    db.counter = 1;
    tcmdbput(all_opened_db, (void *)dbName, strlen(dbName), (void *)&db, sizeof(mOpenedDB));
    return (Database) hdb;
}

/*
 * close DB
 */ 
int closeDB(Database db)
{
    debug;
    TCHDB *hdb = (TCHDB *)db;
    int code;
    int i;
    long sum = (long)tcmdbrnum(all_opened_db);
    tcmdbiterinit(all_opened_db);
    for (i = 0; i < sum; i++)
    {
        int ksize = -1;
        char *dbname = (char *)tcmdbiternext(all_opened_db, &ksize);
        if (dbname == NULL)
        {
            printf("DBClose: can't find the db %s %d\n", __FILE__, __LINE__);
            break;
        }
        int vsize = -1;
        pthread_mutex_lock(&dbmutex);
        mOpenedDB *opendb = (mOpenedDB *)tcmdbget(all_opened_db, (void *)dbname, ksize, &vsize);
        if(opendb != NULL && opendb->hdb == hdb)
        {
            opendb->counter --;
            if(opendb->counter <= 0)
            {
                tcmdbout(all_opened_db,(void *)dbname, ksize);
                pthread_mutex_unlock(&dbmutex);
                free(dbname);
                free(opendb);
                break;
            }
            tcmdbput(all_opened_db, (void *)dbname, ksize, (void *)opendb, vsize);
            pthread_mutex_unlock(&dbmutex);
            free(dbname);
            free(opendb);
            return 0;
        }
        pthread_mutex_unlock(&dbmutex);
        free(dbname);
        free(opendb);
    }
    if (tchdbclose(db))
    {
        tchdbdel(hdb);
        return 0;
    }
    else
    {
        int ecode = tchdbecode(db);
        fprintf(stderr, "close error: %s\n", tchdberrmsg(ecode));
        return -1;
    }
}

/*
 * set value
 */
int putKeyValue(Database db, int key, Data *tdata)
{
    debug;
    int ecode;
    debug_argv("server put key value: %d => %s\n", key, tdata->value);
    if (tchdbput(db, &key, sizeof(int), tdata->value, tdata->length))
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
int getValueByKey(Database db, int key, Data *result)
{
    debug;
    int ecode;
    int count;
    debug_argv("server get key: %d\n", key);
    count = tchdbget3(db, &key, sizeof(int), result->value, 1024);
    if (count == -1)
    {
        ecode = tchdbecode(db);
        fprintf(stderr, "get error: %s\n", tchdberrmsg(ecode));
        return -1;
    }
    else
    {
        result->length = count;
        result->value[count] = '\0';

        // printf("server getvalue: count:%d  value: %d => %s\n",count, key, result->value);
        return 0;

    }

}

/*
 * delete one record
 */
int deleteValueByKey(Database db, int key)
{

    debug;
    int ecode;
    debug_argv("delete key:%d\n",key);
    if (!tchdbout(db, &key, sizeof(key)))
    {
        ecode = tchdbecode(db);
        fprintf(stderr, "delete error: %s\n", tchdberrmsg(ecode));
        return -1;
    }

    return 0;
}

/*
 * Create an Memory Database
 */
Database  create_MDB()
{
    TCMDB * mdb = tcmdbnew();
    return (Database)mdb;
}

/*
 * Delete the Database
 */
int delete_MDB(Database mdb)
{
    tcmdbdel((TCMDB*)mdb);
    return 0;
}

/*
 * Set key/value
 */ 
int putKeyValue_MDB(Database mdb,int key,Data value)
{
    debug_argv("key: %d db: %p\n", key, *value.value);
    tcmdbput((TCMDB*)mdb,(void*)&key,sizeof(int),value.value,value.length); 
    return 0;  
}

/*
 * get key/value
 */
int getValueByKey_MDB(Database mdb,int key,Data *value)
{
    int vsize = -1;
    char *v = tcmdbget((TCMDB*)mdb,(void*)&key,sizeof(int),&vsize);
    debug_argv("key: %d db:%p\n",key,*v);
    if(v != NULL && vsize > 0 && vsize <= value->length)
    {
        memcpy(value->value,v,vsize);
        value->length = vsize;
        return 0;
    }
    return -1;
}

/*
 * delete key/value
 */
int deleteValueByKey_MDB(Database mdb,int key)
{
    tcmdbout((TCMDB*)mdb,(void*)&key,sizeof(int));
    return 0;
}
