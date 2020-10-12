#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>


#define TAILLE_MAX 2048





//rend la ligne : INSERT INTO "tatable" 
void get_table(FILE* source, int ligne){
	rewind(source);

	char chaine[TAILLE_MAX] ="";	
	for(int i=0;i<=ligne && fgets(chaine, TAILLE_MAX, source) != NULL;i++){}
	rewind(source);

	int i = 0;
	while(chaine[i] != '>'){
		if (chaine[i] == '<'){
			chaine[i] = '"';
		}
		i++;
	}

	chaine[i] = '"';
	chaine[i+1] = '\0';

	printf( "INSERT INTO %s", chaine);
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

void rm_space(char * chaine){
	int i = 0;
	while((chaine[i] != '\0') && (chaine[i] != ' ')){
		i++;
	}
	if(chaine[i] == ' '){
		while(chaine[i] != '\0'){
			chaine[i] = chaine[i+1];
			i++;
		}
		chaine[i-1] = '\0';
	}
}


int ispace(char * chaine){
	int i = 0;
	while((chaine[i] != '\0') && (chaine[i] != ' ')){
		i++;
	}
	return (chaine[i] == ' ');
}

//retourne le noms des colonnes de la premiere entree <row>
void get_colname(FILE* fichier, int ligne){
	rewind(fichier);
	char chaine[TAILLE_MAX] ="";


	//on se place sur la premiere ligne <row>
	for(int i=0;i<=ligne && fgets(chaine, TAILLE_MAX, fichier) != NULL;i++){}

	//on parcourt la ligne jusqu'a tomber sur '/>'
	int i = 0;
	while(i < 5){
		chaine[i] = ' ';
		i++;
	}
	chaine[i] = '"';
	i++;

	while(chaine[i] != '>' && chaine[i-1] != '/'){
		if (chaine[i] == '"'){
			i++;
			while(chaine[i] != '"'){
				chaine[i] = ' ';
				i++;
			}
		}
		i++;
	}
	chaine[i-3] = '\0';

	//on reparcourt la ligne pour enlever les '='
	i= 0;
	while(chaine[i] != '\0'){
		if((chaine[i] == '"') && (chaine[i-1] == '=')  ){
			chaine[i-1] = ' ';
		}
		i++;
	}

	//on rajoute les virgules
	i= 0;
	int second = 0;
	while(chaine[i] != '\0'){
		if((chaine[i] == '"')){
			if(second){
				chaine[i+1] = ',';
				second = 0;
			}else{
				second = 1;
			}
		}
		i++;
	}


	//on reparcourt la chaine pour enlever les ' ' 
	while(ispace(chaine)){
		rm_space(chaine);
	}

	//on retire la virgule en trop
	i=0;
	while(chaine[i] != '\0'){
		i++;
	}
	chaine[i-1] = ' ';
	rm_space(chaine);

	printf(" (%s)\n", chaine );


	
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


/*obsolete*/
//lance le match du joueur, affiche les mots qu'ils doit rentrer et compare les mots qu'il(le joueur) rentre avec ceux attendus
void lancer_match(FILE* match,FILE* match_faute){
	char chaine_match[TAILLE_MAX] ="";
	char chaine_joueur[TAILLE_MAX] ="";
	rewind(match);
		
	for(int i=0;fgets(chaine_match, TAILLE_MAX, match) != NULL;i++){
			//afficher_fichier_pos2pos(match, i, 7); //affichage des 7 prochains mots à taper
			//positionner_fichier_curseur(match, i);//remettre le curseur du fichier au bon endroit pour ne pas rentrer en conflit avec le 'fgets()' du for
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






int main(){


	FILE* xml = NULL;
	xml = fopen("data/file.xml", "r");
	if(xml == NULL){perror ("error : fopen xml");return -1;}
	
	FILE* sql = NULL;
	sql = fopen("data/file.sql", "w+");
	if(sql == NULL){perror ("error : fopen sql");return -1;}




						
	

	get_table(xml, 1);

	get_colname(xml, 2);

		
	
	




	
	if(fclose(xml) != 0){perror ("error : fclose file.xml");return -1;}
	if(fclose(sql) != 0){perror ("error : fclose file.sql");return -1;}

	return 0;
}













































