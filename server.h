
/********************************************************************/
/* Copyright (C) MC2Lab-USTC, 2012                                  */
/*                                                                  */
/*  FILE NAME             :  server.h                               */
/*  PRINCIPAL AUTHOR      :  MingLee                                */
/*  SUBSYSTEM NAME        :  network                                */
/*  MODULE NAME           :  server                                 */
/*  LANGUAGE              :  C                                      */
/*  TARGET ENVIRONMENT    :  Linux                                  */
/*  DATE OF FIRST RELEASE :  2013/1/10                              */
/*  DESCRIPTION           :  Interface of  Server Engine            */
/********************************************************************/

/*
 * Revision log:
 *
 * Created by MingLee ,2013/1/10
 *
 */

 #ifndef _SERVER_H_
 #define _SERVER_H_

/*
 * server engine for clients' requests
 * input	: addr, port
 * output	: None
 * in/out	: None
 * return	: if SUCCESS return the size of the data to be sent
 			: if FAIL return  -1 
 */
int service_engine(char *addr, int port);

/*
 * create cluster on master server
 * input	: addr, port
 * output	: None
 * in/out	: None
 * return	: if SUCCESS return the size of the data to be sent
 			: if FAIL return  -1 
 */
int create_cluster(char *addr, int port);

/*
 * Load cluster nodes
 * input	: addr, port
 * output	: None
 * in/out	: None
 * return	: if SUCCESS return the size of the data to be sent
 			: if FAIL return  -1 
 */
int load_cluster_nodes(char *addr, int port);

 #endif /* _SERVER_H_ */