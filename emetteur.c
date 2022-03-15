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
#include "tcp.h"
#include "emetteur.h"

void envoi_bal(int num_port, char *dest, int nb_mess, int taille, int Num_Machine)
{
  // Déclarations
  int option = 0;
  int sock2;
  int taille_pdu = 30;
  int sent = -1;
  struct sockaddr_in adr_distant;
  struct hostent *hp;
  int lg_adr_local = sizeof(adr_distant);
  int connection;
  char *pmesg = malloc(taille * sizeof(char));
  char *pdu = malloc(taille_pdu * sizeof(char));
  char msg = 'a';

  sprintf(pdu, "0 %d %d %d", Num_Machine, nb_mess, taille);
  // creation d'un socket local
  printf("On rentre dans emetteur\n");

  if ((sock2 = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
  {
    printf("échec de création du socket \n");
    exit(1);
  }

  // identification de l'adresse du socket destinataire
  memset((char *)&adr_distant, option, sizeof(adr_distant));
  adr_distant.sin_family = AF_INET;
  adr_distant.sin_port = num_port;
  if ((hp = gethostbyname(dest)) == NULL)
  {
    printf("erreur gethostbyname \n");
    exit(1);
  }
  memcpy((char *)&(adr_distant.sin_addr.s_addr), hp->h_addr, hp->h_length);

  // Etablissement de la connexion

  if ((connection = connect(sock2, (struct sockaddr *)&(adr_distant), sizeof(adr_distant))) == -1)
  {
    printf("Echec connect\n");
    exit(1);
  }

  // création message indiquant que l'on est emetteur
  printf("Création du message indiquant que l'on est emetteur\n");
  if ((sent = write(sock2, pdu, taille)) == -1)
  {
    printf("Echec de l'envoi du message Emetteur\n");
    exit(1);
  }
  printf("La BAL sait que nous sommes emetteur\n");

  // Transmission des données à la BAL
  printf("EMETTEUR : lg_mesg_emis = %d,dest=%s \n", taille, dest);

  for (int i = 1; i <= nb_mess; i++)
  {
    formater_message(pmesg, i);
    construire_message(pmesg, msg, taille);
    printf("EMETTEUR:envoi n°%d (%d) : [", i, taille);

    if ((sent = write(sock2, pmesg, taille)) == -1)
    {
      printf("Erreur write\n");
      exit(1);
    }
    afficher_message(pmesg, sent);
    if (msg == 'z')
    {
      msg = 'a';
    }
    else
    {
      msg++;
    }
  }
  printf("EMETTEUR : FIN \n");
  // fermeture de la connexion

  if (close(sock2) == -1)
  {
    printf("échec de destruction du socket\n");
    exit(1);
  }

  free(pdu); // on libère la mémoire
  free(pmesg);
}
