/********************************************************************/
/* Copyright (C) SSE-USTC, 2012                                     */
/*                                                                  */
/*  FILE NAME             :  testprotocol.c                         */
/*  PRINCIPAL AUTHOR      :  MingLee                                */
/*  SUBSYSTEM NAME        :  network                                */
/*  MODULE NAME           :  protocol                               */
/*  LANGUAGE              :  C                                      */
/*  TARGET ENVIRONMENT    :  Linux                                  */
/*  DATE OF FIRST RELEASE :  2012/12/15                             */
/*  DESCRIPTION           :  protocol between client and server     */
/********************************************************************/


/*
* Revision log:
*
* Created by MingLee,2012/12/15
*
*/
#include "protocol.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_BUF_LEN 1024
int main()
{
    char Buf[MAX_BUF_LEN] = "\0";
    int BufSize = MAX_BUF_LEN;
    char *str = "arming.hdb";
    char val1[MAX_BUF_LEN] = "\0";
    char val2[MAX_BUF_LEN] = "this is a teate ljfdkla  !";
    char val3[MAX_BUF_LEN]="\0";
    char val4[MAX_BUF_LEN]="\0";

    mDataFormat data = (mDataFormat) malloc(sizeof(struct DataFormat));
    data->cmd = SET_CMD;
    data->value_num = 2;
    val1[0]='2';
    data->len_value1 = strlen(val1);
    data->value1 = val1;
    data->len_value2 = strlen(val2);
    data->value2 = val2;
    int size = format_data(Buf, data);
    
    printf("format_data %s  size %d\n", Buf,size);

    mDataFormat rec = (mDataFormat) malloc(sizeof(struct DataFormat));
    rec->value1 = val3;
    rec->value2 = val4;
    parse_data(Buf, rec);
    printf("rec %d, %d, %s \n", rec->cmd, rec->value_num, rec->value1);
    assert(rec->cmd > 0);
    if (rec->cmd == SET_CMD)
    {
        printf("success! value1 %s\n",rec->value1);
    }
    else
    {
        printf("test error!\n");
    }
   

   char ppData[10][MAX_DATA_LEN] = 
    {
        {"127.0.0.1 5001"},
        {"127.0.0.2 5002"},
        {"127.0.0.3 5003"},
        {"127.0.0.4 5004"},
        {"127.0.0.5 5005"},
        {"127.0.0.6 5006"},
        {"127.0.0.7 5007"},
        {"127.0.0.8 5008"},
        {"127.0.0.9 5009"},
        {"127.0.0.10 5010"}
    };
    char ppData1[10][MAX_DATA_LEN] = {0};
    BufSize = MAX_BUF_LEN;
    format_ctl_data(Buf,&BufSize,CTL_REG_CMD,ppData,10);
    int DataNum = 0;
    int cmd = -1;
    parse_ctl_data(Buf,MAX_BUF_LEN,&cmd,&DataNum,ppData1);
    printf("CMD:%d,DataNum:%d\n",cmd,DataNum);
    int i = 0;
    for(i=0;i<10;i++)
    {
        printf("%s\n",ppData1[i]);
    }
    return 0;
}