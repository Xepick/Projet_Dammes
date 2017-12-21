#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

/* --Le--nommage--de--certaines--valeurs----------------------------------------------------------------------------- */

#define NMAX 10

#define PIECES ( N * ( N - 2 ) / 4 )

#define PRISE 12
#define PROF 12

#define OUI 1
#define NON 0

#define NeutreMAX -10000
#define NeutreMIN 10000

#define BonusDame 3

#define DameBL 2
#define PionBL 1
#define DameNO -2
#define PionNO -1

#define BLANC 1
#define RIEN 0
#define NOIR -1

#define AVANT 1
#define ARRIERE -1

#define GAUCHE -1
#define DROITE 1

#define AUCUN_MOUV 0
#define MOUV_SANS_PRISE 1
#define MOUV_AVEC_PRISE 2

/* --Une--structure-------------------------------------------------------------------------------------------------- */

/* Cette structure permet de mémoriser un mouvement avec ou sans prise. Le champ tdepl prend l'une des valeurs
   AUCUN_MOUV, MOUV_SANS_PRISE ou MOUV_AVEC_PRISE. Le reste de la structure donne le point de départ et le type
   de la pièce, ensuite son point et type d'arrivée et, finalement, l'endroit et le type de la prise. */

typedef struct
        {int tdepl ;
         int li ;
         int co ;
         int piece ;
         int liar ;
         int coar ;
         int piecear ;
         int lipr ;
         int copr ;
         int piecepr ;
        }
tmm ;

/* --Les--variables--globales---------------------------------------------------------------------------------------- */

/* N contient la taille du plateau et vaudra 8 ou 10. T donne le plateau. Les valeurs dans T sont PionNO, PionBL,
   DameNO, DameBL ou RIEN. Verbeux_minimax et Verbeux_mouvements sont des variables qui contrôlent un éventuel
   affichage pendant le calcul. Si Verbeux_minimax est positionnée, on affiche le score à chaque cas d'arrêt de
   minimax. Verbeux_mouvements dit si la procédure affichage doit être appelée ou non. Verbeux_mouvements_indent
   dit si les affichages sont inventés en fonction de la profondeur. */

int N ;

int T[ NMAX ][ NMAX ] ;

int Verbeux_minimax = NON ;
int Verbeux_mouvements = NON ;
int Verbeux_mouvements_indent = NON ;

/* Les deux variables suivantes servent à mémoriser les mouvements. La première dimension de taille PROF mémorise le
   résultat pour la profondeur donnée. La seconde dimension mémorise les mouvements successifs d'une pièce. En effet,
   en cas de prise, on peut enchaîner plusieurs mouvements.

   Memo est passée depuis minimax_call et contiendra le résultat final. Dans Memo_local on construit la suite de
   mouvements couramment étudiée. Si cette séquence est optimale elle sera recopiée et finira en Memo. Memo_num
   compte simplement le nombre de solutions optimales ex-aequo. */

tmm Memo[ PROF ][ PRISE ] ;
tmm Memo_local[ PROF ][ PRISE ] ;

int Memo_num ;

/* --Les--prototypes--des--fonctions--------------------------------------------------------------------------------- */

int main ( int argc , char * argv[ ] ) ;

void main_test_prise ( void ) ;
void main_bouge_sans_prise ( void ) ;
void main_bouge_sans_prise_prof_deux ( void ) ;
void main_bouge_avec_prise ( void ) ;
void main_grand_test ( int prof ) ;
void main_petit_test ( int prof ) ;

void remplis_echiquier ( void ) ;
inline void remplis_case ( int li , int co , int piece ) ;
int compte_pieces ( int coul ) ;

inline int est_dame ( int li , int co , int coul ) ;
inline int est_pion ( int li , int co , int coul ) ;
inline int est_piece ( int li , int co , int coul ) ;

void memo_local ( int tdepl , int prof , int index , int li , int co , int piece ,
                  int liar , int coar , int piecear , int lipr , int copr , int piecepr ) ;
void memo ( tmm m[ PRISE ] , int tdepl , int index , int li , int co , int piece ,
            int liar , int coar , int piecear , int lipr , int copr , int piecepr ) ;
void enregistre_alea ( int reset , int prof , tmm to_memo[ PROF ][ PRISE ] , tmm from_memo[ PROF ][ PRISE ]  ) ;
void annule_mouvements ( tmm m[ PROF ][ PRISE ] ) ;

void joue_sequence ( int profdeb , int proffin ) ;
void joue_mouvements ( tmm m[ PRISE ] ) ;
void joue_mouv ( tmm m ) ;

void print_echiquier ( void ) ;
void print_echiquier_ligne ( int li ) ;
void print_echiquier_colonnes ( int li ) ;
void print_line ( char separator_char , char fill_char ) ;
void print_contenu(int contenu);
int contenu_case(int lig, int col);


void affiche ( int prise_ou_non , int li , int co , int piece , int liar , int coar , int piecear ,
               int lipr , int copr , int piecepr , int prof ) ;
void print_les_mouvements ( int prof ) ;
void print_mouvement ( int prof , int indent ) ;
void print_mouv ( tmm m[ PRISE ] , int indent ) ;

int prise_possible_avant ( int coul ) ;
int prise_possible_toutes ( int li , int co , int coul ) ;
int prise_possible_case ( int li , int co , int coul , int sens , int direct ) ;

void itere_jeu ( int coul_ordinateur , int prof ) ;
int suite_ou_pas ( void ) ;
int analyse_mouvement ( int dep , int arr , int prise_ou_pas , int couleur , int * num_mouv ,
                        tmm depl[ PRISE ] , int * suite , int * lisuite , int * cosuite ) ;
int est_coince ( int couleur ) ;

int minimax_call ( int prof , int coul ) ;
int minimax ( int numBL , int numNO , int prof , int coul , tmm m[ PROF ][ PRISE ] ) ;
int relance_minimax ( int numBL , int numNO , int coul , int prof , int minmax , tmm m[ PROF ][ PRISE ] ) ;

int cherche_depl ( int numBL , int numNO , int coul , int prof , int minmax , tmm m[ PROF ][ PRISE ] ) ;

int cherche_depl_avec_prise ( int li , int co , int piece , int coul , int numBL , int numNO , int prof ,
                              int minmax , int num_mouv , int seulement_avant , tmm m[ PROF ][ PRISE ] ) ;
int effectue_depl_avec_prise ( int li , int co , int piece , int coul , int lipr , int copr , int liar , int coar ,
                               int numBL , int numNO , int prof , int minmax , int num_mouv , tmm m[ PROF ][ PRISE ] ) ;

int cherche_depl_pion_sans_prise ( int li , int co , int coul , int numBL ,
                                   int numNO , int prof , int minmax , tmm m[ PROF ][ PRISE ] ) ;
int cherche_depl_dame_sans_prise ( int li , int co , int coul , int numBL ,
                                   int numNO , int prof , int minmax , tmm m[ PROF ][ PRISE ] ) ;
int effectue_depl_sans_prise ( int lidp , int codp , int piece , int coul , int liar , int coar ,
                               int numBL , int numNO , int prof , int minmax , tmm m[ PROF ][ PRISE ] ) ;

/* --La--fonction--principale---------------------------------------------------------------------------------------- */

/* Le fichier out_main contient le résultat produit par main. Si ces tests sont concluants, on peut partir du
   principe que le programmeur saura mettre en place un dialogue avec l'utilisateur. */

int main ( int argc , char * argv[ ] ){
  N=10;
    remplis_echiquier( ) ;
    print_echiquier();
    if ( argc <= 1 )
      {N = 10 ;
       main_test_prise( ) ;
       main_bouge_sans_prise( ) ;
       main_bouge_sans_prise_prof_deux( ) ;
       main_bouge_avec_prise( ) ;
       main_grand_test( 10 ) ;
       main_petit_test( 11 ) ;
      }
   else
      if ( argc == 4 )
         {int coul_ordi , prof ;
          char * texte[ 3 ] = { "noirs" , "" , "blancs" } ;
          N = atoi( argv[ 1 ] ) ;
          coul_ordi = atoi( argv[ 2 ] ) ;
          prof = atoi( argv[ 3 ] ) ;
          if ( coul_ordi )
             coul_ordi = BLANC ;
          else
             coul_ordi = NOIR ;
          if ( ( N == 8 || N == 10 ) && ( 0 < prof && prof < PROF ) )
             {(void)printf( "L'échiquier est de taille %d et l'ordi joue les " , N ) ;
              (void)printf( "%s. La profondeur de recherche est de %d.\n" , texte[ coul_ordi + 1 ] , prof ) ;
              remplis_echiquier( ) ;
              itere_jeu( coul_ordi , prof ) ;
             }
         }
   (void)printf( "Bye !\n" ) ;
   return( 0 ) ;
    }

/* --Les--fonctions--de--test---------------------------------------------------------------------------------------- */

/* Le fichier out_main contient le résultat produit par ces tests. */

void main_test_prise ( void )
     {assert( N == 10 ) ;
      remplis_echiquier( ) ;
      (void)printf( "\n\nTests de prises possibles\n\n" ) ;
      (void)printf( "Prises possibles pour les blancs (grille initiale) : %d\n" , prise_possible_avant( BLANC ) ) ;
      (void)printf( "Prises possibles pour les noirs (grille initiale) : %d\n" , prise_possible_avant( NOIR ) ) ;
      remplis_case( 5 , 5 , PionBL ) ;
      remplis_case( 7 , 7 , RIEN ) ;
      remplis_case( 5 , 9 , DameBL ) ;
      remplis_case( 6 , 8 , RIEN ) ;
      remplis_case( 9 , 5 , RIEN ) ;
      remplis_case( 6 , 2 , DameNO ) ;
      remplis_case( 2 , 6 , RIEN ) ;
      remplis_case( 7 , 3 , PionBL ) ;
      remplis_case( 8 , 4 , RIEN ) ;
      remplis_case( 3 , 7 , PionNO ) ;
      print_echiquier( ) ;
      (void)printf( "Prise depuis %d %d vers la droite et l'avant : %d\n" ,
                    5 , 5 , prise_possible_case( 5 , 5 , BLANC , AVANT , DROITE ) ) ;
      (void)printf( "Prise depuis %d %d vers la gauche et l'avant : %d\n" ,
                    5 , 5 , prise_possible_case( 5 , 5 , BLANC , AVANT , GAUCHE ) ) ;
      (void)printf( "Prise depuis %d %d vers la gauche et l'avant : %d\n" ,
                    6 , 4 , prise_possible_case( 6 , 4 , NOIR , AVANT , GAUCHE ) ) ;
      (void)printf( "Prise depuis %d %d vers la droite et l'avant : %d\n" ,
                    6 , 6 , prise_possible_case( 6 , 6 , NOIR , AVANT , DROITE ) ) ;
      (void)printf( "Prise depuis %d %d vers la gauche et l'avant : %d\n" ,
                    6 , 6 , prise_possible_case( 6 , 6 , NOIR , AVANT , GAUCHE ) ) ;
      (void)printf( "Prise depuis %d %d vers la gauche et l'avant : %d\n" ,
                    5 , 9 , prise_possible_case( 5 , 9 , BLANC , AVANT , GAUCHE ) ) ;
      (void)printf( "Prise depuis %d %d vers la droite et l'avant : %d\n" ,
                    5 , 9 , prise_possible_case( 5 , 9 , BLANC , AVANT , DROITE ) ) ;
      (void)printf( "Prise depuis %d %d vers la gauche et l'avant : %d\n" ,
                    6 , 2 , prise_possible_case( 6 , 2 , NOIR , AVANT , GAUCHE ) ) ;
      (void)printf( "Prise depuis %d %d vers la droite et l'avant : %d\n" ,
                    6 , 2 , prise_possible_case( 6 , 2 , NOIR , AVANT , DROITE ) ) ;
      (void)printf( "Prise depuis %d %d vers la gauche et l'arrière : %d\n" ,
                    6 , 2 , prise_possible_case( 6 , 2 , NOIR , ARRIERE , GAUCHE ) ) ;
      (void)printf( "Prise depuis %d %d vers la droite et l'arrière : %d\n" ,
                    6 , 2 , prise_possible_case( 6 , 2 , NOIR , ARRIERE , DROITE ) ) ;
      (void)printf( "Prise depuis %d %d vers la gauche et l'arrière : %d\n" ,
                    5 , 9 , prise_possible_case( 5 , 9 , BLANC , ARRIERE , GAUCHE ) ) ;
      (void)printf( "Prises possibles pour les blancs : %d\n" , prise_possible_avant( BLANC ) ) ;
      (void)printf( "Prises possibles pour les noirs : %d\n" , prise_possible_avant( NOIR ) ) ;
      (void)printf( "\n\n" ) ;
     }

void main_bouge_sans_prise ( void )
     {assert( N == 10 ) ;
      Verbeux_mouvements = OUI ;
      remplis_echiquier( ) ;
      remplis_case( 3 , 3 , DameBL ) ;
      remplis_case( 2 , 2 , RIEN ) ;
      remplis_case( 1 , 1 , RIEN ) ;
      remplis_case( 0 , 0 , RIEN ) ;
      remplis_case( 6 , 0 , RIEN ) ;
      remplis_case( 3 , 1 , RIEN ) ;
      remplis_case( 2 , 0 , RIEN ) ;
      remplis_case( 6 , 4 , DameNO ) ;
      remplis_case( 7 , 3 , RIEN ) ;
      remplis_case( 2 , 6 , RIEN ) ;
      remplis_case( 8 , 0 , PionBL ) ;
      remplis_case( 9 , 1 , RIEN ) ;
      remplis_case( 1 , 9 , PionNO ) ;
      remplis_case( 0 , 8 , RIEN ) ;
      assert( ! prise_possible_avant( BLANC ) && ! prise_possible_avant( NOIR ) ) ;
      print_echiquier( ) ;
      (void)printf( "\n\nTests des mouvements sans prise\n\n" ) ;
      (void)printf( "Mouvements possibles pour les blancs : \n\n" ) ;
      (void)minimax_call( 1 , BLANC ) ;
      print_mouvement( 1 , 0 ) ;
      print_echiquier( ) ;
      (void)printf( "\nMouvements possibles pour les noirs : \n\n" ) ;
      (void)minimax_call( 1 , NOIR ) ;
      print_mouvement( 1 , 0 ) ;
      (void)printf( "\n\n" ) ;
      Verbeux_mouvements = NON ;
     }

void main_bouge_sans_prise_prof_deux ( void )
     {assert( N == 10 ) ;
      Verbeux_mouvements = OUI ;
      Verbeux_mouvements_indent = OUI ;
      remplis_echiquier( ) ;
      remplis_case( 0 , 2 , RIEN ) ;
      remplis_case( 1 , 3 , RIEN ) ;
      remplis_case( 2 , 4 , DameBL ) ;
      remplis_case( 2 , 6 , RIEN ) ;
      remplis_case( 3 , 3 , RIEN ) ;
      remplis_case( 3 , 9 , RIEN ) ;
      remplis_case( 3 , 7 , RIEN ) ;
      remplis_case( 4 , 0 , PionBL ) ;
      remplis_case( 6 , 0 , RIEN ) ;
      remplis_case( 6 , 2 , RIEN ) ;
      remplis_case( 6 , 6 , RIEN ) ;
      remplis_case( 7 , 5 , DameNO ) ;
      remplis_case( 8 , 0 , RIEN ) ;
      remplis_case( 8 , 6 , RIEN ) ;
      assert( ! prise_possible_avant( BLANC ) && ! prise_possible_avant( NOIR ) ) ;
      print_echiquier( ) ;
      (void)printf( "\n\nTests de deux mouvements sans prise\n\n" ) ;
      (void)printf( "Mouvements possibles pour les blancs : \n\n" ) ;
      (void)minimax_call( 2 , BLANC ) ;
      print_mouvement( 2 , 0 ) ;
      print_mouvement( 1 , 5 ) ;
      print_echiquier( ) ;
      (void)printf( "\n Mouvements possibles pour les noirs : \n\n" ) ;
      Verbeux_minimax = 1 ;
      (void)minimax_call( 2 , NOIR ) ;
      print_mouvement( 2 , 0 ) ;
      print_mouvement( 1 , 5 ) ;
      Verbeux_minimax = 0 ;
      (void)printf( "\n\n" ) ;
      Verbeux_mouvements = NON ;
      Verbeux_mouvements_indent = NON ;
     }

void main_bouge_avec_prise ( void )
     {assert( N == 10 ) ;
      Verbeux_mouvements = OUI ;
      remplis_echiquier( ) ;
      remplis_case( 4 , 2 , PionNO ) ;
      print_echiquier( ) ;
      (void)printf( "\n\nTests des mouvements avec prise\n\n" ) ;
      (void)printf( "Mouvements possibles pour les blancs : \n\n" ) ;
      (void)minimax_call( 1 , BLANC ) ;
      print_mouvement( 1 , 0  ) ;
      remplis_case( 3 , 3 , RIEN ) ;
      remplis_case( 7 , 5 , RIEN ) ;
      print_echiquier( ) ;
      (void)minimax_call( 1 , BLANC ) ;
      print_mouvement( 1 , 0 ) ;
      remplis_case( 9 , 3 , RIEN ) ;
      print_echiquier( ) ;
      (void)minimax_call( 1 , BLANC ) ;
      print_mouvement( 1 , 0 ) ;
      remplis_case( 2 , 4 , RIEN ) ;
      remplis_case( 0 , 2 , RIEN ) ;
      remplis_case( 2 , 0 , RIEN ) ;
      print_echiquier( ) ;
      Verbeux_minimax = 1 ;
      Verbeux_mouvements_indent = OUI ;
      (void)minimax_call( 2 , BLANC ) ;
      print_mouvement( 2 , 0 ) ;
      print_mouvement( 1 , 5 ) ;
      (void)printf( "\n\n" ) ;
      Verbeux_minimax = 0 ;
      Verbeux_mouvements = NON ;
      Verbeux_mouvements_indent = NON ;
     }

void main_grand_test ( int prof )
     {int bilan ;
      assert( N == 10 ) ;
      remplis_echiquier( ) ;
      remplis_case( 4 , 2 , PionNO ) ;
      remplis_case( 3 , 3 , RIEN ) ;
      remplis_case( 7 , 5 , RIEN ) ;
      remplis_case( 9 , 3 , RIEN ) ;
      remplis_case( 2 , 4 , RIEN ) ;
      remplis_case( 0 , 2 , RIEN ) ;
      remplis_case( 2 , 0 , RIEN ) ;
      print_echiquier( ) ;
      (void)printf( "Le bilan initial effectif en pièces BLANC - NOIR est de %d\n" ,
                    compte_pieces( BLANC ) - compte_pieces( NOIR ) ) ;
      bilan = minimax_call( prof , BLANC ) ;
      print_les_mouvements( prof ) ;
      (void)printf( "Le bilan final, avec cette séquence, en pièces BLANC - NOIR est de %d\n\n" , bilan ) ;
      (void)printf( "On effectue les 3 premiers mouvements et on recommence !\n" ) ;
      remplis_case( 3 , 1 , RIEN ) ;
      remplis_case( 4 , 2 , RIEN ) ;
      remplis_case( 6 , 4 , RIEN ) ;
      remplis_case( 8 , 4 , RIEN ) ;
      remplis_case( 6 , 6 , RIEN ) ;
      remplis_case( 6 , 8 , RIEN ) ;
      remplis_case( 3 , 5 , RIEN ) ;
      remplis_case( 1 , 3 , RIEN ) ;
      remplis_case( 1 , 1 , RIEN ) ;
      remplis_case( 2 , 0 , DameNO ) ;
      remplis_case( 0 , 0 , RIEN ) ;
      remplis_case( 1 , 1 , PionBL ) ;
      print_echiquier( ) ;
      (void)printf( "Le bilan initial effectif en pièces BLANC - NOIR est de %d\n" ,
                    compte_pieces( BLANC ) - compte_pieces( NOIR ) ) ;
      bilan = minimax_call( prof , NOIR ) ;
      print_les_mouvements( prof ) ;
      (void)printf( "Le bilan final, avec cette séquence, en pièces BLANC - NOIR est de %d\n\n" , bilan ) ;
      (void)printf( "A jouer cette séquence, le nouvel échiquier sera :\n\n" ) ;
      joue_sequence( prof , 1 ) ;
      print_echiquier( ) ;
     }

void main_petit_test ( int prof )
     {int bilan ;
      N = 8 ;
      remplis_echiquier( ) ;
      print_echiquier( ) ;
      (void)printf( "Le bilan initial effectif en pièces BLANC - NOIR est de %d\n" ,
                    compte_pieces( BLANC ) - compte_pieces( NOIR ) ) ;
      bilan = minimax_call( prof , BLANC ) ;
      print_les_mouvements( prof ) ;
      (void)printf( "Le bilan final, avec cette séquence, en pièces BLANC - NOIR est de %d\n\n" , bilan ) ;
      (void)printf( "A jouer cette séquence, le nouvel échiquier sera :\n\n" ) ;
      joue_sequence( prof , 1 ) ;
      print_echiquier( ) ;
      (void)printf( "Et on détermine la prochaine séquence …\n\n" ) ;
      bilan = minimax_call( prof , NOIR ) ;
      print_les_mouvements( prof ) ;
      (void)printf( "Le bilan final, avec cette séquence, en pièces BLANC - NOIR est de %d\n\n" , bilan ) ;
      (void)printf( "A jouer cette séquence, le nouvel échiquier sera :\n\n" ) ;
      joue_sequence( prof , 1 ) ;
      print_echiquier( ) ;
      (void)printf( "Et on détermine la prochaine séquence …\n\n" ) ;
      bilan = minimax_call( prof , BLANC ) ;
      print_les_mouvements( prof ) ;
      (void)printf( "Le bilan final, avec cette séquence, en pièces BLANC - NOIR est de %d\n\n" , bilan ) ;
      (void)printf( "A jouer cette séquence, le nouvel échiquier sera :\n\n" ) ;
      joue_sequence( prof , 1 ) ;
      print_echiquier( ) ;
      (void)printf( "Et on détermine la prochaine séquence …\n\n" ) ;
      bilan = minimax_call( prof , NOIR ) ;
      print_les_mouvements( prof ) ;
      (void)printf( "Le bilan final, avec cette séquence, en pièces BLANC - NOIR est de %d\n\n" , bilan ) ;
      (void)printf( "A jouer cette séquence, le nouvel échiquier sera :\n\n" ) ;
      joue_sequence( prof , 1 ) ;
      print_echiquier( ) ;
      (void)printf( "Et on détermine la prochaine séquence …\n\n" ) ;
      bilan = minimax_call( prof , BLANC ) ;
      print_les_mouvements( prof ) ;
      (void)printf( "Le bilan final, avec cette séquence, en pièces BLANC - NOIR est de %d\n\n" , bilan ) ;
      (void)printf( "A jouer cette séquence, le nouvel échiquier sera :\n\n" ) ;
      joue_sequence( prof , 1 ) ;
      print_echiquier( ) ;
      (void)printf( "Et on détermine la prochaine séquence …\n\n" ) ;
      bilan = minimax_call( prof , NOIR ) ;
      print_les_mouvements( prof ) ;
      (void)printf( "Le bilan final, avec cette séquence, en pièces BLANC - NOIR est de %d\n\n" , bilan ) ;
      (void)printf( "A jouer cette séquence, le nouvel échiquier sera :\n\n" ) ;
      joue_sequence( prof , 1 ) ;
      print_echiquier( ) ;
     }

/* --Les--fonctions--de--remplissage--------------------------------------------------------------------------------- */

/* Cette fonction construit l'échiquier initial. La case ( 0 , 0 ), en bas à gauche, relit un pion. Sur un
   damier de taille N , les N - 1 premières lignes comportent des pions blancs et les N - 1 dernières des noirs. */

   void remplis_echiquier ( void ) // DONE
   {
       int ligne,colonne, couleur;
       int ligne_max_basse=(N/2)-1,ligne_mediane=(N/2)+1;

       for (ligne = 0; ligne<N; ligne++)
        for (colonne = 0; colonne < N; colonne++){
          if (ligne>=ligne_max_basse)
            if (ligne>=ligne_mediane)
              couleur = NOIR;
            else
              couleur = RIEN;
          else
            couleur = BLANC;
          if((colonne%2 == 0 ^ ligne%2 == 0) == 1)
            couleur = RIEN;
          remplis_case(ligne,colonne,couleur);
        }
   }
/* La fonction qui modifie une case de l'échiquier */

inline void remplis_case ( int li , int co , int piece )
     {
       T[ li ][ co ] = piece ;
     }

/* Ici, on compte le nombre de pièces de la couleur donnée. Chaque dame est créditée d'un bonus BonusDame. */

int compte_pieces ( int coul )
    {
      int i, j, somme = 0;
      for ( i = 0; i < N; i++ ) {
        for ( j = 0; j < N; j++ ) {
          somme += (coul*T[i][j]>0) + ((coul*T[i][j]>1)*BonusDame);
        }
      }
      return somme;
    }

/* --Des--fonctions--auxiliaires------------------------------------------------------------------------------------- */

/* Ces prédicats disent si, oui ou non, on a affaire à une dame, un pion ou une pièce de la couleur indiquée. */

inline int est_dame ( int li , int co , int coul )
    {
      return( T[ li ][ co ] * coul == 2 ) ;
    }

inline int est_pion ( int li , int co , int coul )
    {
      return( T[ li ][ co ] * coul == 1 ) ;
    }

inline int est_piece ( int li , int co , int coul )
    {return( T[ li ][ co ] * coul > 0 ) ;
    }

/* --Des--fonctions--auxiliaires--de--mémorisation------------------------------------------------------------------- */

/* Ces fonctions mémorisent un mouvement de pièce. Suivant tdepl, il y a prise ou non. La profondeur courante est prof
   et c'est le mouvement d'ordre index (en cas de prise de pièces, on peut enchainer plusieurs mouvements). Attention,
   prof est compté à partir de 1, on décale localement. */

void memo_local ( int tdepl , int prof , int index , int li , int co , int piece ,
                  int liar , int coar , int piecear , int lipr , int copr , int piecepr )
     {assert( index + 1 < PRISE ) ;
      memo( Memo_local[ prof - 1 ] , tdepl , index , li , co , piece ,
            liar , coar , piecear , lipr , copr , piecepr ) ;
     }

void memo ( tmm m[ PRISE ] , int tdepl , int index , int li , int co , int piece ,
            int liar , int coar , int piecear , int lipr , int copr , int piecepr )
     {m[ index ].tdepl = tdepl ;
      m[ index ].li = li ;
      m[ index ].co = co ;
      m[ index ].piece = piece ;
      m[ index ].liar = liar ;
      m[ index ].coar = coar ;
      m[ index ].piecear = piecear ;
      m[ index ].lipr = lipr ;
      m[ index ].copr = copr ;
      m[ index ].piecepr = piecepr ;
      m[ index + 1 ].tdepl = AUCUN_MOUV ;
     }

/* On enregistre une séquence optimale, peut-être ex-aequo. Si reset est vrai, le mouvement est strictement meilleur
   et enregistré. En cas d'ex-aequo, on enregistre avec une équi-probabilté par rapport aux optima précédents.

   On enregistre les mouvements optimaux des niveaux 1 .. ( prof - 1 ) , suivi du mouvement optimal de niveau prof.
   Les premiers ont été enregistrés dans from_memo et le dernier en Memo_local[ prof - 1 ].

   ATTENTION : Suivant les plate-formes, la fonction rand ne retourne pas les mêmes values. Si tel était le cas,
   on pourra utiliser la fonction rant pour récupérer les valeurs qui ont conduit au fichier de sortie out_main. */

void enregistre_alea ( int reset , int prof , tmm to_memo[ PROF ][ PRISE ] , tmm from_memo[ PROF ][ PRISE ]  )
     {if ( reset )
         Memo_num = 1 ;
      else
         Memo_num++ ;
      if ( ( rand( ) % Memo_num ) == 0 )
         {memcpy( to_memo , from_memo ,  ( prof - 1 ) * PRISE * sizeof( tmm ));
          memcpy( to_memo[ prof - 1 ] , Memo_local[ prof - 1 ] , PRISE * sizeof( tmm ));
         }
     }

/* Cette fonction assure seulement qu'aucune mémoire aléatoire n'est considérée comme mouvement. En effet, la
   profondeur de parcours effective peut être inférieure à la profondeur prévue. C'est, par exemple, le cas
   lorsqu'un camp perd toutes ces pièces avant que la profondeur prévue ne soit atteinte. */

void annule_mouvements ( tmm m[ PROF ][ PRISE ] )
     {int i ;
      for ( i = 0 ; i < PROF ; i++ )
          m[ i ][ 0 ].tdepl = AUCUN_MOUV ;
     }

/* --Les--fonctions--de--mouvements--sur--l--échiquier--------------------------------------------------------------- */

/* joue_sequence joue les mouvements de profdeb jusqu'à proffin comprise. Bien-sür, profdeb >= proffin. */

void joue_sequence ( int profdeb , int proffin )
     {
       assert(profdeb >= proffin);
           int i;
           for(i = profdeb ; i >= proffin ; --i) {
               joue_mouvements(Memo[i-1]);
           }
     }

/* joue_mouvements déroule les mouvements consécutifs d'une pièce. Il n'y a aucun contrôle effectué, car on suppose
   ici que ce contrôle a été fait au préalable. */

void joue_mouvements ( tmm m[ PRISE ] )
     {int i ;
      for ( i = 0 ; m[ i ].tdepl != AUCUN_MOUV ; i++ )
          joue_mouv( m[ i ] ) ;
     }

void joue_mouv ( tmm m )
     {
       remplis_case(m.li, m.co, RIEN);
       remplis_case(m.liar, m.coar, m.piecear);
    if( m.tdepl == MOUV_AVEC_PRISE)
        remplis_case(m.lipr, m.copr, RIEN);

    if(Verbeux_mouvements)
        affiche(m.tdepl, m.li, m.co, m.piece, m.liar, m.coar, m.piece, m.lipr ,m.copr, m.piecepr, RIEN);

     }

/* --Les--fonctions--d--impression--de--l--échiquier----------------------------------------------------------------- */

/* L'impression de l'échiquier est faite à l'aide de print_echiquier */

void print_echiquier ( void ) // print toutes les lignes de l'échiquier une par une
     {
        int i;
        for(i=(N-1);i>=0;i--)
          print_echiquier_ligne(i);
          printf("    " );
        for(i=0;i<N;i++)
          printf("   %d    ",i);
        printf("\n" );
     }

int contenu_case(int lig, int col) // Retourne le contenu d'une case de l'echiquier
{
    return T[lig][col];
}

void print_contenu(int contenu) // Printf le contenu d'une case avec les lettres associées aux pions
{
    switch(contenu)
        {
            case(DameBL):
                printf("B");
                break;
            case(DameNO):
                printf("N");
                break;
            case(PionBL):
                printf("b");
                break;
            case(PionNO):
                printf("n");
                break;
            case(RIEN):
                printf(" ");
                break;
        }
}
/* print_echiquier_ligne ( <numéro> ) affiche une ligne de l'échiquier, comme par exemple :



*/
/* On print les lignes de haut en bas, càd à par exemple comme ça :
   |-----+-----+-----+-----+-----+-----+-----+-----+-----+-----|
   |     |     |     |     |     |     |     |     |     |     |
 2 |     |     |  b  |     |     |     |  b  |     |  b  |     |
   |     |     |     |     |     |     |     |     |     |     |

*/


void print_echiquier_ligne ( int li )
{
      int sous_ligne=0,contenu=0;
      int colonne=0,ligne=0;
      if(li==N-1) // Si on est à la ligne 0 on a besoin d'une ligne de bord en bas
        print_line('-','-');
      print_line('|',' ');
      print_echiquier_colonnes(li);
      print_line('|',' ');
      if(li != 0)
        print_line('+','-');
      else
        print_line('-','-');
}
/* print_echiquier_colonne ( <numéro> ) affiche une ligne physique, comme par exemple :

 2 |     |     |  b  |     |     |     |  b  |     |  b  |     |

*/
// le petit frère de print ligne, il n'affiche que la ligne avec le contenu
void print_echiquier_colonnes ( int li )
     {
        printf(" %d ",li);
        int colonne;
        for(colonne=0;colonne<N;colonne++)
        {
            printf("|   ");
            print_contenu(T[li][colonne]);
            printf("   ");
        }printf("|\n");
     }

/* print_line( '|' , ' ' ) ou print_line( '+' , '-' ) affichent, par exemple :

   |     |     |     |     |     |     |     |     |     |     |

ou

   |-----+-----+-----+-----+-----+-----+-----+-----+-----+-----|

*/

void print_line ( char separator_char , char fill_char )
{
     int i;
     printf("   |");
     for ( i = 0; i<N-1 ; i++)
            printf("%c%c%c%c%c%c%c%c",fill_char,fill_char,fill_char,fill_char,fill_char,fill_char,fill_char,separator_char);
     printf("%c%c%c%c%c%c%c|\n",fill_char,fill_char,fill_char,fill_char,fill_char,fill_char,fill_char);
}

/* --D--autres--fonctions--d--impression----------------------------------------------------------------------------- */

/* L'appel affiche( MOUV_AVEC_PRISE , 7 , 3 , PionBL , 9 , 5 , DameBL , 8 , 4 , DameNO , 2 ) donne, par exemple :

Le pion blanc ( 7 , 3 ) se déplace en ( 9 , 5 ) et devient dame avec prise de dame en ( 8 , 4 )

   On suppose que la procédure est appelée uniquement pour des profondeurs 1 et 2. Lorsque prof vaut 1 et que
   Verbeux_mouvements_indent est positionné, la ligne sera indentée de 3 caractères supplémentaires. */

void affiche ( int prise_ou_non , int li , int co , int piece , int liar , int coar , int piecear ,
               int lipr , int copr , int piecepr , int prof )
     {int i ;
      char * texte[ 5 ] = { "La dame noire" , "Le pion noir" , "" , "Le pion blanc" , "La dame blanche" } ;
      char * texte_bis[ 5 ] = { "dame" , "pion" , "" , "pion" , "dame" } ;
      if ( Verbeux_mouvements_indent && prof == 1 )
         for ( i = 0 ; i < 3 ; i++ )
             (void)printf( " " ) ;
      (void)printf( "%s ( %d , %d ) se déplace en ( %d , %d )" , texte[ piece + 2 ] , li , co , liar , coar ) ;
      if ( piece != piecear )
         (void)printf( " et devient dame" ) ;
      if ( prise_ou_non )
         (void)printf( " avec prise de %s en ( %d , %d )" , texte_bis[ piecepr + 2 ] , lipr , copr ) ;
      (void)printf( "\n" ) ;
     }

void print_les_mouvements ( int prof )
     {int i , indent = 0 ;
      for ( i = prof ; i > 0 ; i-- )
          {print_mouvement( i , indent ) ;
           indent += 3 ;
          }
     }

/* Cette fonction affiche un mouvement de la séquence optimale. Dans main_bouge_avec_prise, les deux derniers appels

      print_mouvement( 2 , 0 ) ;
      print_mouvement( 1 , 5 ) ;

donnent ceci, vers la fin du fichier out_main ;

Le mouvement du pion blanc va
    de ( 3 , 1 ) à ( 5 , 3 ) et prise de pion en ( 4 , 2 )
    de ( 5 , 3 ) à ( 7 , 5 ) et prise de pion en ( 6 , 4 )
    de ( 7 , 5 ) à ( 9 , 3 ) et prise de pion en ( 8 , 4 ) avec promotion en dame
    de ( 9 , 3 ) à ( 5 , 7 ) et prise de pion en ( 6 , 6 )


     Le mouvement du pion noir va
         de ( 6 , 8 ) à ( 4 , 6 ) et prise de dame en ( 5 , 7 )
         de ( 4 , 6 ) à ( 2 , 4 ) et prise de pion en ( 3 , 5 )
         de ( 2 , 4 ) à ( 0 , 2 ) et prise de pion en ( 1 , 3 ) avec promotion en dame
         de ( 0 , 2 ) à ( 2 , 0 ) et prise de pion en ( 1 , 1 )

*/

void print_mouvement ( int prof , int indent )
     {print_mouv( Memo[ prof - 1 ] , indent ) ;
     }

void print_mouv ( tmm m[ PRISE ] , int indent )
     {
        int i, prise = 0;

        for(i = 0; i < indent; i++)
            printf(" ");
        if (m[0].piece == PionBL)
          printf("Le mouvement du pion blanc va\n");
        else if (m[0].piece == PionNO)
          printf("Le mouvement du pion noir va\n");
        else if (m[0].piece == DameBL)
          printf("Le mouvement de la dame blanche va\n");
        else if (m[0].piece == DameNO)
          printf("Le mouvement de la dame noire va\n");

        while(prise < PRISE && m[prise].piece != RIEN) // on sort si on a plus de mouvement avec la piece ou si fin du tableau
        {
          for(i = 0; i < indent; i++)
              printf(" ");
          printf("    ");
          printf("de ( %d , %d ) à ( %d , %d )", m[prise].li, m[prise].co, m[prise].liar, m[prise].coar);

          if (abs(m[prise].piecepr) == 1)
              printf(" et prise de pion en ( %d , %d )", m[prise].lipr, m[prise].copr);
          else if(abs(m[prise].piecepr) == 2)
              printf(" et prise de dame en ( %d , %d )", m[prise].lipr, m[prise].copr);

          if (m[prise].piece != m[prise].piecear)
              printf(" avec promotion dame");
          printf("\n");
          prise++;
      }
    }

/* --Les--fonctions--pour--tester--les--prises----------------------------------------------------------------------- */

/* On teste si une prise AVANT est possible. Pour les pions, on est limité aux options AVANT-GAUCHE et AVANT-DROITE.
   Une dame peut se diriger sur les 4 axes AVANT-GAUCHE, AVANT-DROITE, ARRIERE-GAUCHE ou ARRIERE-DROITE. Ceci est
   analysée dans prise_possible_toutes. */

int prise_possible_avant ( int coul )
    {
      int ligne=0,colonne=0;
      for(ligne=0;ligne<=(N-1);ligne++)
        for(colonne=0;colonne<=(N-1);colonne++)
            if(est_pion(ligne,colonne,coul))
              if(prise_possible_case(ligne,colonne,coul,AVANT,DROITE)==1 || prise_possible_case(ligne,colonne,coul,AVANT,GAUCHE)==1)
                return 1;
            else if(est_dame(ligne,colonne,coul))
              if((prise_possible_case(ligne,colonne,coul,AVANT,DROITE)!=0) || (prise_possible_case(ligne,colonne,coul,AVANT,GAUCHE)!=0))
                  return 1;
              else if(prise_possible_case(ligne,colonne,coul,ARRIERE,DROITE)!=0 || prise_possible_case(ligne,colonne,coul,ARRIERE,GAUCHE)!=0)
                return 1;
      return 0;
    }

/* On teste toutes les possibités de prise à partir du point de départ. */

  int prise_possible_toutes ( int li , int co , int coul )
  {
    int prise_possible = prise_possible_case(li, co, coul, AVANT, GAUCHE);
    if( prise_possible == NON )
        prise_possible = prise_possible_case(li, co, coul, AVANT, DROITE);
    if( prise_possible == NON )
        prise_possible = prise_possible_case(li, co, coul, ARRIERE, GAUCHE);
    if( prise_possible == NON )
        prise_possible = prise_possible_case(li, co, coul, ARRIERE, DROITE);
    return (prise_possible != 0);
  }

/* prise_possible_case reçoit les coordonnées li et co d'une pièce de la couleur coul. On peut avoir affaire à une
   dame ou un pion. sens peut valoir AVANT ou ARRIERE et direct GAUCHE ou DROITE. Cette fonction est appelée par les
   deux prédicats prise_possible_avant et prise_possible_toutes.

   Pour une pièce blanche AVANT concernera li + 1, GAUCHE concernera co - 1, etc. Pour une pièce noire, qui est
   tournée dans l'autre sens, AVANT donnera li - 1, GAUCHE donnera co + 1, etc. Comme BLANC est codé par 1 et NOIR
   par -1, les produits coul * sens et coul * direct donnent la bonne valeur dans les deux cas.

   Un pion peut prendre une pièce qui est sa voisine directe si la case terrière cette pièce est vide. Une dame
   peut prendre la première pièce sur sa diagonale, pour peu que la pièce derrière celle-ci soit vide.

   La fonction rend un booléen qui dit si, oui ou non, une prise est possible dans la direction indiquée. De plus,
   plutôt que de répondre simplement VRAI, elle donne la distance de la pièce prise. Ainsi, un pion prendra toujours
   à distance 1, mais une dame pourra prendre des pièces à d'autres distances. */

int prise_possible_case ( int li , int co , int coul , int sens , int direct )
{
  int piece=contenu_case(li,co);
    int i=0,compteur=0;
      if(piece*coul==1)
      {
        if(contenu_case((li+sens*coul),(co+coul*direct))*(-coul)==1 || contenu_case((li+sens*coul),(co+coul*direct))*(-coul)==2)
            if(contenu_case((li+2*sens*coul),(co+2*coul*direct))==RIEN)
               return compteur+1; // On peut prendre à 1 de distance
        return 0;
      }
      else// DameBL
      { // changer li<(N-2)
        if(li>1 && li <(N-2) && co>1 && co<(N-2)) // Si on est dans La partie où on peut prendre
          {
            while(contenu_case((li+sens*coul),(co+coul*direct))==RIEN && li>1 && li<(N-2) && co>1 && co<(N-2)) // continue jusqu'à être au bord de la zone de prise
            {
              compteur=compteur+1;
              li=li+coul*sens;
              co=co+coul*direct;
            }// Stop si on trouve un pion
            li=li+coul*sens;
            co=co+coul*direct;
            compteur=compteur+1;
            if(contenu_case(li,co)*(-coul)==1 || contenu_case(li,co)*(-coul)==2)
              {
                if(contenu_case((li+sens*coul),(co+coul*direct))==RIEN && (li<=(N-1) && li>=0 && co<=(N-1) && co>=0))   // vérifie la dernière case derrière le pion
                  return compteur;
              }
          }
        return 0;
      }
}

/* --Le--test--de--cases--vides-------------------------------------------------------------------------------------- */

/* cases_vides est un prédicat qui dit si, oui ou non, toutes les cases jusqu'à la distance num sont vides ou non ?
   Ainsi, cases_vides( 5 , 5 , 3 , BLANC , AVANT , DROITE ) est vrai seulement si les cases ( 6 , 6 ) , ( 7 , 7 )
   et ( 8 , 8 ) sont vides.

   Cette fonction est utile pour détecter tous les mouvements d'une dame dans le cas où il n'y a pas de prise. */

int cases_vides ( int li , int co , int num , int coul , int sens , int direct )
    {
      int nb_cases_vides = 0, case_vide = OUI, cpt_cases = 1;
      if( (li + num*coul*sens >= N || li + num*coul*sens < 0) || ( co + num*coul*direct >= N || co + num*coul*direct < 0 ))
          case_vide = NON;
      else
          while( case_vide && nb_cases_vides < num) {
              if( T[li + cpt_cases*coul*sens][co + cpt_cases*coul*direct] == RIEN )
                  nb_cases_vides++;
              else
                  case_vide = NON;
              cpt_cases++;
          }
      return case_vide;
    }

/* --La--boucle--de--jeu--principale--------------------------------------------------------------------------------- */

/* itere_jeu déroule le jeu. */

void itere_jeu ( int coul_ordinateur , int prof )
     {int minmax , cont = OUI , couleur = BLANC , depart , arrivee , ok , num_mouv , prise_ou_pas ;
      int suite , lisuite , cosuite ;
      tmm depl[ PRISE ] ;
      char * texte[ 3 ] = { "NOIRS" , "" , "BLANCS" } ;
      remplis_echiquier( ) ;
      if ( coul_ordinateur == NOIR )
         print_echiquier( ) ;
      while ( cont )
            {if ( couleur != coul_ordinateur && ! est_coince( couleur) )
                {num_mouv = 0 ;
                 suite = NON ;
                 prise_ou_pas = prise_possible_avant( couleur ) ;
                 if ( prise_ou_pas )
                    (void)printf( "Vous devez prendre une pièce !\n" ) ;
                 do {(void)printf( "%s : Entrée sous forme 'ab cd' pour ( a , b ) vers ( c , d ) ou -1 -1 (info) :\n" ,
                                    texte[ couleur + 1 ] ) ;
                     scanf( " %d %d" , & depart , & arrivee ) ;
                     if ( depart == - 1 )
                        {(void)printf( "J'ai calculé la séquence suivante \n\n" ) ;
                         print_les_mouvements( prof - 1 ) ;
                         ok = NON ;
                        }
                     else
                        ok = analyse_mouvement( depart , arrivee , prise_ou_pas , couleur , & num_mouv ,
                                                depl , & suite , & lisuite , & cosuite ) ;
                    }
                 while ( ! ok ) ;
                 print_mouv( depl , 0 ) ;
                }
             couleur *= - 1 ;
             if ( couleur != coul_ordinateur && ! est_coince( couleur ) )
                {(void)printf( "Je calcule . . . \n" ) ;
                 minmax = minimax_call( prof , coul_ordinateur ) ;
                 (void)printf( "Je joue . . . \n\n" ) ;
                 print_mouvement( prof , 0 ) ;
                 joue_sequence( prof , prof ) ;
                }
             print_echiquier( ) ;
             cont = suite_ou_pas( ) ;
            }
     }

/* On détente si la partie est terminée ou non ? */

int suite_ou_pas ( void )
    {int numBL = compte_pieces( BLANC ) , numNO = compte_pieces( NOIR ) ;
     if ( numBL == 0 || est_coince( BLANC ) )
        (void)printf( "Les blancs perdent !\n\n" ) ;
     if ( numNO == 0 || est_coince( NOIR ) )
        (void)printf( "Les noirs perdent !\n\n" ) ;
     return( numBL && numNO && ! est_coince( BLANC ) && ! est_coince( NOIR ) ) ;
    }

/* Le mouvement proposé et analysé et rejeté s'il le faut. S'il s'agir d'une prise, il se peut que le processus
   doive être relancé. dep et arr sont traduits en des coordonnées qui doivent appartenir à la grille et se situer
   sur une même diagonale. Le point de départ doit comporter une pièce et le point d'arrivée doit être libre. Dès
   qu'il y a un problème, on rend NON. Eventuellement, il y a promotion de pion vers dame.

   Lorsqu'une pièce a passé tous les tests et qu'il n'y a pas de prise, on enregistre le mouvement à l'aide de
   mémo et on l'effectue avec joue_mouv, cavant de rendre OUI. Lorsqu'il y a prise, on doit éventuellement
   continuer les mouvements. Ceci est déterminé à l'aide de prise_possible_toutes. Dans l'affirmative, on rend
   NON afin de relancer le processus. Les variables suite, lisuite et cosuite passées en référence sont
   positionnées comme nécessaire. */

int analyse_mouvement ( int dep , int arr , int prise_ou_pas , int couleur , int * num_mouv ,
                        tmm depl[ PRISE ] , int * suite , int * lisuite , int * cosuite )
    {
      int mouvement_autoriser = OUI;

      int depLi = dep / 10;
      int depCo = dep % 10;
      int arrLi = arr / 10;
      int arrCo = arr % 10;

     if ( (arrLi - depLi != arrCo - depCo) && (arrLi - depLi != -(arrCo - depCo)) )// les coordonnées sont sur des diagonales différentes
         mouvement_autoriser = NON;

     if(mouvement_autoriser && T[depLi][depCo] == RIEN)
         mouvement_autoriser = NON;

     if(mouvement_autoriser && T[arrLi][arrCo] != RIEN)
         mouvement_autoriser = NON;

     int pieceArr = T[depLi][depCo];
     if(mouvement_autoriser && T[depLi][depCo]*couleur == 1 && arrLi == (couleur == BLANC ? N-1 : 0) )
         pieceArr = (couleur == BLANC ? DameBL : DameNO);

     if(mouvement_autoriser) {
         if (! prise_ou_pas) {
             memo ( depl , MOUV_SANS_PRISE , *num_mouv , depLi , depCo , T[depLi][depCo] , arrLi , arrCo , pieceArr , RIEN , RIEN, RIEN ) ;
             joue_mouv(depl[0]);
         }
         else {
             int newPrise = prise_possible_toutes(arrLi, arrCo, couleur);
             if(newPrise != 0) {
                 *suite = OUI;
                 *lisuite = arrLi+newPrise;
                 *cosuite = arrCo+newPrise;
                 mouvement_autoriser = NON;
                 *num_mouv++;
             }
             int liPr = arrLi-1*(depLi < arrLi ? 1 : -1);
             int coPr = arrCo-1*(depCo < arrCo ? 1 : -1);
             memo ( depl , MOUV_AVEC_PRISE , *num_mouv , depLi , depCo , T[depLi][depCo] , arrLi , arrCo , pieceArr , liPr , coPr, T[liPr][coPr]) ;
             joue_mouv(depl[*num_mouv]);
         }
     }
     return mouvement_autoriser;
    }

/* Un camp a perdu s'il ne peut plus bouger aucune pièce. */

int est_coince ( int couleur )
    {int li , co , mouv = NON ;
     if ( prise_possible_avant ( couleur ) )
        return( NON ) ;
     else
        {for ( li = 0 ; li < N && ! mouv ; li++ )
             for ( co = 0 ; co < N && ! mouv ; co++ )
                 {if ( li + couleur >= 0 && li + couleur < N )
                     mouv = ( co + 1 < N && T[ li + couleur ][ co + 1 ] == 0 ) ||
                            ( co - 1 >= 0 && T[ li + couleur ][ co - 1 ] == 0 ) ;
                  if ( est_dame( li , co , couleur ) && li - couleur >= 0 && li - couleur < N )
                     mouv = mouv || ( co + 1 < N && T[ li - couleur ][ co + 1 ] == 0 ) ||
                                    ( co - 1 >= 0 && T[ li - couleur ][ co - 1 ] == 0 ) ;
                 }
         return( ! mouv ) ;
        }
    }

/* --La--fonction--minimax--et--ses--acolytes------------------------------------------------------------------------ */

/* Un appel comme minimax( 5 , BLANC ) lance une recherche de profondeur 5 en commençant par les blancs. La
   fonction pass le contrôle à minimax en lui passant en particulier la variable globale Memo dans laquelle la
   séquence optimale de mouvements sera mémorisée. */

int minimax_call ( int prof , int coul )
    {assert( prof <= PROF ) ;
     annule_mouvements( Memo ) ;
     return( minimax( compte_pieces( BLANC ) , compte_pieces( NOIR ) , prof , coul , Memo ) ) ;
    }

/* Cette fonction est triviale. numBL et numNO sont le nombre de pièces blanches, respectivement noires, avec un bonus
   pour les dames. prof est la profondeur courante et coul la couleur considérée. tmm m[ PROF ][ PRISE ] est la
   structure de mémorisation qui est simplement passée aux autres fonctions. On retrouve la plupart de ces
   paramètres dans les fonctions qui suivent, avec bien-sûr la même signification.

   La fonction rend la différence entre les pions blancs et les noirs. Une valeur positive correspond donc à un
   avantage pour les blancs. Dans le cas contraire, l'avantage est aux noirs. La recherche est arrêtée dès que la
   profondeur est nulle ou que l'un des camps n'a pas plus pièces. Dans le cas contraire, on passe le contrôle à
   la fonction cherche_depl en lui indiquant la couleur et en fournissant, soit le neutre (artificiel) du max, soit
   celui du min. */

int minimax ( int numBL , int numNO , int prof , int coul , tmm m[ PROF ][ PRISE ] )
    {if ( ! ( numBL && numNO && prof ) )
        {if ( Verbeux_minimax )
            (void)printf( "Minimax retourne %d\n" , numBL - numNO ) ;
         return( numBL - numNO ) ;
        }
     else
        if ( coul == BLANC )
           return( cherche_depl( numBL , numNO , coul , prof , NeutreMAX , m ) ) ;
        else
           return( cherche_depl( numBL , numNO , coul , prof , NeutreMIN , m ) ) ;
    }

/* relance_minimax sert à relancer minimax via l'appel minimax( numBL , numNO , prof - 1 , - coul , new_memo ) ;
   C'est la mémorisation de la séquence de mouvements qui complique la situation. Le paramètre m reçu donne
   l'endroit où on est sensé déposer la meilleure solution.

   minimax est donc relancée avec un nouveau tableau de mémorisation new_memo. Au retour de minimax, on compare
   le résultat à l'ancien meilleur résultat, c'est-à-dire minmax. Pour les blancs, la solution est meilleure si
   resu >= minmax et pour les noirs lorsque resu <= minmax.

   Si tel est le cas, on recopie la solution new_memo remplie par minimax dans le tableau m qui a été fourni. Ceci
   se fait à l'aide de enregistre_alea. */

int relance_minimax ( int numBL , int numNO , int coul , int prof , int minmax , tmm m[ PROF ][ PRISE ] )
    {int resu ;
     tmm new_memo[ PROF ][ PRISE ] ;
     annule_mouvements( new_memo ) ;
     resu = minimax( numBL , numNO , prof - 1 , - coul , new_memo ) ;
     if ( coul == BLANC )
        {if ( resu >= minmax && resu != NeutreMAX )
            enregistre_alea( ( resu > minmax ) , prof , m , new_memo ) ;
         if ( resu > minmax )
            minmax = resu ;
        }
     else
        {if ( resu <= minmax && resu != NeutreMIN )
            enregistre_alea( ( resu < minmax ) , prof , m , new_memo ) ;
         if ( resu < minmax )
            minmax = resu ;
        }
     return( minmax ) ;
    }

/* --La--recherche--des--deplacements--avec--prise------------------------------------------------------------------- */

/* cherche_depl est essentiellement une fonction qui dispatcher le contrôle en appelant cherche_depl_avec_prise,
   cherche_depl_pion_sans_prise ou cherche_depl_dame_sans_prise. Si une prise est possible (le prédicat
   prise_possible_avant) elle appelle cherche_depl_avec_prise pour chaque pièce de la couleur coul sur l'échiquier.

   Si aucune prise n'est possible, elle appelle pour chaque pièce de la bonne couleur, soit la fonction
   cherche_depl_pion_sans_prise, soit la fonction cherche_depl_dame_sans_prise, suivant le type de pièce.

   cherche_depl passe la valeur minmax aux fonctions appelées et récupère un résultat qui devient la nouvelle
   valeur de minmax. De la sorte, minmax contient toujours la valeur de la meilleure solution connue. A la
   fin de son exécution, elle rend bien-sûr cette valeur. m est juste passée aux fonctions appelées. */

int cherche_depl ( int numBL , int numNO , int coul , int prof , int minmax , tmm m[ PROF ][ PRISE ] )
    {
      int li, col, piece;
      int num_mouv = 0;
      int seulement_avant;
      if( prise_possible_avant(coul) )
        for(li = 0; li < N; ++li)
          for(col = 0; col < N; ++col)
            if(est_piece(li, col, coul)) {
             piece = T[li][col];
             if(est_pion(li, col, coul))
                 seulement_avant = OUI;
             else
                 seulement_avant = NON;
             minmax = cherche_depl_avec_prise(li ,col ,piece ,coul , numBL, numNO, prof, minmax ,num_mouv ,seulement_avant ,m);
           }
       else // deplacement sans prise
        for(li = 0; li < N; ++li)
          for(col = 0; col < N; ++col)
            if(est_piece(li, col, coul)) {
                if(est_pion(li, col, coul))
                  minmax = cherche_depl_pion_sans_prise(li, col, coul ,numBL , numNO, prof, minmax , m);
                else    // c'est une dame
                  minmax = cherche_depl_dame_sans_prise(li, col, coul ,numBL , numNO, prof, minmax , m);
            }
       return minmax;
    }

/* cherche_depl_avec_prise considère la pièce située en li et co et de couleur coul. Elle cherche les mouvements qui
   sont possibles avec prise. Si seulement_avant est vraie, elle cherche seulement les mouvements en avant pour les
   pions. Ceci est le cas pour la première prise effectuée par un pion. Sinon, elle cherche dans les 4 directions.

   Si elle trouve une prise possible, elle passe le contrôle à effectue_depl_avec_prise qui réalise la prise et
   poursuit les calculs. La gestion de minmax et m est la même que pour cherche_depl. */

int cherche_depl_avec_prise ( int li , int co , int piece , int coul , int numBL , int numNO , int prof ,
                              int minmax , int num_mouv , int seulement_avant , tmm m[ PROF ][ PRISE ] )
    {
      if(seulement_avant && piece*coul == 1)// si c'est le premier mouvement d'un pion on ne peut prendre que vers l'avant.
      {
        // Deplacement vers AVANT GAUCHE
        if(prise_possible_case(li, co, coul, AVANT, GAUCHE) == 1)
           minmax = effectue_depl_avec_prise(li, co, piece, coul, li+AVANT*coul, co+GAUCHE*coul , li+AVANT*coul*2,
                                             co+GAUCHE*coul*2 , numBL, numNO, prof, minmax, num_mouv, m);
       // Deplacement vers AVANT DROITE
       if(prise_possible_case(li, co, coul, AVANT, DROITE) == 1)
           minmax = effectue_depl_avec_prise(li, co, piece, coul, li+AVANT*coul, co+DROITE*coul , li+AVANT*coul*2,
                                             co+DROITE*coul*2 , numBL, numNO, prof, minmax, num_mouv, m);
   }
   else {
       int distanceP;
       // Deplacement vers AVANT GAUCHE
       distanceP = prise_possible_case(li, co, coul, AVANT, GAUCHE);
       if(distanceP > 0 && ( (piece*coul==2) || (distanceP == 1) ))   // Si c'est une dame ou si c'est un pion qui à une prise à distance de 1
           minmax = effectue_depl_avec_prise(li, co, piece, coul, li+AVANT*coul*distanceP, co+GAUCHE*coul*distanceP , li+AVANT*coul*(distanceP+1),
                                             co+GAUCHE*coul*(distanceP+1) , numBL, numNO, prof, minmax, num_mouv, m);
       // Deplacement AVANT DROITE
       distanceP = prise_possible_case(li, co, coul, AVANT, DROITE);
       if(distanceP > 0 && ( (piece*coul==2) || (distanceP == 1) ))
           minmax = effectue_depl_avec_prise(li, co, piece, coul, li+AVANT*coul*distanceP, co+DROITE*coul*distanceP , li+AVANT*coul*(distanceP+1),
                                             co+DROITE*coul*(distanceP+1) , numBL, numNO, prof, minmax, num_mouv, m);
       // Deplacement vers ARRIERE GAUCHE
       distanceP = prise_possible_case(li, co, coul, ARRIERE, GAUCHE);
       if(distanceP > 0 && ( (piece*coul==2) || (distanceP == 1) ))
           minmax = effectue_depl_avec_prise(li, co, piece, coul, li+ARRIERE*coul*distanceP, co+GAUCHE*coul*distanceP , li+ARRIERE*coul*(distanceP+1),
                                             co+GAUCHE*coul*(distanceP+1) , numBL, numNO, prof, minmax, num_mouv, m);
       // Deplacement vers ARRIERE DROITE
       distanceP = prise_possible_case(li, co, coul, ARRIERE, DROITE);
       if(distanceP > 0 && ( (piece*coul==2) || (distanceP == 1) ))
           minmax = effectue_depl_avec_prise(li, co, piece, coul, li+ARRIERE*coul*distanceP, co+DROITE*coul*distanceP , li+ARRIERE*coul*(distanceP+1),
                                             co+DROITE*coul*(distanceP+1) , numBL, numNO, prof, minmax, num_mouv, m);
   }

   return minmax;
    }

/* La fonction suivante considère une pièce de couleur coul en ( li , co ) qui va prendre une pièce adverse en
   ( lipr , copr ) pour atterrir en ( lira , coar ). En un premier lieu il s'agit de recalculer les numXY. Le
   camp adverse se voit diminué de 1 ou 1 + BonusDame, suivant le cas. Le camp coul voit éventuellement une
   promotion de pion vers dame.

   Ensuite, il s'agit de réaliser le mouvements à l'aide de remplis_case. On prendra soin de mémoriser toute
   information sur la situation antérieure afin de pouvoir la rétablir. Si Verbeux_mouvements est positonné et
   que la profondeur n'excède pas 2, on appelle ensuite affiche. Dans tous les cas, on appelle memo_local afin
   de mémoriser le mouvement.

   A partir de ( liar , coar ), il se peut qu'une nouvelle prise soit possible, et donc obligatoire. Si tel est
   le cas, on rappelle cherche_depl_avec_prise en prenant soin d'augmenter d'unité num_mouv et d'indiquer que
   dorénavant toutes les directions sont permises pour les poins en positionnant seulement_avant à faux. Par
   contre, si aucune nouvelle prise n'est possible, on passe la main à relance_minimax.

   Finalement, on rétablit le damier initial et on rend la valeur minmax la plus récente. */

int effectue_depl_avec_prise ( int li , int co , int piece , int coul , int lipr , int copr , int liar , int coar ,
                               int numBL , int numNO , int prof , int minmax , int num_mouv , tmm m[ PROF ][ PRISE ] )
    {
      int ancPiece = piece; // sauvegarde l'ancienne valeur
      int piecepr = T[lipr][copr];
      remplis_case(li,co,RIEN);
      remplis_case(lipr,copr,RIEN);

      if( piece*coul == 1 && liar == (coul == BLANC ? N-1 : 0) )
          piece = (coul == BLANC ? DameBL : DameNO);
      remplis_case (liar, coar ,piece);
      numBL = compte_pieces(BLANC);
      numNO = compte_pieces(NOIR);
      memo_local(MOUV_AVEC_PRISE , prof , num_mouv, li, co, ancPiece, liar, coar, piece, lipr, copr, piecepr);
      if(Verbeux_mouvements && prof < 2)
          affiche(MOUV_AVEC_PRISE, li, co, ancPiece, liar, coar, piece, lipr ,copr, piecepr, prof);
      minmax = cherche_depl_avec_prise(liar, coar, piece, coul, numBL, numNO, prof, minmax, num_mouv+1 , NON , m);
      minmax = relance_minimax(numBL ,numNO ,coul ,prof ,minmax ,m);
      remplis_case (li, co ,ancPiece);
      remplis_case(lipr,copr,piecepr);
      remplis_case(liar,coar, RIEN);
      return minmax;
    }

/* --La--recherche--des--deplacements--sans--prise------------------------------------------------------------------- */

/* cherche_depl_pion_sans_prise part d'un pion de couleur coul en ( li , co ) et détermine à l'aide de cases_vides
   si la case AVANT-GAUCHE ou AVANT-DROITE est vide. Si tel est le cas, elle appelle effectue_depl_sans_prise avec
   les paramètres convenables. La gestion de minmax et m sont dorénavant standard. */

int cherche_depl_pion_sans_prise ( int li , int co , int coul , int numBL ,
                                   int numNO , int prof , int minmax , tmm m[ PROF ][ PRISE ] )
    {
      int piece = T[li][co];
      if(cases_vides(li, co, 1, coul, AVANT, DROITE))
          minmax = effectue_depl_sans_prise( li, co, piece, coul, li+AVANT*coul, co+DROITE*coul, numBL, numNO, prof, minmax , m);
      if(cases_vides(li, co, 1, coul, AVANT, GAUCHE))
          minmax = effectue_depl_sans_prise( li, co, piece, coul, li+AVANT*coul, co+GAUCHE*coul, numBL, numNO, prof, minmax , m);
      return minmax;
    }

/* cherche_depl_dame_sans_prise part d'une dame de couleur coul en ( li , co ) et détermine à l'aide de cases_vides
   toutes les cases atteingables sans prise. Ppour chacune d'elles, elle appelle effectue_depl_sans_prise avec
   les paramètres convenables. La gestion de minmax et m reste la même. */

int cherche_depl_dame_sans_prise ( int li , int co , int coul , int numBL ,
                                   int numNO , int prof , int minmax , tmm m[ PROF ][ PRISE ] )
    {
      int dist = 0;
      int piece = T[li][co];
      while(cases_vides(li ,co , dist+1, coul, AVANT , GAUCHE)) {
          dist++;
          if(dist > 0)
              minmax = effectue_depl_sans_prise(li, co, piece, coul, li+AVANT*coul*dist, co+GAUCHE*coul*dist, numBL, numNO, prof, minmax, m);
      }
      dist = 0;
      while(cases_vides(li ,co , dist+1, coul, AVANT , DROITE)) {
          dist++;
          if(dist > 0)
              minmax = effectue_depl_sans_prise(li, co, piece, coul, li+AVANT*coul*dist, co+DROITE*coul*dist, numBL, numNO, prof, minmax, m);
      }
      dist = 0;
      while(cases_vides(li ,co , dist+1, coul, ARRIERE , GAUCHE)) {
          dist++;
          if(dist > 0)
              minmax = effectue_depl_sans_prise(li, co, piece, coul, li+ARRIERE*coul*dist, co+GAUCHE*coul*dist, numBL, numNO, prof, minmax,m);
      }
      dist = 0;
      while(cases_vides(li ,co , dist+1, coul, ARRIERE , DROITE)) {
          dist++;
          if(dist > 0)
              minmax = effectue_depl_sans_prise(li, co, piece, coul, li+ARRIERE*coul*dist, co+DROITE*coul*dist, numBL, numNO, prof, minmax, m);
      }
      return minmax;
    }

/* effectue_depl_sans_prise considère une pièce de couleur coul en ( li , co ) qui se déplace vers ( liar , coar )
   sans prise. La pièce peut être promue en dame et il faut en tenir compte. Ensuite, des appels à remplis_case
   établissent le nouveau damier et on rappelle relance_minimax. A l'issue de cet appel, on rétablit l'ancien damier
   avant de retourner la valeur convenable pour minmax. */

int effectue_depl_sans_prise ( int li , int co , int piece , int coul , int liar , int coar ,
                               int numBL , int numNO , int prof , int minmax , tmm m[ PROF ][ PRISE ] )
    {
      int changement = NON;
      int oldPiece = piece;
      T[li][co] = RIEN;
      if( piece*coul == 1 && liar == (coul == BLANC ? N-1 : 0) ) {
          piece = (coul == BLANC ? DameBL : DameNO);
          changement = OUI;
      }
      T[liar][coar] = piece;
      if(changement) {
          if(coul == BLANC)
              numBL = compte_pieces(coul);
          else
              numNO = compte_pieces(coul);
      }
      memo_local(MOUV_SANS_PRISE, prof , 0, li, co, oldPiece, liar, coar, piece, RIEN, RIEN, RIEN); // QUE FAUT IL METTRE DANS LIPR ET COPR QUAND PAS DE PRISE!!!!!
      if(Verbeux_mouvements && prof < 2)
          affiche(MOUV_SANS_PRISE, li, co, oldPiece, liar, coar, piece, RIEN ,RIEN, RIEN, prof);
      minmax = relance_minimax(numBL ,numNO ,coul ,prof ,minmax ,m);
      remplis_case (li, co ,oldPiece);
      T[liar][coar] = RIEN;
      return minmax;
    }

/* --C--est--tout---------------------------------------------------------------------------------------------------- */
