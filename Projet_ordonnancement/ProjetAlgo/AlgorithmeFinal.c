#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#define NB_TASKS 5
#define NB_LIGNE NB_TASKS + 1
/*
* --------------------------------
* -                              -
* -           WARNING            -
* -                              -
* --------------------------------
*
* Il faut rajouter a la main un ou pls *2 suivant le nombre de tache
* Impossible d'utiliser la fonction pow(2, NB_TASKS) pour definir la taille d'un tableau
* Erreur compilateur
* Au dela de 12 : Erreur Stack Overflow
*/
#define NB_COLONNE 2*2*2*2*2//(int)pow(2, NB_TASKS) = 2^NB_TASKS

typedef struct Etat Etat;
struct Etat
{
    double dureeOpeM2;
    double Lmax;
};

typedef struct Tache Tache;
struct Tache
{
    double dureeOpe;
    double dureeLatence;
    int id;
};

double sommeDureeOperatoire(Tache* tacheArray, int nbTaches)
{
    double sommeDureesOperatoires = 0;

    for (int i = 0; i < nbTaches; i++) {
        sommeDureesOperatoires += tacheArray[i].dureeOpe;
    }

    return sommeDureesOperatoires;
}

double findMinFromArray(Tache* array, int size)
{
    int min = array[0].dureeLatence;
    for (int i = 1; i < size; i++)
        if (array[i].dureeLatence < min)
            min = array[i].dureeLatence;

    return min;
}

double* copieArray(double* array, int size)
{
    static double dest[NB_TASKS];
    for (int i = 0; i < size; i++)
    {
        dest[i] = array[i];
    }
    return dest;
}

void triDecroissant(double* array, int size)
{
    int i, j, a;
    for (i = 0; i < size; ++i)
    {

        for (j = i + 1; j < size; ++j)
        {
            if (array[i] < array[j])
            {
                a = array[i];
                array[i] = array[j];
                array[j] = a;
            }
        }
    }
}

void triDecroissantDureeLatence(Tache* array, int size)
{
    int i, j;
    Tache t;
    for (i = 0; i < size; ++i)
    {

        for (j = i + 1; j < size; ++j)
        {
            if (array[i].dureeLatence < array[j].dureeLatence)
            {
                t.dureeLatence = array[i].dureeLatence;
                t.dureeOpe = array[i].dureeOpe;
                t.id = array[i].id;

                array[i].dureeLatence = array[j].dureeLatence;
                array[i].dureeOpe = array[j].dureeOpe;
                array[i].id = array[j].id;

                array[j].dureeLatence = t.dureeLatence;
                array[j].dureeOpe = t.dureeOpe;
                array[j].id = t.id;
            }
        }
    }
}

bool etatIsInitialize(Etat etat) { return !(etat.dureeOpeM2 == -1 && etat.Lmax == -1); }

Etat findMinLmaxFromArray(Etat* array, int size)
{
    Etat min;
    //On defnit un Etat minimum qui est initialise
    for (int i = 0; i < size; i++)
    {
        if (etatIsInitialize(array[i]))
            min = array[i];
    }

    //Parcours du tableau et effectue les comparaisons
    for (int j = 0; j < size; j++)
    {
        if (array[j].Lmax < min.Lmax && etatIsInitialize(array[j]))
            min = array[j];
    }

    return min;

}

void keepOnlyLmaxMinFromSameDureeOp(Etat* array, int indexEtat, int size)
{

    for (int i = 0; i < size; i++)
    {
        if (!etatIsInitialize(array[i]) || i == indexEtat)
            continue;
        if (array[i].dureeOpeM2 == array[indexEtat].dureeOpeM2)
        {
            if (array[indexEtat].Lmax <= array[i].Lmax)
            {
                array[i].dureeOpeM2 = -1;
                array[i].Lmax = -1;
            }
            else
            {
                array[indexEtat].dureeOpeM2 = -1;
                array[indexEtat].Lmax = -1;
                indexEtat = i;
            }
        }
    }
}

void keepOnlyOneLmaxMin(Etat* array, int indexEtat, int size)
{
    for (int i = 0; i < size; i++)
    {
        if (!etatIsInitialize(array[i]) || i == indexEtat)
            continue;

        if (array[indexEtat].Lmax <= array[i].Lmax)
        {
            array[i].dureeOpeM2 = -1;
            array[i].Lmax = -1;
        }
        else
        {
            array[indexEtat].dureeOpeM2 = -1;
            array[indexEtat].Lmax = -1;
            indexEtat = i;
        }
    }
}

void printEnsembleEtat(Etat ensembleEtat[NB_TASKS][NB_COLONNE])
{
    printf("L'ensemble d'etat selon l'algorithme greedy est le suivant :\n\n");
    for (int i = 0; i <= NB_TASKS; i++) {
        printf("E(%d) = [ ", i);
        for (int j = 0; j < NB_COLONNE; j++) {
            if (!etatIsInitialize(ensembleEtat[i][j]))
                continue;
            printf(" { %g", ensembleEtat[i][j].dureeOpeM2);
            printf(" %g } ", ensembleEtat[i][j].Lmax);
        }
        printf(" ]\n\n");
    }
    printf("\nL'etat avec le plus petit Lmax est : \n\t { %g , %g }", findMinLmaxFromArray(ensembleEtat[NB_TASKS], NB_COLONNE).dureeOpeM2, findMinLmaxFromArray(ensembleEtat[NB_TASKS], NB_COLONNE).Lmax);

}

void printOrdreJackson(Tache* tacheArray)
{
    printf("\nUne tache est sous la forme : \n\t{ dureeOperatoire, dureeLatence, id }\n\n");
    printf("L'agencement des taches selon l'ordre de Jackson est le suivant :\n\n");
    printf("[ ");
    for (int i = 0; i < NB_TASKS; ++i) {
        printf(" { %g, %g, %d } ", tacheArray[i].dureeOpe, tacheArray[i].dureeLatence, tacheArray[i].id);
    }
    printf(" ]\n\n");
}

void heuristique(Tache* tacheArray, int nbTaches)
{

    triDecroissantDureeLatence(tacheArray, nbTaches);
    printOrdreJackson(tacheArray);
    printf("Lmax = %g\n", sommeDureeOperatoire(tacheArray, NB_TASKS) + findMinFromArray(tacheArray, NB_TASKS));
}

void greedy(Tache* tacheArray, int nbTaches)
{
    int Ai = 0;
    int nbEtats = 1;
    int indexColonne;

    triDecroissantDureeLatence(tacheArray, nbTaches);

    printOrdreJackson(tacheArray);

    //Array a 2 dimension avec :
    //      MAX_SIZE + 1 lignes
    //      MAX_SIZE^2 colonnes

    Etat ensembleEtat[NB_LIGNE][NB_COLONNE];

    //Initialisation de l'ensemble des etats a -1
    //Pour la logique metier cela veut dire que cese?tats sont consideres comme etant NULL
    for (int i = 0; i < NB_TASKS + 1; i++) {
        for (int j = 0; j < NB_COLONNE; j++) {
            ensembleEtat[i][j].dureeOpeM2 = -1;
            ensembleEtat[i][j].Lmax = -1;
        }
    }

    //Initialisation de l'etat inital E(0)
    //E(0) = {0,0}
    ensembleEtat[0][0].dureeOpeM2 = 0;
    ensembleEtat[0][0].Lmax = 0;

    for (int i = 1; i <= nbTaches; i++) {

        Ai += tacheArray[i - 1].dureeOpe;

        indexColonne = 0;
        for (int j = 0; j < nbEtats; j++) {

            if (!etatIsInitialize(ensembleEtat[i - 1][j]))
                continue;
            double t = ensembleEtat[i - 1][j].dureeOpeM2; //On modifie la valeur de t lorsque l'on incremente j, c'est a dire quand on change de tache
            double f = ensembleEtat[i - 1][j].Lmax;       //Idem pour la valeur de f

            //Creation 1ere Etat | ajout sur M1
            ensembleEtat[i][indexColonne].dureeOpeM2 = t; //On place la tache sur M1 donc t ne change pas
            ensembleEtat[i][indexColonne].Lmax = fmax(((Ai - t) / 4 + tacheArray[i - 1].dureeLatence), f); //Max{ (Ai - t)/4 + qi , f }

            //On garde les Etat avec un Lmax minimum (uniquement si DureeOp est similaire entre les Etat) en comparant le 1er Etat qui vient d'etre ajoute a ceux du tableau d'Etat
            keepOnlyLmaxMinFromSameDureeOp(ensembleEtat[i], indexColonne, nbEtats * 2);



            //Cr?ation 2eme Etat | ajout sur M2
            ensembleEtat[i][indexColonne + 1].dureeOpeM2 = t + tacheArray[i - 1].dureeOpe; //t devient t + Ai //ICI BUG
            ensembleEtat[i][indexColonne + 1].Lmax = fmax(t + tacheArray[i - 1].dureeOpe + tacheArray[i - 1].dureeLatence, f);

            //On garde les Etat avec un Lmax minimum (uniquement si DureeOp est similaire entre les Etat) en comparant le 2eme Etat qui vient d'etre ajoute a ceux du tableau d'Etat
            keepOnlyLmaxMinFromSameDureeOp(ensembleEtat[i], indexColonne + 1, nbEtats * 2);

            indexColonne += 2;
        }
        nbEtats *= 2;
    }

    printf("\nUn etat est sous la forme : \n\t{ DureeOperatoireM2, Lmax }\n\n");
    printEnsembleEtat(ensembleEtat);
}

void fptas(Tache* tacheArray, int nbTaches)
{
    int Ai = 0;
    int nbEtats = 1;
    int indexColonne;

    triDecroissantDureeLatence(tacheArray, nbTaches);
    printOrdreJackson(tacheArray);

    //Array a 2 dimension avec :
    //      MAX_SIZE + 1 lignes
    //      MAX_SIZE^2 colonnes

    Etat ensembleEtat[NB_LIGNE][NB_COLONNE];

    //Initialisation de l'ensemble des etats a -1
    //Pour la logique metier cela veut dire que ces etats sont consideres comme etant NULL
    for (int i = 0; i < NB_TASKS + 1; i++) {
        for (int j = 0; j < NB_COLONNE; j++) {
            ensembleEtat[i][j].dureeOpeM2 = -1;
            ensembleEtat[i][j].Lmax = -1;
        }
    }

    //Initialisation de l'etat inital E(0)
    //E(0) = {0,0}
    ensembleEtat[0][0].dureeOpeM2 = 0;
    ensembleEtat[0][0].Lmax = 0;

    for (int i = 1; i <= nbTaches; i++) {

        Ai += tacheArray[i - 1].dureeOpe;

        indexColonne = 0;
        for (int j = 0; j < nbEtats; j++) {

            if (!etatIsInitialize(ensembleEtat[i - 1][j]))
                continue;

            double t = ensembleEtat[i - 1][j].dureeOpeM2; //On modifie la valeur de t lorsque l'on incremente j, c'est a dire quand on change de tache
            double f = ensembleEtat[i - 1][j].Lmax;       //Idem pour la valeur de f

            //Creation 1ere Etat | ajout sur M1
            ensembleEtat[i][indexColonne].dureeOpeM2 = t; //On place la tache sur M1 donc t ne change pas
            ensembleEtat[i][indexColonne].Lmax = fmax(((Ai - t) / 4 + tacheArray[i - 1].dureeLatence), f); //Max{ (Ai - t)/4 + qi , f }

            //On garde un unique Etat avec un Lmax minimum en comparant le 1er Etat qui vient d'etre ajoute a ceux du tableau d'Etat
            keepOnlyOneLmaxMin(ensembleEtat[i], indexColonne, nbEtats * 2);



            //Creation 2eme Etat | ajout sur M2
            ensembleEtat[i][indexColonne + 1].dureeOpeM2 = t + tacheArray[i - 1].dureeOpe; //t devient t + Ai //ICI BUG
            ensembleEtat[i][indexColonne + 1].Lmax = fmax(t + tacheArray[i - 1].dureeOpe + tacheArray[i - 1].dureeLatence, f);

            //On garde un unique Etat avec un Lmax minimum en comparant le 2eme Etat qui vient d'etre ajoute a ceux du tableau d'Etat
            keepOnlyOneLmaxMin(ensembleEtat[i], indexColonne, nbEtats * 2);

            indexColonne += 2;
        }
        nbEtats *= 2;
    }
    printf("\nUn etat est sous la forme : \n\t{ DureeOperatoireM2, Lmax }\n\n");
    printEnsembleEtat(ensembleEtat);
}

void showRes(Tache* tacheArray, int nbTaches)
{
    clock_t t;
    double time_taken;

    //Resultat H()
    printf("\n----- Heuristique -----\n");
    t = clock();
    heuristique(tacheArray, nbTaches);
    t = clock() - t;
    time_taken = ((double)t) / CLOCKS_PER_SEC; // in seconds
    printf("\nTemps d'execution de heuristique() en sec :\n\t%f", time_taken);
    printf("\n\n\n");

    //Resultat Optimal()
    printf("\n----- Greedy -----\n");
    t = clock();
    greedy(tacheArray, nbTaches);
    t = clock() - t;
    time_taken = ((double)t) / CLOCKS_PER_SEC; // in seconds
    printf("\nTemps d'execution de greedy() en sec :\n\t%f", time_taken);
    printf("\n\n\n");

    //Resultat FPTAS()
    printf("\n----- Fptas -----\n");
    t = clock();
    fptas(tacheArray, nbTaches);
    t = clock() - t;
    time_taken = ((double)t) / CLOCKS_PER_SEC; // in seconds
    printf("\nTemps d'execution de fptas() en sec :\n\t%f", time_taken);
    printf("\n\n\n");
}

int main()
{
    srand(time(NULL));
    double dureeOperatoire[NB_TASKS];
    double dureeLatence[NB_TASKS];

    //Pour tester avec des taches aleatoires
    for (int i = 0; i < NB_TASKS; i++)
    {
        /* random int between 0 and 100 */
        dureeOperatoire[i] = rand() % 100;
        dureeLatence[i] = rand() % 100;
    }

    //Valeur statique
    /*dureeOperatoire[0] = 10;
    dureeOperatoire[1] = 10;
    dureeOperatoire[2] = 20;
    dureeOperatoire[3] = 8;

    dureeLatence[0] = 3;
    dureeLatence[1] = 15;
    dureeLatence[2] = 2;
    dureeLatence[3] = 9;*/

    Tache* tacheArray = (Tache*)malloc(NB_TASKS * sizeof(Tache));
    for (int i = 0; i < NB_TASKS; i++)
    {
        tacheArray[i].dureeOpe = dureeOperatoire[i];
        tacheArray[i].dureeLatence = dureeLatence[i];
        tacheArray[i].id = i;
    }
    showRes(tacheArray, NB_TASKS);

    free(tacheArray);

    return 0;
}