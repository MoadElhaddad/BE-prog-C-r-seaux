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

void afficher_message (char *message, int lg)
{
int i;
printf("message construit : ");
for (i=0 ; i<lg ; i++) printf("%c", message[i]) ; printf("\n");
}


void udp_puit(int num_port, int taille) {
//Creation du socket local
int sock1 = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
struct sockaddr_in adr_local;
int lg_adr_local=sizeof(adr_local);

if (sock1==-1)
  {printf("échec de création du socket \n");
    exit(1);}

  
//Adressage  du socket
adr_local.sin_family=AF_INET;
adr_local.sin_port=num_port;
adr_local.sin_addr.s_addr=INADDR_ANY;

//Association de l'adresse au socket

if (bind(sock1,(struct sockaddr *)&adr_local,lg_adr_local)==-1)
  { printf("échec du bind \n");
    exit(1) ;}
 else
   printf("succès du bind \n");

//Recuperation  des messages
 struct sockaddr*padr_em;
 int*plg_adr_em;
 char * pmesg = malloc(taille*sizeof(char));
 int option=0;
 recvfrom(sock1,pmesg,taille,option,padr_em,plg_adr_em);

//Affichage du message
 afficher_message(pmesg,taille);

}
void udp_source()
//Creation du socket local
{int sock2 = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
struct sockaddr_in adr_distant;
//identification de l'adresse du socket destinataire

  


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

	if (source == 1 & udp == 1) {
	  nb_message = 10; 
	  //udp_source();
	  exit(1);
	} else if(source == 1 & udp == 0){
	  nb_message = 10;
	  //tcp_source();
	  exit(1);
	} else if(source == 0 & udp == 1)
	  udp_puit(num_port,lg_message);
	else
	  //tcp_puit();
	  exit(1);
}

