/********************************************************************/
/* Copyright (C) SSE-USTC, 2012                                     */
/*                                                                  */
/*  FILE NAME             :  testsocketwrapperclient.c              */
/*  PRINCIPAL AUTHOR      :  MingLee                                */
/*  SUBSYSTEM NAME        :  test                                   */
/*  MODULE NAME           :  client                                 */
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
    char szMsg[MAX_BUF_LEN] = "hello\0";
    ServiceHandler h = -1;
    h = open_remote_service(IP_ADDR,PORT);
    send_data(h,szMsg,strlen(szMsg));
    printf("client send:%s\n",szMsg); 
    receive_data(h,szBuf,&size); 
    printf("client recv:%s\n",szBuf);     
    close_remote_service(h);
    return 0;
}