/********************************************************************/
/* Copyright (C) SSE-USTC, 2012                                     */
/*                                                                  */
/*  FILE NAME             :  socketWrapper.c                        */
/*  PRINCIPAL AUTHOR      :  MingLee                                */
/*  SUBSYSTEM NAME        :  network                                */
/*  MODULE NAME           :  socketwraper                           */
/*  LANGUAGE              :  C                                      */
/*  TARGET ENVIRONMENT    :  Linux                                  */
/*  DATE OF FIRST RELEASE :  2012/11/22                             */
/*  DESCRIPTION           :  the interface to Linux system(socket)  */
/********************************************************************/

/*
 * Revision log:
 * Created by MingLee,2012/12/09
 * Modify some macros to public method, by Minglee, 2012/12/14
 *
 */
int sockfd = -1;
 /* private macro */
#define PrepareSocket(addr,port)                        \
        int sockfd = -1;                                \
        struct sockaddr_in serveraddr;                  \
        struct sockaddr_in clientaddr;                  \
        socklen_t addr_len = sizeof(struct sockaddr);   \
        serveraddr.sin_family = AF_INET;                \
        serveraddr.sin_port = htons(port);              \
        serveraddr.sin_addr.s_addr = inet_addr(addr);   \
        memset(&serveraddr.sin_zero, 0, 8);             \
        sockfd = socket(PF_INET,SOCK_STREAM,0);
        
#define InitServer()                                    \
        int ret = bind( sockfd,                         \
                        (struct sockaddr *)&serveraddr, \
                        sizeof(struct sockaddr));       \
        if(ret == -1)                                   \
        {                                               \
            fprintf(stderr,"Bind Error,%s:%d\n",        \
                            __FILE__,__LINE__);         \
            close(sockfd);                              \
            return -1;                                  \
        }                                               \
        listen(sockfd,MAX_CONNECT_QUEUE); 

#define InitClient()                                    \
        int ret = connect(sockfd,                       \
            (struct sockaddr *)&serveraddr,             \
            sizeof(struct sockaddr));                   \
        if(ret == -1)                                   \
        {                                               \
            fprintf(stderr,"Connect Error,%s:%d\n",     \
                __FILE__,__LINE__);                     \
            return -1;                                  \
        }


int initialize_service(char *addr, int port)
{
	PrepareSocket(IP_ADDR,PORT);
	InitServer();
}
  
int shutdown_service()
{
	if(sockfd == -1)
    {
        return -1;
    }
    close(sockfd);
    return 0;
}		
         
ServiceHandler open_remote_service(char *addr, int port)
{
	PrepareSocket(addr,port);
    InitClient();
    return (ServiceHandler)sockfd;
}

int close_remote_service()
{
	if(h <= 0)
    {
        return -1;
    }
    close(h);
    return 0;
}
              
ServiceHandler service_start()
{
	struct sockaddr_in clientaddr;
    socklen_t addr_len = sizeof(struct sockaddr);
    int newfd = accept( sockfd,(struct sockaddr *)&clientaddr,&addr_len);
    if(newfd == -1) 
    {
        fprintf(stderr,"Accept Error,%s:%d\n", __FILE__,__LINE__);
    }
    return (ServiceHandler)newfd;
}                            

int service_stop()
{
	if(h <= 0)
    {
        return -1;
    }
    close(h);
    return 0;   
}

int send_data(ServiceHandler h, char *buf, int bufSize)
{
	int ret = send(h,pBuf,BufSize,0);
    if(ret < 0 || ret != BufSize)
    {
        fprintf(stderr,"Send Error,%s:%d\n",__FILE__,__LINE__);
        return -1;
    }
    return ret;
}

int receive_data(ServiceHandler h, char *buf, int bufSize)
{
	int ret = recv(h,pBuf,*pBufSize,0);
    if(ret < 0)
    {
        fprintf(stderr,"Recv Error,%s:%d\n",__FILE__,__LINE__);
        *pBufSize = 0;
        return -1;
    }
    *pBufSize = ret;
    return ret;
}