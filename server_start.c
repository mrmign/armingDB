/********************************************************************/
/* Copyright (C) MC2Lab-USTC, 2012                                  */
/*                                                                  */
/*  FILE NAME             :  server_start.c                         */
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
#include "server.h"
#include <stdio.h>

#define PORT                5001
#define IP_ADDR             "127.0.0.1"

int main(int argc, char **argv)
{
    /* Server Engine for Client's Connections */
    printf("Arming Database Server starts!\n");
    char *addr;
    int port;
    if(argc < 3)
    {
        addr = IP_ADDR;
        port = PORT;
        create_cluster(addr,port);
    }
    else
    {
        addr = argv[1];
        port = atoi(argv[2]);
        load_cluster_nodes(addr,port);
    }

    service_engine(addr, port);
    return 0;    
}