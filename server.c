
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
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define PORT                5001
#define IP_ADDR             "127.0.0.1"
#define MAX_BUF_LEN         1024

#define MAX_TASK_NUM    3
pthread_t thread_id[MAX_TASK_NUM];
sem_t event[MAX_TASK_NUM];
typedef struct task_node
{
    ServiceHandler req;// request sockfd
    char buf[MAX_BUF_LEN];//
    int buf_size;

    struct task_node *next;//next node
    struct task_node *last;//if it is the first node, it will point to the last node,if not, it is null 
}task_node_t;
task_node_t *task_list[MAX_TASK_NUM];// all the task 

/* generate random int */
int random_int(int x)
{
    srand((unsigned)time(0));
    return random() % x;
}


Database  mdb = NULL;
void init_mdb()
{
    mdb = create_MDB();
}
void close_mdb()
{
    delete_MDB(mdb);
}
void match_sockfd_mdb(int sockfd, Database db)
{
    debug_argv("sock:%d\n", sockfd);
    Data data;
    data.value = (char *)&db;
    data.length = sizeof(Database);
    putKeyValue_MDB(mdb, sockfd, data);
}
void detach_sockfd_mdb(int sockfd)
{
    deleteValueByKey_MDB(mdb, sockfd);
}
void get_mdb(int sockfd, Database* db)
{
    char val[1024] = "\0";
    Data data;
    data.value = (char *)db;
    data.length = sizeof(Database);
    debug_argv("key:%d\n",sockfd);
    getValueByKey_MDB(mdb, sockfd, &data);
    debug_argv("db:%p\n",*data.value);
}

int handle_requests(int task_num);
int handle_one_request(ServiceHandler h, char *buf, int buf_size);

int main(int argc, char **argv)
{
    /* Server Engine for Client's Connections */
    printf(" Arming Database Server starts!\n");
    char *addr;
    int port;
    if(argc < 3)
    {
        addr = IP_ADDR;
        port = PORT;
    }
    else
    {
        addr = argv[1];
        port = atoi(argv[2]);
    }
    int i;
    if(MAX_TASK_NUM > 0)
    {
        for (i = 0; i < MAX_TASK_NUM; i++)
        {
            sem_init(&event[i], 0, 0);
            int temp = i;
            if (pthread_create(&thread_id[i], NULL, (void *)handle_requests, (void *)temp) != 0)
            {
                printf("pthread_create error, %s:%d\n",__FILE__, __LINE__);
                exit(-1);
            }

        }
    }
    /* 
     * command line console
     */

    init_mdb();

    ServiceHandler h = -1;
    initialize_service(addr,port);
    while(1)
    {
        h = service_start();
        //        handle_request(h);  
        task_node_t *tnode = malloc( sizeof(task_node_t));
        tnode->buf_size = MAX_BUF_LEN;
        if (receive_data (h, tnode->buf, &(tnode->buf_size)) == 0)
        {
            service_stop(h);
            continue;
        }

        if (MAX_TASK_NUM > 0)
        {
            tnode->req = h;
            int i = random_int(MAX_TASK_NUM);
            /* put tnode into task_list */
            if (task_list[i] == NULL)
            {
                task_list[i] = tnode;
                tnode->next = NULL;
                tnode->last = tnode;
            }
            else
            {
                task_node_t *p = task_list[i];
                p->last->next = tnode;
                p->last = tnode;
                tnode->next = NULL;
            }
            sem_post(&event[i]);

            //        service_stop(h); 
        }
        else 
        {
            handle_one_request(h, tnode->buf, tnode->buf_size);
            free(tnode);
        }
    }
    shutdown_service();
    close_mdb();
    if (MAX_TASK_NUM > 0 )
    {
        for (i = 0; i< MAX_TASK_NUM; i++)
        {
            task_node_t *p = task_list[i];
            task_node_t *q;
            while (p)
            {
                q = p->next;
                free(p);
                p = q;
            }
        }
    }
    return 0;
}

/*
 * deal with incorrect operation
 */
int error_response(ServiceHandler h,char * errorinfo)
{
    char Buf[MAX_BUF_LEN] = "\0";
    int BufSize = MAX_BUF_LEN;
    mDataFormat data = (mDataFormat) malloc(sizeof(struct DataFormat));
    data->cmd = ERROR_RESP;
    data->value_num = 1;
    data->len_value1 = strlen(errorinfo);
    data->value1 = errorinfo;

    BufSize = format_data(Buf, data);
    send_data(h,Buf,BufSize);
    free(data);
    return 0;    
}

void printRec(mDataFormat data)
{
    printf("cmd:%d\nnum:%d\nlen1:%d\nvalu1:%s\nlen2:%d\nvalu2:%s\n", data->cmd,data->value_num,data->len_value1,data->value1,data->len_value2,data->value2);
    // printf("%s:%d  %s\n",__FILE__,__LINE__,__FUNCTION__);
}

/*
 * handle user's requests
 */
int handle_requests(int task_num)
{
    ServiceHandler h = -1;
    int i = task_num;
    while (1)
    {
        sem_wait(&event[i]);
        task_node_t *pnode = NULL;
        pnode = task_list[i];
        h = pnode->req;

        // if (handle_one_request(h, pnode->buf, pnode->buf_size) == -1)
        //     continue;
        handle_one_request(h, pnode->buf, pnode->buf_size);     

        /* deal with pnode and its next node */
        if (pnode->next == NULL)
            task_list[i] = 0;
        else
        {
            task_node_t *p = pnode->next;
            p->last = pnode->last;
            task_list[i] = p;
        }
        free(pnode);
    }
}

int handle_one_request(ServiceHandler h, char *buf, int buf_size)
{

    debug;
    char Buf[MAX_BUF_LEN] = "\0";
    int BufSize = MAX_BUF_LEN;
    char Data1[MAX_BUF_LEN] = "\0";
    char Data2[MAX_BUF_LEN] = "\0";

    mDataFormat data = (mDataFormat) malloc(sizeof(struct DataFormat));
    data->value1 = Data1;
    data->value2 = Data2;

    memset(Data1, 0, MAX_BUF_LEN);
    memset(Data2, 0, MAX_BUF_LEN);
    data->value1 = Data1;
    data->value2 = Data2;

    /*if(receive_data(h,Buf,&BufSize) == 0)
      {
          fprintf(stderr,"Connection Error,%s:%d\n",__FILE__,__LINE__);
          return -1;            
      }*/
    if(BufSize == 0)
    {
        free(data);
        return -1;
    }
    int ret = parse_data(buf,data);

    // print the received data
    // printRec(data);

    if(ret == -1)
    {
        error_response(h,"Data Format Error!\n");
        printf("server parse_data error!\n");
        free(data);
        return -1;
    }
    if(data->cmd == OPEN_CMD)
    {
        debug;
        Database db = createNewDB(data->value1);
        debug_argv("open db:%p\n",db);
        match_sockfd_mdb(h, db);
        BufSize = MAX_BUF_LEN;
        data->value_num = 0;
        BufSize = format_data(Buf,data);
        assert(BufSize > 0);
        send_data(h,Buf,BufSize);
    }
    else if(data->cmd == CLOSE_CMD)
    {
        debug_argv("close db\n");
        Database db = NULL;
        get_mdb(h, &db);
        closeDB(db);
        BufSize = MAX_BUF_LEN;
        data->value_num = 0;
        BufSize = format_data(Buf,data);
        send_data(h,Buf,BufSize);
        detach_sockfd_mdb(h);
        service_stop(h);
        free(data);
        return 0;
    }
    else if(data->cmd == GET_CMD)
    {
        Database db = NULL;
        get_mdb(h, &db);

        //debug_argv("get db:%p\n",db);
        int key = *(int *)data->value1;
        Data value;

        char tem[MAX_BUF_LEN] = "\0";
        value.value = tem;
        ret = getValueByKey(db, key, &value);
       // printf("server getdata value => %s\n", value.value);
        if(ret == -1)
        {
            error_response(h,"The key NOT FOUND!\n");
            free(data);
            return -1;              
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
        int key = *(int *)data->value1;
        Data value;
        value.value = data->value2;
        value.length = data->len_value2;
        Database db = NULL;
        get_mdb(h, &db);
        debug_argv("set: db:%p\n",db);
        ret = putKeyValue(db,key,&value);
        if (ret == -1)
        {
            error_response(h,"Save key & value error!\n");
            printf("set error : %d, %s\n", key, value.value);
            free(data);
            return -1;
        }
        BufSize = MAX_BUF_LEN;
        data->value_num = 0;
        BufSize = format_data(Buf,data);
        send_data(h,Buf,BufSize);    

    }
    else if(data->cmd == DEL_CMD)
    {
        int key = *(int *)data->value1;;           
        Database db = NULL;
        get_mdb(h, &db);
        debug_argv("delete: db:%p\n",db);
        ret = deleteValueByKey(db,key);
        if(ret == -1)
        {
            error_response(h,"The key NOT FOUND!\n");
            free(data);
            return -1;
        }            
        BufSize = MAX_BUF_LEN;
        data->value_num = 0;
        BufSize = format_data(Buf,data);
        send_data(h,Buf,BufSize);             
    }
    else
    {
        printf("Unknow Request!\n");
        error_response(h, "Unknow Request!\n");
    }                                   
    free(data);
}
