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

#include "udp.h"
#include "affichage.h"
#include "tcp.h"

void recepteur_BAL(int num_port, char *dest, int Num_Machine)
{

  // Déclarations
  int maxtaille = 99999; 
  int option = 0;
  int sockr;
  int taille_pdu = 30;
  struct sockaddr_in adr_distant;
  int lg_adr_local = sizeof(adr_distant);
  struct hostent *hp;
  char *pdu = malloc(taille_pdu * sizeof(char));
  char *pmesg;
  int connection;
  int sent = -1;
  int nb_lettres;
  int etat_recv = -1;
  int lg;
  sprintf(pdu, "1 %d", Num_Machine);
  // Creation du socket local
  if ((sockr = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
  {
    printf("échec de création du socket \n");
    exit(1);
  }

  memset((char *)&adr_distant, option, sizeof(adr_distant));

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

  if ((connection = connect(sockr, (struct sockaddr *)&(adr_distant), sizeof(adr_distant))) == -1)
  {
    printf("Echéc connect\n");
    exit(1);
  }

  // création du message indiquant que l'on est recepteur

  if ((sent = write(sockr, pdu, taille_pdu)) == -1)
  {
    printf("Echec de l'envoi du message récepteur\n");
    exit(1);
  }
  char *lgmsg = malloc(maxtaille * sizeof(char));
  nb_lettres = 10;
  int n = 1;
  etat_recv = 1;
  printf("             BAL : Réception du contenu de la BAL n°%d\n\n", Num_Machine);

  while (n <= nb_lettres)
  {

    if ((etat_recv = recv(sockr, lgmsg, taille_pdu, 0)) == -1)
    {
      printf("Erreur à la réception du message");
      exit(1);
    }
    sscanf(lgmsg, "%d %d", &lg, &nb_lettres);
    if (lg == -1)
    {
      printf(" Pas de courrier à récupérer dans la BAL n°%d\n\n", Num_Machine);
      exit(0);
    }
    pmesg = malloc(lg * sizeof(char));
    if ((etat_recv = read(sockr, pmesg, lg)) == -1)
    {
      printf("Erreur à la réception du message\n");
      exit(1);
    }
    printf("Récepteur : Réception de la lettre n°%d : [", n);
    afficher_message(pmesg, lg);
    n++;
  }
  if (close(sockr) == -1)
  {
    {
      printf("échec fermeture du socket\n");
      exit(1);
    }
  }
}
