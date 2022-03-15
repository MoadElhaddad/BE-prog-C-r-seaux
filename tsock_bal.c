/* librairie standard ... */
#include <stdlib.h>
#include <string.h>
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
/* Sous-programmes*/

#include "affichage.h"
#include "udp.h"
#include "tcp.h"
#include "emetteur.h"
#include "recepteur.h"
#include "bal.h"
void main(int argc, char **argv)
{
	int c;
	extern char *optarg;
	extern int optind;
	int nb_message = -1; /* Nb de messages à envoyer ou à recevoir, par défaut : 10 en émission, infini en réception */
	int source = -1;	 /* 0=puits, 1=source */
	int udp = 0;		 /*  indique le mode utilisé */
	int num_port = atoi(argv[argc - 1]);
	//	num_port=htons(num_port);
	int lg_message = 30; /*taille par défaut d'un message*/
	int maxsize = 30;
	int messmax = 100000 - 1;
	int num_machine;
	char *dest = argv[argc - 2];
	int e = 0;
	int r = 0;
	int srv_bal = 0;
	int Mode_BAL = 0;
	while ((c = getopt(argc, argv, "pn:sul:e:r:b")) != -1)
	{
		switch (c)
		{
		case 'p':
			if (source == 1)
			{
				printf("usage: cmd [-p|-s][-n ##]\n");
				exit(1);
			}
			source = 0;
			break;

		case 's':
			if (source == 0)
			{
				printf("usage: cmd [-p|-s][-n ##]\n");
				exit(1);
			}
			source = 1;
			break;

		case 'n':
			nb_message = atoi(optarg);
			if (nb_message > messmax)
			{
				printf("nb de message max est de %d :\n", messmax);
				exit(1);
			}
			break;

		case 'u':
			udp = 1;
			break;

		case 'l':
			lg_message = atoi(optarg);
			if (lg_message > maxsize)
			{
				printf("vous avez depasse la taille max permise de %d octets: envoi impossible\n", maxsize);
				exit(1);
			}
			break;

		case 'e':
			num_machine = atoi(optarg); // on rentre le destinataire des messages
			e = 1;
			break;
		case 'r':
			num_machine = atoi(optarg); // on indique notre numero de destinataire
			r = 1;
			break;
		case 'b':
			srv_bal = 1;
			break;
		default:
			printf("usage: cmd [-p|-s][-n ##]\n");
			break;
		}
	}
	if (e == 1 || r == 1 || srv_bal == 1)
	{
		Mode_BAL = 1;
	}
	if (Mode_BAL == 0)
	{ //évite de rentrer dans la partie 1 du TP si nous sommes en mode BAL
		if (source == -1)
		{
			printf("usage: cmd [-p|-s][-n ##]\n");
			exit(1);
		}
		if (source == 1)
			printf("on est dans le source\n");
		else
			printf("on est dans le puits\n");

		if (nb_message != -1)
		{
			if (source == 1)
				printf("nb de tampons à envoyer : %d\n", nb_message);
			else
				printf("nb de tampons à recevoir : %d\n", nb_message);
		}
		else
		{
			if (source == 1)
			{
				nb_message = 10;
				printf("nb de tampons à envoyer = 10 par défaut\n");
			}
			else
				printf("nb de tampons à envoyer = infini\n");
		}

		// Appel de la fonction à executer

		if (source == 1 & udp == 1)
		{
			udp_source(num_port, lg_message, nb_message, dest);
			exit(1);
		}
		else if (source == 1 & udp == 0)
		{
			tcp_source(num_port, lg_message, nb_message, dest);
			exit(1);
		}

		else if (source == 0 & udp == 1)
		{
			udp_puit(num_port, lg_message, nb_message);
		}
		else
		{
			tcp_puit(num_port, lg_message, nb_message);
			exit(1);
		}
	}

	//#####################################################
	//---------Mode BAL------------------------------------
	//#####################################################
	if (e == 1)
	{
		if (nb_message == -1)
		{
			nb_message = 10; // 10 messages par défaut
		}
		envoi_bal(num_port, dest, nb_message, lg_message, num_machine);
		exit(1);
	}
	if (srv_bal == 1)
	{
		Serv_BAL(num_port);
		exit(1);
	}
	if (r == 1)
	{
		recepteur_BAL(num_port, dest, num_machine);
	}
}
