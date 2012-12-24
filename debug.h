/****************************************************************** */
/* Copyright (C) SSE-USTC, 2012                                     */
/*                                                                  */
/*  FILE NAME             :  debug.h                                */
/*  PRINCIPAL AUTHOR      :  MingLee                                */
/*  SUBSYSTEM NAME        :  network                                */
/*  MODULE NAME           :  tool                                   */
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

#define debug  printf("")
#define debug_argv(fmt, args...)  printf("")
//#define debug_argv(fmt, args...)  printf("%s : %d : %s "fmt,__FILE__,__LINE__,__FUNCTION__, ##args)

// #define debug  printf("%s : %d : %s\n",__FILE__,__LINE__, __FUNCTION__)
