/*
 * test_cours_UL1IN002.c
 *
 * Rappels de cours — Concepts du langage C necessaires pour LU1IN002
 * Sorbonne Universite — Elements de programmation 2
 *
 * Framework : tau (header-only, cross-platform Windows/Linux/macOS)
 *   https://github.com/jasmcaus/tau
 *
 * Chaque TEST(suite, nom) illustre UN concept fondamental du langage C.
 * Les tests sont decouverts automatiquement — aucune liste a maintenir.
 * TAU_MAIN() en bas genere le point d'entree.
 */

#include "tau/tau.h"
#include <stdlib.h>

/* ===================================================================
 * CONCEPT 1 : Pointeurs — adresse et dereferencement
 * ---------------------------------------------------
 * Mnemo : "& donne l'Adresse,  * lit/ecrit la Valeur"
 *
 *   int x = 42;
 *   int *p = &x;   // p contient l'adresse de x
 *   *p = 99;       // ecrit 99 a l'adresse pointee => x vaut 99
 * =================================================================== */
TEST(pointeurs, adresse_et_deref) {
    int x = 42;
    int *p = &x;              /* p = adresse de x */

    REQUIRE_EQ(*p, 42, "Le pointeur doit dereferencer la valeur initiale de x");      /* *p lit la valeur a l'adresse p */

    *p = 99;                  /* *p modifie la valeur a l'adresse p */
    REQUIRE_EQ(x, 99, "Ecrire via *p doit modifier la variable x");       /* x a ete change via le pointeur */
}

/* ===================================================================
 * CONCEPT 2 : Parametre de sortie (passage par adresse)
 * -------------------------------------------------------
 * Mnemo : "& pour Donner l'adresse,  * pour Ecrire dedans"
 *
 *   void f(int *res) { *res = 42; }   // signature
 *   int r;  f(&r);                    // appel : on passe &r, pas r
 *
 * Utile quand une fonction doit retourner PLUSIEURS valeurs.
 * =================================================================== */
static void doubler(int *val) {
    *val = *val * 2;  /* ecrit dans la variable de l'appelant */
}

TEST(pointeurs, parametre_sortie) {
    int n = 5;
    doubler(&n);         /* on passe l'adresse, la fonction modifie n */
    REQUIRE_EQ(n, 10, "Le parametre de sortie doit doubler la valeur");
}

/* ===================================================================
 * CONCEPT 3 : Structures — definition et acces via le point
 * ----------------------------------------------------------
 * Mnemo : "struct = regrouper des champs sous un nom"
 *         "variable.champ  pour lire ou ecrire"
 * =================================================================== */
typedef struct {
    int code;
    int nbObs;
} Espece;

TEST(structures, acces_point) {
    Espece e;
    e.code  = 3;
    e.nbObs = 7;

    REQUIRE_EQ(e.code,  3, "L'acces par point doit lire correctement le champ code");
    REQUIRE_EQ(e.nbObs, 7, "L'acces par point doit lire correctement le champ nbObs");
}

/* ===================================================================
 * CONCEPT 4 : Pointeur de structure — l'operateur fleche ->
 * ----------------------------------------------------------
 * Mnemo : "p->champ  identique a  (*p).champ"
 *         "-> quand POINTEUR,  . quand valeur directe"
 * =================================================================== */
TEST(structures, acces_fleche) {
    Espece  e = {5, 2};
    Espece *p = &e;

    REQUIRE_EQ(p->code,  5, "L'operateur -> doit acceder au champ code via pointeur");    /* p->code  ==  (*p).code */
    REQUIRE_EQ(p->nbObs, 2, "L'operateur -> doit acceder au champ nbObs via pointeur");

    p->nbObs = 10;              /* modifier via le pointeur */
    REQUIRE_EQ(e.nbObs, 10, "Modifier via un pointeur doit impacter la structure cible");   /* la structure originale est modifiee */
}

/* ===================================================================
 * CONCEPT 5 : Allocation dynamique — malloc / free
 * -------------------------------------------------
 * Mnemo : "ALLOUER -> VERIFIER NULL -> UTILISER -> LIBERER"
 *
 *   T *p = malloc(sizeof(T));
 *   if (!p) { ... }   // toujours verifier !
 *   free(p);
 *
 * Apres free, p est invalide ("dangling pointer") -> ne plus l'utiliser.
 * =================================================================== */
TEST(memoire, malloc_free) {
    Espece *p = malloc(sizeof(Espece));
    REQUIRE_NOT_NULL(p, "malloc doit retourner un pointeur valide");    /* toujours verifier ! */

    p->code  = 7;
    p->nbObs = 3;
    REQUIRE_EQ(p->code, 7, "La zone allouee doit etre accessible en lecture/ecriture");

    free(p);
    /* Bonne pratique : p = NULL; apres free */
}

/* ===================================================================
 * CONCEPT 6 : typedef struct auto-referent (maillon de liste)
 * ------------------------------------------------------------
 * Mnemo : "maillon = valeur + pointeur vers le SUIVANT (ou NULL)"
 *
 *   struct Maillon {
 *       int val;
 *       struct Maillon *suiv;  // "struct Maillon" obligatoire ici,
 *   };                         //  car le typedef n'est pas encore connu
 *
 * Fin de liste : le dernier maillon a  suiv == NULL.
 * =================================================================== */
typedef struct Maillon {
    int            val;
    struct Maillon *suiv;
} Maillon;

TEST(listes, typedef_maillon) {
    /* construction a la main sur la pile : 10 -> 20 -> NULL */
    Maillon m1 = {10, NULL};
    Maillon m2 = {20, &m1};

    REQUIRE_EQ(m2.val,       20, "Le maillon de tete doit conserver sa valeur");
    REQUIRE_EQ(m2.suiv->val, 10, "Le lien suiv doit pointer vers le maillon precedent");   /* acces au suivant via -> */
    REQUIRE_NULL(m2.suiv->suiv, "Le dernier maillon doit avoir suiv == NULL");    /* fin de liste = NULL */
}

/* ===================================================================
 * CONCEPT 7 : Parcours de liste chainee
 * --------------------------------------
 * Mnemo : "while(p != NULL) { traiter(p) ; p = p->suiv ; }"
 *
 * p est un CURSEUR local. Ne jamais avancer la variable tete elle-meme.
 * =================================================================== */
TEST(listes, parcours) {
    /* 1 -> 2 -> 3 -> NULL (sur la pile) */
    Maillon m3 = {3, NULL};
    Maillon m2 = {2, &m3};
    Maillon m1 = {1, &m2};

    int somme = 0;
    Maillon *p = &m1;
    while (p != NULL) {
        somme += p->val;
        p = p->suiv;
    }

    REQUIRE_EQ(somme, 6, "Le parcours de liste doit sommer 1 + 2 + 3");    /* 1 + 2 + 3 */
}

/* ===================================================================
 * CONCEPT 8 : Insertion en tete de liste chainee
 * ------------------------------------------------
 * Mnemo : "CREER -> POINTER vers l'ancienne tete -> RETOURNER"
 *
 *   Maillon *nv = malloc(sizeof(Maillon));
 *   nv->val  = v;
 *   nv->suiv = liste;   // nv pointe vers l'ancienne tete
 *   return nv;          // la nouvelle tete est retournee
 *
 * L'appelant DOIT recuperer le retour : liste = inserer_tete(liste, v);
 * =================================================================== */
static Maillon *inserer_tete(Maillon *liste, int val) {
    Maillon *nv = malloc(sizeof(Maillon));
    if (!nv) return liste;
    nv->val  = val;
    nv->suiv = liste;
    return nv;
}

static Maillon *liberer_liste(Maillon *p) {
    while (p != NULL) {
        Maillon *tmp = p->suiv;
        free(p);
        p = tmp;
    }
    return NULL;
}

TEST(listes, insertion_tete) {
    Maillon *liste = NULL;
    liste = inserer_tete(liste, 3);    /* 3 -> NULL          */
    liste = inserer_tete(liste, 2);    /* 2 -> 3 -> NULL     */
    liste = inserer_tete(liste, 1);    /* 1 -> 2 -> 3 -> NULL */

    REQUIRE_EQ(liste->val,             1, "Apres insertion en tete, la nouvelle tete doit valoir 1");
    REQUIRE_EQ(liste->suiv->val,       2, "Le deuxieme maillon doit valoir 2");
    REQUIRE_EQ(liste->suiv->suiv->val, 3, "Le troisieme maillon doit valoir 3");
    REQUIRE_NULL(liste->suiv->suiv->suiv, "Le dernier maillon doit terminer la liste avec NULL");

    liberer_liste(liste);
}

/* ===================================================================
 * CONCEPT 9 : Liberation d'une liste chainee dynamique
 * -------------------------------------------------------
 * Mnemo : "SAUVER suiv AVANT de free le maillon courant"
 *
 *   Maillon *tmp = p->suiv;   // 1. SAUVER
 *   free(p);                  // 2. LIBERER
 *   p = tmp;                  // 3. AVANCER
 *
 * ERREUR classique : free(p); p = p->suiv;
 *   -> acces a p->suiv apres liberation ! (comportement indefini)
 * =================================================================== */
TEST(memoire, liberation_liste) {
    Maillon *a = malloc(sizeof(Maillon)); a->val = 10; a->suiv = NULL;
    Maillon *b = malloc(sizeof(Maillon)); b->val = 20; b->suiv = a;

    Maillon *p = b;
    while (p != NULL) {
        Maillon *tmp = p->suiv;    /* 1. SAUVER */
        free(p);                   /* 2. LIBERER */
        p = tmp;                   /* 3. AVANCER */
    }

    REQUIRE_EQ(1, 1, "La liberation complete ne doit provoquer aucun crash");   /* arriver ici sans crash valide la liberation */
}

/* ===================================================================
 * CONCEPT 10 : Tableaux statiques — indexation et taille
 * --------------------------------------------------------
 * Mnemo : "indices de 0 a N-1 ; sizeof(tab)/sizeof(tab[0]) = N"
 *
 *   tab[0]  =>  premier element
 *   tab[N-1]=>  dernier element
 *   tab[N]  =>  HORS BORNES (pas d'erreur de compil !)
 * =================================================================== */
TEST(tableaux, statique) {
    int tab[5] = {10, 20, 30, 40, 50};
    int n = (int)(sizeof(tab) / sizeof(tab[0]));

    REQUIRE_EQ(n,      5, "Le calcul de taille du tableau statique doit donner 5");
    REQUIRE_EQ(tab[0], 10, "L'indice 0 doit contenir le premier element");    /* premier element : indice 0 */
    REQUIRE_EQ(tab[4], 50, "L'indice N-1 doit contenir le dernier element");    /* dernier element : indice N-1 */
}

/* ===================================================================
 * CONCEPT 11 : Tableau passe a une fonction — decroissance en pointeur
 * ---------------------------------------------------------------------
 * Mnemo : "tableau passe = POINTEUR sur tab[0]"
 *         "sizeof(tab) dans la fonction = sizeof(int*), pas N*sizeof(int)"
 *         "=> toujours passer n separement"
 * =================================================================== */
static int somme_tableau(int *tab, int n) {
    int s = 0;
    for (int i = 0; i < n; i++) s += tab[i];
    return s;
}

TEST(tableaux, parametre) {
    int t[] = {1, 2, 3, 4, 5};
    int n   = (int)(sizeof(t) / sizeof(t[0]));   /* calcul possible ICI seulement */

    REQUIRE_EQ(somme_tableau(t, n), 15, "La somme des elements du tableau doit etre 15");
}

/* ===================================================================
 * CONCEPT 12 : NULL comme sentinelle
 * ------------------------------------
 * Mnemo : "NULL = rien ; p == NULL => liste vide ou fin atteinte"
 *
 *   Maillon *liste = NULL;               // liste vide
 *   while (p != NULL) { p = p->suiv; }  // parcours complet
 *
 * Toujours initialiser un pointeur a NULL s'il ne pointe sur rien.
 * =================================================================== */
static int longueur(Maillon *p) {
    int n = 0;
    while (p != NULL) { n++; p = p->suiv; }
    return n;
}

TEST(listes, null_sentinelle) {
    Maillon *vide = NULL;
    REQUIRE_EQ(longueur(vide), 0, "Une liste vide doit avoir une longueur de 0");    /* liste vide => longueur 0 */

    Maillon un = {42, NULL};
    REQUIRE_EQ(longueur(&un), 1, "Une liste a un maillon doit avoir une longueur de 1");

    Maillon deux = {7, &un};
    REQUIRE_EQ(longueur(&deux), 2, "Une liste a deux maillons doit avoir une longueur de 2");
}

/* ===================================================================
 * CONCEPT 13 : Suppression d'un maillon en tete
 * -----------------------------------------------
 * Mnemo : "SAUVER suiv -> FREE tete -> retourner suiv"
 *
 *   Maillon *tmp = liste->suiv;
 *   free(liste);
 *   return tmp;
 *
 * L'appelant recupere le retour : liste = supprimer_tete(liste);
 * =================================================================== */
static Maillon *supprimer_tete(Maillon *liste) {
    if (liste == NULL) return NULL;
    Maillon *tmp = liste->suiv;
    free(liste);
    return tmp;
}

TEST(listes, suppression_tete) {
    Maillon *liste = NULL;
    liste = inserer_tete(liste, 3);
    liste = inserer_tete(liste, 2);
    liste = inserer_tete(liste, 1);  /* 1 -> 2 -> 3 -> NULL */

    liste = supprimer_tete(liste);
    REQUIRE_EQ(liste->val, 2, "Apres suppression de tete, la nouvelle tete doit valoir 2");

    liste = supprimer_tete(liste);
    REQUIRE_EQ(liste->val, 3, "Apres une deuxieme suppression, la tete doit valoir 3");

    liste = supprimer_tete(liste);
    REQUIRE_NULL(liste, "Supprimer la derniere tete doit rendre la liste vide");

    liste = supprimer_tete(liste);   /* liste deja vide -> ne plante pas */
    REQUIRE_NULL(liste, "Supprimer la tete d'une liste vide doit laisser NULL");
}

/* =================================================================== */
TAU_MAIN()   /* point d'entree — les tests sont decouverts automatiquement */
