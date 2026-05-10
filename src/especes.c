#include "especes.h"
#include <stdlib.h>
#include <stdio.h>

/* ===================================================================
 * REF: Examen_2026_UL1IN002_Prediction.pdf
 *
 * Chaque fonction contient un commentaire TODO indiquant la question.
 * Implementez-les une par une : les tests correspondants passeront
 * au fur et a mesure.
 * =================================================================== */

/* --- Q1 (p.1) ---------------------------------------------------- */

int EstValide(int code) {
    /* TODO: retourner 1 si 1 <= code <= NB_ESPECES, 0 sinon */
    (void)code;
    return 0;
}

int CompterOccurrences(int *releve, int n, int code) {
    /* TODO: retourner le nombre de fois que code apparait dans releve[0..n-1] */
    (void)releve; (void)n; (void)code;
    return 0;
}

/* --- Q2 (p.1) ---------------------------------------------------- */

void AfficherReleve(int *releve, int n) {
    /* TODO: afficher les n codes sur une seule ligne, separes par
     *       des espaces, avec un '\n' final */
    (void)releve; (void)n;
}

/* --- Q3 (p.1-2) -------------------------------------------------- */

int *ReleveAleatoire(int n) {
    /* TODO: allouer un tableau de n entiers, le remplir avec des codes
     *       valides tires aleatoirement (rand() % NB_ESPECES + 1),
     *       retourner l'adresse du tableau */
    (void)n;
    return NULL;
}

/* --- Q4 (p.2) ---------------------------------------------------- */

int AnalyserReleve(int *releve, int n, int *pNbDistinct, int *pCodeMax) {
    /* TODO: calculer *pNbDistinct (especes distinctes) et *pCodeMax
     *       (code le plus frequent, plus petit code en cas d'egalite),
     *       retourner n (nombre total d'observations) */
    (void)releve; (void)n;
    *pNbDistinct = 0;
    *pCodeMax    = 0;
    return 0;
}

/* --- Q5 (p.2) ---------------------------------------------------- */

tyEspece *InsererTrie(tyEspece *registre, int code) {
    /* TODO: si code est deja dans la liste, incrementer nbObs.
     *       Sinon allouer un nouveau maillon (nbObs=1) et l'inserer
     *       a la bonne position pour conserver l'ordre croissant.
     *       Retourner la tete de liste. */
    (void)code;
    return registre;
}

/* --- Q6 (p.2) ---------------------------------------------------- */

int NbEspeces(tyEspece *registre) {
    /* TODO: retourner le nombre d'elements dans la liste */
    (void)registre;
    return 0;
}

/* --- Q7 (p.2) ---------------------------------------------------- */

tyEspece *DetruireRegistre(tyEspece *registre) {
    /* TODO: liberer toute la memoire allouee pour la liste,
     *       retourner NULL */
    (void)registre;
    return NULL;
}

/* --- Q8 (p.2-3) -------------------------------------------------- */

void AfficherRegistre(tyEspece *registre, int nbTotal) {
    /* TODO: afficher le registre au format :
     *   Registre des especes observees (N observations) :
     *   espece X : Y observations (ZZ.ZZ%)
     *   ...
     *   Nombre d'especes distinctes : K
     * Appeler NbEspeces() pour obtenir K. */
    (void)registre; (void)nbTotal;
}

/* --- Q11 (p.3) --------------------------------------------------- */

tyEspece *FiltrerRares(tyEspece *registre, int seuilMin) {
    /* TODO: supprimer de la liste tous les maillons dont nbObs < seuilMin.
     *       Liberer la memoire des maillons supprimes.
     *       Retourner la tete de liste apres filtrage. */
    (void)seuilMin;
    return registre;
}
