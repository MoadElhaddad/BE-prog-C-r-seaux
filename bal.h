typedef struct BAL
{
    int num;
    int nb;
    struct LETTRE * lettre_debut;
    struct LETTRE * lettre_fin;
    struct LETTRE * lettre_actuel;
    struct BAL * suiv;
}BAL;

typedef struct LISTE_BAL
{
    struct BAL * first;
    struct BAL * last;
    struct BAL * current;
    int nb;
}LISTE_BAL;


void printLISTE(struct LISTE_BAL* liste);
void Creer_BAL(int n, LISTE_BAL * liste);
LISTE_BAL* init_BAL();
void Vide(BAL *bal) ;
BAL* Trouve_BAL(LISTE_BAL*liste, int num);
int Trouve_BAL_Recepteur(LISTE_BAL*liste, int num);
void Ajoute_Lettre (int n, int lg, BAL* bal , char* mess);
void Affiche_BAL(BAL* bal,int lg);
void Serv_BAL(int port);