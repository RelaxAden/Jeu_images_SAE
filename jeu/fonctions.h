#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct Association
{
    char francais[255];
    char anglais[255];
    char chemin[255];
}Association;

//prototypes des fonctions permettant la manipulation des structures de données
void remplirTab(FILE *F, char tab[][255], int nombreLignes); 

int fonctionHachage(char *mot, int taille); 

void remplirTable(FILE *F, Association **table, int nombreLignes, int taille); 

//prototypes des fonctions nécessaires au fonctionnement du jeu
void Reinitialiser(char *chaine); 

void PasserUneLigne(FILE *F); 

int NombreDeLignes(FILE *F); 

void ChoixLangue(SDL_Renderer *renderer, int *langue); 

void ReponseJoueur(int langue, Association Image, int *Pscore, int *Ptotal, SDL_Renderer *renderer); 

int TraiterReponse(char *reponse, int langue, Association Image, int *Pscore, int *Ptotal, SDL_Renderer *renderer); 

int Rejouer(SDL_Renderer *renderer, int score, int total); 

int AssociationAleatoire(Association **table, int taille);

void afficherFond(SDL_Renderer *renderer);

void EffacerSaisie(SDL_Renderer *renderer);

int ChoixJoueur(SDL_Renderer *renderer);

void afficherScore(SDL_Renderer *renderer, int score, int total);

int choixImage(SDL_Renderer *renderer, Association **table, int taille);