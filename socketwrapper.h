/********************************************************************/
/* Copyright (C) SSE-USTC, 2012                                     */
/*                                                                  */
/*  FILE NAME             :  socketWrapper.h                        */
/*  PRINCIPAL AUTHOR      :  MingLee                                */
/*  SUBSYSTEM NAME        :  network                                */
/*  MODULE NAME           :  socketwraper                           */
/*  LANGUAGE              :  C                                      */
/*  TARGET ENVIRONMENT    :  Linux                                  */
/*  DATE OF FIRST RELEASE :  2012/12/07                             */
/*  DESCRIPTION           :  the interface to Linux system(socket)  */
/********************************************************************/

/*
 * Revision log:
 *
 * Created by MingLee,2012/12/09
 *
 */

#ifndef _SOCKET_WRAPPER_H_
#define _SOCKET_WRAPPER_H_

#include<stdio.h> 
#include<arpa/inet.h> /* internet socket */
#include<string.h>
//#define NDEBUG
#include<assert.h>

// #define PORT                5001
// #define IP_ADDR             "127.0.0.1"
// #define MAX_BUF_LEN         1024
#define MAX_CONNECT_QUEUE   128
typedef int ServiceHandler;
/* public api */

/*
 * initialize_service
 * input    : addr - server ip address
 * input    : port - server port to open
 * output   : None
 * in/out   : None
 * return   : if SUCCESS return 0
 *          : if FAILURE return (-1)
 */
int initialize_service(char *addr, int port);                             
 
/*
 * shutdown_service
 * input    : None
 * input    : None
 * output   : None
 * in/out   : None
 * return   : if SUCCESS return 0
 *          : if FAILURE return (-1)
 */        
int shutdown_service();                               
      
/*
 * open_remote_service
 * input    : addr - server ip address
 * input    : port - server port to open
 * output   : None
 * in/out   : None
 * return   : return the ServiceHandler (sockfd)
 */   
ServiceHandler open_remote_service(char *addr, int port);

/*
 * close_remote_service
 * input    : None
 * input    : None
 * output   : None
 * in/out   : None
 * return   : if SUCCESS return 0
 *          : if FAILURE return (-1)
 */ 
int close_remote_service(ServiceHandler h);
 
 /*
 * service_start
 * input    : addr - server ip address
 * input    : port - server port to open
 * output   : None
 * in/out   : None
 * return   : return the new ServiceHandler (sockfd)
 */              
ServiceHandler service_start();                               

/*
 * service_stop
 * input    : None
 * input    : None
 * output   : None
 * in/out   : None
 * return   : if SUCCESS return 0
 *          : if FAILURE return (-1)
 */ 
int service_stop(ServiceHandler h);


/*
 * send_data - send data to peer side
 * input    : h - it MUST BE what ServiceStart or OpenRemoteService returns.
 * input    : buf - point to data
 * input    : bufSize - the length of data bytes
 * output   : None
 * in/out   : None
 * return   : if SUCCESS return bufSize sended
 *          : if FAILURE return (-1)
 *          : if Connection Terminals return 0
 */
int send_data(ServiceHandler h, char *buf, int bufSize);

/*
 * receive_data - send data to peer side
 * input    : h - it MUST BE what ServiceStart or OpenRemoteService returns.
 * input    : buf - point to data
 * input    : bufSize - the length of data bytes
 * output   : None
 * in/out   : None
 * return   : if SUCCESS return bufSize sended
 *          : if FAILURE return (-1)
 *          : if Connection Terminals return 0
 */
int receive_data(ServiceHandler h, char *buf, int *bufSize);
        
#endif /* _SOCKET_WRAPPER_H_ */


