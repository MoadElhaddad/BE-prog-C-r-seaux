#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
#include "affichage2.h"

void construire_message2 (char *message, char motif, int lg)
{
int i;
for (i=0 ; i<lg; i++) message[i] = motif;
}

void formater_message (char *message, int n)
{
if (n>=100000)
    {
        printf("Trop de messages à envoyer (n>1000000 \n");
        exit(1);
    }

    if (n<10)
    {
        for (int i=0 ; i<4 ; i++)
        {
            *(message+i)='-';
        }
        //char nb[1];      
        *(message+4)= n + '0';
    }
    if (n>=10 & n<100)
    {
    	char nb[2];

        for (int i=0 ; i<3 ; i++)
        {
            *(message+i)='-';
        }
        sprintf(nb,"%d",n);
        *(message+3)=*nb;
        n=n%10;
        sprintf(nb,"%d", n);
        *(message+4)=*nb;
    }
    if (n>=100 & n<1000)
    {
        char nb[3];
        for (int i=0 ; i<2 ; i++)
        {
            *(message+i)='-';
        }
        sprintf(nb,"%d",n);
        *(message+2)=*nb;
        n=n%100;
        sprintf(nb,"%d", n);
        *(message+3)=*nb;
        n=n%10;
        sprintf(nb,"%d", n);
        *(message+4)=*nb;
    }
    if (n>=1000 & n<10000)
    {
    	int j=1000;

        char nb[4];
        for (int i=0 ; i<2 ; i++)
        {
            *(message+i)='-';
        }

        sprintf(nb,"%d",n);
        *(message+1)=*nb;
        n=n%1000;
        sprintf(nb,"%d", n);
        *(message+2)=*nb;
        n=n%100;
        sprintf(nb,"%d", n);
        *(message+3)=*nb;
        n=n%10;
        sprintf(nb,"%d", n);
        *(message+4)=*nb;
    }
    if (n>=10000 & n<100000)
    {
        char nb[4];
        for (int i=0 ; i<2 ; i++)
        {
            *(message+i)='-';
        }
        sprintf(nb,"%d",n);
        *(message)=*nb;
        n=n%10000;
        sprintf(nb,"%d", n);
        *(message+1)=*nb;
        n=n%1000;
        sprintf(nb,"%d", n);
        *(message+2)=*nb;
        n=n%100;
        sprintf(nb,"%d", n);
        *(message+3)=*nb;
        n=n%10;
        sprintf(nb,"%d", n);
        *(message+4)=*nb;
    }
}

