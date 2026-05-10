#ifndef ESPECES_H
#define ESPECES_H

/* REF: Examen_2026_UL1IN002_Prediction.pdf — p.1 */
#define NB_ESPECES 8

/* Structure d'un element du registre des especes observees.
 * REF: Examen_2026_UL1IN002_Prediction.pdf — p.2 (Section 2) */
typedef struct _tyEspece tyEspece;
struct _tyEspece {
    int       code;   /* code de l'espece (1..NB_ESPECES) */
    int       nbObs;  /* nombre d'observations */
    tyEspece *suiv;   /* pointeur vers le suivant          */
};

/* --- Section 1 : Tableaux --------------------------------------- */

/* REF: Q1 (p.1) */
int  EstValide(int code);
int  CompterOccurrences(int *releve, int n, int code);

/* REF: Q2 (p.1) */
void AfficherReleve(int *releve, int n);

/* REF: Q3 (p.1-2) */
int *ReleveAleatoire(int n);

/* REF: Q4 (p.2) */
int  AnalyserReleve(int *releve, int n, int *pNbDistinct, int *pCodeMax);

/* --- Section 2 : Listes ---------------------------------------- */

/* REF: Q5 (p.2) */
tyEspece *InsererTrie(tyEspece *registre, int code);

/* REF: Q6 (p.2) */
int       NbEspeces(tyEspece *registre);

/* REF: Q7 (p.2) */
tyEspece *DetruireRegistre(tyEspece *registre);

/* REF: Q8 (p.2) */
void      AfficherRegistre(tyEspece *registre, int nbTotal);

/* REF: Q11 (p.3) */
tyEspece *FiltrerRares(tyEspece *registre, int seuilMin);

#endif /* ESPECES_H */
