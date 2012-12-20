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
    // if(cmd == CLOSE_CMD && DataNum == 0)
    // {
    //     printf("Test Zero Data Result: Pass\n");
    // }
    // BufSize = MAX_BUF_LEN;
    // FormatData1(Buf,&BufSize,OPEN_CMD,str,strlen(str));
    // ParseData(Buf,MAX_BUF_LEN,&cmd,&DataNum,Data1,&Data1Size,Data2,&Data2Size);
    // if(cmd == OPEN_CMD && DataNum == 1
    //     && Data1Size == strlen(str)
    //     && strcmp(Data1,str) == 0)
    // {
    //     printf("%s\n",Data1);
    //     printf("Test One Data Result: Pass\n");
    // }
    // BufSize = MAX_BUF_LEN;
    // FormatData2(Buf,&BufSize,SET_CMD,str,strlen(str),str,strlen(str));
    // ParseData(Buf,MAX_BUF_LEN,&cmd,&DataNum,Data1,&Data1Size,Data2,&Data2Size);
    // if(cmd == SET_CMD && DataNum == 2
    //     && Data1Size == strlen(str) && Data2Size == strlen(str)
    //     && strcmp(Data1,str) == 0 && strcmp(Data2,str) == 0)
    // {
    //     printf("%s\n",Data1);
    //     printf("%s\n",Data2);
    //     printf("Test Two Data Result: Pass\n");
    // }
    return 0;
}