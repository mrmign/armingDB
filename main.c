/********************************************************************/
/* Copyright (C) SSE-USTC, 2012                                     */
/*                                                                  */
/*  FILE NAME             :  main.c                                 */
/*  PRINCIPAL AUTHOR      :  LeeMing                                */
/*  SUBSYSTEM NAME        :  db                                     */
/*  MODULE NAME           :  db                                     */
/*  LANGUAGE              :  C                                      */
/*  TARGET ENVIRONMENT    :  Linux                                  */
/*  DATE OF FIRST RELEASE :  2012/11/29                             */
/*  DESCRIPTION           :  the main application                   */
/********************************************************************/

/*
 * Revision log:
 *
 * Created by LeeMing,2012/11/29
 *
 */


#include <stdio.h>
#include <string.h>
#include "dbApi.h"
#define MAX 1024
#define SUCCESS 0
#define FAIL -1

char cmd[6];
int key_in=0;
char value[200]; 
char dbname[10];
int main(int argc, char **argv)
{
	
	

	char buf[MAX];
	int i =3;
	
	while(1)
	{
		if(!getLine(buf,MAX))
			continue;
		else
		{
			checkCmd(buf);
		}

	}
}

int getLine(char *buf, int size)
{
	int length = 0; // record the input size
	printf("armingDB::");
	length = fgets(buf,size,stdin);

	// buf[length] = '\0';

	if(length==0)
		return 0;
	else
		return length;
	
}

int checkCmd(char *buf)
{
	sscanf(buf,"%s%d%s",cmd,&key_in,value);
	printf("%s%d%s\n",cmd,key_in,value );
	// if(strcmp(cmd,"open"))
	key_in = 0;
	value[0] = '\0';
}


	/*
	Database db;
	db = createNewDB();

	Data putData, getData;
	putData.value = "hello my db!";
	putData.length = strlen(putData.value);
	printf("val: %s", putData.value);
	int key = 0;

	if (SUCCESS == putKeyValue(db, key, &putData))
	{
		printf("put success!\n");
	}
	else
	{
		printf("put failed!\n");
	}
	
	char buf[MAX] = "\0";
	getData.value = buf;
	getData.length = MAX;

	int ret;
	ret = getValue(db, key, &getData);
	if (SUCCESS == ret)
	{
		printf("the get data, value: %s\n", getData.value);
	}
	else
	{
		printf("get value failed!\n");
	}

	if(closeDB(db))
	{
		printf("closeDB successfully!\n");
	}
	*/

