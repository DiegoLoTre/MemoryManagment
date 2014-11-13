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

        /*if(TLB[i].status == 0)
            printf(" S: I\n");
        else if(TLB[i].status == 1)*/
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
    
    FILE   *fp;                                /* Pointer to the file */
    reference TLB [TLBsize];
    reference Frame [FRAMEsize];
    reference Page [PAGEsize];
    
    Initialize(TLB, Frame, Page);

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
        int fullFrame  = 0;
        int j = 0;
        i = 0;

        int tMem = GetInt(fp);
        int tTLB = GetInt(fp);
        int tFault = GetInt(fp);


        #ifdef DEBUG
            printf("Mem: %d \nTLB: %d \nFault: %d\n", tMem, tTLB, tFault);
        #endif


        while (!feof(fp))
        {
            events++;
            
            //Get the address
            reference temp = GetAddress(fp);

            int found = 0;
            printf("%d\n",i );
            for(j=0; j<= i; j++)
            {
                //Check if the address is on TLB 
                if((TLB[j].status != INVALID ) && (TLB[j].address == temp.address))
                {
                    //found en el TLB
                    found = 1;
                    hit++;
                    averageAccessTime = averageAccessTime + tTLB;

                    #ifdef DEBUG
                        printf("Address %x is already in TLB, its last use was in %d and now its %d\n",TLB[j].address,TLB[j].lastUse,events);
                    #endif

                    TLB[j].lastUse = events;
                }
            }
            if (found == 0) /*Not found in TLB*/
            {
                //Get last used resource
                int lastUsed = TLB[0].lastUse, lastUsedPosition=0; 
                for (j = 1; j < TLBsize; ++j)
                {
                    if (lastUsed>TLB[j].lastUse)
                    {
                        lastUsed = TLB[j].lastUse;
                        lastUsedPosition = j;
                    }
                } 


                temp.lastUse = events;
                temp.status = VALID;

                /*Variable para saber si hay que llegar a tabla de Paginas*/
                int mandarDisco = 1; /*1 = Si    2= No*/
                
                for(j=0; j< FRAMEsize; j++)
                {
                    if(Frame[j].address== temp.address)
                    {
                        averageAccessTime = averageAccessTime + tMem;
                        temp.frame = j;
                        Frame[j] = temp;
                        mandarDisco = 0; /*Quitar bandera para mandar al disco*/
                        fullFrame = 1;
                        break;
                    }
                    else
                    {
                        if(Frame[j].address== 0)
                        {
                            averageAccessTime = averageAccessTime + tMem;
                            temp.frame = j;
                            Frame[j] = temp;
                            fullFrame=1;
                            break;
                        }
                    }                
                }

                if(fullFrame == 0)
                {
                    pageOuts++;
                    fullFrame = 0;
                }    

                if(mandarDisco == 1)
                {
                    
                    averageAccessTime = averageAccessTime + tFault;
                }

                 /*Replace the value on TLB*/
                TLB[lastUsedPosition] = temp;
                
                i++;
                averageAccessTime = averageAccessTime + tTLB;

                pageIns++;


            } /* Termina if no encontrado en TLB*/
        }
#ifdef DEBUG
        printf("\n\n\n\n\n");
#endif
        averageAccessTime = averageAccessTime/events;
        //printf("My results for this example are:\n");
        printf("Total number of events: %d\nAverage access time %f\n", events, averageAccessTime);
        printf("Number of page-ins %d\nNumber of page-outs %d\n",pageIns, pageOuts);
        
        printf("TLB hit ratio %f\n\n", hit/events);


#ifdef DEBUG
        printTables(TLB,Frame);
#endif
    }
}