/**************************************************************** */
/* Copyright (C) SSE-USTC, 2012                                     */
/*                                                                  */
/*  FILE NAME             :  protocol.h                             */
/*  PRINCIPAL AUTHOR      :  MingLee                                */
/*  SUBSYSTEM NAME        :  network                                */
/*  MODULE NAME           :  protocol                               */
/*  LANGUAGE              :  C                                      */
/*  TARGET ENVIRONMENT    :  Linux                                  */
/*  DATE OF FIRST RELEASE :  2012/12/14                             */
/*  DESCRIPTION           :  the interface to Linux system(socket)  */
/********************************************************************/

/*
 * Revision log:
 *
 * Created by MingLee,2012/12/14
 *
 */
#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

#define OPEN_CMD  	1
#define CLOSE_CMD 	2
#define SET_CMD		3
#define GET_CMD		4
#define DEL_CMD		5
// #define ERROR		0
typedef struct DataFormat
{
	int cmd;
	int value_num;
	int len_value1;
	char *value1;
	int len_value2;
	char *value2;
}*mDataFormat;
// typedef struct DataFormat *mDataFormat;

int format_data(char *buf, mDataFormat data);
int parse_data(char *buf, mDataFormat data);

#endif /* _PROTOCOL_H_*/ 