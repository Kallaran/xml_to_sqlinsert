#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>


#define TAILLE_MAX 64



/* obsolete*/
//affiche un timer de 60 secondes
void min(){
	int t = time(NULL);
	int tmax = t + 60;
	int compteur = t;
	int i = 1;
		
	while(t != tmax){	
		if(t > compteur){
			compteur  = t;
			printf("%d\n", i);
			i++;
		}
		t = time(NULL);
	}
}



//retourne le nombre de lignes que contient un fichier
int nbr_lignes_fichier(FILE* fichier){
	rewind(fichier);
	int res = 0;
	char chaine[TAILLE_MAX] ="";	
	while(fgets(chaine, TAILLE_MAX, fichier) != NULL){
		res = res + 1;
	}
	rewind(fichier);
	return res;	
}

//retourne le nombre de caracteres du fichier 'fichier'
int nbr_caracteres_fichier(FILE* fichier){
	rewind(fichier);
	int res = 0;
	while(fgetc(fichier) != EOF){
		res = res + 1;
	}
	rewind(fichier);
	return res;
}



//ecrit la ligne numéro 'ligne' de 'source' dans 'match'
void ecrire_ligne(FILE* source, FILE* match, int ligne){
	char chaine[TAILLE_MAX] ="";	
	for(int i=0;i<=ligne && fgets(chaine, TAILLE_MAX, source) != NULL;i++){
			if(i==ligne){fprintf(match, "%s", chaine);}
	}
	rewind(source);
}



//à partir d'un fichier 'source' remplit un fichier 'match' avec les mêmes lignes que 'source' mais dans un ordre aléatoire avec des doublons et des lignes absentes
void creer_match(FILE* source, FILE* match){ 

	int nombre_aleatoire;
	int nbr_lignes = nbr_lignes_fichier(source);
	srand(time(NULL)); //initialisation de la sequence de rand en position time(NULL)
	for(int i=0;i<nbr_lignes;i++){
		nombre_aleatoire = rand()%nbr_lignes;
		ecrire_ligne(source, match, nombre_aleatoire);		
	}
	rewind(source);
	rewind(match);
}




//affiche le contenu de 'fichier' en ligne dans la console à partir de la position 'position'jusqu'à la fin du fichier
void afficher_fichier_position(FILE* fichier, int position){
	rewind(fichier);
	int i = 0;
	char chaine[TAILLE_MAX] ="";	
	while(fgets(chaine, TAILLE_MAX, fichier) != NULL){
		if(i>= position){
			printf("%s ", chaine);
		}
		i++;
	}
	printf("\n");
	rewind(fichier);
}


//affiche le contenu de 'fichier' en ligne dans la console à partir de la position 'position' jusq'à 'position'+ 'intervalle'
void afficher_fichier_pos2pos(FILE* fichier, int position, int intervalle){
	rewind(fichier);
	int i = 0;
	int imax = position + intervalle;
	char chaine[TAILLE_MAX] ="";	
	while(fgets(chaine, TAILLE_MAX, fichier) != NULL && i < imax){
		if(i>= position){
			chaine[strlen(chaine)-1] = '\0'; //on retire le '\n' pour pouvoir afficher le contenu en ligne
			printf("%s ", chaine);
		}
		i++;
	}
	printf("\n");
	rewind(fichier);
}

//place le curseur du fichier 'fichier' à la position 'position' souhaitee
void positionner_fichier_curseur(FILE* fichier, int position){
	rewind(fichier);
	int i = 0;
	char chaine[TAILLE_MAX] ="";	
	while(fgets(chaine, TAILLE_MAX, fichier) != NULL && i < position){
		i++;
	}
}

//affiche le score du dernier match
void routine_fin(FILE* match_faute,FILE* match_correct, FILE* scores, int temps,char* nom_joueur){
	//int total = nbr_lignes_fichier(match);
	
	int faute = nbr_lignes_fichier(match_faute);
	int correct = nbr_lignes_fichier(match_correct);
	//float precision = 100 - (100 *(faute / total));
	int caracteres = nbr_caracteres_fichier(match_correct);
	int MPS = caracteres/6;
	printf("Temps : %d \n", temps);
	printf("MPM : %d \n", MPS);
	printf("Caractères corrects : %d \n", caracteres);
	printf("Mots corrects : %d \n", correct);
	//printf("Précision : \n");
	printf("Fautes : %d\n", faute);
	afficher_fichier_position(match_faute, 0);

	//on enregistre le nom du joueur et son score dans le fichier score
	fprintf(scores,"%s	 %d		%d			%d		%d\n", nom_joueur, MPS, caracteres, correct, faute);
}

/*obsolete*/
//lance le match du joueur, affiche les mots qu'ils doit rentrer et compare les mots qu'il(le joueur) rentre avec ceux attendus
void lancer_match(FILE* match,FILE* match_faute){
	char chaine_match[TAILLE_MAX] ="";
	char chaine_joueur[TAILLE_MAX] ="";
	rewind(match);
		
	for(int i=0;fgets(chaine_match, TAILLE_MAX, match) != NULL;i++){
			afficher_fichier_pos2pos(match, i, 7); //affichage des 7 prochains mots à taper
			positionner_fichier_curseur(match, i);//remettre le curseur du fichier au bon endroit pour ne pas rentrer en conflit avec le 'fgets()' du for
			printf(">");
			scanf("%s", chaine_joueur);
			printf("\n");
			printf("\n");			
			chaine_match[strlen(chaine_match)-1] = '\0'; // on retire le '\n'
			if(strcmp(chaine_joueur,chaine_match)!=0){
				fprintf(match_faute,"%s\n", chaine_joueur); //si le mot du joueur est different du mot attendu on écrit le mot du joueur dans le fichier 'match_faute'
			}
	}
}




//lance le match du joueur, et tant qu'il reste du temps : affiche les mots qu'ils doit rentrer et compare les mots qu'il(le joueur) rentre avec ceux attendus
void lancer_match_temps(FILE* match,FILE* match_faute,FILE* match_correct, int temps){
	char chaine_match[TAILLE_MAX] ="";
	char chaine_joueur[TAILLE_MAX] ="";
	rewind(match);
	
	
	int t = time(NULL);
	int tmax = t + temps;
		
	for(int i=0;t < tmax && fgets(chaine_match, TAILLE_MAX, match) != NULL;i++){


			printf(">");
			afficher_fichier_pos2pos(match, i, 7); //affichage des 7 prochains mots à taper

			printf(">>> %s\n", chaine_joueur); //test
			printf(">");




			positionner_fichier_curseur(match, i);//remettre le curseur du fichier au bon endroit pour ne pas rentrer en conflit avec le 'fgets()' du for
			scanf("%s", chaine_joueur);

			printf(">>> %s\n", chaine_joueur); //test
			printf("\n");
			printf("\n");			
			chaine_match[strlen(chaine_match)-1] = '\0'; // on retire le '\n'
			if(strcmp(chaine_joueur,chaine_match)!=0){
				fprintf(match_faute,"%s\n", chaine_joueur); //si le mot du joueur est different du mot attendu on écrit le mot du joueur dans le fichier 'match_faute'
			}
			else{
				fprintf(match_correct,"%s\n", chaine_joueur);
			}
			
			t = time(NULL);
	}
}

int main(){


	FILE* dico = NULL;
	dico = fopen("data/mots/dico.mots", "r");
	if(dico == NULL){perror ("error : fopen dico");return -1;}
	
	FILE* match = NULL;
	match = fopen("data/mots/match.mots", "w+");
	if(match == NULL){perror ("error : fopen match");return -1;}

	FILE* match_faute = NULL;
	match_faute = fopen("data/mots/match_faute.mots", "w+");
	if(match_faute == NULL){perror ("error : fopen match_faute");return -1;}

	FILE* match_correct = NULL;
	match_correct = fopen("data/mots/match_correct.mots", "w+");
	if(match_correct == NULL){perror ("error : fopen match_correct");return -1;}

	FILE* scores = NULL;
	scores = fopen("data/stats/scores.stats", "a+");
	if(scores == NULL){perror ("error : fopen scores");return -1;}
	

	
	int temps = 60;
	char nom_joueur[TAILLE_MAX] ="";
	
	afficher_fichier_position(scores,0);


	printf("Nom joueur :\n");
	printf(">");					
	scanf("%s", nom_joueur);
	printf("\n\n");						
	

	
	creer_match(dico,match);
	
	lancer_match_temps(match,match_faute,match_correct,temps);// lance un match de 'temps' secondes 

		
	routine_fin(match_faute,match_correct,scores,temps,nom_joueur);
	
	




	
	if(fclose(dico) != 0){perror ("error : fclose dico");return -1;}
	if(fclose(match) != 0){perror ("error : fclose match");return -1;}
	if(fclose(match_faute) != 0){perror ("error : fclose match_faute");return -1;}
	if(fclose(match_correct) != 0){perror ("error : fclose match_correct");return -1;}
	if(fclose(scores) != 0){perror ("error : fclose scores");return -1;}
	
	return 0;
}













































