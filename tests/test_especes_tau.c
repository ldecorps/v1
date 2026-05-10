/*
 * test_especes_tau.c
 *
 * Tests unitaires — Examen 2026 LU1IN002 (Sorbonne Universite)
 * REF: Examen_2026_UL1IN002_Prediction.pdf
 *
 * Framework : tau (header-only, cross-platform Windows/Linux/macOS)
 *   https://github.com/jasmcaus/tau
 *
 * Chaque suite de tests (TEST(suite, nom)) correspond a une question.
 * Le commentaire REF indique la question et la page du sujet PDF.
 * Implementez les fonctions dans src/especes.c une par une :
 * les tests passeront au fur et a mesure.
 *
 * TAU_MAIN() en bas genere le point d'entree.
 */

#include "tau/tau.h"
#include "especes.h"
#include <stdlib.h>

/* ===================================================================
 * REF: Q1 (p.1) — EstValide
 * =================================================================== */

TEST(q1_tableaux, est_valide_code_zero) {
    REQUIRE_EQ(EstValide(0), 0);           /* en dessous de la borne inferieure */
}

TEST(q1_tableaux, est_valide_borne_inf) {
    REQUIRE_EQ(EstValide(1), 1);           /* borne inferieure valide */
}

TEST(q1_tableaux, est_valide_borne_sup) {
    REQUIRE_EQ(EstValide(NB_ESPECES), 1);  /* borne superieure valide */
}

TEST(q1_tableaux, est_valide_au_dessus) {
    REQUIRE_EQ(EstValide(NB_ESPECES + 1), 0); /* au-dessus de la borne */
}

/* ===================================================================
 * REF: Q1 (p.1) — CompterOccurrences
 * =================================================================== */

TEST(q1_tableaux, compter_occurrences_releve_exemple) {
    int releve[] = {3, 1, 5, 3, 1, 3, 7, 5, 1, 3};
    REQUIRE_EQ(CompterOccurrences(releve, 10, 3), 4); /* espece 3 : 4 fois */
    REQUIRE_EQ(CompterOccurrences(releve, 10, 1), 3); /* espece 1 : 3 fois */
    REQUIRE_EQ(CompterOccurrences(releve, 10, 5), 2); /* espece 5 : 2 fois */
    REQUIRE_EQ(CompterOccurrences(releve, 10, 7), 1); /* espece 7 : 1 fois */
}

TEST(q1_tableaux, compter_occurrences_absent) {
    int releve[] = {3, 1, 5, 3, 1, 3, 7, 5, 1, 3};
    REQUIRE_EQ(CompterOccurrences(releve, 10, 2), 0); /* espece 2 absente */
}

/* ===================================================================
 * REF: Q2 (p.1) — AfficherReleve
 * Test visuel uniquement : verifie l'absence de crash.
 * (La sortie printf ne peut pas etre comparee ici sans redirection.)
 * =================================================================== */

TEST(q2_tableaux, afficher_releve_no_crash) {
    int releve[] = {1, 3, 5};
    AfficherReleve(releve, 3); /* doit s'executer sans crash */
    REQUIRE_EQ(1, 1);
}

/* ===================================================================
 * REF: Q3 (p.1-2) — ReleveAleatoire
 * =================================================================== */

TEST(q3_tableaux, releve_aleatoire_non_null) {
    int *r = ReleveAleatoire(10);
    REQUIRE_NOT_NULL(r);
    free(r);
}

TEST(q3_tableaux, releve_aleatoire_codes_valides) {
    int *r = ReleveAleatoire(200);
    REQUIRE_NOT_NULL(r);
    for (int i = 0; i < 200; i++) {
        REQUIRE_GE(r[i], 1);           /* code >= 1           */
        REQUIRE_LE(r[i], NB_ESPECES);  /* code <= NB_ESPECES  */
    }
    free(r);
}

/* ===================================================================
 * REF: Q4 (p.2) — AnalyserReleve
 * =================================================================== */

TEST(q4_tableaux, analyser_releve_un_element) {
    /* Cas trivial : un seul element */
    int releve[] = {5};
    int nbDistinct = 0, codeMax = 0;
    int total = AnalyserReleve(releve, 1, &nbDistinct, &codeMax);
    REQUIRE_EQ(total,      1); /* 1 observation */
    REQUIRE_EQ(nbDistinct, 1); /* 1 espece distincte */
    REQUIRE_EQ(codeMax,    5); /* seule espece presente */
}

IGNORE_TEST(q4_tableaux, analyser_releve_toutes_memes) {
    /* Toutes les observations sont la meme espece */
    int releve[] = {3, 3, 3, 3};
    int nbDistinct = 0, codeMax = 0;
    int total = AnalyserReleve(releve, 4, &nbDistinct, &codeMax);
    REQUIRE_EQ(total,      4); /* 4 observations */
    REQUIRE_EQ(nbDistinct, 1); /* 1 seule espece */
    REQUIRE_EQ(codeMax,    3); /* espece 3 */
}

IGNORE_TEST(q4_tableaux, analyser_releve_retour_et_code_max) {
    int releve[] = {3, 1, 5, 3, 1, 3, 7, 5, 1, 3};
    int nbDistinct = 0, codeMax = 0;
    int total = AnalyserReleve(releve, 10, &nbDistinct, &codeMax);
    REQUIRE_EQ(total,      10); /* nombre total d'observations */
    REQUIRE_EQ(nbDistinct,  4); /* especes 1, 3, 5, 7          */
    REQUIRE_EQ(codeMax,     3); /* espece 3 observee 4 fois     */
}

IGNORE_TEST(q4_tableaux, analyser_releve_egalite_petit_code) {
    /* En cas d'egalite on conserve le plus petit code */
    int releve[] = {1, 2, 1, 2};  /* especes 1 et 2 : 2 obs chacune */
    int nbDistinct = 0, codeMax = 0;
    AnalyserReleve(releve, 4, &nbDistinct, &codeMax);
    REQUIRE_EQ(nbDistinct, 2);
    REQUIRE_EQ(codeMax,    1); /* egalite => plus petit code */
}

/* ===================================================================
 * REF: Q5 (p.2) — InsererTrie
 * =================================================================== */

IGNORE_TEST(q5_listes, inserer_premier_element) {
    tyEspece *reg = NULL;
    reg = InsererTrie(reg, 3);
    REQUIRE_NOT_NULL(reg);                 /* garde : abort si NULL */
    REQUIRE_EQ(reg->code,  3);
    REQUIRE_EQ(reg->nbObs, 1);
    REQUIRE_NULL(reg->suiv);
    reg = DetruireRegistre(reg);
}

IGNORE_TEST(q5_listes, inserer_incremente_nbobs) {
    tyEspece *reg = NULL;
    reg = InsererTrie(reg, 3);
    reg = InsererTrie(reg, 3);             /* deuxieme observation de code 3 */
    REQUIRE_NOT_NULL(reg);                 /* garde : abort si NULL */
    REQUIRE_EQ(reg->nbObs, 2);
    reg = DetruireRegistre(reg);
}

IGNORE_TEST(q5_listes, inserer_ordre_croissant) {
    tyEspece *reg = NULL;
    reg = InsererTrie(reg, 5);
    reg = InsererTrie(reg, 1);
    reg = InsererTrie(reg, 3);     /* doit etre trie : 1 -> 3 -> 5 -> NULL */
    REQUIRE_NOT_NULL(reg);                     /* garde niveau 1 */
    REQUIRE_EQ(reg->code, 1);
    REQUIRE_NOT_NULL(reg->suiv);               /* garde niveau 2 */
    REQUIRE_EQ(reg->suiv->code, 3);
    REQUIRE_NOT_NULL(reg->suiv->suiv);         /* garde niveau 3 */
    REQUIRE_EQ(reg->suiv->suiv->code, 5);
    REQUIRE_NULL(reg->suiv->suiv->suiv);
    reg = DetruireRegistre(reg);
}

IGNORE_TEST(q5_listes, inserer_releve_exemple_complet) {
    /* Releve : 3 1 5 3 1 3 7 5 1 3  =>  (1,3)->(3,4)->(5,2)->(7,1)->NULL */
    int releve[] = {3, 1, 5, 3, 1, 3, 7, 5, 1, 3};
    tyEspece *reg = NULL;
    for (int i = 0; i < 10; i++) reg = InsererTrie(reg, releve[i]);

    REQUIRE_NOT_NULL(reg);                                  /* garde n1 */
    REQUIRE_EQ(reg->code, 1);  REQUIRE_EQ(reg->nbObs, 3);
    REQUIRE_NOT_NULL(reg->suiv);                            /* garde n2 */
    REQUIRE_EQ(reg->suiv->code, 3);  REQUIRE_EQ(reg->suiv->nbObs, 4);
    REQUIRE_NOT_NULL(reg->suiv->suiv);                      /* garde n3 */
    REQUIRE_EQ(reg->suiv->suiv->code, 5);  REQUIRE_EQ(reg->suiv->suiv->nbObs, 2);
    REQUIRE_NOT_NULL(reg->suiv->suiv->suiv);                /* garde n4 */
    REQUIRE_EQ(reg->suiv->suiv->suiv->code, 7);  REQUIRE_EQ(reg->suiv->suiv->suiv->nbObs, 1);
    REQUIRE_NULL(reg->suiv->suiv->suiv->suiv);
    reg = DetruireRegistre(reg);
}

/* ===================================================================
 * REF: Q6 (p.2) — NbEspeces
 * =================================================================== */

IGNORE_TEST(q6_listes, nb_especes_liste_vide) {
    REQUIRE_EQ(NbEspeces(NULL), 0);
}

IGNORE_TEST(q6_listes, nb_especes_compte) {
    tyEspece *reg = NULL;
    reg = InsererTrie(reg, 1);
    reg = InsererTrie(reg, 3);
    reg = InsererTrie(reg, 5);
    REQUIRE_EQ(NbEspeces(reg), 3);
    reg = DetruireRegistre(reg);
}

/* ===================================================================
 * REF: Q7 (p.2) — DetruireRegistre
 * =================================================================== */

IGNORE_TEST(q7_listes, detruire_retourne_null) {
    tyEspece *reg = NULL;
    reg = InsererTrie(reg, 2);
    reg = InsererTrie(reg, 4);
    reg = DetruireRegistre(reg);
    REQUIRE_NULL(reg);
}

IGNORE_TEST(q7_listes, detruire_liste_vide_ne_crash_pas) {
    tyEspece *reg = DetruireRegistre(NULL);
    REQUIRE_NULL(reg);
}

/* ===================================================================
 * REF: Q8 (p.2-3) — AfficherRegistre
 * Test visuel uniquement : verifie l'absence de crash.
 * =================================================================== */

IGNORE_TEST(q8_listes, afficher_registre_no_crash) {
    tyEspece *reg = NULL;
    reg = InsererTrie(reg, 1);
    reg = InsererTrie(reg, 3);
    AfficherRegistre(reg, 2); /* doit s'executer sans crash */
    REQUIRE_EQ(1, 1);
    reg = DetruireRegistre(reg);
}

/* ===================================================================
 * REF: Q11 (p.3) — FiltrerRares
 * =================================================================== */

IGNORE_TEST(q11_listes, filtrer_supprime_sous_seuil) {
    /* Avant : (1,3)->(3,4)->(5,2)->(7,1)   seuil=3
     * Apres : (1,3)->(3,4)                              */
    int releve[] = {3, 1, 5, 3, 1, 3, 7, 5, 1, 3};
    tyEspece *reg = NULL;
    for (int i = 0; i < 10; i++) reg = InsererTrie(reg, releve[i]);

    reg = FiltrerRares(reg, 3);

    REQUIRE_NOT_NULL(reg);                  /* garde n1 */
    REQUIRE_EQ(reg->code,  1);
    REQUIRE_EQ(reg->nbObs, 3);
    REQUIRE_NOT_NULL(reg->suiv);            /* garde n2 */
    REQUIRE_EQ(reg->suiv->code, 3);
    REQUIRE_NULL(reg->suiv->suiv);
    reg = DetruireRegistre(reg);
}

IGNORE_TEST(q11_listes, filtrer_liste_vide) {
    tyEspece *reg = FiltrerRares(NULL, 5);
    REQUIRE_NULL(reg);
}

IGNORE_TEST(q11_listes, filtrer_seuil_supprime_tout) {
    tyEspece *reg = NULL;
    reg = InsererTrie(reg, 1); /* nbObs = 1 */
    reg = FiltrerRares(reg, 5);
    REQUIRE_NULL(reg);
}

/* =================================================================== */
TAU_MAIN()  /* point d'entree — les tests sont decouverts automatiquement */
