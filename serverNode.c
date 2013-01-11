/*************************************************************************/
/* Copyright (C) Network Programming -USTC, 2012                         */
/*                                                                       */
/*  File Name              : serverNode.c                                */
/*  Pricipal Author        : Ming Lee                                    */
/*  Subsystem Name         : Distribute                                  */
/*  Module Name            : node                                        */
/*  Language               : C                                           */
/*  Target Environment     : Linux                                       */
/*  Created Time           : Thu 10 Jan 2013 08:01:23 PM CST             */
/*  Description            : The implementation of cluster node          */
/*************************************************************************/

#include<stdio.h>
#include "serverNode.h"
#include "socketwrapper.h"
#include "protocol.h"
#include "serverNode.h"
#include <string.h>
#include <stdlib.h>
//#include "debug.h"

#define debug   printf

#define MAX_BUF_LEN     1024

/* init cluster */
cluster_t *init_cluster()
{
    cluster_t *cluster = (cluster_t *)malloc(sizeof(cluster_t));
    cluster->nodes_num = 0;
    int i;
    for (i = 0; i < MAX_NODE_NUM; i++)
    {
        cluster->nodes[i] = NULL;
    }
    return cluster;
}

/* destroy cluster */
int destroy_cluster(cluster_t *cluster)
{
    int i = 0;
    server_node_t *node = cluster->nodes[i];
    while(1)
    {
        i = node->hash;
        free(node);
        node = cluster->nodes[i];
        if(node->hash >= MAX_NODE_NUM)
            break;
    }
    free(node);
    return 0;
}

/* get hash value */
int get_hash_value(cluster_t *cluster, int nodes_num, int *start)
{
    int i = 0;
    int max = 0, prev = 0, next = 0;
    while(i < MAX_NODE_NUM)
    {
        server_node_t *node = cluster->nodes[i];
        if (max < node->hash - i)
        {
            max = node->hash - i;
            prev = i;
            next = node->hash;
        }
        i = node->hash;
    }
    *start = prev;
    return (next - prev)%2 == 0 ? (next - prev)/2 : (next - prev - 1)/2;
}

/* get cluster node info */
server_node_t * get_node(cluster_t *cluster, int hash)
{
    return cluster->nodes[hash];
}

/* add node to cluster */
int add_node(cluster_t *cluster, char *addr, int port)
{
    int i = 0;
    if(cluster->nodes_num >= MAX_NODE_NUM)
        return -1;
    server_node_t *node = (server_node_t *)malloc(sizeof(server_node_t));
    memcpy(node->addr, addr, strlen(addr));
    node->port = port;
    node->fd = -1;
    if(cluster->nodes_num == 0)
    {
        node->hash = MAX_NODE_NUM;
        for(i = 0;i<MAX_NODE_NUM;i++)
            cluster->nodes[i] = node;
    }
    else
    {
        int start;
        node->hash = get_hash_value(cluster, cluster->nodes_num, &start);
        for (i = start; i<node->hash;i++)
            cluster->nodes[i] = node;
    }
    cluster->nodes_num ++;
    return 0;
}

/* remove node */
int remove_node(cluster_t *cluster, int hash, char *addr, int port, int fd)
{
    return -1;
}

/* register and load cluster nodes */
cluster_t * register_and_load_cluster_nodes(char *addr, int port)
{
    int i;
    int DataNum = -1;
    char Buf[MAX_BUF_LEN] = "\0";
    int BufSize = MAX_BUF_LEN;
    char szMsg[MAX_BUF_LEN] = "\0";
    char ppData[MAX_DATA_NUM][MAX_DATA_LEN] = {0};
    ServiceHandler h = -1;
    h = open_remote_service(addr,port);
    if(addr == NULL)
    {
        DataNum = 0;
    }
    else
    {
        DataNum = 1;
        sprintf(ppData[0],"%s %d\0",addr,port);
        debug("pasrer sprintf :%s\n",ppData[0]);     
    }

    format_ctl_data(Buf,&BufSize,CTL_REG_CMD,ppData,DataNum);
    send_data(h,Buf,BufSize);
    receive_data(h,Buf,&BufSize);
    int cmd = -1;
    cluster_t *cluster = init_cluster();
    add_cluster_nodes(cluster, ppData, DataNum);
    close_remote_service(h);
    return cluster;
}

/* add cluster node */
int add_cluster_nodes(cluster_t *cluster, char ppData[MAX_DATA_LEN][MAX_DATA_LEN], int DataNum)
{
    int i;
    for(i=0;i<DataNum;i++)
    {
        char addr[MAX_DATA_LEN];
        int port;
        sscanf(ppData[i],"%s%d",addr,&port);
        debug("pasrer %s:%d\n",addr,port);
        add_node(cluster,addr,port);
    }
    return 0;
}

/* cluster nodes to array */
int cluster_nodes_info(cluster_t *cluster, char ppData[MAX_DATA_LEN][MAX_DATA_LEN], int *NodeNum)
{
    if(*NodeNum < cluster->nodes_num)
    {
        fprintf(stderr,"CloudNodesInfo Error,%s:%d\n",__FILE__,__LINE__);
        return -1;
    }
    *NodeNum = cluster->nodes_num;
    int i = 0; 
    int hash = 0;   
    while(hash < MAX_NODE_NUM)
    {
        server_node_t* pNode = cluster->nodes[hash];
        sprintf(ppData[i],"%s %d\0",pNode->addr,pNode->port);
        debug("pasrer sprintf :%s\n",ppData[i]);
        hash = pNode->hash;
        i++;
    }
    return 0; 
}

    
