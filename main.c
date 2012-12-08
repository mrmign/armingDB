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
#include <stdlib.h>
#include <regex.h>
#include "dbApi.h"

#define MAX 1024
#define SUCCESS 0
#define FAIL -1

char cmd[6];
char key[5];
char value[1024]; 
char dbname[100];
Database db = NULL;
const char *name = "armingDB::";

regmatch_t pm[0];

void substr(const char*str, char* buf,unsigned start, unsigned end);
int check(char *pat, char *str);
int getLine(char *buf, int size);
int exeCmd(char *str);

int main(int argc, char **argv)
{
	
	

	char buf[MAX];
	int i =3;
	
	while(1)
	{
		if(getLine(buf,MAX)==0)
			continue;
		else
		{
			exeCmd(buf);
		}

	}
}

int getLine(char *buf, int size)
{
	unsigned int length = 1; // record the input size
	printf("%s%s>>",name,dbname);
	// length = fgets(buf,size,stdin);
	fgets(buf,size,stdin);
	// buf[length] = '\0';

	if(length==0)
		return 0;
	else
		return length;
	
}


void substr(const char*str,char* buf, unsigned start, unsigned end)
{
    unsigned n = end - start;
    // static char stbuf[256];
    strncpy(buf, str + start, n);
    buf[n] = 0;
    // return stbuf;
}
int check(char *pat, char *str)
{
    int res;
    regex_t reg;
    //    regmatch_t pm[10];
    const size_t nmatch = 1;
    res = regcomp(&reg, pat, REG_EXTENDED | REG_ICASE);
    if (res != 0)
    {
        printf("Invalid pattern.\n");

        regfree(&reg);
        return -1;
    }
    res = regexec(&reg, str, nmatch, pm, 0);
    // printf("--%u %u\n",pm[0].rm_so,pm[0].rm_eo );
    regfree(&reg);

    if (res == REG_NOMATCH)
        return -1;
    else if (res != 0)
    {
        printf("Execution error.\n");
        return -1;
    }
    else
    {
       // printf("%s %u %u\n",substr(str,pm[0].rm_so,pm[0].rm_eo),pm[0].rm_so,pm[0].rm_eo);

        
        return 0;
    }
}
int exeCmd(char *str)
{

    if (check("[a-zA-Z]+",str) != 0)
        return;
    
    substr(str,cmd,pm[0].rm_so,pm[0].rm_eo);
    // printf("%s %u %u\n",cmd,pm[0].rm_so,pm[0].rm_eo);

    if(strcmp(cmd,"open") == 0)
    {
        if(db)
        {
        	printf("please close %s first!\n", dbname);
        }
        else
        {
        	unsigned pre = pm[0].rm_eo;
        	check("\\w+\\.\\w+",str+pm[0].rm_eo);
	        substr(str,dbname,pre+pm[0].rm_so,pre+pm[0].rm_eo);
	        db = createNewDB(dbname);
	        // printf("%s%s", name,dbname);
        }
        
    }
    else if(strcmp(cmd,"close") == 0)
    {
    	if(db)
    	{
    		closeDB(db);
    		db = NULL;
    	}
    	dbname[0] = '\0';
    }
    else if(strcmp(cmd,"set") == 0)
    {
    	if(db == NULL)
    	{
    		printf("Please open db first!\n");
    		return;
    	}	
    	unsigned pre = pm[0].rm_eo;// record the previous end
    	check("[0-9]+",str+pm[0].rm_eo);
    	substr(str,key,pre + pm[0].rm_so,pre + pm[0].rm_eo);
    	
    	int key_v = atoi(key);

    	pre += pm[0].rm_eo;
    	check(".*",str+pm[0].rm_eo);
    	substr(str,value,pre + pm[0].rm_so,pre + pm[0].rm_eo);

    	Data tdata;
    	tdata.length = MAX;
    	tdata.value = value;
    	// printf("%d-%s\n",key_v,value );
    	if(putKeyValue(db,key_v,&tdata) != 0)
    	{
    		printf("ERROR:set %d %s\n",key_v,value);
    	}	

    }
    else if(strcmp(cmd,"get") == 0)
    {
    	if(db == NULL)
    	{
    		printf("Please open db first!\n" );
    		return;
    	}	

    	unsigned pre = pm[0].rm_eo;
    	check("[0-9]+",str+pm[0].rm_eo);
    	substr(str,key,pre + pm[0].rm_so,pre + pm[0].rm_eo);
    	int key_v = atoi(key);
    	char get[MAX]="\0";
    	Data tdata;
    	tdata.value = get;
    	if(getValue(db,key_v,&tdata)==0)
    	{
    		printf("%d => %s", key_v,get);
    	}
    	else
    	{
    		printf("ERROR:get %d Not found!\n",key_v);
    	}
    }
    else if(strcmp(cmd,"delete") == 0)
    {
    	if(db == NULL)
    	{
    		printf("Please open db first!\n" );
    		return;
    	}	
    	unsigned pre = pm[0].rm_eo;
    	check("[0-9]+",str+pm[0].rm_eo);
    	substr(str,key,pre + pm[0].rm_so,pre + pm[0].rm_eo);
    	int key_v = atoi(key);
    	if(deleteValueByKey(db,key_v) != 0)
    		printf("ERROR:delete %d\n",key_v);
    }
    else if(strcmp(cmd,"help") == 0)
    {
    	printf("open filename - EX:open db.hdb\n"); 
        printf("set key value - EX:set 100 helloworld\n");
        printf("get key       - EX:get 100\n");
        printf("delete key    - EX:delete 100\n");
        printf("close         - leave nezha.hdb\n"); 
        printf("help          - list cmds info\n"); 
    }
    else if(strcmp(cmd,"exit") == 0)
    {
    	if(db)
    	{
    		closeDB(db);
    	}
    	exit(0);
    }
    else
    {
    	printf("Unknow Command!\n"); 
    }
    return 0;

}