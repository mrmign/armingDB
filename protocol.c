/********************************************************************/
/* Copyright (C) SSE-USTC, 2012                                     */
/*                                                                  */
/*  FILE NAME             :  protocol.c                             */
/*  PRINCIPAL AUTHOR      :  MingLee                                */
/*  SUBSYSTEM NAME        :  network                                */
/*  MODULE NAME           :  protocol                               */
/*  LANGUAGE              :  C                                      */
/*  TARGET ENVIRONMENT    :  Linux                                  */
/*  DATE OF FIRST RELEASE :  2012/12/14                             */
/*  DESCRIPTION           :  the interface to Linux system(socket)  */
/********************************************************************/

/*
 * Revision log:
 *
 * Created by MingLee,2012/12/14
 *
 */
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include "protocol.h"
#include "debug.h"

// typedef struct DataHeader
// {
// 	int cmd;
// 	int value_num;

// } mDataHeader;

/*
 * format data
 */
int format_data( char *buf, mDataFormat data)
{
	debug;
	int size = 0;
	int *cmd = (int *) buf;
	*cmd = htonl(data->cmd);
	size += sizeof(data->cmd);
	int *num = (int *)(buf + size);
	*num = htonl(data->value_num);
	size += sizeof(data->value_num);

	if(data->value_num == 1)
	{
		int *len1 = (int *)(buf + size);
		*len1 = htonl(data->len_value1);
		size += sizeof(data->len_value1);
		memcpy(buf + size, data->value1, data->len_value1);
		size += data->len_value1;

	}
	else if(data->value_num == 2)
	{
		int *len1 = (int *)(buf + size);
		*len1 = htonl(data->len_value1);
		size += sizeof(data->len_value1);
		memcpy(buf + size, data->value1, data->len_value1);
		size += data->len_value1;

		int *len2 = (int *)(buf + size);
		*len2 = htonl(data->len_value2);

		size += sizeof(data->len_value2);
		memcpy(buf + size, data->value2, data->len_value2);
		size += data->len_value2;
	}

	return size;

}

/*
 * parse data
 */
int parse_data(char *buf, mDataFormat data)
{
	debug;
	int size = 0;
	int *cmd = (int *)buf;
	data->cmd = ntohl(*cmd);
	// printf("cmd %d\n", ntohl(*cmd));
	size += sizeof(data->cmd);
	int *num = (int *)(buf + size);
	data->value_num = ntohl(*num);
	size += sizeof(data->value_num);
	// int *le1 = (int *)(buf + size);
	// printf("cmd %d num %d len1 %d\n", ntohl(*cmd), ntohl(*num), ntohl(*le1));
	if (data->value_num == 0)
	{
		return 0;
	}
	else if (data->value_num == 1)
	{	
		int *len1 = (int *)(buf + size);
		data->len_value1 = ntohl(*len1);
		size += sizeof(data->len_value1);
		memcpy(data->value1, buf + size, data->len_value1);
		return 0;
	}
	else if( data->value_num == 2)
	{
		int *len1 = (int *)(buf + size);
		data->len_value1 = ntohl(*len1);
		size += sizeof(data->len_value1);
		memcpy(data->value1, buf + size, data->len_value1);
		size += data->len_value1;

		int *len2 = (int *)(buf + size);
		data->len_value2 = ntohl(*len2);
		size += sizeof(data->len_value2);
		memcpy(data->value2, buf + size, data->len_value2);
		return 0;
	}
	
	else 
	{
		printf("Parse Error !\n");
		return -1;
	}
}