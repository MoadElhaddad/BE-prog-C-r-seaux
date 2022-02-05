/* librairie standard ... */
#include <stdlib.h>
/* pour getopt */
#include <unistd.h>
/* déclaration des types de base */
#include <sys/types.h>
/* constantes relatives aux domaines, types et protocoles */
#include <sys/socket.h>
/* constantes et structures propres au domaine UNIX */
#include <sys/un.h>
/* constantes et structures propres au domaine INTERNET */
#include <netinet/in.h>
/* structures retournées par les fonctions de gestion de la base de
données du réseau */
#include <netdb.h>
/* pour les entrées/sorties */
#include <stdio.h>
/* pour la gestion des erreurs */
#include <errno.h>
void construire_message (char *message, char motif, int lg)
{
int i;
for (i=0 ; i<lg; i++) message[i] = motif;
}


void afficher_message (char *message, int lg)
{
int i;
printf("message construit : ");
for (i=0 ; i<lg ; i++) printf("%c", message[i]) ; printf("\n");
}


void udp_puit(int num_port, int taille,int nb_message)
{
//Creation du socket local
int option=0;
int sock1 = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
 struct sockaddr_in adr_local,adr_distant;
int lg_adr_local=sizeof(adr_local);

if (sock1==-1)
  {printf("échec de création du socket \n");
    exit(1);}

 memset((char*) &adr_local,option,sizeof(adr_local));

  
//Adressage  du socket
adr_local.sin_family=AF_INET;
adr_local.sin_port=num_port;
adr_local.sin_addr.s_addr=INADDR_ANY;

//Association de l'adresse au socket

if (bind(sock1,(struct sockaddr *)&adr_local,lg_adr_local)==-1)
  { printf("échec du bind \n");
    exit(1) ;}


//Recuperation  des messages
 char * pmesg = malloc(taille*sizeof(char));
 int lg_dist;
 int recv;
 lg_dist=sizeof(adr_distant);
 int i=1;
 while (recv !=0)
   {
     recv= recvfrom(sock1,pmesg,taille,option,(struct sockaddr *) &(adr_distant),&lg_dist);

     if (recv ==-1)
       {printf("erreur recivfrom\n");
	 exit(1);}
     if (recv!=0)
       {
	 printf("PUITS : Réception n°%d (%d): [",i,taille);
	 afficher_message(pmesg,recv);
       }
      
     if (i==nb_message)
       {
	 recv=0;
	 printf(" on a atteint le nombre de message à recevoir \n");
	 i++;
       }

}
}
void udp_source(int num_port,int taille,int nb_mess,char*dest)
{
//Creation du socket local
int sock2 = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
struct sockaddr_in adr_distant;
if (sock2==-1)
  {printf("échec de création du socket \n");
    exit(1);
  }
//identification de l'adresse du socket destinataire
 struct hostent *hp;
 char * pmesg = malloc(taille*sizeof(char));
 int option=0;
 memset((char *)& adr_distant,0,sizeof(adr_distant));
 adr_distant.sin_family=AF_INET;
 adr_distant.sin_port=num_port;
 if ((hp=gethostbyname(dest))==NULL)
   {
     printf("erreur gethostbyname \n");
     exit(1);
   }
 memcpy((char*)&(adr_distant.sin_addr.s_addr),hp->h_addr,hp->h_length);
//envoi du message + affichage
 int sent;
 for (int i=1; i<=nb_mess;i++)
   {
     construire_message(pmesg,'a',taille);
     printf("SOURCE:envoi n°%d (%d) : [",i,taille);

     if ((sent= sendto(sock2,pmesg,taille,option,(struct sockaddr*)&(adr_distant),sizeof(adr_distant)))==-1)
       { printf("Erreur sendto\n");
	 exit(1);}
     afficher_message(pmesg,sent);
   }
}
void main (int argc, char **argv)
{
	int c;
	extern char *optarg;
	extern int optind;
	int nb_message = -1; /* Nb de messages à envoyer ou à recevoir, par défaut : 10 en émission, infini en réception */
	int source = -1 ; /* 0=puits, 1=source */
	int udp= 0;          /*  indique le mode utilisé */
	int num_port = argv[argc-1];
	int lg_message = 30; /*taille par défut d'un message*/
	char * dest=argv[argc-2];
	while ((c = getopt(argc, argv, "pn:su")) != -1) {
		switch (c) {
		case 'p':
			if (source == 1) {
				printf("usage: cmd [-p|-s][-n ##]\n");
				exit(1);
			}
			source = 0;
			break;

		case 's':
			if (source == 0) {
				printf("usage: cmd [-p|-s][-n ##]\n");
				exit(1) ;
			}
			source = 1;
			break;

		case 'n':
			nb_message = atoi(optarg);
			break;


			
		case 'u':
		       udp=1;
		       break;
		  

		default:
			printf("usage: cmd [-p|-s][-n ##]\n");
			break;
		}
	}

	if (source == -1) {
		printf("usage: cmd [-p|-s][-n ##]\n");
		exit(1) ;
	}

	if (source == 1)
		printf("on est dans le source\n");
	else
		printf("on est dans le puits\n");

	if (nb_message != -1) {
		if (source == 1)
			printf("nb de tampons à envoyer : %d\n", nb_message);
		else
			printf("nb de tampons à recevoir : %d\n", nb_message);
	} else {
		if (source == 1) {
			nb_message = 10 ;
			printf("nb de tampons à envoyer = 10 par défaut\n");
		} else
		printf("nb de tampons à envoyer = infini\n");

	}
	//Appel de la fonction à executer
        nb_message = 10;
	if (source == 1 & udp == 1)
	  {
	  udp_source(num_port,lg_message,nb_message,dest);
	  exit(1);
	  }
	else if(source == 1 & udp == 0)
	  {
	  // tcp_source();
	  exit(1);
	  }

	 else if(source == 0 & udp == 1)
	   { udp_puit(num_port,lg_message,nb_message);}
	 else {
	  // tcp_puit();
	  exit(1);}
	     
	   }


