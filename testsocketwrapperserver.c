/********************************************************************/
/* Copyright (C) SSE-USTC, 2012                                     */
/*                                                                  */
/*  FILE NAME             :  testsocketwrapperserver.c              */
/*  PRINCIPAL AUTHOR      :  MingLee                                */
/*  SUBSYSTEM NAME        :  test                                   */
/*  MODULE NAME           :  server                                 */
/*  LANGUAGE              :  C                                      */
/*  TARGET ENVIRONMENT    :  Any                                    */
/*  DATE OF FIRST RELEASE :  2012/12/14                             */
/*  DESCRIPTION           :  Reply Service Provider                 */
/********************************************************************/

/*
 * Revision log:
 *
 * Created by MingLee,2012/12/14
 *
 */
 
#include"socketwrapper.h"
#include<stdio.h>
#include<stdlib.h> 
#include<string.h>

#define PORT                5001
#define IP_ADDR             "127.0.0.1"
#define MAX_BUF_LEN         1024

int main()
{
    char szBuf[MAX_BUF_LEN] = "\0";
    int size = MAX_BUF_LEN;
    char szReplyMsg[MAX_BUF_LEN] = "hi\0";
    ServiceHandler h = -1;
    initialize_service(IP_ADDR,PORT);
    while(1)
    {
        h = service_start();
        receive_data(h,szBuf,&size);
        printf("server recv:%s\n",szBuf); 
        send_data(h,szReplyMsg,strlen(szReplyMsg));
        printf("server send:%s\n",szReplyMsg);
        service_stop(h); 
    }
    shutdown_service();
    return 0;
}