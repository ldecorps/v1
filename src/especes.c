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

int EstValide(int code){
    if((code >= 1) && (code <= NB_ESPECES)) return 1;
    return 0;
}

int CompterOccurrences(int *releve, int n, int code) {
    int c = 0;
    for(int i = 0; i < n; i++){
        if( *(releve + i) == code){
            c ++ ;
        }
    }
    return c;
}

/* --- Q2 (p.1) ---------------------------------------------------- */

void AfficherReleve(int *releve, int n) {
    for (int i = 0; i < n; i++){
        printf("%d ", *(releve + i));
    }
    printf("\n");
}

/* --- Q3 (p.1-2) -------------------------------------------------- */

int *ReleveAleatoire(int n) {
    int *tab = malloc(sizeof(int) * n );
    for (int i = 0; i < n; i++){
        int esp = rand() % NB_ESPECES + 1;
        *(tab + i) = esp;
        }
    return tab;
}

/* --- Q4 (p.2) ---------------------------------------------------- */

int AnalyserReleve(int *releve, int n, int *pNbDistinct, int *pCodeMax) {
    int freq[8] = {0};
    int index = 1;
    for(index; index <= 8; index ++){
        for(int i = 0; i < n; i++){
            if (releve[i] == index) freq[index - 1]++;
        }
        
    } //donc notre tableau de frequences est remplie
    //continuons pour trouver NbDist
    *pNbDistinct = 0;
    *pCodeMax = 0;
    //on remarque que pCodeMax doit avoir le code plus petit en cas d'egalité,
    //donc nous devons utiliser un for inversé
    for (int j = 7; j>= 0; j--){
        if(freq[j] >= freq[*pCodeMax - 1]) *pCodeMax = j + 1;
        if (freq[j] != 0) *pNbDistinct ++;
    }
    return n;
    
    

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
