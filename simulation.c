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
 *          13 November - Mehtods refactored
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
 *                              Functions                              *
 **********************************************************************/

void printTables(reference TLB[TLBsize], reference Frame[FRAMEsize])
{
    int i;
    printf("\nThe final state of the TLB and frame tables are:\n\nTLB contents\n");
    for (i = 0; i < TLBsize; ++i)
    {
        if(TLB[i].address > 100)
            printf("Page#: %d frame#: %lu lru : %d",TLB[i].address, TLB[i].frame, TLB[i].lastUse);
        else if(TLB[i].address > 10)
            printf("Page#: %d  frame#: %lu lru : %d",TLB[i].address, TLB[i].frame, TLB[i].lastUse);
        else
            printf("Page#: %d   frame#: %lu lru : %d",TLB[i].address, TLB[i].frame, TLB[i].lastUse);
        
        if(TLB[i].lastUse > 100)
            printf(" A:  ");
        else if(TLB[i].lastUse > 10)
            printf("  A:  ");
        else
            printf("   A:  ");

        if(TLB[i].access == 0)
            printf("R");
        else
            printf("W");

        if(TLB[i].status == INVALID)
            printf(" S: I\n");
        else if(TLB[i].status == VALID)
            printf(" S: V\n");
    }
    printf("\nFrame table contents\n");
    for (i = 0; i < FRAMEsize; ++i)
    {
        if(Frame[i].status == INVALID)
            printf("Frame#: I page#: I   lru : I   A:  I S: I\n");
        else
        {
            if(Frame[i].address > 100)
                printf("Frame#: %lu page#: %d lru : %d",Frame[i].frame, Frame[i].address, Frame[i].lastUse);
            else if(Frame[i].address > 10)
                printf("Frame#: %lu page#: %d  lru : %d",Frame[i].frame, Frame[i].address, Frame[i].lastUse);
            else
                printf("Frame#: %lu page#: %d   lru : %d",Frame[i].frame, Frame[i].address, Frame[i].lastUse);

            if(Frame[i].lastUse > 100)
                printf(" A:  2 S: V\n");
            else if(Frame[i].lastUse > 10)
                printf("  A:  2 S: V\n");
            else
                printf("   A:  2 S: V\n");
        }
    }
}

void Initialize(reference TLB[TLBsize], reference Frame[FRAMEsize], reference Page[PAGEsize])
{
    int i = 0;
    for (i = 0; i < TLBsize; ++i)
    {
        TLB[i].status = INVALID;
        //TLB[i].address=0;
        TLB[i].lastUse=0;
    }

    for (i = 0; i < FRAMEsize; ++i)
    {
        //Frame[i].address=0;
        Frame[i].status = INVALID;
    }

    for (i = 0; i < PAGEsize; ++i)
    {
        //Page[i].address=0;
        Page[i].status = INVALID;
    }
}

/***********************************************************************
 *                          Main entry point                           *
 **********************************************************************/
int main (int argc, const char * argv[]) {
    
     /* Variable initialization */

    FILE   *fp;                                /* Pointer to the file */
    reference TLB [TLBsize];
    reference Frame [FRAMEsize];
    reference Page [PAGEsize];
    int events = 0;
    float averageAccessTime = 0;
    int pageIns = 0;
    int pageOuts = 0;
    float hit = 0;
    int fullFrame  = 0;
    int j = 0, i = 0;

    Initialize(TLB, Frame, Page);

    /* Open the file and check that it exists */
    fp = fopen (argv[1],"r");	  /* Open file for read operation */
    
    /* There is an error */
    if (!fp)
    {                               
        ErrorMsg("main","filename does not exist or is corrupted");
    }
    /* File works */
    else
    {
        /* Gets the values of each access time*/
        int tMem = GetInt(fp);
        int tTLB = GetInt(fp);
        int tFault = GetInt(fp);

        #ifdef DEBUG DEBUGHARD
            printf("Mem: %d \nTLB: %d \nFault: %d\n", tMem, tTLB, tFault);
        #endif

        /* Iterates over the elements in th traze */
        while (!feof(fp))
        {
            events++;
            
            /* Get the address */
            reference temp = GetAddress(fp);            

            int foundInTLB = 0;

            /* Check if the address is on TLB */ 
            for(j=0; j<= i; j++)
            {
                if((TLB[j].status != INVALID ) && (TLB[j].address == temp.address))
                {
                    /* Address found in TLB */
                    foundInTLB = 1;
                    hit++;
                    averageAccessTime = averageAccessTime + tTLB;

                    #ifdef DEBUGHARD
                        printf("Address %x is already in TLB, its last use was in %d and now its %d\n",TLB[j].address,TLB[j].lastUse,events);
                    #endif

                    TLB[j].lastUse = events;
                    Frame[TLB[j].frame].lastUse = events;
                }
            }

            /* Address not found in TLB */
            if (foundInTLB == 0) 
            {
                temp.lastUse = events;
                temp.status = VALID;

                int foundInMem = 0;
                
                /* Check if is in Frame Page */
                for(j=0; j< FRAMEsize; j++)
                {
                    /* Found in Frame,  */
                    if((Frame[j].status != INVALID ) && (Frame[j].address== temp.address))
                    {   
                        #ifdef DEBUGHARD
                            printf("Address %d is in Frame Table\n",temp.address);
                        #endif
                        averageAccessTime = averageAccessTime + tMem;
                        foundInMem = 1;
                        temp.frame = j;
                    }             
                }

                /* Address not in Frame Table */
                if (foundInMem==0)
                {
                    #ifdef DEBUGHARD
                        printf("Address %d have to be pulled from HD\n",temp.address);
                    #endif

                    /* Look for the last used Frame to replace */
                    int lastUsed = Frame[0].lastUse, lastUsedPosition=0; 
                    for (j = 1; j < FRAMEsize; ++j)
                    {
                        if (lastUsed>Frame[j].lastUse)
                        {
                            lastUsed = Frame[j].lastUse;
                            lastUsedPosition = j;
                        }
                    }
                    temp.frame = lastUsedPosition;

                    if (Frame[lastUsedPosition].status!=INVALID)
                        pageOuts++;

                    /* Replace the oldest Frame*/
                    Frame[lastUsedPosition] = temp;

                    averageAccessTime = averageAccessTime + tFault + tMem;
                    pageIns++;
                }

                /* Look for the last used TLB to replace */
                int lastUsed = TLB[0].lastUse, lastUsedPosition=0; 
                for (j = 1; j < TLBsize; ++j)
                {
                    if (lastUsed>TLB[j].lastUse)
                    {
                        lastUsed = TLB[j].lastUse;
                        lastUsedPosition = j;
                    }
                } 

                /*Replace the oldest TLB*/
                TLB[lastUsedPosition] = temp;
                
                if (i<TLBsize)
                    i++;

                averageAccessTime = averageAccessTime + tTLB;

            }
        }

        /* Calculates the results and the prints them */
        averageAccessTime = averageAccessTime/events;
        printf("\nTotal number of events: %d\nAverage access time %f\n", events, averageAccessTime);
        printf("Number of page-ins %d\nNumber of page-outs %d\n",pageIns, pageOuts);
        printf("TLB hit ratio %f\n\n", hit/events);

        #ifdef DEBUG DEBUGHARD
                printTables(TLB,Frame);
        #endif
    }
}