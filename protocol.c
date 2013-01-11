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

#define debug 	printf
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

/* format the control data */
int format_ctl_data(char *pBuf,int *pBufSize,int cmd,char ppData[MAX_DATA_NUM][MAX_DATA_LEN],int DataNum)
{
	int i = 0;
    int DataSize = 0;
    for(i=0;i<DataNum;i++)
    {
        DataSize += strlen(ppData[i]);
    }
   
    mDataFormat pData = (mDataFormat)pBuf;
    pData->cmd = htonl(cmd);
    pData->value_num = htonl(DataNum);
    // pData->len_value1 = htonl(0);
    char * p = pBuf + 2 * sizeof(int); /* point to pData->len_value1 */
    for(i=0;i<DataNum;i++)
    {
        *(int*)p = htonl(strlen(ppData[i]));
        p += sizeof(int);        
        memcpy(p,ppData[i],strlen(ppData[i]));
        p += strlen(ppData[i]);
    }     
    return strlen(pBuf); 
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

/* parse control data */
int parse_ctl_data(char *pBuf,int BufSize,int *pcmd,int *pDataNum,char ppData[MAX_DATA_NUM][MAX_DATA_LEN])
{
	mDataFormat pData = (mDataFormat)pBuf;
    *pcmd = ntohl(pData->cmd);
    *pDataNum = ntohl(pData->value_num);
    debug_argv("parse control data CMD:%d,DataNum:%d\n",*pcmd,*pDataNum);
    if(*pDataNum > MAX_DATA_NUM)
    {
        fprintf(stderr,"Parse control Error:*pDataNum > MAX_DATA_NUM,%s:%d\n", __FILE__,__LINE__);
        debug_argv("\n");
        return -1;        
    }
    int DataSize = 0;
    char * p = pBuf + 2 * sizeof(int);//sizeof(mDataFormat) -2 * ( sizeof(int) + sizeof(char*));
    int i = 0;
    for(i=0;i<(*pDataNum);i++)
    {
        DataSize = ntohl(*(int*)p);
        debug_argv("%d,%s\n",DataSize,p + sizeof(int));
        if(DataSize > MAX_DATA_LEN)
        {
            fprintf(stderr,"Parse control Error:DataSize > MAX_DATA_LEN,%s:%d\n", __FILE__,__LINE__);
            debug_argv("\n");
            return -1;        
        }        
        p += sizeof(int);
        memcpy(ppData[i],p,DataSize);
        ppData[i][DataSize] = '\0';
        debug_argv("%d %s\n",DataSize,ppData[i]);
        p += DataSize;
    }
    return 0;   
}

/* parse cmd */
int parse_cmd(char * pBuf,int BufSize,int *pcmd,int *pDataNum)
{
	mDataFormat pData = (mDataFormat)pBuf;
    *pcmd = ntohl(pData->cmd);

    *pDataNum = ntohl(pData->value_num);
    return 0;  
}