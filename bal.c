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
int messmax = 99999; // taille max d'un message
//####################################################################################
//---------------------------------------------------#################################
//--------------------SP BAL-------------------------#################################
//---------------------------------------------------#################################
//####################################################################################
LISTE_BAL *init_BAL()
{
    LISTE_BAL *liste = (LISTE_BAL *)malloc(sizeof(struct LISTE_BAL));
    liste->first = NULL;
    liste->last = NULL;
    liste->current = NULL;
    liste->nb = 0;
    return liste;
}

void Affiche_BAL(BAL *bal, int lg) // Affiche le contenue d'une BAL
{
    printf("La BAL n°%d contient %d lettres \n", bal->num, bal->nb);
    bal->lettre_actuel = bal->lettre_debut;
    printf("\n");
    int n = 1;
    while (bal->lettre_actuel != NULL)
    {
        printf("BAL n°%d | %d Lettres, lettre n°%d : [", bal->num, bal->nb, n);
        afficher_message(bal->lettre_actuel->message, lg);
        bal->lettre_actuel = bal->lettre_actuel->suiv;
        n++;
    }
    printf("\n\n");
}

void printLISTE(LISTE_BAL *liste) // nombre de Bals
{
    printf("              ##############################################\n");
    printf("                Etat des lieux des BALs:\n\n");
    printf("                        %d BALs      \n\n", liste->nb);
    liste->current = liste->first;
    while (liste->current != NULL)
    {
        printf("                        BAL n°%d : %d Lettres          \n", liste->current->num, liste->current->nb);
        liste->current = liste->current->suiv;
    }
    printf("              ##############################################\n");
}
void Creer_BAL(int num, LISTE_BAL *liste)
{
    BAL *New_BAL = malloc(sizeof(struct BAL));
    New_BAL->num = num; 
    New_BAL->nb = 0;
    New_BAL->lettre_debut = NULL;
    New_BAL->lettre_fin = NULL;
    New_BAL->lettre_actuel = NULL;
    New_BAL->suiv = NULL;

    if (liste->first == NULL)
    {
        liste->first = New_BAL;
        liste->last = New_BAL;
    }
    else
    {
        liste->last->suiv = New_BAL;
        liste->last = New_BAL;
    }
    liste->nb++;
}



int Trouve_BAL_Recepteur(LISTE_BAL *liste, int num) // retourne la première lettre de la BAl si existance, -1 sinon
{
    int ret;
    if (liste->first == NULL)
    {
        ret = -1;
    }
    else
    {
        liste->current = liste->first;
        if (liste->current == liste->last)
        {
            if (liste->current->num == num)
            {
                if (liste->current->lettre_debut == NULL)
                    ret = -1;
                else
                    ret = liste->current->lettre_debut->lg;
            }
            else
                ret = -1;
        }
        else if (liste->first->num == num)
        {
            if (liste->current->lettre_debut == NULL)
                ret = -1;

            else
                ret = liste->current->lettre_debut->lg;
        }
        else
        {
            int var = 0;
            while (var == 0)
            {
                if (liste->current->suiv == NULL)
                    var = -1;
                else
                {
                    liste->current = liste->current->suiv;
                    if (liste->current->num == num)
                        var = 1;
                    if (liste->current == NULL)
                        var = -1;
                }
            }
            if (var == 1)
            {
                if (liste->current->lettre_debut == NULL)
                    ret = -1;
                else
                    ret = liste->current->lettre_debut->lg;
            }
            else
                ret = -1;
        }
    }
    return ret;
}
BAL *Trouve_BAL(LISTE_BAL *liste, int num) // trouve ou crée BAL
{
    BAL *bal = malloc(sizeof(struct BAL));
    liste->current = liste->first;
    if (liste->first == NULL)
    {
        Creer_BAL(num, liste);
        bal = liste->first;
    }
    else
    {
        liste->current = liste->first;

        if (liste->first == liste->last)
        {
            if (liste->first->num == num)
                bal = liste->current;
            else
            {
                Creer_BAL(num, liste);
                bal = liste->last;
            }
        }
        else if (liste->first->num == num)
            bal = liste->first;
        else
        {
            int var = 0;
            while (var == 0)
            {
                if (liste->current->suiv == NULL)
                    var = -1;
                else
                {
                    liste->current = liste->current->suiv;
                    if (liste->current->num == num)
                        var = 1;
                    if (liste->current == NULL)
                        var = -1;
                }
            }
            if (var == 1)
                bal = liste->current;
            else
            {
                Creer_BAL(num, liste);
                bal = liste->last;
            }
        }
    }
    return bal;
}


void Ajoute_Lettre(int n, int lg, BAL *bal, char *mess) //Ajoute lettre en fin de BAL
{
    bal->nb = (bal->nb) + 1;
    LETTRE *New_BAL;
    New_BAL = (LETTRE *)malloc(sizeof(LETTRE));
    New_BAL->num = n + 1;
    New_BAL->lg = lg;
    New_BAL->suiv = NULL;

    if (bal->lettre_debut == NULL)
    {
        bal->lettre_debut = New_BAL;
        bal->lettre_fin = New_BAL;
        bal->lettre_actuel = New_BAL;
    }

    else
    {
        bal->lettre_fin->suiv = New_BAL;
        bal->lettre_fin = bal->lettre_fin->suiv;
    }

    New_BAL->message = malloc(lg * sizeof(char));
    for (int i = 0; i < lg; i++)
        New_BAL->message[i] = mess[i];
}

void Vide(BAL *bal) // détruit la bal
{
    bal->lettre_actuel = bal->lettre_debut;
    while (bal->lettre_actuel != NULL)
    {
        bal->lettre_actuel = bal->lettre_actuel->suiv;
        free(bal->lettre_debut);
        bal->lettre_debut = bal->lettre_actuel;
        (bal->nb)--;
    }
}
//##########################################################
//#----------Programme Principal---------------------------#
//##########################################################
void Serv_BAL(int port)
{
    printf("\n"
           "  ______                                                   ______        _       _____     \n"
           ".' ____ \\                                                 |_   _ \\      / \\     |_   _|    \n"
           "| (___ \\_| .---.  _ .--.  _   __  .---.  __   _   _ .--.    | |_) |    / _ \\      | |      \n"
           " _.____`. / /__\\\\[ `/'`\\][ \\ [  ]/ /__\\\\[  | | | [ `/'`\\]   |  __'.   / ___ \\     | |   _  \n"
           "| \\____) || \\__., | |     \\ \\/ / | \\__., | \\_/ |, | |      _| |__) |_/ /   \\ \\_  _| |__/ | \n"
           " \\______.' '.__.'[___]     \\__/   '.__.' '.__.'_/[___]    |_______/|____| |____||________| \n"
           "                                                                                           \n"
           "");
         
    // Déclarations    
    int sock, sock2;
    struct sockaddr *addr_distant;
    struct sockaddr_in addr_local;
    int lg_addr_distant = sizeof(addr_distant);
    int lg_addr_local = sizeof(addr_local);
    struct hostent *hp;
    char motif;
    char *message;
    int lg_recv = -1;
    int lg_sent = -1;
    int taille = 30;
    int type = -1;
    int nb;
    int lg;
    int n = 1;
    int nBAL;
    BAL *bal = malloc(sizeof(struct BAL));
    char *pdu;
    LISTE_BAL *liste;


    // Création socket local
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        printf("Echec de la création d'un socket local\n");
        exit(1);
    }
      // identification de l'adresse du socket destinataire
    memset((char *)&addr_local, 0, sizeof(addr_local));
    addr_local.sin_family = AF_INET;
    addr_local.sin_addr.s_addr = INADDR_ANY;
    addr_local.sin_port = port;

    // Association socket
    if (bind(sock, (struct sockaddr *)&addr_local, lg_addr_local) == -1)
    {
        printf("Echec du bind\n");
        exit(1);
    }
    // Empeche un trop grand nombre de connexions simultanés
    if (listen(sock, 100) < 0)
    {
        printf("Trop de connexions en attentes\n");
        exit(1);
    }
    liste = init_BAL();
    while (1)
    {
        if ((sock2 = accept(sock, (struct sockaddr *)&addr_distant, &lg_addr_distant)) == -1)
        {
            printf("Connexion refusée\n");
            exit(1);
        }

        pdu = malloc(50 * sizeof(char));
        if ((taille = read(sock2, pdu, taille)) < 0)
        {
            printf("Echec de lecture du PDU entrant\n");
            exit(1);
        }
        sscanf(pdu, "%d %d %d %d", &type, &nBAL, &nb, &lg);
        // Machine distante est émetrice 
        if (atoi(pdu) == 0)
        {
            printf("Réception des lettres à destination de la machine n°%d\n\n", nBAL);
            message = malloc(lg * sizeof(char));
            int n = 0;
            sscanf(pdu, "%d %d %d %d", &type, &nBAL, &nb, &lg);
            bal = Trouve_BAL(liste, nBAL);

            while (n != nb)
            {
                message = malloc(lg * sizeof(char));
                if ((lg_recv = read(sock2, message, lg)) == -1)
                {
                    printf("Erreur de lecture\n");
                    exit(1);
                }
                if (lg_recv > 0)
                {
                    Ajoute_Lettre(n, lg, bal, message);
                }
                n++;
            }

            Affiche_BAL(bal, lg_recv);
        }
        // Gestion réception
        else if (atoi(pdu) == 1)
        {
            sscanf(pdu, "%d %d", &type, &nBAL);
            printf("        Restitution des lettres à destination de la machine n%d \n\n", nBAL);
            lg = Trouve_BAL_Recepteur(liste, nBAL);
            if (lg == -1) // Si BAL vide ->
            {
                printf("La BAL n'existe pas, on informe la machine distante\n\n");
                sprintf(pdu, "%d %d", lg, nb);
                int lg_sent = -1;
                nb = 1;
                if ((lg_sent = write(sock2, pdu, taille)) == -1)
                {
                    printf("Echec de l'envoi du message \n");
                    exit(1);
                }
            }
            else
            {
                bal = Trouve_BAL(liste, nBAL);
                bal->lettre_actuel = bal->lettre_debut;

                while (bal->lettre_actuel != NULL)
                {
                    lg = bal->lettre_actuel->lg;
                    nb = bal->nb;
                    sprintf(pdu, "%d %d", lg, nb);

                    if ((lg_sent = write(sock2, pdu, taille)) == -1)
                    {
                        printf("Echec de l'envoi du message Emetteur\n");
                        exit(1);
                    }
                    message = malloc(lg * sizeof(char));
                    message = bal->lettre_actuel->message;

                    if ((lg_sent = write(sock2, message, lg)) == -1)
                    {
                        printf("Erreur lors de l'envoi du message n°%d\n", n);
                        exit(1);
                    }
                    printf("Restitution de la lettre n°%d (%d) [", n, lg);
                    afficher_message(message, lg);
                    bal->lettre_actuel = bal->lettre_actuel->suiv;
                    n++;
                }
                Vide(bal);
                //fermeture connexion
                if ((close(sock2)) == -1)
                {
                    printf("Echec fermetude de connexion \n");
                    exit(1);
                }
            }
        }
        else
        {
            printf("PDU non reconnu, on quitte par sécurité\n");
            exit(1);
        }
        printLISTE(liste);
        free(pdu);
        free(message);
    }
}