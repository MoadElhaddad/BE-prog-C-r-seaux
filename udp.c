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
  void udp_puit(int num_port, int taille,int nb_message)
  {
  printf("On rentre dans udp_puit\n");
  //Creation du socket local
  int option=0;
  int sock1;
  struct sockaddr_in adr_local,adr_distant;
  int lg_adr_local=sizeof(adr_local);

  if ((sock1=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP))==-1)       //Creation du socket local
    {
      printf("échec de création du socket \n");
      exit(1);
    }

   memset((char*)&adr_local,option,lg_adr_local);


  //Adressage  du socket
  adr_local.sin_family=AF_INET;
  adr_local.sin_port=num_port;
  adr_local.sin_addr.s_addr=INADDR_ANY;
  //Association de l'adresse au socket

  if ((bind(sock1,(struct sockaddr*)&adr_local,lg_adr_local))==-1)
    {
      printf("echec du bind \n");
      exit(1) ;
    }


  //Recuperation  des messages
   char * pmesg = malloc(taille*sizeof(char));
   int recv=1;
   int lg_dist=sizeof(adr_distant);
   int i=1;
   printf("PUITS : lg_mesg_lu= %d,port= %d,receptions = infini  \n",taille,num_port);
   while (recv !=0)
     {
       recv= recvfrom(sock1,pmesg,taille,option,(struct sockaddr*)&adr_distant,&lg_dist);

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
         }
       i++;
     }
  }




void udp_source(int num_port,int taille,int nb_mess,char*dest)
{
  //Creation du socket local
  printf("On rentre dans udp source\n");
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
  char msg='a';
  printf("SOURCE : lg_mesg_emis = %d,port= %d,dest=%s \n",taille,num_port,dest);

  for (int i=1; i<=nb_mess;i++)
    {
      formater_message(pmesg,i);
      construire_message(pmesg,msg,taille);
      printf("SOURCE:envoi n°%d (%d) : [",i,taille);
      

      if ((sent= sendto(sock2,pmesg,taille,option,(struct sockaddr*)&(adr_distant),sizeof(adr_distant)))==-1)
	{ printf("Erreur sendto\n");
	  exit(1);}
      afficher_message(pmesg,sent);
       if (msg=='z') 
	{ msg='a';}
      else
	{msg++;}
       

    }
  printf("SOURCE : FIN \n");
}
