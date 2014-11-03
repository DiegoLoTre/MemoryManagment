/*
 * Copyright (c) 2014 Edgar Jacob Rivera Rios
 * 
 * Program: simulation.c
 *
 * Author:  Edgar Jacob Rivera Rios
 *			Oscar Sanchez Jimenez
 *			Diego Lopez Trevilla 
 *
 * Purpose: This program implements a Virtual Memory Simulator. 
 *
 * References:
 *          Based on our own code
 *
 *
 * Revision history:
 *
 *          27 october - File created
 *
 * Error handling:
 *          None
 *
 * Notes:
 *          Many methods may require refactoring, specially Preemptive
 *			and Round Robin. Also it does not handle voids in the arrival
 * 			time 
 *			
 * Repository;
 *			https://github.com/DiegoLoTre/Scheduler2
 *
 */

 #include <glib.h>
#include <stdlib.h>                     /* Used for malloc definition */
#include <stdio.h>                                /* Used for fprintf */
#include <string.h>                                /* Used for strcmp */
#include <assert.h>                      /* Used for the assert macro */
#include "FileIO.h"    /* Definition of file access support functions */
#include "Structure.h" /* Define the change of hexadecimal to decimal*/

/***********************************************************************
 *                       Global constant values                        *
 **********************************************************************/
#define TLBsize 8
#define FRAMEsize 15
/***********************************************************************
 *                          Main entry point                           *
 **********************************************************************/
int main (int argc, const char * argv[]) {
    
    FILE   *fp;                                /* Pointer to the file */
    int TLB[TLBsize];
    int i = 0;
    
    /* Open the file and check that it exists */
    fp = fopen (argv[1],"r");	  /* Open file for read operation */
    /* There is an error */
    if (!fp)
    {                               
        ErrorMsg("main","filename does not exist or is corrupted");
    }
    else
    {
        /* Variable initialization */
		int events = 0;
		int averageAccessTime = 0;
		int pageIns = 0;
		int pageOuts = 0;
		int hit = 0;

        /*
        * Read the process information until the end of file
        * is reached.
        */
        while (!feof(fp))
        {
            
            TLB[i] = Translate(fp);
            events++;


            printf("TLB[%d]\n%d",TLB[i],i);
            i++;
        }
    }
}