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
    
    for (i = 0; i < TLBsize; ++i)
    {
        TLB[i].address=0;
    }
    for (i = 0; i < FRAMEsize; ++i)
    {
        Frame[i].status=0;
    }

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
        i = 0;

        int costoPagina = GetInt(fp);
        int costoTLB = GetInt(fp);
        int costoDisco = GetInt(fp);

#ifdef DEBUG
        printf("Pagina: %d \nTLB: %d \nDisco: %d\n", costoPagina,costoTLB, costoDisco);
#endif  
        while (!feof(fp))
        {
            events++;
            
            reference temp = GetAddress(fp);
            int encontrado = 0;
            for(j=0; j<= i; j++)
            {
                //Comprobar que este en el TLB 
                if(TLB[j].address == temp.address)
                {
                    //Encontrado en el TLB
                    encontrado = 1;
                    hit++;
#ifdef DEBUG
                    printf("La direccion %x ya esta en TLB, su uso pasado fue en %d y su ultimo uso ahora es %d\n",TLB[j].address,TLB[j].lastUse,events);
#endif                    
                    TLB[j].lastUse = events;
                    
                }
            }
            if (encontrado == 0)
            {
                //No encontrado en el TLB
                if ((TLB[i].address == 0) && (i<8))
                {
#ifdef DEBUG
                    printf("La direccion %x no encontrado en el TLB, se trata de asignar a un vacio, su primer uso es %d\n",temp.address, events);
#endif  
                    TLB[i] = temp;
                    TLB[i].lastUse = events;
                    i=(i+1);
                }
                else
                {
           
                    //Reemplazar()
                    int llru = TLB[0].lastUse,posicion=0; 
                    for (j = 1; j < TLBsize; ++j)
                    {
                        if (llru>TLB[j].lastUse)
                        {
                            llru = TLB[j].lastUse;
                            posicion = j;
                        }
                        
                    }
#ifdef DEBUG
                    printf("No encontrado en el TLB, se trata de reemplazar a\n");
#endif          
                    temp.lastUse = events;
                    TLB[posicion] = temp;


                }
                pageIns++;
            }
        }
#ifdef DEBUG
        printf("\n\n\n\n\n");
#endif
        printf("My results for this example are:\n");
        printf("Total number of events: %d\nAverage access time %f\n", events, averageAccessTime);
        printf("Number of page-ins %d\nNumber of page-outs %d\n",pageIns, pageOuts);
        
        printf("TLB hit ratio %f\n\n", hit/events);


#ifdef DEBUG
        printf("The final state of the TLB and frame tables are:\n\nTLB contents\n");
        for (i = 0; i < TLBsize; ++i)
        {
            if(TLB[i].address > 100)
                printf("Page#: %d frame#: 0 lru = %d",TLB[i].address, TLB[i].lastUse);
            else if(TLB[i].address > 10)
                printf("Page#: %d  frame#: 0 lru = %d",TLB[i].address, TLB[i].lastUse);
            else
                printf("Page#: %d   frame#: 0 lru = %d",TLB[i].address, TLB[i].lastUse);

            if(TLB[i].lastUse > 100)
                printf(" A:  2 S: %d\n",TLB[i].access);
            else if(TLB[i].lastUse > 10)
                printf("  A:  2 S: %d\n",TLB[i].access);
            else
                printf("   A:  2 S: %d\n",TLB[i].access);
        }
        printf("\nFrame table contents\n");
        for (i = 0; i < FRAMEsize; ++i)
        {
            if(Frame[i].address > 100)
                printf("Frame#: %d page#: 0 lru = %d",Frame[i].address, Frame[i].lastUse);
            else if(Frame[i].address > 10)
                printf("Frame#: %d  page#: 0 lru = %d",Frame[i].address, Frame[i].lastUse);
            else
                printf("Frame#: %d   page#: 0 lru = %d",Frame[i].address, Frame[i].lastUse);

            if(Frame[i].lastUse > 100)
                printf(" A:  2 S: %d\n",Frame[i].access);
            else if(Frame[i].lastUse > 10)
                printf("  A:  2 S: %d\n",Frame[i].access);
            else
                printf("   A:  2 S: %d\n",Frame[i].access);
        }
#endif
    }
}