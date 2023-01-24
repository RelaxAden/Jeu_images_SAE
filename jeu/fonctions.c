#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "SDL2/SDL_ttf.h"

#include "fonctions.h"

void afficherScore(SDL_Renderer *renderer, int score, int total) //fonction permettant d'afficher le score
{
	char scoreChaine[255];
	sprintf(scoreChaine, "Score: %d/%d", score, total);
	TTF_Font *font = TTF_OpenFont("polices/256BYTES.TTF", 50);
	SDL_Color couleurAvant = {0, 0, 0};
	SDL_Color couleurArriere = {255, 255, 255};
	SDL_Surface *surfTexte = TTF_RenderUTF8_Shaded(font, scoreChaine, couleurAvant, couleurArriere);
	SDL_Texture *texteTexte = SDL_CreateTextureFromSurface(renderer, surfTexte);
	SDL_Rect rectTexte = {500, 0, 140, 50};
	SDL_RenderCopy(renderer, texteTexte, NULL, &rectTexte);
	SDL_RenderPresent(renderer);
}

void Reinitialiser(char *chaine) //fonction permettant de "remettre à zéro" une chaine de caractères, utilisation char nom[] et pas char *
{
    chaine[0]='\0';
}

void PasserUneLigne(FILE *F) //fonction permettant de passer une ligne dans un fichier, le fichier doit être ouvert en lecture
{
    char c;
    while (c != '\n')
    {
        c = fgetc(F);
    }
    
}

int NombreDeLignes(FILE *F) //fonction retournant le nombre d'images
{
    char c;
    int cpt = 0;        
    fseek(F, 0, SEEK_SET); 
    PasserUneLigne(F);
    while ((c = fgetc(F)) != EOF)
    {
        if(c == '\n')
            cpt++;
    }
    return cpt;
}

void afficherFond(SDL_Renderer *renderer) //Fonction permettant d'afficher / réafficher le fond
{
    SDL_Surface *Sfond = NULL;
    SDL_Texture *fond = NULL;
    Sfond = IMG_Load("images/ressources/fond.jpg");
    if(Sfond == NULL)
	{
		fprintf(stderr, "Erreur Sfond : %s\n", SDL_GetError());
		goto Quit;
	}
    fond = SDL_CreateTextureFromSurface(renderer, Sfond);
    if(fond == NULL)
	{
		fprintf(stderr, "Erreur SDL_CreateTexture fond: %s\n", SDL_GetError());
		goto Quit;
	}
    SDL_Rect rectFond = {0, 0, 640, 480};
    SDL_RenderCopy(renderer, fond, NULL, &rectFond);
    SDL_RenderPresent(renderer);

Quit:
    if(Sfond != NULL)
		SDL_FreeSurface(Sfond);
	if(fond!= NULL)
		SDL_DestroyTexture(fond);
}

void ChoixLangue(SDL_Renderer *renderer, int *langue) //fonction retournant un entier correspondant à la langue choisie
{
    SDL_Surface *Bfr = NULL;
	SDL_Surface *Ben = NULL;
	SDL_Texture *fr = NULL;
	SDL_Texture *en = NULL;
    TTF_Font *font = TTF_OpenFont("polices/256BYTES.TTF", 100);
    SDL_Color couleur = {255, 255, 255};
    SDL_Surface *surfTexte = TTF_RenderText_Blended(font, "Choisissez la langue:", couleur);
    SDL_Texture *texteTexte = SDL_CreateTextureFromSurface(renderer, surfTexte);
    SDL_Rect rectTexte = {175, 30, 265, 100};
    SDL_RenderCopy(renderer, texteTexte, NULL, &rectTexte);
    SDL_RenderPresent(renderer);
	Bfr = IMG_Load("images/ressources/fr.png");
	Ben = IMG_Load("images/ressources/en.png");
	if(Bfr == NULL)
	{
		fprintf(stderr, "Erreur Bfr : %s\n", SDL_GetError());
		goto Quit;
	}
	if(Ben == NULL)
	{
		fprintf(stderr, "Erreur Ben : %s\n", SDL_GetError());
		goto Quit;
	}
	fr = SDL_CreateTextureFromSurface(renderer, Bfr);
	en = SDL_CreateTextureFromSurface(renderer, Ben);
	if(fr == NULL)
	{
		fprintf(stderr, "Erreur SDL_CreateTexture : %s\n", SDL_GetError());
		goto Quit;
	}
	if(en == NULL)
	{
		fprintf(stderr, "Erreur SDL_CreateTexture : %s\n", SDL_GetError());
		goto Quit;
	}
	SDL_Rect rectFR = {50, 140, 200, 200};
	SDL_Rect rectEN = {350, 140, 200, 200};
	SDL_RenderCopy(renderer, fr, NULL, &rectFR);
	SDL_RenderCopy(renderer, en, NULL, &rectEN);
	SDL_RenderPresent(renderer);
	SDL_bool boolLangue = SDL_FALSE;
	SDL_Event eventLangue;
	while (!boolLangue)
	{
		SDL_WaitEvent(&eventLangue);
		if(eventLangue.type == SDL_MOUSEBUTTONDOWN)
		{
			if(eventLangue.button.x>=rectFR.x && eventLangue.button.x<rectFR.x+200 && eventLangue.button.y>=rectFR.y && eventLangue.button.y+200)
			{
				*langue = 1;
                afficherFond(renderer);
				boolLangue = SDL_TRUE;
			}
			if(eventLangue.button.x>=rectEN.x && eventLangue.button.x<rectEN.x+200 && eventLangue.button.y>=rectEN.y && eventLangue.button.y+200)
			{
				*langue = 2;
                afficherFond(renderer);
				boolLangue = SDL_TRUE;
			}
		}
	}
Quit:
	if(Bfr != NULL)
		SDL_FreeSurface(Bfr);
	if(Ben != NULL)
		SDL_FreeSurface(Ben);
	if(fr != NULL)
		SDL_DestroyTexture(fr);
	if(en != NULL)
		SDL_DestroyTexture(en);
}

int TraiterReponse(char *reponse, int langue, Association Image, int *Pscore, int *Ptotal, SDL_Renderer *renderer) //fonction retournant si le joueur à bien répondu
{
    if (langue == 1)
    {
        if(strcmp(reponse, Image.francais) == 0)
        {
            (*Pscore)++;
            (*Ptotal)++;
            printf("C'est gagné !\n");
            return 1;
        }
        else
        {
            (*Ptotal)++;
            printf("C'est perdu ! La réponse était : %s\n", Image.francais);
            return 0;
        }
    }
    else if (langue == 2)
    {
        if(strcmp(reponse, Image.anglais) == 0)
        {
            (*Pscore)++;
            (*Ptotal)++;
            printf("C'est gagné !\n");
            return 1;
        }
        else
        {
            (*Ptotal)++;
            printf("C'est perdu ! La réponse était : %s\n", Image.anglais);
            return 0;
        }
    }
    else
    {
        fprintf(stderr, "Erreur: langue non-valide !\n");
        return EXIT_FAILURE;
    }
}

void EffacerSaisie(SDL_Renderer *renderer) //Fonction permettant de "réinitialiser" la saisie (ne sert pas)
{
    SDL_Rect saisie = {50, 320, 540, 100};
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &saisie);
	SDL_RenderPresent(renderer);
}

void ReponseJoueur(int langue, Association Image, int *Pscore, int *Ptotal, SDL_Renderer *renderer) //fonction qui récupère la réponse du joueur
{
    SDL_Surface *Bvalider = NULL;
    SDL_Texture *valider = NULL;
    Bvalider = IMG_Load("images/ressources/valider.png");
    if(Bvalider == NULL)
	{
		fprintf(stderr, "Erreur Bvalider : %s\n", SDL_GetError());
		goto Quit;
	}
    valider = SDL_CreateTextureFromSurface(renderer, Bvalider);
    if(valider == NULL)
	{
		fprintf(stderr, "Erreur SDL_CreateTexture : %s\n", SDL_GetError());
		goto Quit;
	}
    SDL_Rect rectValider = {450, 140, 140, 140};
    SDL_RenderCopy(renderer, valider, NULL, &rectValider);
    SDL_RenderPresent(renderer);

    char reponse[255];
    printf("Entrez votre réponse\n");
    scanf("%s", reponse);
    TTF_Font *font = TTF_OpenFont("polices/256BYTES.TTF", 40);
    SDL_Color couleur = {0, 0, 0};
    SDL_Surface *surfTexte = TTF_RenderUTF8_Blended(font, reponse, couleur); //UTF8 pour les accents
    SDL_Texture *texteTexte = SDL_CreateTextureFromSurface(renderer, surfTexte);
    SDL_Rect rectTexte = {50, 320, 540, 100};
    SDL_RenderCopy(renderer, texteTexte, NULL, &rectTexte);
    SDL_RenderPresent(renderer);

	SDL_bool boolReponse = SDL_FALSE;
	SDL_Event eventReponse;
	while (!boolReponse)
	{
		SDL_WaitEvent(&eventReponse);
		if(eventReponse.type == SDL_MOUSEBUTTONDOWN)
		{
			if(eventReponse.button.x>=rectValider.x && eventReponse.button.x<rectValider.x+140 && eventReponse.button.y>=rectValider.y && eventReponse.button.y+140)
			{
                boolReponse = SDL_TRUE;
				TraiterReponse(reponse, langue, Image, Pscore, Ptotal, renderer);
			}
		}
	}

Quit:
	if(Bvalider != NULL)
		SDL_FreeSurface(Bvalider);
	if(valider != NULL)
		SDL_DestroyTexture(valider);
}

int choixImage(SDL_Renderer *renderer, Association **table, int taille) //Fonction qui permet à un joueur de choisir l'image qu'il souhaite
{
	SDL_Surface *Bvalider = NULL;
    SDL_Texture *valider = NULL;
    Bvalider = IMG_Load("images/ressources/valider.png");
    if(Bvalider == NULL)
	{
		fprintf(stderr, "Erreur Bvalider : %s\n", SDL_GetError());
		goto Quit;
	}
    valider = SDL_CreateTextureFromSurface(renderer, Bvalider);
    if(valider == NULL)
	{
		fprintf(stderr, "Erreur SDL_CreateTexture : %s\n", SDL_GetError());
		goto Quit;
	}
    SDL_Rect rectValider = {450, 140, 140, 140};
    SDL_RenderCopy(renderer, valider, NULL, &rectValider);
    SDL_RenderPresent(renderer);

    char reponse[255];
    printf("Entrez le mot en français\n");
    scanf("%s", reponse);
    TTF_Font *font = TTF_OpenFont("polices/256BYTES.TTF", 40);
    SDL_Color couleur = {0, 0, 0};
    SDL_Surface *surfTexte = TTF_RenderUTF8_Blended(font, reponse, couleur); //UTF8 pour les accents
    SDL_Texture *texteTexte = SDL_CreateTextureFromSurface(renderer, surfTexte);
    SDL_Rect rectTexte = {50, 320, 540, 100};
    SDL_RenderCopy(renderer, texteTexte, NULL, &rectTexte);
    SDL_RenderPresent(renderer);

	SDL_bool boolReponse = SDL_FALSE;
	SDL_Event eventReponse;

	int indice; 
	while (!boolReponse)
	{
		SDL_WaitEvent(&eventReponse);
		if(eventReponse.type == SDL_MOUSEBUTTONDOWN)
		{
			if(eventReponse.button.x>=rectValider.x && eventReponse.button.x<rectValider.x+140 && eventReponse.button.y>=rectValider.y && eventReponse.button.y+140)
			{
				indice = fonctionHachage(reponse, taille);
				while(strcmp(table[indice]->francais,reponse) != 0)
				{
					indice++;
					if(indice == taille)
                	{
                    	indice = 0;
                	}
				}
                boolReponse = SDL_TRUE;
				return indice;
			}
		}
	}

Quit:
	if(Bvalider != NULL)
		SDL_FreeSurface(Bvalider);
	if(valider != NULL)
		SDL_DestroyTexture(valider);
}

int Rejouer(SDL_Renderer *renderer, int score, int total) //fonction permettant au joueur de rejouer si il le souhaite
{
    afficherFond(renderer);
    SDL_Surface *Bvalider = NULL;
	SDL_Surface *Brefuser = NULL;
	SDL_Texture *valider = NULL;
	SDL_Texture *refuser = NULL;
    TTF_Font *font = TTF_OpenFont("polices/256BYTES.TTF", 100);
    SDL_Color couleur = {255, 255, 255};
    SDL_Surface *surfTexte = TTF_RenderText_Blended(font, "Voulez-vous rejouer?", couleur);
    SDL_Texture *texteTexte = SDL_CreateTextureFromSurface(renderer, surfTexte);
    SDL_Rect rectTexte = {175, 30, 265, 100};
    SDL_RenderCopy(renderer, texteTexte, NULL, &rectTexte);
    SDL_RenderPresent(renderer);
	Bvalider = IMG_Load("images/ressources/valider.png");
	Brefuser = IMG_Load("images/ressources/refuser.png");
	if(Bvalider == NULL)
	{
		fprintf(stderr, "Erreur Bvalider : %s\n", SDL_GetError());
		goto Quit;
	}
	if(Brefuser == NULL)
	{
		fprintf(stderr, "Erreur Brefuser : %s\n", SDL_GetError());
		goto Quit;
	}
	valider = SDL_CreateTextureFromSurface(renderer, Bvalider);
	refuser = SDL_CreateTextureFromSurface(renderer, Brefuser);
	if(valider == NULL)
	{
		fprintf(stderr, "Erreur SDL_CreateTexture : %s\n", SDL_GetError());
		goto Quit;
	}
	if(refuser == NULL)
	{
		fprintf(stderr, "Erreur SDL_CreateTexture : %s\n", SDL_GetError());
		goto Quit;
	}
	SDL_Rect rectValider = {50, 140, 200, 200};
	SDL_Rect rectRefuser= {350, 140, 200, 200};
	SDL_RenderCopy(renderer, valider, NULL, &rectValider);
	SDL_RenderCopy(renderer, refuser, NULL, &rectRefuser);
	SDL_RenderPresent(renderer);
	afficherScore(renderer, score, total);
	SDL_bool boolChoix = SDL_FALSE;
	SDL_Event eventChoix;
	while (!boolChoix)
	{
		SDL_WaitEvent(&eventChoix);
		if(eventChoix.type == SDL_MOUSEBUTTONDOWN)
		{
			if(eventChoix.button.x>=rectValider.x && eventChoix.button.x<rectValider.x+200 && eventChoix.button.y>=rectValider.y && eventChoix.button.y+200)
			{
                afficherFond(renderer);
                boolChoix = SDL_TRUE;
				return 1;
			}
			if(eventChoix.button.x>=rectRefuser.x && eventChoix.button.x<rectRefuser.x+200 && eventChoix.button.y>=rectRefuser.y && eventChoix.button.y+200)
			{
                afficherFond(renderer);
                boolChoix = SDL_TRUE;
				return 0;
			}
		}
	}
Quit:
	if(Bvalider != NULL)
		SDL_FreeSurface(Bvalider);
	if(Brefuser != NULL)
		SDL_FreeSurface(Brefuser);
	if(valider != NULL)
		SDL_DestroyTexture(valider);
	if(refuser != NULL)
		SDL_DestroyTexture(refuser);
}

void remplirTab(FILE *F, char tab[][255], int nombreLignes) //fonction permettant de remplir le tableau de mots en français
{
    char chemin[255];
    char fr[255];
    char en[255];
    fseek(F, 0, SEEK_SET);
    PasserUneLigne(F);
    for (size_t i = 0; (i < nombreLignes) && (!feof(F)); i++)
    {
        Reinitialiser(chemin);
        Reinitialiser(fr);
        Reinitialiser(en);
        fscanf(F, "%[^;];%[^;];%[^;]\n", chemin, fr, en); //Récupérer chaque lignes formatées
        PasserUneLigne(F);
        strcpy(tab[i], fr);
    }
}

int fonctionHachage(char *mot, int taille) //fonction de hachage
{
    int cle = 0;
    for (size_t i = 0; i < strlen(mot); i++)
    {
        cle += mot[i];
    }
    cle %= taille;
    return cle;
}

void remplirTable(FILE *F, Association **table, int nombreLignes, int taille) //fonction permettant de remplir la table de hachage
{

    for (int i=0; i<(NombreDeLignes(F)*2)-1; i++) { //Initialiser la table à NULL
        table[i] = NULL;
    }
    char chemin[255];
    char fr[255];
    char en[255];
    int indice = 0;
    fseek(F, 0, SEEK_SET);
    PasserUneLigne(F);

    for (size_t i = 0; (i < nombreLignes) && (!feof(F)); i++)
    {
        Reinitialiser(chemin);
        Reinitialiser(fr);
        Reinitialiser(en);
        fscanf(F, "%[^;];%[^;];%[^;]\n", chemin, fr, en); //Récupérer chaque lignes formatées
        PasserUneLigne(F);
        indice = fonctionHachage(fr,taille);
        if (table[indice] == NULL)
        {
            table[indice] = (Association*)malloc(sizeof(Association));
            strcpy(table[indice]->chemin, chemin);
            strcpy(table[indice]->francais, fr);
            strcpy(table[indice]->anglais, en);
        }
        else //Gestion des collisions
        {
            while(table[indice] != NULL)
            {
                indice++;
                if(indice == taille)
                {
                    indice = 0;
                }
            }
            table[indice] = (Association*)malloc(sizeof(Association));
            strcpy(table[indice]->chemin, chemin);
            strcpy(table[indice]->francais, fr);
            strcpy(table[indice]->anglais, en);
        }
    }
}

int AssociationAleatoire(Association **table, int taille) //fonction permettant de sélectionner une association aléatoire dans la table remplie
{
    srand(time(NULL));
    int indice = rand() % taille;
    while (table[indice] == NULL)
    {
        indice = rand() % taille;
    }
    return indice;
}

int ChoixJoueur(SDL_Renderer *renderer) //fonction permettant de choisir le mode de jeu
{
    SDL_Surface *BJ1 = NULL;
	SDL_Surface *BJ2 = NULL;
	SDL_Texture *J1 = NULL;
	SDL_Texture *J2 = NULL;
    TTF_Font *font = TTF_OpenFont("polices/256BYTES.TTF", 100);
    SDL_Color couleur = {255, 255, 255};
    SDL_Surface *surfTexte = TTF_RenderText_Blended(font, "Choisissez le mode de jeu:", couleur);
    SDL_Texture *texteTexte = SDL_CreateTextureFromSurface(renderer, surfTexte);
    SDL_Rect rectTexte = {175, 30, 265, 100};
    SDL_RenderCopy(renderer, texteTexte, NULL, &rectTexte);
    SDL_RenderPresent(renderer);
	BJ1 = IMG_Load("images/ressources/UnJoueur.png");
	BJ2 = IMG_Load("images/ressources/DeuxJoueurs.png");
	if(BJ1 == NULL)
	{
		fprintf(stderr, "Erreur BJ1 : %s\n", SDL_GetError());
		goto Quit;
	}
	if(BJ2 == NULL)
	{
		fprintf(stderr, "Erreur BJ2 : %s\n", SDL_GetError());
		goto Quit;
	}
	J1 = SDL_CreateTextureFromSurface(renderer, BJ1);
	J2 = SDL_CreateTextureFromSurface(renderer, BJ2);
	if(J1 == NULL)
	{
		fprintf(stderr, "Erreur SDL_CreateTexture : %s\n", SDL_GetError());
		goto Quit;
	}
	if(J2 == NULL)
	{
		fprintf(stderr, "Erreur SDL_CreateTexture : %s\n", SDL_GetError());
		goto Quit;
	}
	SDL_Rect rectJ1 = {50, 140, 200, 200};
	SDL_Rect rectJ2 = {350, 140, 200, 200};
	SDL_RenderCopy(renderer, J1, NULL, &rectJ1);
	SDL_RenderCopy(renderer, J2, NULL, &rectJ2);
	SDL_RenderPresent(renderer);
	SDL_bool boolJoueur = SDL_FALSE;
	SDL_Event eventJoueur;
	while (!boolJoueur)
	{
		SDL_WaitEvent(&eventJoueur);
		if(eventJoueur.type == SDL_MOUSEBUTTONDOWN)
		{
			if(eventJoueur.button.x>=rectJ1.x && eventJoueur.button.x<rectJ1.x+200 && eventJoueur.button.y>=rectJ1.y && eventJoueur.button.y+200)
			{
                afficherFond(renderer);
                boolJoueur = SDL_TRUE;
				return 1;
			}
			if(eventJoueur.button.x>=rectJ2.x && eventJoueur.button.x<rectJ2.x+200 && eventJoueur.button.y>=rectJ2.y && eventJoueur.button.y+200)
			{
                afficherFond(renderer);
                boolJoueur = SDL_TRUE;
				return 0;
			}
		}
	}
Quit:
	if(BJ1 != NULL)
		SDL_FreeSurface(BJ1);
	if(BJ2 != NULL)
		SDL_FreeSurface(BJ2);
	if(J1 != NULL)
		SDL_DestroyTexture(J1);
	if(J2 != NULL)
		SDL_DestroyTexture(J2);
}
