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
#define ERROR_RESP  0

#define CTL_CMD 	10
#define CTL_REG_CMD 11

#define MAX_DATA_NUM    128
#define MAX_DATA_LEN    128
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

/*
 * Format the data to be sent
 * input	: buf, data
 * output	: None
 * in/out	: buf, the formated data to be sent
 * return	: if SUCCESS return the size of the data to be sent
 			: if FAIL return  -1 
 */
int format_data(char *buf, mDataFormat data);

/*
 * Format the  control data to be sent
 * input	: buf, bufsize,cmd, data, dataNum
 * output	: None
 * in/out	: buf, the formated data to be sent
 * return	: if SUCCESS return the size of the data to be sent
 			: if FAIL return  -1 
 */
int format_ctl_data(char *pBuf,int *pBufSize,int cmd,char ppData[MAX_DATA_NUM][MAX_DATA_LEN],int DataNum);
/*
 * parse the data received from net
 * input	: buf, data
 * output	: None
 * in/out	: data, parse the data from net format to local formt
 * return	: if SUCCESS return 0
 			: if FAIL    return -1
 */
int parse_data(char *buf, mDataFormat data);

/*
 * parse the control data received from server
 * input	: buf, bufsize, data, dataNum
 * output	: None
 * in/out	: data, parse the data from net format to local formt
 * return	: if SUCCESS return 0
 			: if FAIL    return -1
 */
int parse_ctl_data(char *pBuf,int BufSize,int *pcmd,int *pDataNum,char ppData[MAX_DATA_NUM][MAX_DATA_LEN]);

/*
 * parse the cmd in data header
 * input	: buf, data
 * output	: None
 * in/out	: data, parse the data from net format to local formt
 * return	: if SUCCESS return 0
 			: if FAIL    return -1
 */
int parse_cmd(char * pBuf,int BufSize,int *pcmd,int *pDataNum);

#endif /* _PROTOCOL_H_*/ 