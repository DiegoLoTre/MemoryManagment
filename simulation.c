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
//#include "Structure.h" /* Define the change of hexadecimal to decimal*/

/***********************************************************************
 *                       Global constant values                        *
 **********************************************************************/
#define TLBsize 8
#define FRAMEsize 16
#define PAGEsize 128

/***********************************************************************
 *                          Main entry point                           *
 **********************************************************************/
int main (int argc, const char * argv[]) {
    
    FILE   *fp;                                /* Pointer to the file */
    reference TLB [TLBsize];
    reference Frame [FRAMEsize];
    reference Page [PAGEsize];
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
		float averageAccessTime = 0;
		int pageIns = 0;
		int pageOuts = 0;
		float hit = 0;
        int j = 0;

        int costoPagina = GetInt(fp);
        int costoTLB = GetInt(fp);
        int costoDisco = GetInt(fp);

        printf("Pagina: %d \nTLB: %d \nDisco: %d\n", costoPagina,costoTLB, costoDisco);

        while (!feof(fp))
        {
            
            Page[i] = GetAddress(fp);
            events++;

            /*printf("HEAXADECIMAL = Page[%d]=%x  %d\n",i,Page[i].address/512, Page[i].access);
            printf("DECIMAL = Page[%d]=%d  %d\n",i,Page[i].address/512, Page[i].access);*/

            printf("HEAXADECIMAL = Page[%d]=%x  %d\n",i,Page[i].address, Page[i].access);
            printf("DECIMAL = Page[%d]=%d  %d\n",i,Page[i].address, Page[i].access);

            i++;
        }

        printf("Total number of events: %d\n Average access time %f\n", events, averageAccessTime);
        printf("Number of page-ins %d\n Number of page-outs %d\n",pageIns, pageOuts);
        
        printf("TLB hit ratio %f\n\n", hit);
        printf("The final state of the TLB and frame tables are:\nTLB contents\n");

        /*for (j = 0; j < TLBsize; ++j)
        {
            printf("Page#: %d frame#:  %d ",TLB[i][0],TLB[i][1]);
            printf("lru:      %d A:   ",TLB[i][2],TLB[i][4]);
        }*/

        printf("Frame table contents");
        printf("Frame#:   0 page#:  33 lru:      4 A:   W S:   D\n");
    }
}