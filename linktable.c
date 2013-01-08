/*************************************************************************/
/* Copyright (C) Network Programming -USTC, 2012                         */
/*                                                                       */
/*  File Name              : linktabl.c                                  */
/*  Pricipal Author        : Ming Lee                                    */
/*  Subsystem Name         :                                             */
/*  Module Name            :                                             */
/*  Language               :                                             */
/*  Target Environment     :                                             */
/*  Created Time           : Sun 06 Jan 2013 02:00:44 PM CST             */
/*  Description            : The implementation of link table            */
/*************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#inlcude "linktabl.h"

/* create link table */
link_table_t *create_linktable()
{
    link_table_t *linktable = (link_table_t *)malloc(sizeof(link_table_t));
    if (linktable == NULL)
    {
        return NULL;
    }
    linktable->head = NULL;
    linktable->tail = NULL;
    linktable->nodes_num = 0;
    return linktable;
}
    
/* delete link table */
int delete_link_table_node(link_table_t *linktable)
{
    if(linktable == NULL)
        return -1;
    link_table_node_t *p = linktable->head;
    link_table_node_t *q;
    while(p != NULL)
    {
        q = p->next;
        free(p);
        p = q;
    }
    linktable->head = NULL;
    linktable->tail = NULL;
    linktable->nodes_num = 0;
    return 0;
}

/* add link_table_node */
int add_link_table_node(link_table_t *linktable, link_table_node_t *node)
{
    if(linktable->head == NULL)
    {
        linktable->head = node;
        linktable->tail = node;
    }
    else
    {
        linktable->tail->next = node;
        linktable->tail = node;
        linktable->nodes_num ++;
    }
    return 0;
}

/* delete a link_table_node */
int delete_link_table_node(link_table_t *linktable, link_table_node_t *node)
{
    if(node == linktable->head)
    {
        linktable->head = node->next;
        linktable->nodes_num --;
        if(linktable->nodes_num == 0)
            linktable->tail = NULL;
        return 0;
    }
    link_table_node_t *p = linktable->head;
    //link_table_node_t *q;
    while(p != NULL)
    {
       // q = p;
        if(p->next == node)
        {
            p->next = node->next;
            if(node == linktable->tail)
                linktable->tail = p;
            linktable->nodes_num --;
            return 0;
        }
        p = p->next;
    }

    return -1;
}

/* get link table header */
link_table_node_t *get_link_table_header(link_table_t *linktable)
{
    return linktable->head;

}

/* get next link table node */
link_table_node_t *get_next_link_table_node(link_table_t *linktable, link_table_node_t *node)
{
    return node->next;
}
