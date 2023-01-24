#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "fonctions.h"

int main(int argc, char const *argv[])
{
	//Ouverture du dictionnaire en mode lecture seule
    FILE *F = fopen("dico.csv", "r");
    if (F == NULL)
    {
        fprintf(stderr, "Erreur ouverture fichier csv !\n");
        return EXIT_FAILURE;
    }
    
	//Préparation de la partie
	printf("Remplissage du tableau...\n");
	int NbLignes = NombreDeLignes(F);
	char tab[NbLignes][255];
	remplirTab(F, tab, NbLignes);

	printf("Remplissage de la table...\n");
	Association* table[NbLignes*2-1];
	remplirTable(F, table, NbLignes, NbLignes*2-1);
	
	//Initialisation du score
	int score = 0;
	int *Pscore = &score;
	int total = 0;
	int *Ptotal = &total;

//Affichage
    SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
	SDL_Surface *image = NULL;
	SDL_Texture *texture = NULL;
	
	//Vérification SDL_Init
	if(0 != SDL_Init(SDL_INIT_VIDEO))
	{ 
		fprintf(stderr, "Erreur SDL_Init : %s\n", SDL_GetError());
		goto Quit;	
	}
	//Vérification TTF_Init
	if(0 != TTF_Init())
	{
		fprintf(stderr, "Erreur SDL_ttf : %s\n", TTF_GetError());
		return 1;
	}
	//Ouverture de la police d'écriture
	TTF_Font *font = TTF_OpenFont("polices/256BYTES.TTF", 100);
	//Créer une fenêtre
	window = SDL_CreateWindow("Jeu_images_SAE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
	if(window == NULL)
	{
		fprintf(stderr, "Erreur SDL_CreateWindow : %s\n", SDL_GetError());
		goto Quit;
	}
	SDL_RaiseWindow(window); //Mettre la fenêtre au 1er plan
	
	//Créer un renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if(renderer == NULL)
	{
		fprintf(stderr, "Erreur SDL_CreateRenderer : %s\n", SDL_GetError());
		goto Quit;
	}
	
	//Changer la couleur de fond
	afficherFond(renderer);

	if(ChoixJoueur(renderer)) //Si mode un joueur selectionné 
	{
		printf("Mode un joueur !\n");
	RejouerUn:
		//Selection d'une image
		printf("Image choisie...\n");
		int indice = AssociationAleatoire(table, NbLignes*2-1);
		Association *AssociationChoisie = table[indice];
		//Choisir la langue
		int langue;
		int *Plangue = &langue; 
		ChoixLangue(renderer, Plangue);

		//Créer une surface
		image = IMG_Load(AssociationChoisie->chemin); //Charger une image autre que .bmp
		if(image == NULL)
		{
			fprintf(stderr, "Erreur image : %s\n", SDL_GetError());
			goto Quit;
		}
		
		//Créer une texture
		texture = SDL_CreateTextureFromSurface(renderer, image);
		if(texture == NULL)
		{
			fprintf(stderr, "Erreur SDL_CreateTexture : %s\n", SDL_GetError());
			goto Quit;
		}

		//Afficher des textures
		SDL_Rect rect = {220, 100, 200, 200}; //Définition de rectangle => "dimension des textures" ; image centrée
		SDL_Rect saisie = {50, 320, 540, 100};
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderFillRect(renderer, &saisie);
		SDL_RenderCopy(renderer, texture, NULL, &rect);

		SDL_Color couleur = {255, 255, 255};
		SDL_Surface *surfTexte1 = TTF_RenderUTF8_Blended(font, "Entrez votre réponse:", couleur); //UTF8 pour les accents
		SDL_Surface *surfTexte2 = TTF_RenderUTF8_Blended(font, "(en minuscule/remplacer espace par underscore)", couleur);
		SDL_Texture *texteTexte1 = SDL_CreateTextureFromSurface(renderer, surfTexte1);
		SDL_Texture *texteTexte2 = SDL_CreateTextureFromSurface(renderer, surfTexte2);
		SDL_Rect rectTexte1 = {175, 5, 265, 80};
		SDL_Rect rectTexte2 = {0, 75, 640, 20};
		SDL_RenderCopy(renderer, texteTexte1, NULL, &rectTexte1);
		SDL_RenderCopy(renderer, texteTexte2, NULL, &rectTexte2);
		
		SDL_RenderPresent(renderer);
		
	//Boucle de jeu
		SDL_bool quit = SDL_FALSE;
		while (!quit)
		{
			ReponseJoueur(*Plangue, *AssociationChoisie, Pscore, Ptotal, renderer);
			if(Rejouer(renderer, *Pscore, *Ptotal)) //Le joueur veut rejouer
			{
				goto RejouerUn;
			}
			else 
			{
				quit = SDL_TRUE;
				goto Quit;
			}
		}
	}
	else //Si mode deux joueurs selectionné
	{
		printf("Mode deux joueur !\n");
		int langue = 2;
		int *Plangue = &langue;



	RejouerDeux:
		SDL_Rect saisieImage = {50, 320, 540, 100};
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderFillRect(renderer, &saisieImage);
		SDL_Color couleurImage = {255, 255, 255};
		SDL_Surface *surfTexte1Image = TTF_RenderUTF8_Blended(font, "Entrez le mot à trouver(en français):", couleurImage); //UTF8 pour les accents
		SDL_Surface *surfTexte2Image = TTF_RenderUTF8_Blended(font, "(en minuscule/remplacer espace par underscore)", couleurImage);
		SDL_Texture *texteTexte1Image = SDL_CreateTextureFromSurface(renderer, surfTexte1Image);
		SDL_Texture *texteTexte2Image = SDL_CreateTextureFromSurface(renderer, surfTexte2Image);
		SDL_Rect rectTexte1Image = {125, 5, 315, 80};
		SDL_Rect rectTexte2Image = {0, 75, 640, 20};
		SDL_RenderCopy(renderer, texteTexte1Image, NULL, &rectTexte1Image);
		SDL_RenderCopy(renderer, texteTexte2Image, NULL, &rectTexte2Image);
		SDL_RenderPresent(renderer);
		
		int indice = choixImage(renderer, table, NbLignes*2-1);
		Association *AssociationChoisie = table[indice];
		printf("Image choisie...\n");
		afficherFond(renderer);

		//Créer une surface
		image = IMG_Load(AssociationChoisie->chemin); //Charger une image autre que .bmp
		if(image == NULL)
		{
			fprintf(stderr, "Erreur image : %s\n", SDL_GetError());
			goto Quit;
		}
		
		//Créer une texture
		texture = SDL_CreateTextureFromSurface(renderer, image);
		if(texture == NULL)
		{
			fprintf(stderr, "Erreur SDL_CreateTexture : %s\n", SDL_GetError());
			goto Quit;
		}

		//Afficher des textures
		SDL_Rect rect = {220, 100, 200, 200}; //Définition de rectangle => "dimension des textures" ; image centrée
		SDL_Rect saisie = {50, 320, 540, 100};
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderFillRect(renderer, &saisie);
		SDL_RenderCopy(renderer, texture, NULL, &rect);

		SDL_Color couleur = {255, 255, 255};
		SDL_Surface *surfTexte1 = TTF_RenderUTF8_Blended(font, "Entrez votre réponse:", couleur); //UTF8 pour les accents
		SDL_Surface *surfTexte2 = TTF_RenderUTF8_Blended(font, "(en minuscule/remplacer espace par underscore)", couleur);
		SDL_Texture *texteTexte1 = SDL_CreateTextureFromSurface(renderer, surfTexte1);
		SDL_Texture *texteTexte2 = SDL_CreateTextureFromSurface(renderer, surfTexte2);
		SDL_Rect rectTexte1 = {175, 5, 265, 80};
		SDL_Rect rectTexte2 = {0, 75, 640, 20};
		SDL_RenderCopy(renderer, texteTexte1, NULL, &rectTexte1);
		SDL_RenderCopy(renderer, texteTexte2, NULL, &rectTexte2);
		
		SDL_RenderPresent(renderer);
		
	//Boucle de jeu
		SDL_bool quit = SDL_FALSE;
		while (!quit)
		{
			ReponseJoueur(*Plangue, *AssociationChoisie, Pscore, Ptotal, renderer);
			if(Rejouer(renderer, *Pscore, *Ptotal)) //Le joueur veut rejouer
			{
				goto RejouerDeux;
			}
			else 
			{
				quit = SDL_TRUE;
				goto Quit;
			}
		}
	}

//Destruction des composants	
Quit:
	if(image != NULL)
		SDL_FreeSurface(image);
	if(texture != NULL)
		SDL_DestroyTexture(texture);
	if(renderer != NULL)
		SDL_DestroyRenderer(renderer);
	if(window != NULL)
		SDL_DestroyWindow(window);
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_Quit(); //Quitter la SDL
	fclose(F);
	for (size_t i = 0; i < (NbLignes*2)-1; i++) //libération de la mémoire allouée avec malloc
	{
		free(table[i]);
	}
	
	return EXIT_SUCCESS;
}
