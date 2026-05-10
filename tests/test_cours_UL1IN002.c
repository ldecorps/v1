/*
 * test_cours_UL1IN002.c
 *
 * Rappels de cours — Concepts du langage C nécessaires pour LU1IN002
 * Sorbonne Université — Éléments de programmation 2
 *
 * Chaque test illustre UN concept fondamental du langage C,
 * avec un moyen mnémotechnique pour le retenir.
 * Tout est dans ce fichier : aucune bibliothèque externe hormis cmocka.
 *
 * NE PAS LIRE : ce fichier ne donne pas les réponses à l'examen.
 * Il explique les briques du langage C dont on a besoin pour les écrire.
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdlib.h>
#include <string.h>

/* ===================================================================
 * CONCEPT 1 : Pointeurs — adresse et déréférencement
 * ───────────────────────────────────────────────────
 * Mnémo : "& donne l'Adresse,  * lit/écrit la Valeur"
 *
 *   int x = 42;
 *   int *p = &x;   // p contient l'adresse de x
 *   *p = 99;       // écrit 99 à l'adresse pointée → x vaut 99
 * =================================================================== */
static void test_pointeur_adresse_et_deref(void **state) {
    (void)state;

    int x = 42;
    int *p = &x;             /* p = adresse de x */

    assert_int_equal(*p, 42);    /* *p lit la valeur à l'adresse p */

    *p = 99;                 /* *p modifie la valeur à l'adresse p */
    assert_int_equal(x, 99);     /* x a été changé via le pointeur */
}

/* ===================================================================
 * CONCEPT 2 : Paramètre de sortie (passage par adresse)
 * ───────────────────────────────────────────────────────
 * Mnémo : "& pour Donner l'adresse,  * pour Écrire dedans"
 *
 *   void f(int *res) { *res = 42; }   // signature
 *   int r;  f(&r);                    // appel : on passe &r, pas r
 *
 * Utile quand une fonction doit retourner PLUSIEURS valeurs.
 * =================================================================== */
static void doubler(int *val) {
    *val = *val * 2;  /* écrit dans la variable de l'appelant */
}

static void test_parametre_sortie(void **state) {
    (void)state;

    int n = 5;
    doubler(&n);         /* on passe l'adresse, la fonction modifie n */
    assert_int_equal(n, 10);
}

/* ===================================================================
 * CONCEPT 3 : Structures — définition et accès via le point
 * ──────────────────────────────────────────────────────────
 * Mnémo : "struct = regrouper des champs sous un nom"
 *         "variable.champ  pour lire ou écrire"
 * =================================================================== */
typedef struct {
    int code;
    int nbObs;
} Espece;

static void test_struct_acces_point(void **state) {
    (void)state;

    Espece e;            /* variable de type Espece (sur la pile) */
    e.code  = 3;
    e.nbObs = 7;

    assert_int_equal(e.code,  3);
    assert_int_equal(e.nbObs, 7);
}

/* ===================================================================
 * CONCEPT 4 : Pointeur de structure — l'opérateur flèche ->
 * ──────────────────────────────────────────────────────────
 * Mnémo : "p->champ  ≡  (*p).champ"
 *         "flèche = déréférencer ET accéder au champ en une étape"
 *
 * On utilise -> dès qu'on a un POINTEUR sur une structure.
 * On utilise .  dès qu'on a la structure DIRECTEMENT.
 * =================================================================== */
static void test_struct_acces_fleche(void **state) {
    (void)state;

    Espece  e = {5, 2};
    Espece *p = &e;           /* pointeur sur la structure */

    assert_int_equal(p->code,  5);    /* p->code  ==  (*p).code  */
    assert_int_equal(p->nbObs, 2);

    p->nbObs = 10;            /* modifier via le pointeur */
    assert_int_equal(e.nbObs, 10);    /* la structure originale est modifiée */
}

/* ===================================================================
 * CONCEPT 5 : Allocation dynamique — malloc / free
 * ─────────────────────────────────────────────────
 * Mnémo : "ALLOUER → VÉRIFIER NULL → UTILISER → LIBÉRER"
 *
 *   T *p = malloc(sizeof(T));
 *   if (!p) { /* erreur */ }
 *   p->champ = ...;
 *   free(p);
 *
 * malloc retourne NULL si la mémoire est épuisée.
 * Après free, p est invalide ("dangling pointer") → ne plus l'utiliser.
 * =================================================================== */
static void test_malloc_free(void **state) {
    (void)state;

    Espece *p = malloc(sizeof(Espece));
    assert_non_null(p);      /* toujours vérifier ! */

    p->code  = 7;
    p->nbObs = 3;
    assert_int_equal(p->code, 7);

    free(p);
    /* Bonne pratique : p = NULL;  après free, pour éviter les bugs */
}

/* ===================================================================
 * CONCEPT 6 : typedef struct auto-référent (maillon de liste)
 * ─────────────────────────────────────────────────────────────
 * Mnémo : "maillon = valeur + pointeur vers le SUIVANT (ou NULL)"
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

static void test_typedef_maillon(void **state) {
    (void)state;

    /* construction à la main sur la pile : 10 → 20 → NULL */
    Maillon m1 = {10, NULL};
    Maillon m2 = {20, &m1};

    assert_int_equal(m2.val,       20);
    assert_int_equal(m2.suiv->val, 10);    /* accès au suivant via -> */
    assert_null(m2.suiv->suiv);            /* fin de liste = NULL */
}

/* ===================================================================
 * CONCEPT 7 : Parcours de liste chaînée
 * ──────────────────────────────────────
 * Mnémo : "while(p != NULL) { traiter(p) ; p = p->suiv ; }"
 *
 * p est un CURSEUR qui avance de maillon en maillon.
 * Ne pas modifier p directement si on en a besoin ensuite → utiliser un curseur.
 * =================================================================== */
static void test_parcours_liste(void **state) {
    (void)state;

    /* 1 → 2 → 3 → NULL (sur la pile) */
    Maillon m3 = {3, NULL};
    Maillon m2 = {2, &m3};
    Maillon m1 = {1, &m2};

    int somme = 0;
    Maillon *p = &m1;         /* p est le curseur de parcours */
    while (p != NULL) {
        somme += p->val;
        p = p->suiv;          /* avancer au maillon suivant */
    }

    assert_int_equal(somme, 6);    /* 1 + 2 + 3 */
}

/* ===================================================================
 * CONCEPT 8 : Insertion en tête de liste chaînée
 * ────────────────────────────────────────────────
 * Mnémo : "CRÉER → POINTER vers l'ancienne tête → RETOURNER"
 *
 *   Maillon *nv = malloc(sizeof(Maillon));
 *   nv->val  = v;
 *   nv->suiv = liste;    // nv pointe vers l'ancienne tête
 *   return nv;           // la nouvelle tête est retournée
 *
 * L'appelant DOIT récupérer le retour : liste = inserer_tete(liste, v);
 * =================================================================== */
static Maillon *inserer_tete(Maillon *liste, int val) {
    Maillon *nv = malloc(sizeof(Maillon));
    if (!nv) return liste;    /* malloc peut échouer */
    nv->val  = val;
    nv->suiv = liste;         /* nv devient la nouvelle tête */
    return nv;
}

static Maillon *liberer_liste(Maillon *p) {
    while (p != NULL) {
        Maillon *tmp = p->suiv;    /* SAUVER suiv avant free */
        free(p);
        p = tmp;
    }
    return NULL;
}

static void test_insertion_tete(void **state) {
    (void)state;

    Maillon *liste = NULL;
    liste = inserer_tete(liste, 3);    /* 3 → NULL         */
    liste = inserer_tete(liste, 2);    /* 2 → 3 → NULL     */
    liste = inserer_tete(liste, 1);    /* 1 → 2 → 3 → NULL */

    assert_int_equal(liste->val,             1);
    assert_int_equal(liste->suiv->val,       2);
    assert_int_equal(liste->suiv->suiv->val, 3);
    assert_null(liste->suiv->suiv->suiv);

    liberer_liste(liste);
}

/* ===================================================================
 * CONCEPT 9 : Libération d'une liste chaînée dynamique
 * ─────────────────────────────────────────────────────
 * Mnémo : "SAUVER suiv AVANT de free le maillon courant"
 *
 *   Maillon *tmp = p->suiv;   // 1. SAUVER le lien
 *   free(p);                  // 2. LIBÉRER le maillon
 *   p = tmp;                  // 3. AVANCER
 *
 * ERREUR classique : free(p); p = p->suiv;
 *   → on accède à p->suiv après avoir libéré p !  (comportement indéfini)
 * =================================================================== */
static void test_liberation_liste(void **state) {
    (void)state;

    /* créer dynamiquement : 10 → 20 → NULL */
    Maillon *a = malloc(sizeof(Maillon)); a->val = 10; a->suiv = NULL;
    Maillon *b = malloc(sizeof(Maillon)); b->val = 20; b->suiv = a;

    Maillon *p = b;
    while (p != NULL) {
        Maillon *tmp = p->suiv;    /* 1. SAUVER */
        free(p);                   /* 2. LIBÉRER */
        p = tmp;                   /* 3. AVANCER */
    }

    /* arriver ici sans crash valide la libération */
    assert_true(1);
}

/* ===================================================================
 * CONCEPT 10 : Tableaux statiques — indexation et taille
 * ────────────────────────────────────────────────────────
 * Mnémo : "indices de 0 à N-1 ; sizeof(tab)/sizeof(tab[0]) = N"
 *
 *   int tab[5] = {10, 20, 30, 40, 50};
 *   tab[0]  → 10    (premier élément)
 *   tab[4]  → 50    (dernier élément, indice N-1)
 *   tab[5]  → HORS BORNES — comportement indéfini, pas d'erreur de compil !
 * =================================================================== */
static void test_tableau_statique(void **state) {
    (void)state;

    int tab[5] = {10, 20, 30, 40, 50};
    int n = (int)(sizeof(tab) / sizeof(tab[0]));    /* = 5 */

    assert_int_equal(n,      5);
    assert_int_equal(tab[0], 10);    /* premier élément : indice 0 */
    assert_int_equal(tab[4], 50);    /* dernier élément : indice N-1 */
}

/* ===================================================================
 * CONCEPT 11 : Tableau passé à une fonction — décroissance en pointeur
 * ─────────────────────────────────────────────────────────────────────
 * Mnémo : "tableau passé = POINTEUR sur le premier élément"
 *         "la taille N doit être passée séparément !"
 *
 *   void f(int *tab, int n)     // ou f(int tab[], int n)  — identique
 *   // Dans f : sizeof(tab) == sizeof(int *), PAS sizeof du tableau !
 *              → toujours passer n explicitement
 * =================================================================== */
static int somme_tableau(int *tab, int n) {
    int s = 0;
    for (int i = 0; i < n; i++) s += tab[i];
    return s;
}

static void test_tableau_parametre(void **state) {
    (void)state;

    int t[] = {1, 2, 3, 4, 5};
    int n   = (int)(sizeof(t) / sizeof(t[0]));    /* calcul possible ICI seulement */

    assert_int_equal(somme_tableau(t, n), 15);
}

/* ===================================================================
 * CONCEPT 12 : NULL comme sentinelle — liste vide et fin de liste
 * ────────────────────────────────────────────────────────────────
 * Mnémo : "NULL = rien ; p == NULL → liste vide ou fin atteinte"
 *
 *   Maillon *liste = NULL;           // liste vide
 *   if (liste == NULL) { ... }       // test liste vide
 *   while (p != NULL) { p = p->suiv; }  // parcours jusqu'à la fin
 *
 * Toujours initialiser un pointeur à NULL s'il ne pointe sur rien.
 * =================================================================== */
static int longueur(Maillon *p) {
    int n = 0;
    while (p != NULL) { n++; p = p->suiv; }
    return n;
}

static void test_null_sentinelle(void **state) {
    (void)state;

    Maillon *vide = NULL;
    assert_int_equal(longueur(vide), 0);     /* liste vide → longueur 0 */

    Maillon un = {42, NULL};
    assert_int_equal(longueur(&un), 1);      /* liste à un élément */

    Maillon deux = {7, &un};
    assert_int_equal(longueur(&deux), 2);    /* liste à deux éléments */
}

/* ===================================================================
 * CONCEPT 13 : Suppression d'un maillon en tête
 * ───────────────────────────────────────────────
 * Mnémo : "SAUVER le suivant → FREE la tête → retourner le suivant"
 *
 *   Maillon *tmp = liste->suiv;   // sauver
 *   free(liste);                  // libérer
 *   return tmp;                   // nouvelle tête
 *
 * Comme pour l'insertion, l'appelant DOIT récupérer le retour :
 *   liste = supprimer_tete(liste);
 * =================================================================== */
static Maillon *supprimer_tete(Maillon *liste) {
    if (liste == NULL) return NULL;   /* cas liste vide : rien à faire */
    Maillon *tmp = liste->suiv;
    free(liste);
    return tmp;
}

static void test_suppression_tete(void **state) {
    (void)state;

    /* créer 1 → 2 → 3 → NULL */
    Maillon *liste = NULL;
    liste = inserer_tete(liste, 3);
    liste = inserer_tete(liste, 2);
    liste = inserer_tete(liste, 1);

    liste = supprimer_tete(liste);    /* supprime 1 → reste 2 → 3 */
    assert_int_equal(liste->val, 2);

    liste = supprimer_tete(liste);    /* supprime 2 → reste 3 */
    assert_int_equal(liste->val, 3);

    liste = supprimer_tete(liste);    /* supprime 3 → liste vide */
    assert_null(liste);

    liste = supprimer_tete(liste);    /* liste déjà vide → ne plante pas */
    assert_null(liste);
}

/* =================================================================== */

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_pointeur_adresse_et_deref),
        cmocka_unit_test(test_parametre_sortie),
        cmocka_unit_test(test_struct_acces_point),
        cmocka_unit_test(test_struct_acces_fleche),
        cmocka_unit_test(test_malloc_free),
        cmocka_unit_test(test_typedef_maillon),
        cmocka_unit_test(test_parcours_liste),
        cmocka_unit_test(test_insertion_tete),
        cmocka_unit_test(test_liberation_liste),
        cmocka_unit_test(test_tableau_statique),
        cmocka_unit_test(test_tableau_parametre),
        cmocka_unit_test(test_null_sentinelle),
        cmocka_unit_test(test_suppression_tete),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}

    for (int i = 0; i < n; i++) {
        if (releve[i] == code) count++;
    }
    return count;
}

/*
 * Q4 — AnalyserReleve
 * ───────────────────
 * Mnémo : "RETOUR = n, POINTEURS = résultats"
 *   Passe *pNbDistinct et *pCodeMax PAR ADRESSE  →  & côté appelant, * côté fonction
 *   En cas d'égalité de fréquence → plus petit code (on itère 1..NB_ESPECES).
 */
static int AnalyserReleve(int *releve, int n, int *pNbDistinct, int *pCodeMax) {
    int nbDistinct = 0;
    int codeMax    = 0;
    int maxObs     = 0;

    for (int code = 1; code <= NB_ESPECES; code++) {
        int occ = CompterOccurrences(releve, n, code);
        if (occ > 0) {
            nbDistinct++;
            if (occ > maxObs) {   /* > strict → en cas d'égalité le premier (plus petit) gagne */
                maxObs  = occ;
                codeMax = code;
            }
        }
    }

    *pNbDistinct = nbDistinct;
    *pCodeMax    = codeMax;
    return n;
}

/*
 * Structure liste simplement chaînée
 * ───────────────────────────────────
 * Mnémo : "CODE + NbObs + SUIV"  (3 champs, tri croissant sur code)
 *
 *   (code:1, nbObs:3) → (code:3, nbObs:4) → … → NULL
 */
typedef struct _tyEspece tyEspece;
struct _tyEspece {
    int       code;   /* identifiant de l'espèce */
    int       nbObs;  /* nombre d'observations   */
    tyEspece *suiv;   /* pointeur vers le suivant */
};

/*
 * Q5 — InsererTrie
 * ────────────────
 * Mnémo : "CHERCHE → CRÉE → CHAÎNE"
 *   3 cas :
 *     1. espèce déjà présente  →  nbObs++  (parcours complet)
 *     2. insertion en tête     →  code < registre->code
 *     3. insertion au milieu   →  après le dernier nœud dont code < code
 */
static tyEspece *InsererTrie(tyEspece *registre, int code) {
    /* Cas 1 : doublon → incrémenter */
    for (tyEspece *p = registre; p != NULL; p = p->suiv) {
        if (p->code == code) { p->nbObs++; return registre; }
    }

    /* Nouveau nœud */
    tyEspece *nv = (tyEspece *)malloc(sizeof(tyEspece));
    nv->code  = code;
    nv->nbObs = 1;
    nv->suiv  = NULL;

    /* Cas 2 : insertion en tête */
    if (registre == NULL || code < registre->code) {
        nv->suiv = registre;
        return nv;
    }

    /* Cas 3 : insertion milieu / queue */
    tyEspece *p = registre;
    while (p->suiv != NULL && p->suiv->code < code) p = p->suiv;
    nv->suiv = p->suiv;
    p->suiv  = nv;
    return registre;
}

/*
 * Q6 — NbEspeces
 * ──────────────
 * Mnémo : "WALK + COUNT"  →  parcours linéaire, incrément à chaque nœud
 */
static int NbEspeces(tyEspece *registre) {
    int n = 0;
    for (tyEspece *p = registre; p != NULL; p = p->suiv) n++;
    return n;
}

/*
 * Q7 — DetruireRegistre
 * ─────────────────────
 * Mnémo : "SAUVE suiv, FREE current, AVANCE"
 *   idiome : reg = DetruireRegistre(reg);   // reg vaut NULL après
 */
static tyEspece *DetruireRegistre(tyEspece *registre) {
    while (registre != NULL) {
        tyEspece *suiv = registre->suiv;
        free(registre);
        registre = suiv;
    }
    return NULL;
}

/*
 * Q11 — FiltrerRares
 * ──────────────────
 * Mnémo : "SEUIL → nbObs >= seuilMin survit, sinon DELETE"
 *   Attention : libérer la mémoire des nœuds supprimés.
 *   Traiter d'abord la tête (boucle while), puis le corps (chaîner).
 */
static tyEspece *FiltrerRares(tyEspece *registre, int seuilMin) {
    /* Supprimer en tête tant que nécessaire */
    while (registre != NULL && registre->nbObs < seuilMin) {
        tyEspece *tmp = registre;
        registre = registre->suiv;
        free(tmp);
    }
    if (registre == NULL) return NULL;

    /* Supprimer dans le reste */
    tyEspece *p = registre;
    while (p->suiv != NULL) {
        if (p->suiv->nbObs < seuilMin) {
            tyEspece *tmp = p->suiv;
            p->suiv = tmp->suiv;
            free(tmp);
        } else {
            p = p->suiv;
        }
    }
    return registre;
}

/* =====================================================================
 * TESTS — chacun est autonome (données locales, pas de fixture partagée)
 * ===================================================================== */

/* -------------------------------------------------------------------
 * Q1 — EstValide
 * Mnémo : "1 to N = VALID"
 * ------------------------------------------------------------------- */
static void test_estvalide_valeurs_limites(void **state) {
    (void)state;

    /* Valeurs valides : borne inférieure, supérieure, milieu */
    assert_int_equal(EstValide(1),           1);
    assert_int_equal(EstValide(NB_ESPECES),  1);
    assert_int_equal(EstValide(4),           1);

    /* Valeurs invalides : 0, NB_ESPECES+1, négatif */
    assert_int_equal(EstValide(0),               0);
    assert_int_equal(EstValide(NB_ESPECES + 1),  0);
    assert_int_equal(EstValide(-1),              0);
}

/* -------------------------------------------------------------------
 * Q1 — CompterOccurrences
 * Mnémo : "SCAN + COMPTE les égaux"
 * Relevé exemple du sujet : 3 1 5 3 1 3 7 5 1 3
 * ------------------------------------------------------------------- */
static void test_compter_occurrences_releve_exemple(void **state) {
    (void)state;
    int releve[] = {3, 1, 5, 3, 1, 3, 7, 5, 1, 3};
    int n = 10;

    assert_int_equal(CompterOccurrences(releve, n, 3), 4); /* espèce 3 → 4× */
    assert_int_equal(CompterOccurrences(releve, n, 1), 3); /* espèce 1 → 3× */
    assert_int_equal(CompterOccurrences(releve, n, 5), 2); /* espèce 5 → 2× */
    assert_int_equal(CompterOccurrences(releve, n, 7), 1); /* espèce 7 → 1× */
    assert_int_equal(CompterOccurrences(releve, n, 2), 0); /* espèce 2 → absente */
}

/* -------------------------------------------------------------------
 * Q4 — AnalyserReleve : résultat général
 * Mnémo : "RETOUR = n, *p = résultat"
 * Rappel syntaxe d'appel : AnalyserReleve(r, n, &nbDistinct, &codeMax)
 *                                                ^            ^
 *                                          adresse de   adresse de
 * ------------------------------------------------------------------- */
static void test_analyser_releve_releve_exemple(void **state) {
    (void)state;
    int releve[] = {3, 1, 5, 3, 1, 3, 7, 5, 1, 3};
    int nbDistinct = 0, codeMax = 0;

    int ret = AnalyserReleve(releve, 10, &nbDistinct, &codeMax);

    assert_int_equal(ret,        10); /* retourne n */
    assert_int_equal(nbDistinct,  4); /* espèces 1, 3, 5, 7 */
    assert_int_equal(codeMax,     3); /* espèce 3 : 4 observations */
}

/* -------------------------------------------------------------------
 * Q4 — AnalyserReleve : égalité → plus petit code
 * Mnémo : "Ex aequo → le plus PETIT code gagne"
 * ------------------------------------------------------------------- */
static void test_analyser_releve_egalite_plus_petit_code(void **state) {
    (void)state;
    /* codes 1 et 2 observés autant de fois → codeMax = 1 */
    int releve[] = {1, 2, 1, 2};
    int nbDistinct = 0, codeMax = 0;

    AnalyserReleve(releve, 4, &nbDistinct, &codeMax);

    assert_int_equal(nbDistinct, 2);
    assert_int_equal(codeMax,    1); /* 1 < 2, même fréquence */
}

/* -------------------------------------------------------------------
 * Q5 — InsererTrie : ordre croissant + doublon
 * Mnémo : "CHERCHE → CRÉE → CHAÎNE"
 *   Insertion : en tête (code plus petit), au milieu, en queue.
 *   Doublon : même code → nbObs incrémenté, pas de nouveau nœud.
 * ------------------------------------------------------------------- */
static void test_inserer_trie_ordre_et_doublon(void **state) {
    (void)state;
    tyEspece *reg = NULL;

    reg = InsererTrie(reg, 3);   /* liste : 3 */
    reg = InsererTrie(reg, 1);   /* insertion en tête → 1 → 3 */
    reg = InsererTrie(reg, 5);   /* insertion en queue → 1 → 3 → 5 */
    reg = InsererTrie(reg, 3);   /* doublon : nbObs(3) passe à 2 */

    /* Vérifier l'ordre et les compteurs */
    assert_non_null(reg);
    assert_int_equal(reg->code,  1);
    assert_int_equal(reg->nbObs, 1);

    assert_non_null(reg->suiv);
    assert_int_equal(reg->suiv->code,  3);
    assert_int_equal(reg->suiv->nbObs, 2);   /* doublon comptabilisé */

    assert_non_null(reg->suiv->suiv);
    assert_int_equal(reg->suiv->suiv->code,  5);
    assert_int_equal(reg->suiv->suiv->nbObs, 1);

    assert_null(reg->suiv->suiv->suiv);       /* dernier suiv == NULL */

    DetruireRegistre(reg);
}

/* -------------------------------------------------------------------
 * Q5 — InsererTrie : relevé complet du sujet
 * Produit : 1(×3) → 3(×4) → 5(×2) → 7(×1) → NULL
 * ------------------------------------------------------------------- */
static void test_inserer_trie_releve_complet(void **state) {
    (void)state;
    int releve[] = {3, 1, 5, 3, 1, 3, 7, 5, 1, 3};
    tyEspece *reg = NULL;
    for (int i = 0; i < 10; i++) reg = InsererTrie(reg, releve[i]);

    int codes_attendus[] = {1, 3, 5, 7};
    int nbObs_attendus[] = {3, 4, 2, 1};
    tyEspece *p = reg;
    for (int i = 0; i < 4; i++) {
        assert_non_null(p);
        assert_int_equal(p->code,  codes_attendus[i]);
        assert_int_equal(p->nbObs, nbObs_attendus[i]);
        p = p->suiv;
    }
    assert_null(p);  /* fin de liste */

    DetruireRegistre(reg);
}

/* -------------------------------------------------------------------
 * Q6 — NbEspeces
 * Mnémo : "WALK the list, COUNT nodes"
 * Cas limites : liste vide → 0
 * ------------------------------------------------------------------- */
static void test_nb_especes(void **state) {
    (void)state;
    tyEspece *reg = NULL;

    assert_int_equal(NbEspeces(reg), 0);  /* liste vide */

    reg = InsererTrie(reg, 1);
    reg = InsererTrie(reg, 3);
    reg = InsererTrie(reg, 5);
    reg = InsererTrie(reg, 7);

    assert_int_equal(NbEspeces(reg), 4);

    DetruireRegistre(reg);
}

/* -------------------------------------------------------------------
 * Q7 — DetruireRegistre
 * Mnémo : "SAVE suiv → FREE → AVANCE → retourne NULL"
 * Idiome d'appel : reg = DetruireRegistre(reg);
 * ------------------------------------------------------------------- */
static void test_detruire_registre_retourne_null(void **state) {
    (void)state;
    tyEspece *reg = NULL;
    reg = InsererTrie(reg, 2);
    reg = InsererTrie(reg, 4);

    reg = DetruireRegistre(reg);

    assert_null(reg);   /* toujours NULL → safe à réutiliser */
}

/* -------------------------------------------------------------------
 * Q11 — FiltrerRares : suppression en milieu/queue
 * Mnémo : "nbObs < seuilMin → DELETE + free, sinon SKIP"
 * Relevé : 1(×3) → 3(×4) → 5(×2) → 7(×1)   seuilMin=2 → 7 supprimé
 * ------------------------------------------------------------------- */
static void test_filtrer_rares_supprime_en_queue(void **state) {
    (void)state;
    int releve[] = {3, 1, 5, 3, 1, 3, 7, 5, 1, 3};
    tyEspece *reg = NULL;
    for (int i = 0; i < 10; i++) reg = InsererTrie(reg, releve[i]);

    /* seuilMin = 2 : supprime code 7 (nbObs=1 < 2) */
    reg = FiltrerRares(reg, 2);

    assert_non_null(reg);
    assert_int_equal(reg->code,              1);
    assert_int_equal(reg->suiv->code,        3);
    assert_int_equal(reg->suiv->suiv->code,  5);
    assert_null(reg->suiv->suiv->suiv);       /* code 7 supprimé */

    DetruireRegistre(reg);
}

/* -------------------------------------------------------------------
 * Q11 — FiltrerRares : suppression de la tête
 * Mnémo : "La tête peut aussi être rare → boucle while séparée"
 * ------------------------------------------------------------------- */
static void test_filtrer_rares_supprime_tete(void **state) {
    (void)state;
    tyEspece *reg = NULL;
    reg = InsererTrie(reg, 1);         /* code 1 → nbObs=1 */
    reg = InsererTrie(reg, 3);
    reg = InsererTrie(reg, 3);         /* code 3 → nbObs=2 */

    /* seuilMin=2 : la tête (code 1, nbObs=1) doit être supprimée */
    reg = FiltrerRares(reg, 2);

    assert_non_null(reg);
    assert_int_equal(reg->code, 3);
    assert_null(reg->suiv);

    DetruireRegistre(reg);
}

/* -------------------------------------------------------------------
 * Q11 — FiltrerRares : tout supprimé → NULL
 * ------------------------------------------------------------------- */
static void test_filtrer_rares_tout_supprime(void **state) {
    (void)state;
    tyEspece *reg = NULL;
    reg = InsererTrie(reg, 1);   /* nbObs=1 */
    reg = InsererTrie(reg, 2);   /* nbObs=1 */

    reg = FiltrerRares(reg, 5);  /* seuilMin=5 → tout supprimé */

    assert_null(reg);
}

/* -------------------------------------------------------------------
 * RAPPEL — Passage par adresse (pointeurs de sortie)
 * Mnémo : "& pour donner l'adresse, * pour écrire dedans"
 *
 *   int x;
 *   f(&x);          ← côté appelant : on donne l'adresse
 *   void f(int *p) { *p = 42; }   ← côté fonction : on écrit via *
 * ------------------------------------------------------------------- */
static void test_rappel_passage_par_adresse(void **state) {
    (void)state;
    int releve[] = {1, 1, 2};
    int nbDistinct = -1, codeMax = -1;

    /* nbDistinct et codeMax sont remplis VIA LES POINTEURS par AnalyserReleve */
    AnalyserReleve(releve, 3, &nbDistinct, &codeMax);

    assert_int_equal(nbDistinct, 2);  /* codes 1 et 2 */
    assert_int_equal(codeMax,    1);  /* code 1 vu 2 fois */
}

/* -------------------------------------------------------------------
 * RAPPEL — Structure d'une liste triée après relevé complet du sujet
 * Mnémo : "tête → nœud → nœud → NULL  (ordre croissant des codes)"
 * ------------------------------------------------------------------- */
static void test_rappel_structure_liste_triee(void **state) {
    (void)state;
    int releve[] = {3, 1, 5, 3, 1, 3, 7, 5, 1, 3};
    tyEspece *reg = NULL;
    for (int i = 0; i < 10; i++) reg = InsererTrie(reg, releve[i]);

    /* Attendu : (1,3) → (3,4) → (5,2) → (7,1) → NULL */
    int codes[] = {1, 3, 5, 7};
    int nbObs[] = {3, 4, 2, 1};
    tyEspece *p = reg;
    for (int i = 0; i < 4; i++) {
        assert_non_null(p);
        assert_int_equal(p->code,  codes[i]);
        assert_int_equal(p->nbObs, nbObs[i]);
        p = p->suiv;
    }
    assert_null(p);

    DetruireRegistre(reg);
}

/* =====================================================================
 * main
 * ===================================================================== */
int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_estvalide_valeurs_limites),
        cmocka_unit_test(test_compter_occurrences_releve_exemple),
        cmocka_unit_test(test_analyser_releve_releve_exemple),
        cmocka_unit_test(test_analyser_releve_egalite_plus_petit_code),
        cmocka_unit_test(test_inserer_trie_ordre_et_doublon),
        cmocka_unit_test(test_inserer_trie_releve_complet),
        cmocka_unit_test(test_nb_especes),
        cmocka_unit_test(test_detruire_registre_retourne_null),
        cmocka_unit_test(test_filtrer_rares_supprime_en_queue),
        cmocka_unit_test(test_filtrer_rares_supprime_tete),
        cmocka_unit_test(test_filtrer_rares_tout_supprime),
        cmocka_unit_test(test_rappel_passage_par_adresse),
        cmocka_unit_test(test_rappel_structure_liste_triee),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
