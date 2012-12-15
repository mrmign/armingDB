
/********************************************************************/
/* Copyright (C) MC2Lab-USTC, 2012                                  */
/*                                                                  */
/*  FILE NAME             :  server.c                               */
/*  PRINCIPAL AUTHOR      :  MingLee                                */
/*  SUBSYSTEM NAME        :  network                                */
/*  MODULE NAME           :  server                                 */
/*  LANGUAGE              :  C                                      */
/*  TARGET ENVIRONMENT    :  Linux                                  */
/*  DATE OF FIRST RELEASE :  2012/12/14                             */
/*  DESCRIPTION           :  Impement of Socket Server Engine       */
/*                           to handle clients requests.            */
/********************************************************************/

/*
 * Revision log:
 *
 * Created by MingLee ,2012/12/14
 *
 */

#include "dbApi.h"
#include "socketwrapper.h"
#include "protocol.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "debug.h"
#include <assert.h>
#define PORT                5001
#define IP_ADDR             "127.0.0.1"
#define MAX_BUF_LEN         1024


Database  db = NULL;

int handle_request(ServiceHandler h);

int main()
{
    /* Server Engine for Client's Connections */
    ServiceHandler h = -1;
    initialize_service(IP_ADDR,PORT);
    while(1)
    {
        h = service_start();
        handle_request(h);  
        service_stop(h); 
    }
    shutdown_service();
    return 0;
}
int error_response(ServiceHandler h,char * errorinfo)
{
   /* char Buf[MAX_BUF_LEN] = "\0";
    int BufSize = MAX_BUF_LEN;
    FormatData1(Buf,&BufSize,ERROR_RSP,errorinfo,strlen(errorinfo));
    SendData(h,Buf,BufSize);*/
    return 0;    
}

void printRec(mDataFormat data)
{
    printf("cmd:%d\nnum:%d\nlen1:%d\nvalu1:%s\nlen2:%d\nvalu2:%s\n", data->cmd,data->value_num,data->len_value1,data->value1,data->len_value2,data->value2);
}

int handle_request(ServiceHandler h)
{
   
    debug;
    char Buf[MAX_BUF_LEN] = "\0";
    int BufSize = MAX_BUF_LEN;
    char Data1[MAX_BUF_LEN] = "\0";
    char Data2[MAX_BUF_LEN] = "\0";

    mDataFormat data = (mDataFormat) malloc(sizeof(struct DataFormat));
    data->value1 = Data1;
    data->value2 = Data2;
    /* Handle Requests of One Client */
    while(1)
    {
        BufSize = MAX_BUF_LEN;

        if(receive_data(h,Buf,&BufSize) == 0)
        {
            fprintf(stderr,"Connection Error,%s:%d\n",__FILE__,__LINE__);
            return -1;            
        }
        if(BufSize == 0)
        {
            continue;
        }
        int ret = parse_data(Buf,data);
        printRec(data);
        if(ret == -1)
        {
            // ErrorResponse(h,"Data Format Error!\n");
            printf("server parse_data error!\n");
            continue;
        }
        if(data->cmd == OPEN_CMD)
        {
            debug;
            db = createNewDB(data->value1);
            BufSize = MAX_BUF_LEN;
            data->value_num = 0;
            BufSize = format_data(Buf,data);
            assert(BufSize > 0);
            send_data(h,Buf,BufSize);
        }
        else if(data->cmd == CLOSE_CMD)
        {
            closeDB(db);
            BufSize = MAX_BUF_LEN;
            data->value_num = 0;
            BufSize = format_data(Buf,data);
            send_data(h,Buf,BufSize);        
            return 0;
        }
        else if(data->cmd == GET_CMD)
        {
            /*if(Data1Size != sizeof(tKey))
            {
                fprintf(stderr,"Data Format Error,%s:%d\n",__FILE__,__LINE__);
                continue;            
            }*/

            int key = atoi(data->value1);
            Data value;
            // Data2Size = MAX_BUF_LEN;
            // value.length = Data2Size;
            value.value = Data2;
            ret = getValueByKey(db, key, &value);
            if(ret == -1)
            {
                error_response(h,"The key NOT FOUND!\n");
                continue;
            }
            BufSize = MAX_BUF_LEN;
            data->value_num = 1;
            data->len_value1 = strlen(value.value);
            data->value1 = value.value;
            BufSize = format_data(Buf,data);
            send_data(h,Buf,BufSize);
        }
        else if(data->cmd == SET_CMD)
        {
            /*if(Data1Size != sizeof(tKey))
            {
                fprintf(stderr,"Data Format Error,%s:%d\n",__FILE__,__LINE__);
                continue;            
            }*/
            int key = atoi(data->value1);
            Data value;
            // value.len = Data2Size;
            value.value = data->value2;
            // debug("SET_CMD:%d -> %s\n",*(tKey*)(Buf + 12),(char*)(Buf + 20));
            // debug("SET_CMD:%d -> %s\n",key,value.str);
            putKeyValue(db,key,&value);
            BufSize = MAX_BUF_LEN;
            data->value_num = 0;
            BufSize = format_data(Buf,data);
            send_data(h,Buf,BufSize);                             
        }
        else if(data->cmd == DEL_CMD)
        {
            /*if(Data1Size != sizeof(tKey))
            {
                fprintf(stderr,"Data Format Error,%s:%d\n",__FILE__,__LINE__);
                continue;            
            }*/
            int key = atoi(data->value1);           
            ret = deleteValueByKey(db,key);
            if(ret == -1)
            {
                error_response(h,"The key NOT FOUND!\n");
                continue;
            }            
            BufSize = MAX_BUF_LEN;
            data->value_num = 0;
            BufSize = format_data(Buf,data);
            send_data(h,Buf,BufSize);             
        }
        else
        {
            printf("Unknow Request!\n");
        }                                   
    }    
}
