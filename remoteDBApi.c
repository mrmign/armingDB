/********************************************************************/
/* Copyright (C) SSE-USTC, 2012                                     */
/*                                                                  */
/*  FILE NAME             :  remoteDBApi.c                          */
/*  PRINCIPAL AUTHOR      :  MingLee                                */
/*  SUBSYSTEM NAME        :  network                                */
/*  MODULE NAME           :  dbapi                                  */
/*  LANGUAGE              :  C                                      */
/*  TARGET ENVIRONMENT    :  Linux                                  */
/*  DATE OF FIRST RELEASE :  2012/12/14                             */
/*  DESCRIPTION           :  Implementation of the db api for client*/
/********************************************************************/

/*
 * Revision log:
 * Created by MingLee,2012/12/14
 *
 */
#include "debug.h"
#include "dbApi.h"
#include "protocol.h"
#include "socketwrapper.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#define PORT                5001
#define IP_ADDR             "127.0.0.1"
#define MAX_BUF_LEN         1024

ServiceHandler h = -1;

void printRec(mDataFormat data)
{
    printf("cmd:%d\nnum:%d\nlen1:%d\nvalu1:%s\nlen2:%d\nvalu2:%s\n", data->cmd,data->value_num,data->len_value1,data->value1,data->len_value2,data->value2);
}

Database createNewDB(char *dbName)
{
    debug;
    h = open_remote_service(IP_ADDR,PORT);
    if(h == -1)
    {
        exit(-1);
    }
    mDataFormat d = (mDataFormat) malloc(sizeof(struct DataFormat));
    d->cmd = OPEN_CMD;
    d->value_num = 1;
    d->len_value1 = strlen(dbName);
    d->value1 = dbName;

    char buf[MAX_BUF_LEN] = "\0";
    int bufSize = 0;
    bufSize = format_data(buf, d);
    if (bufSize == 0)
    {
        printf("Formatdata error!\n");
    }

    send_data(h,buf,bufSize);
    bufSize = MAX_BUF_LEN;
    receive_data(h,buf,&bufSize);
    parse_data(buf, d);
    if(d->cmd == 0)
    {
        fprintf(stderr,"Remote DBCreate Error,%s:%d\n", __FILE__,__LINE__);
    }
    free(d);
    return (Database)&h;
}

/*
 * close DB
 */ 
int closeDB(Database db)
{
    debug;
    mDataFormat d = (mDataFormat) malloc(sizeof(struct DataFormat));
    d->cmd = CLOSE_CMD;
    d->value_num = 0;

    char buf[MAX_BUF_LEN] = "\0";
    int bufSize = 0;
    bufSize = format_data(buf, d);
    if (bufSize == 0)
    {
        printf("Formatdata error!\n");
    }

    send_data(*(ServiceHandler *)db,buf,bufSize);
    bufSize = MAX_BUF_LEN;
    receive_data(*(ServiceHandler *)db,buf,&bufSize);
    parse_data(buf, d);

    if(d->cmd != CLOSE_CMD)
    {
        fprintf(stderr,"Remote close Error,%s:%d\n", __FILE__,__LINE__);
        free(d);
        return -1;
    } 
    free(d);
    return 0;

}

/*
 * set value
 */
int putKeyValue(Database db, int key, Data *tdata)
{
    mDataFormat d = (mDataFormat) malloc(sizeof(struct DataFormat));
    d->cmd = SET_CMD;
    d->value_num = 2;
    d->len_value2 = strlen(tdata->value);
    d->value2 = tdata->value;

    d->len_value1 = sizeof(int);
    d->value1 = (char *)&key;

    char buf[MAX_BUF_LEN] = "\0";
    int bufSize = 0;
    bufSize = format_data(buf, d);
    if (bufSize == 0)
    {
        printf("Formatdata error!\n");
    }

    send_data(*(ServiceHandler *)db,buf,bufSize);
    bufSize = MAX_BUF_LEN;
    receive_data(*(ServiceHandler *)db,buf,&bufSize);
    parse_data(buf, d);
    if(d->cmd != SET_CMD)
    {
        fprintf(stderr,"Remote DBCreate Error,%s:%d\n", __FILE__,__LINE__);
        free(d);
        return -1;
    } 
    free(d);
    return 0;
}
/*
 * get value with the key
 */
int getValueByKey(Database db, int key, Data *result)
{
    mDataFormat d = (mDataFormat) malloc(sizeof(struct DataFormat));
    d->cmd = GET_CMD;
    d->value_num = 1;

    d->len_value1 = sizeof(int);
    d->value1 = (char *)&key;

    char buf[MAX_BUF_LEN] = "\0";
    int bufSize = 0;
    bufSize = format_data(buf, d);
    if (bufSize == 0)
    {
        printf("Formatdata error!\n");
    }

    send_data(*(ServiceHandler *)db,buf,bufSize);
    bufSize = MAX_BUF_LEN;
    char value[MAX_BUF_LEN] = "\0";
    d->value1 = value;
    receive_data(*(ServiceHandler *)db,buf,&bufSize);
    parse_data(buf, d);

    // printRec(d);

    result->length = d->len_value1;
    strncpy(result->value, d->value1, result->length);

    if(d->cmd != GET_CMD)
    {
        fprintf(stderr,"Remote DBCreate Error,%s:%d\n", __FILE__,__LINE__);
        fprintf(stderr, "Get message from server:%s\n", result->value);
        free(d);
        return -1;
    } 
    free(d);
    return 0;
}

/*
 * delete one record
 */
int deleteValueByKey(Database db, int key)
{
    mDataFormat d = (mDataFormat) malloc(sizeof(struct DataFormat));
    d->cmd = DEL_CMD;
    d->value_num = 1;
    // char keybuf[5] = "\0";
    // sprintf(keybuf,"%d",key);
    // d->len_value1 = strlen(keybuf);
    // d->value1 = keybuf;
    d->len_value1 = sizeof(int);
    d->value1 = (char *)&key;

    char buf[MAX_BUF_LEN] = "\0";
    int bufSize = 0;
    bufSize = format_data(buf, d);
    if (bufSize == 0)
    {
        printf("Formatdata error!\n");
        free(d);
        return -1;
    }

    send_data(*(ServiceHandler *)db,buf,bufSize);
    bufSize = MAX_BUF_LEN;
    receive_data(*(ServiceHandler *)db,buf,&bufSize);
    parse_data(buf, d);
    if(d->cmd != DEL_CMD)
    {
        fprintf(stderr,"Remote Delete Error,%s:%d\n", __FILE__,__LINE__);
        free(d);
        return -1;
    } 
    free(d);
    return 0;
}
