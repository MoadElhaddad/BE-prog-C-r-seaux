typedef struct LETTRE 
{
    int num ;
    int lg;
    char *message ;
    struct LETTRE * suiv ;
}LETTRE ;

void envoi_bal(int num_port,char *dest, int nb_mess,int taille, int Num_Machine);
