/*************************************************************************/
/* Copyright (C) Network Programming -USTC, 2012                         */
/*                                                                       */
/*  File Name              : remoteDBApi.h                               */
/*  Pricipal Author        : Ming Lee                                    */
/*  Subsystem Name         : db                                          */
/*  Module Name            : remotedbapi                                 */
/*  Language               : C                                           */
/*  Target Environment     : ANY                                         */
/*  Created Time           : Sat 29 Dec 2012 04:11:33 PM CST             */
/*  Description            : remote db api interfaces                    */
/*************************************************************************/

/*
 * Revision log:
 *
 * created by Ming Lee, 2012/12/29
 *
 */ 

#ifndef _REMOTE_DB_API_H__
#define _REMOTE_DB_API_H__

#include<stdio.h>
#include "dbApi.h"

/*
 * Create an Database
 * input: dbname,addr,port
 * output: None
 * in/output: None
 * return: if SUCCESS return (int)Database handler
 *       : if FAILURE return (-1)
 */
int remote_create_new_db(char *dbname, char *addr, int port);

/*
 * Delete the Database
 * input: int db
 * output: None
 * in/output: None
 * return: SUCCESS(0)/exit(-1)
 */
int remote_close_db(int db);

/*
 * Set key/value
 * input: key,value - one key/value
 * output: None
 * in/output: None
 * return: SUCCESS(0)/FAILURE(-1)
 */
int remote_set_key_value(int db, int key, Data *value);

/*
 * get key/value
 * input: key
 * output: None
 * in/output: result->value must point to a buffer to store the value
            : if return SUCCESS(0),value will stored in pvalue(str,len).
 * return: SUCCESS(0)/FAILURE(-1)
 */
int remote_get_value_by_key(int db, int key, Data *result);

/*
 * delete key/value
 * input: key
 * output: None
 * in/output: None
 * return: SUCCESS(0)/FAILURE(-1)
 */
int remote_delete_value_by_key(int db, int key);

#endif /* _REMOTE_DB_API_H__ */
