#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>


#define TAILLE_MAX 4096
#define TAILLE_MAX2  8192






//rend la ligne : INSERT INTO "tatable" 
void get_table(FILE* source, char* chaine, int ligne){
	rewind(source);

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
void get_colname(char* chaine){


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
				if(chaine[i+1] == '"'){
					chaine[i+1] = ',';
					chaine[i+2] = '"';
				}else{
					chaine[i+1] = ',';
				}
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


	}

//remplace tous les caractères par un espace jusqu'a un '"' et retourne sa position
int rmtogui (char* chaine, int e){

	int i =e;
	while(chaine[i] != '"'){
		chaine[i] = ' ';
		i++;
	}

	return i;
	
}


// donne la position du prochain guillemet
int nextgui (char* chaine, int e){
	int i = e;
	while((chaine[i] != '"') ){
		i++;
	}
	return i;
}

//indique si il reste des guillemets dans la chaine
int gui (char* chaine, int e){
	int i = e;
	while((chaine[i] != '\0') && (chaine[i] != '"') ){
		i++;
	}

	return (chaine[i] == '"');
}


//indique si il y a d'autres apostrophes
int  ifquote(char* chaine, int e){
	int i = e;
	while((chaine[i] != '\0') && (chaine[i] != '\'') ){
		i++;
	}

	return (chaine[i] == '\'');
}


// donne la position du prochain apostrophe
int nextquote (char* chaine, int e){
	int i = e;
	while((chaine[i] != '\'') ){
		i++;
	}
	return i;
}



//decale une chaine pour inserer en e+1 une copie du caratere en e
void poussechaine(char * chaine, char* chaineres, int e, int b){
	int a = 0;
	for (int i = b; i <= e; ++i){
		chaineres[i] = chaine[i];
		a = i;
	}
	chaineres[a+1] = chaine[a];

	for(int i = a+1; chaine[i] != '\0';i++){
		chaineres[i+1] = chaine[i];

	}


}


//affiche les valeurs
void get_values (char* chaine, int ligne){




	//on supprime tout ce qui n'est pas entre guillemet
	int e = 0;
	int a = 0;

	while(gui(chaine, e)){

		a = rmtogui(chaine, e);
		e = a + 1;

		if(gui(chaine, e)){
			e = nextgui(chaine, a+1);
			e++;
			chaine[e] = ',';
			e++;
		}

	}

	//on supprime le bout de la fin
	chaine[e-1] = '\0';

	/*//on retire les espaces (probl : les espaces entre les guillemets sont aussi retires)
	while(ispace(chaine)){
		rm_space(chaine);
	}*/

	
	//on double quote les simples quote pour faire comprendre au sql que c est des simple quote
	e = 0;
	a = 0;
	char chaineres[TAILLE_MAX2] ="";

	if(ifquote(chaine,e)){
		int b = 0;
		int d =0;
		while(ifquote(chaine, e)){
			a = nextquote(chaine, e); //on prend la position de la quote
			poussechaine(chaine, chaineres, a, b);
			e = a+1;
			d++;
			b = e + d;
		}
	}else{
		for (int i = 0; chaine[i] != '\0'; ++i)
		{
			chaineres[i] = chaine[i];
		}		
	}




	

	//on remplace les guillemets pas des ''
	for(int i = 0; chaineres[i] != '\0'; i++){
		if(chaineres[i] == '"'){
			chaineres[i] = '\'';
		}
	}
	

	printf("VALUES \n" );
	printf("(%s);\n", chaineres );
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


//chaine2 devient une copie de chaine1
void cpchaine(char* chaine1, char* chaine2){
	for (int i = 0; chaine1[i] != '\0'; ++i)
	{
		chaine2[i] = chaine1[i];
	}

}



void get_allvalues (FILE* fichier,char* intable, int e){
	int ligne = nbr_lignes_fichier(fichier);
	char chaine[TAILLE_MAX] ="";
	char chainecol[TAILLE_MAX] ="";	


	for(int i=1;i<ligne && fgets(chaine, TAILLE_MAX, fichier) != NULL;i++){
		if(i>e){
			printf( "INSERT INTO %s ", intable);

			cpchaine(chaine,chainecol);
			get_colname(chainecol);


			get_values(chaine, i);
		}
	}


}










int main(int argc, char** argv){


	FILE* xml = NULL;
	xml = fopen(argv[1], "r");
	if(xml == NULL){perror ("error : fopen xml : please enter an xml file ! ");return -1;}
	



	//on recupere le nom de la table et on stock la chaine INSERT INTO latable dans 'intable'
	char intable[TAILLE_MAX] ="";
	get_table(xml, intable, 1);


	get_allvalues(xml, intable, 2);
	
	




	
	if(fclose(xml) != 0){perror ("error : fclose file.xml");return -1;}

	return 0;
}













































