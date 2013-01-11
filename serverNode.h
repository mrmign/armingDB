/*************************************************************************/
/* Copyright (C) Network Programming -USTC, 2012                         */
/*                                                                       */
/*  File Name              : serverNode.h                                */
/*  Pricipal Author        : Ming Lee                                    */
/*  Subsystem Name         : distribute                                  */
/*  Module Name            : node                                        */
/*  Language               : C                                           */
/*  Target Environment     : Linux                                       */
/*  Created Time           : Thu 10 Jan 2013 06:44:58 PM CST             */
/*  Description            :                                             */
/*************************************************************************/

/*
 * Revision log:
 * created by MingLee 2012/1/8
 */
#ifndef _SERVER_NODE_H_
#define _SERVER_NODE_H_
#include <stdio.h>
#include "protocol.h"

#define MAX_NODE_NUM    1024
#define ADDR            "127.0.0.1"
#define PORT            5001

#define ADDR_LENGTH     128

typedef struct server_node
{
    int hash;
    char addr[ADDR_LENGTH];
    int port;
    int fd;
}server_node_t;

typedef struct cluster
{
    int nodes_num;
    server_node_t *nodes[MAX_NODE_NUM];
}cluster_t;

/*
 * Init cluster node
 * input	: None
 * output	: None
 * in/out	: None
 * return	: icluster node
 */
cluster_t *init_cluster();

/*
 * delete cluster
 * input    : cluster node
 * output   : None
 * in/out   : None
 * return   : 0 if SUCCESS, -1 if FAIL
 */ 
int destroy_cluster(cluster_t *cluster);

/*
 * get nodes info 
 * input    : cluster, hash
 * output   : None
 * in/out   : None
 * return   : server node
 */
server_node_t *get_node(cluster_t *cluster, int hash);

/*
 * Add node to cluster
 * input    : cluster, addr, port
 * outpur   : None
 * in/out   : None
 * return   : 0 if SUCCESS, -1 if FAIL
 */
int add_node(cluster_t *cluster, char *addr, int port);

/*
 * remove node
 * input    : cluster, hash, addr, port, fd
 * output   : None
 * in/out   : None
 * return   : 0 if SUCCESS, -1 if FAIL
 */
int remove_node(cluster_t *cluster, int hash, char *addr, int port, int fd);

/*
 * Register and request nodes info
 * input    : addr, port
 * output   : None
 * in/out   : None
 * return   : cluster_t
 */
cluster_t *  register_and_load_cluster_nodes(char *addr, int port);

/*
 * Add cluster nodes
 * input    : cluster,nodeinfo, node num
 * output   : None
 * in/out   : None
 * return   : 0 if SUCCESS, -1 if FAIL
 */
int add_cluster_nodes(cluster_t *cluster, char ppData[MAX_DATA_LEN][MAX_DATA_LEN], int dataNum);

/*
 * Get cluster nodes info
 * input    : cluster, ppData, nodenum
 * output   : None
 * in/out   : ppData
 * return   : 0 if SUCCESS, -1 if FAIL
 */
int cluster_nodes_info(cluster_t *cluster, char ppData[MAX_DATA_LEN][MAX_DATA_LEN], int *dataNum);

#endif /* _SERVER_NODE_H_ */
