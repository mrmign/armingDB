/********************************************************************/
/* Copyright (C) SSE-USTC, 2012                                     */
/*                                                                  */
/*  FILE NAME             :  socketWrapper.c                        */
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
 * Created by MingLee,2012/12/09
 * Modify some macros to public method, by Minglee, 2012/12/14
 *
 */


#include <stdio.h>
#include <stdlib.h> 
#include <arpa/inet.h> /* internet socket */
#include <string.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include "socketwrapper.h"
#include "socketwrapper_priv.h"
#include "debug.h" 
int sockfd = -1;
int epollfd = -1;
struct sockaddr_in serveraddr;                  
struct sockaddr_in clientaddr;  
// #define MAX_CONNECT_QUEUE 1023
//  private macro 
void PrepareSocket(char *addr,int port)                        
{
    debug;
    // struct sockaddr_in serveraddr;                  
    //      struct sockaddr_in clientaddr;                  
    socklen_t addr_len = sizeof(struct sockaddr);   
    serveraddr.sin_family = AF_INET;                
    serveraddr.sin_port = htons(port);              
    serveraddr.sin_addr.s_addr = inet_addr(addr);   
    memset(&serveraddr.sin_zero, 0, 8);             
    sockfd = socket(PF_INET,SOCK_STREAM,0);         
    if(sockfd == -1)                                
    {                                               
        fprintf(stderr,"Socket Error,%s:%d\n",      
                __FILE__,__LINE__);         
        close(sockfd);                              
        return ;                                  
    } 
}  

void InitServer()                                    
{
    debug;
    int ret = bind( sockfd,                         
            (struct sockaddr *)&serveraddr, 
            sizeof(struct sockaddr));       
    if(ret == -1)                                   
    {                                               
        fprintf(stderr,"Bind Error,%s:%d\n",        
                __FILE__,__LINE__);         
        close(sockfd);                              
        exit(-1);                                   
    }                                               
    listen(sockfd,MAX_CONNECT_QUEUE);  

}

void InitClient()                                    
{
    debug;
    int ret = connect(sockfd,                    
            (struct sockaddr *)&serveraddr,             
            sizeof(struct sockaddr));                   
    if(ret == -1)                                   
    {                                               
        fprintf(stderr,"Connect Error,%s:%d\n",     
                __FILE__,__LINE__);                     
        exit(-1);                                   
    } 

}   

int set_nonblocking(int fd)
{
    int opts;
    opts = fcntl(fd, F_GETFL);
    if (opts < 0)
    {
        printf("fcntl(sockfd, GETFL) error! %s %d\n", __FILE__, __LINE__);
        exit(-1);
    }

    opts = opts | O_NONBLOCK;
    if (fcntl(fd, F_SETFL opts) < 0 )
    {
        printf("fcntl(sockfd, SETFL, opts) error! %s %d \n", __FILE__, __LINE__);
        exit(-1);
    }
}

int initialize_service(char *addr, int port)
{
    debug;
    struct epoll_event event;
    PrepareSocket(addr,port);
    epollfd = epoll_create(MAX_CONNECT_FD);
    set_nonblocking(sockfd);
    event.data.fd = sockfd;
    event.events = EPOLLIN | EPOLLRDHUP;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &event);
    InitServer();
    return 0;
}

int shutdown_service()
{
    debug;
    if(sockfd == -1)
    {
        return -1;
    }
    close(sockfd);
    if ( epollfd == -1)
        return -1;
    close(epollfd);
    return 0;
}		

ServiceHandler open_remote_service(char *addr, int port)
{
    debug;
    PrepareSocket(addr,port);
    InitClient();
    return (ServiceHandler)sockfd;
}

int close_remote_service(ServiceHandler h)
{
    debug;
    if(h <= 0)
    {
        return -1;
    }
    close(h);
    return 0;
}

ServiceHandler service_start()
{
    debug;
    while (1)
    {
        struct epoll_event event;
        int fdnum = epoll_wait(epollfd, &event, 1, -1);
        if(event.data.fd == sockfd)
        {

            struct sockaddr_in clientaddr;
            socklen_t addr_len = sizeof(struct sockaddr);
            int newfd = accept( sockfd,(struct sockaddr *)&clientaddr,&addr_len);
            if(newfd == -1) 
            {
                fprintf(stderr,"Accept Error,%s:%d\n", __FILE__,__LINE__);
            }
            set_nonblocking(newfd);
            event.data.fd = newfd;
            event.events = EPOLLIN;
            epoll_ctl(epollfd, EPOLL_CTL_ADD, newfd, &event);
        }
        else if( (event.events & EPOLLIN) && (event.events & EPOLLRDHUP))
            close(event.data.fd);
        else

            return (ServiceHandler)newfd;
    }                            
}
int service_stop(ServiceHandler h)
{
    debug;
    if(h <= 0)
    {
        return -1;
    }
    close(h);
    return 0;   
}

int send_data(ServiceHandler h, char *buf, int bufSize)
{
    debug;
    int ret = send(h,buf,bufSize,0);
    if(ret < 0 || ret != bufSize)
    {
        fprintf(stderr,"Send Error,%s:%d\n",__FILE__,__LINE__);
        return -1;
    }
    return ret;
}

int receive_data(ServiceHandler h, char *buf, int *bufSize)
{
    debug;
    int ret = recv(h,buf,*bufSize,0);
    if(ret < 0)
    {
        fprintf(stderr,"Recv Error,%s:%d\n",__FILE__,__LINE__);
        *bufSize = 0;
        return -1;
    }
    *bufSize = ret;
    return ret;
}
