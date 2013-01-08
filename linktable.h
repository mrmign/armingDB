/*************************************************************************/
/* Copyright (C) Network Programming -USTC, 2012                         */
/*                                                                       */
/*  File Name              : linktable.h                                 */
/*  Pricipal Author        : Ming Lee                                    */
/*  Subsystem Name         :                                             */
/*  Module Name            :                                             */
/*  Language               :                                             */
/*  Target Environment     :                                             */
/*  Created Time           : Sun 06 Jan 2013 01:27:49 PM CST             */
/*  Description            : The interface of link table                 */
/*************************************************************************/

#include<stdio.h>

#ifndef _LINK_TABLE_H_
#define _LINK_TABLE_H_

/* link table node */
typedef struct link_table_node
{
    struct link_table_node *next;
}link_table_node_t;

/* link table type */
typedef struct link_table
{
    struct link_table_node_t *head;
    struct link_table_node_t *tail;
    int nodes_num;
}link_table_t;

/*
 * create an empty link table
 * input: None
 * output: None
 * return:the pointer of the new link table 
 */
link_table_t * create_linktable();


/*
 * delete link table
 * input: Link table pointer
 * output: None
 * return:SUCCESS(0)/FAILURE(-1)
 */
int delete_link_table(link_table_t *plinktable);


/*
 * Add a link_table_node
 * input: link_table, link_table_node
 * output: None
 * return:SUCCESS(0)/FAILURE(-1)
 */
int add_link_table_node(link_table_t *linktable, link_table_node_t *node);


/*
 * Delete a link_table_node from link table
 * input: link_table, link_table_node
 * output: None
 * return:SUCCESS(0)/FAILURE(-1)
 */
int delete_link_table_node(link_table_t *linktable, link_table_node_t *node);


/*
 * Get the header of link table
 * input: link_table
 * output: None
 * return: link_table_t-> head
 *
 */
link_table_node_t *get_link_table_header(link_table_t *linktable);


/*
 * Get the next link table_node after the input node
 * input: link_table, link_table_node
 * output: None
 * return: link_table_node_t
 *
 */
link_table_node_t *get_next_link_table_node(link_table_t *linktable, link_table_node_t *node);
#endif /* _LINK_TABLE_H_ */
