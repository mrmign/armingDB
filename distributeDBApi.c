/*************************************************************************/
/* Copyright (C) Network Programming -USTC, 2012                         */
/*                                                                       */
/*  File Name              : distributeDBApi.c                           */
/*  Pricipal Author        : Ming Lee                                    */
/*  Subsystem Name         : distribute                                  */
/*  Module Name            : distribute api                              */
/*  Language               : C                                           */
/*  Target Environment     : ANY                                         */
/*  Created Time           : Sat 29 Dec 2012 04:49:48 PM CST             */
/*  Description            : Implementation of distribute api for client */
/*                           to call                                     */
/*************************************************************************/

#include<stdio.h>
#include "dbApi.h"
#include "remoteDBApi.h"

#define PORT                5001
#define IP_ADDR             "127.0.0.1"
#define ADDR_LENGTH         128
#define MAX_DB_NODE_NUM     2

typedef struct db_node
{
    char addr[ADDR_LENGTH];
    int  port;
    int db;
} db_node_t;// db server node 

db_node_t nodes[MAX_DB_NODE_NUM] = 
{
    {IP_ADDR, 5001, -1},
    {"192.168.1.12", 5001, -1}
};

/* create db */
Database createNewDB(char *dbName)
{
    int i = 0;
    for (i = 0; i < MAX_DB_NODE_NUM; i++)
    {
        nodes[i].db = remote_create_new_db(dbName,nodes[i].addr, nodes[i].port);
        if (nodes[i].db == -1)
        {
            return NULL;
        }
    }
    return (Database)nodes;
}

/* delete database */
int closeDB(Database db)
{
    db_node_t *dbnodes = (db_node_t *)db;
    int i = 0;
    for(i = 0; i<MAX_DB_NODE_NUM; i++)
    {
        remote_close_db(dbnodes[i].db);
    }
    return 0;
}

/* set key/value */
int putKeyValue(Database db, int key, Data *tdata)
{
    db_node_t *dbnodes = (db_node_t *)db;
    int index = key % MAX_DB_NODE_NUM;
    return remote_set_key_value(dbnodes[index].db, key, tdata);
}

/* get value by key */
int getValueByKey(Database db, int key, Data *result)
{
    db_node_t *dbnodes = (db_node_t *)db;
    int index = key % MAX_DB_NODE_NUM;
    return remote_get_value_by_key(dbnodes[index].db, key, result);
}    
/* delete value by key */
int deleteValueByKey(Database db, int key)
{
    db_node_t *dbnodes = (db_node_t *)db;
    int index = key % MAX_DB_NODE_NUM;
    return remote_delete_value_by_key(dbnodes[index].db, key);

}
