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

#include <stdio.h>
#include "dbApi.h"
#include "remoteDBApi.h"
#include "serverNode.h"
// #define PORT                5001
// #define IP_ADDR             "127.0.0.1"
#define ADDR_LENGTH         128
#define MAX_DB_NODE_NUM     2
#define debug           printf
/*typedef struct db_node
{
    char addr[ADDR_LENGTH];
    int  port;
    int  db;
} db_node_t;// db server node 

db_node_t nodes[MAX_DB_NODE_NUM] = 
{
    {IP_ADDR, 5001, -1},
    {"192.168.1.12", 5001, -1}
};*/

/* create db */
Database createNewDB(char *dbName)
{
    debug("createDB\n");
    cluster_t *cluster = register_and_load_cluster_nodes(NULL,0);
    int i = 0;
    while(i<MAX_NODE_NUM)
    {
        server_node_t *node = (server_node_t*)get_node(cluster,i);
        debug("createdb: %s %d\n",node->addr,node->port);
        node->fd = remote_create_new_db(dbName,node->addr,node->port);
        if(node->fd == -1)
            return NULL;
        i = node->hash;
    }

    /*for (i = 0; i < MAX_DB_NODE_NUM; i++)
    {
        
        nodes[i].db = remote_create_new_db(dbName,nodes[i].addr, nodes[i].port);
        if (nodes[i].db == -1)
        {
            return NULL;
        }
    }*/
    return (Database)cluster;
}

/* delete database */
int closeDB(Database db)
{
    cluster_t *cluster = (cluster_t *)db;
    int i = 0;
    while(i<MAX_NODE_NUM)
    {
        server_node_t *node = (server_node_t*)get_node(cluster,i);
        if(node->fd != -1)
            remote_close_db(node->fd);
        i = node->hash;
    }
    
    return 0;
}

/* set key/value */
int putKeyValue(Database db, int key, Data *tdata)
{
    cluster_t *cluster = (cluster_t *)db;
    int index = key % MAX_NODE_NUM;
    server_node_t *node = (server_node_t*)get_node(cluster,index);
    return remote_set_key_value(node->fd, key, tdata);
}

/* get value by key */
int getValueByKey(Database db, int key, Data *result)
{
    cluster_t *cluster = (cluster_t *)db;
    int index = key % MAX_NODE_NUM;
    server_node_t *node = (server_node_t*)get_node(cluster,index);
    return remote_get_value_by_key(node->fd, key, result);
}    
/* delete value by key */
int deleteValueByKey(Database db, int key)
{
    cluster_t *cluster = (cluster_t *)db;
    int index = key % MAX_NODE_NUM;
    server_node_t *node = (server_node_t*)get_node(cluster,index);
    return remote_delete_value_by_key(node->fd, key);

}
