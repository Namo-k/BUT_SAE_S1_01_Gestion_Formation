/**
* @file SAE_S1_01_Namodacane_Alexandre.c
* Application de gestion de formation universitaire
* @author KALIAMOORTHY Namodacane et CAROUNANITHI Alexandre
* Date de création : 30/09/2022
* @version 8.7 11/11/2022
* Codé sur Visual Studio 2022
*/

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#pragma warning(disable : 4996 6031 6262 4244 6054 6385)

//Declaration de constante
enum {
	NB_SEMESTRES = 2,
	MIN_UE = 3,
	MAX_UE = 6,
	MAX_MATIERES = 10,
	MAX_EPREUVES = 5,
	MAX_ETUDIANTS = 100,
	MAX_CHAR = 30,
};
typedef enum { true = 1, false = 0 }bool;
const float MIN_NOTE = 0.f, MAX_NOTE = 20.f;
typedef char CH30[MAX_CHAR];

//Definition de structures
typedef struct {
//Structure pour sauvegarder les coefficients de chaque UE en fonction de l'épreuve et les notes de chaque UE en fonction de l'épreuve
	CH30 nom;
	float coef[MAX_UE];
	float note[MAX_UE];
}Epreuve;

typedef struct {
//Structure pour sauvegarder les épreuves pour chaque Matiere
	CH30 nom;
	unsigned int nbEpreuves;
	unsigned int nbNotes;
	Epreuve epreuves[MAX_EPREUVES];
}Matiere;

typedef struct {
//Structure pour sauvegarder les matieres pour chaque Semestre
	unsigned int nbMatieres;
	Matiere matieres[MAX_MATIERES];
	float moyenneNoteS[MAX_UE];
}Semestre;

typedef struct {
//Structure pour sauvegarder les noms des étudiants
	CH30 nom;
	Semestre semestres[NB_SEMESTRES];
}Etudiant;

typedef struct {
//Structure pour sauvegarder les semestres et leurs UE respectives
	unsigned int nbUE;
	unsigned int nbEtudiant;
	Etudiant etudiants[MAX_ETUDIANTS];
}Formation;

//Les fonctions
void init(Formation* f) {
/**
* Initialise les valeurs des structures à 0 ou -1
* @param[in] Structure Formation f
* @param[out] Structure Formation f après initialisation
*/

	f->nbUE = 0;
	f->nbEtudiant = 0;
	for (int l = 0; l < MAX_UE; ++l) {
		for (int k = 0; k < MAX_EPREUVES; k++) {
			for (int j = 0; j < MAX_MATIERES; ++j) {
				for (int i = 0; i < NB_SEMESTRES; ++i) {
					for (int h = 0; h < MAX_ETUDIANTS; ++h) {
						f->etudiants[h].semestres[i].matieres[j].epreuves[k].coef[l] = 0;
						f->etudiants[h].semestres[i].matieres[j].epreuves[k].note[l] = -1; //Pas de note
						f->etudiants[h].semestres[i].nbMatieres = 0;
						f->etudiants[h].semestres[i].moyenneNoteS[l] = 0;
						f->etudiants[h].semestres[i].matieres[j].nbEpreuves = 0;
						f->etudiants[h].semestres[i].matieres[j].nbNotes = 0;
					}

				}
			}
		}

	}
}

void cmdFormation(Formation* f, unsigned int tempUE) {
/**
* Initialise le nombre d'UE de la formation et affiche les messages correspondant à la commande 2
* @param[in] Structure Formation f
* @param[in] tempUE le nombre d'UE entrée
* @param[out] Structure Formation f après initialisation du nombre d'UE
* @pre tempUE valide, compris entre 3 et 6 inclus
*/

	scanf("%i", &tempUE);

	if (tempUE >= MIN_UE && tempUE <= MAX_UE) {
		if (f->nbUE > 0) printf("Le nombre d'UE est deja defini\n");
		else {
			f->nbUE = tempUE;
			printf("Le nombre d'UE est defini\n");
		}
	}
	else printf("Le nombre d'UE est incorrect\n");
}

bool verifCoef(const Formation* f, unsigned int nSemestre, unsigned int e, CH30 cmd) {
/**
* Verifie si les coefficients sont correctes
* @param[in] Structure Formation f
* @param[in] nSemestre correspondant au numéro de semestre entré
* @param[in] e correspondant au numéro de l'étudiant
* @param[in] cmd correspondant à la commande entrée
* @pre nSemestre et e valides, nSemestre doit être egal à 0 ou 1 dans la partie decision et 1 ou 2 dans les autres parties, e doit être compris entre 0 et 99
* @return true ou false si les coefficients sont correctes
*/
	if (!strcmp(cmd, "decision")) {

		assert(((nSemestre == 0 || nSemestre == 1)) && ((e >= 0 && e< MAX_ETUDIANTS)));

		//Somme des coefficients
		//Table où on met la somme des coefficients par UE
		float sumUE[MAX_UE] = { 0,0,0,0,0,0 };
		//3 boucles parcourant tous les UE, toutes les matières et toutes les epreuves
		for (unsigned int k = 0; k < f->nbUE; ++k) {
			float sumUEpEP = 0; //Apres avoir fait la somme d'une UE, on la reinitialise à 0
			for (unsigned int j = 0; j < f->etudiants[e].semestres[nSemestre].nbMatieres; ++j) {
				for (unsigned int i = 0; i < f->etudiants[e].semestres[nSemestre].matieres[j].nbEpreuves; ++i) {
					//Faire la somme des coefficients par UE, par matieres, par epreuves
					sumUEpEP = sumUEpEP + f->etudiants[e].semestres[nSemestre].matieres[j].epreuves[i].coef[k];
				}//Fin boucle parcourant toutes les epreuves
			}//Fin boucle parcourant toutes les matieres
			sumUE[k] = sumUEpEP;
		}// Fin boucle parcourant tous les UE

		//Verification si tous les coefficients entrés sont null
		float vsumUE = 0; //valeur non null de la table sumUE
		for (unsigned int i = 0; i < f->nbUE; ++i) if (sumUE[i] != 0) vsumUE++;
		if (vsumUE == f->nbUE) return true;
		else return false;
		
	}
	else {

		assert(((nSemestre == 1 || nSemestre == 2)) && ((e >= 0 && e < MAX_ETUDIANTS)));

		//Somme des coefficients
		float sumUE[MAX_UE] = { 0,0,0,0,0,0 };
		for (unsigned int k = 0; k < f->nbUE; ++k) {
			float sumUEpEP = 0; 
			for (unsigned int j = 0; j < f->etudiants[e].semestres[nSemestre - 1].nbMatieres; ++j) {
				for (unsigned int i = 0; i < f->etudiants[e].semestres[nSemestre - 1].matieres[j].nbEpreuves; ++i) {
					sumUEpEP = sumUEpEP + f->etudiants[e].semestres[nSemestre - 1].matieres[j].epreuves[i].coef[k];
				}
			}
			sumUE[k] = sumUEpEP;
		}

		//Verification si tous les coefficients entrés sont null
		float vsumUE = 0; //valeur non null de la table sumUE
		for (unsigned int i = 0; i < f->nbUE; ++i) if (sumUE[i] != 0) vsumUE++;
		if (vsumUE == f->nbUE) return true;
		else return false;
	}
	
}
unsigned int maxStringMat(const Formation* f) {
/**
* Trouve la matière qui a la plus longue chaine de caractère
* @param[in] Structure Formation f
* @return max la plus longue chaine de caractère des matières
*/

	unsigned int tab[MAX_MATIERES], max = 0;

	for (unsigned int i = 0; i < MAX_MATIERES; ++i) tab[i] = 0; //init
	for (unsigned int i = 0; i<f->etudiants[0].semestres[0].nbMatieres; ++i) tab[i] = strlen(f->etudiants[0].semestres[0].matieres[i].nom); //recherche
	for (unsigned int i = 0; i < MAX_MATIERES; ++i) if (tab[i] > max) max = tab[i]; //recherche le max
	
	return max;
}
void afficheUE(const Formation* f, CH30 cmd) {
/**
* Affiche les UE
* @param[in] Structure Formation f
* @param[in] cmd correspondant à la commande entrée
*/
	if (!strcmp(cmd, "releve")) {
		for (unsigned int i = 0; i < maxStringMat(*&f) + 2; ++i) printf(" "); //Espace
		for (unsigned int i = 0; i < f->nbUE; ++i) {
			if (i == f->nbUE - 1) printf("UE%i \n", i + 1); else printf("UE%i  ", i + 1); //Affiche
		}
	}
	else { 
		//Pour commande decision
		for (unsigned int i = 0; i < strlen("Moyennes annuelles") + 2; ++i) printf(" ");
		for (unsigned int i = 0; i < f->nbUE; ++i) {
			if (i == f->nbUE - 1) printf("UE%i \n", i + 1); else printf("UE%i  ", i + 1);
		}
	}
}

void afficheNomMatAvecMoyenne(const Formation* f, unsigned int nSemestre, unsigned int e) {
/**
* Affiche le nom de la matière et ses moyennes par UE
* @param[in] Structure Formation f
* @param[in] nSemestre correspondant au numéro de semestre entré
* @pre nSemestre et e valides, nSemestre doit être egal à 1 ou 2, e doit être compris entre 0 et 99
* @param[in] e correspondant au numéro de l'étudiant
*/

	assert(((nSemestre == 1 || nSemestre == 2)) && ((e >= 0 && e < MAX_ETUDIANTS)));

	//Affiche le nom de la matière
	for (unsigned int k = 0; k < f->etudiants[e].semestres[nSemestre - 1].nbMatieres; ++k) {
		printf("%s", f->etudiants[e].semestres[nSemestre - 1].matieres[k].nom);
		for (unsigned int j = 0; j < (maxStringMat(*&f) + 2) - strlen(f->etudiants[e].semestres[nSemestre - 1].matieres[k].nom) - 1; ++j) printf(" ");
		
		//Affiche les moyennes des matières par UE
		float sumNoteMatT[MAX_UE] = { 0,0,0,0,0,0 };
		float sumCoeffMatT[MAX_UE] = { 0,0,0,0,0,0 };
		float moyenneNoteMatT[MAX_UE] = { 0,0,0,0,0,0 };
		for (unsigned int i = 0; i < f->nbUE; ++i) {
			float sumNoteMat = 0;
			float sumCoeffMat = 0;
			for (unsigned int j = 0; j < f->etudiants[e].semestres[nSemestre - 1].matieres[k].nbEpreuves; ++j) { //Epreuve
				sumNoteMat = sumNoteMat + f->etudiants[e].semestres[nSemestre - 1].matieres[k].epreuves[j].note[i] * f->etudiants[e].semestres[nSemestre - 1].matieres[k].epreuves[j].coef[i];
				sumCoeffMat = sumCoeffMat + f->etudiants[e].semestres[nSemestre - 1].matieres[k].epreuves[j].coef[i];
			}
			sumNoteMatT[i] = sumNoteMat;
			sumCoeffMatT[i] = sumCoeffMat;

			if (sumCoeffMatT[i] == 0) {
				if (i == f->nbUE - 1) printf("  ND \n");
				else printf("  ND ");
			}
			else {

				moyenneNoteMatT[i] = (sumNoteMatT[i] / sumCoeffMatT[i]);

				if (i == f->nbUE - 1) {
					if (moyenneNoteMatT[i] < 10) printf(" %.1f \n", floorf(moyenneNoteMatT[i] * 10.f) / 10.f); //Pour arrondir à son entier naturel inférieur 
					else printf("%.1f \n", floorf(moyenneNoteMatT[i] * 10.f) / 10.f);
				}
				else {
					if (moyenneNoteMatT[i] < 10) printf(" %.1f ", floorf(moyenneNoteMatT[i] * 10.f) / 10.f);
					else printf("%.1f ", floorf(moyenneNoteMatT[i] * 10.f) / 10.f);
				}

			}
		}
	}
}
void afficheMoyenneUEparSemestre(Formation* f, unsigned int nSemestre, unsigned int e, CH30 cmd) {
/**
* Affiche les moyennes par UE du semestre
* @param[in] Structure Formation f
* @param[in] nSemestre correspondant au numéro de semestre entré
* @param[in] e correspondant au numéro de l'étudiant
* @pre nSemestre et e valides, nSemestre doit être egal à 1 ou 2 dans la partie releve et 0 ou 1 dans la partie decision, e doit être compris entre 0 et 99
* @param[in] cmd correspondant à la commande entrée
*/

	if (!strcmp(cmd, "releve")) {
		
		assert(((nSemestre == 1 || nSemestre == 2)) && ((e >= 0 && e < MAX_ETUDIANTS)));

		//Moyenne par UE

		printf("Moyennes"); for (unsigned int i = 0; i < (maxStringMat(*&f) + 2) - strlen("Moyennes") - 1; ++i) printf(" ");
		
		float sumNoteT[MAX_UE] = { 0,0,0,0,0,0 };
		float sumCoeffT[MAX_UE] = { 0,0,0,0,0,0 };
		float moyenneNoteUET[MAX_UE] = { 0,0,0,0,0,0 };
		for (unsigned int i = 0; i < f->nbUE; ++i) {
			float sumNote = 0;
			float sumCoeff = 0;
			float moyenneNoteUE = 0;
			for (unsigned int k = 0; k < f->etudiants[e].semestres[nSemestre - 1].nbMatieres; ++k) {
				for (unsigned int j = 0; j < f->etudiants[e].semestres[nSemestre - 1].matieres[k].nbEpreuves; ++j) {
					sumNote = sumNote + f->etudiants[e].semestres[nSemestre - 1].matieres[k].epreuves[j].note[i] * f->etudiants[e].semestres[nSemestre - 1].matieres[k].epreuves[j].coef[i];
					sumCoeff = sumCoeff + f->etudiants[e].semestres[nSemestre - 1].matieres[k].epreuves[j].coef[i];
				}

			}
			sumNoteT[i] = sumNote;
			sumCoeffT[i] = sumCoeff;
			moyenneNoteUET[i] = (sumNoteT[i] / sumCoeffT[i]);

			if (i == f->nbUE - 1) {
				if (moyenneNoteUET[i] < 10) printf(" %.1f \n", floorf(moyenneNoteUET[i] * 10.f) / 10.f);
				else printf("%.1f \n", floorf(moyenneNoteUET[i] * 10.f) / 10.f);
			}
			else {
				if (moyenneNoteUET[i] < 10) printf(" %.1f ", floorf(moyenneNoteUET[i] * 10.f) / 10.f);
				else printf("%.1f ", floorf(moyenneNoteUET[i] * 10.f) / 10.f);
			}
		}
	}
	else {

		assert(((nSemestre == 0 || nSemestre == 1)) && ((e >= 0 && e < MAX_ETUDIANTS)));

		//Moyenne par UE d'un Semestre

		printf("S%i", nSemestre + 1); for (unsigned int i = 0; i < (strlen("Moyennes annuelles") + 1) - strlen("Sn"); ++i) printf(" ");

		float sumNoteT[MAX_UE] = { 0,0,0,0,0,0 };
		float sumCoeffT[MAX_UE] = { 0,0,0,0,0,0 };
		float moyenneNoteUET[MAX_UE] = { 0,0,0,0,0,0 }; //Tableau contenant la moyenne des UE par Semestre
		for (unsigned int i = 0; i < f->nbUE; ++i) {
			float sumNote = 0;
			float sumCoeff = 0;
			float moyenneNoteUE = 0;
			for (unsigned int k = 0; k < f->etudiants[e].semestres[nSemestre].nbMatieres; ++k) {
				for (unsigned int j = 0; j < f->etudiants[e].semestres[nSemestre].matieres[k].nbEpreuves; ++j) {
					sumNote = sumNote + f->etudiants[e].semestres[nSemestre].matieres[k].epreuves[j].note[i] * f->etudiants[e].semestres[nSemestre].matieres[k].epreuves[j].coef[i];
					sumCoeff = sumCoeff + f->etudiants[e].semestres[nSemestre].matieres[k].epreuves[j].coef[i];
				}

			}
			sumNoteT[i] = sumNote;
			sumCoeffT[i] = sumCoeff;
			moyenneNoteUET[i] = (sumNoteT[i] / sumCoeffT[i]);
			f->etudiants[e].semestres[nSemestre].moyenneNoteS[i] = moyenneNoteUET[i];

			if (i == f->nbUE - 1) {
				if (moyenneNoteUET[i] < 10) printf(" %.1f \n", floorf(moyenneNoteUET[i] * 10.f) / 10.f);
				else printf("%.1f \n", floorf(moyenneNoteUET[i] * 10.f) / 10.f);
			}
			else {
				if (moyenneNoteUET[i] < 10) printf(" %.1f ", floorf(moyenneNoteUET[i] * 10.f) / 10.f);
				else printf("%.1f ", floorf(moyenneNoteUET[i] * 10.f) / 10.f);
			}
		}
	}
}
void afficheMoyenneAnnuelleAcqDev(const Formation *f, unsigned int e) {
/**
* Calcul et affichage des moyennes annuelles d'un étudiant par UE, des UE acquises, de la decision
* @param[in] Structure Formation f
* @param[in] e correspondant au numéro de l'étudiant
* @pre e valide, compris entre 0 et 99
*/

	assert(e >= 0 && e < MAX_ETUDIANTS);

	unsigned int moyP = 0,temp = 0; //Compteur de moyenne au dessus de la moyenne (>=10)

	//Moyenne annuelle
	float moyenneNoteSTab[MAX_UE] = { 0,0,0,0,0,0 }; //Tableau contenant la moyenne annuelle des Semestres par UE
	printf("Moyennes annuelles"); for (unsigned int i = 0; i < (strlen("Moyennes annuelles") + 1) - strlen("Moyennes annuelles"); ++i) printf(" ");
	
	for (unsigned int i = 0; i < f->nbUE; ++i) {
		float sumMoyenne = 0, comptSem = 0, moyenneNoteUE = 0;
		for (unsigned int k = 0; k < NB_SEMESTRES; ++k) {
			sumMoyenne = sumMoyenne + f->etudiants[e].semestres[k].moyenneNoteS[i];
			++comptSem;
		}
		moyenneNoteSTab[i] = (sumMoyenne / comptSem);
		if (i == f->nbUE - 1) {
			if (moyenneNoteSTab[i] < 10) printf(" %.1f \n", floorf(moyenneNoteSTab[i] * 10.f) / 10.f);
			else printf("%.1f \n", floorf(moyenneNoteSTab[i] * 10.f) / 10.f);
		}
		else {
			if (moyenneNoteSTab[i] < 10) printf(" %.1f ", floorf(moyenneNoteSTab[i] * 10.f) / 10.f);
			else printf("%.1f ", floorf(moyenneNoteSTab[i] * 10.f) / 10.f);
		}
	}

	//Acquisition
	printf("Acquisition"); for (unsigned int i = 0; i < (strlen("Moyennes annuelles")) - (strlen("Acquisition") - 1); ++i) printf(" ");

	for (unsigned int i = 0; i < f->nbUE; ++i) {
		if (moyenneNoteSTab[i] >= 10) {
			++moyP;
			if (i == 0 || moyP == 1) printf("UE%i", i + 1);
			else {
				if (i == f->nbUE - 1) {
					printf(", UE%i\n", i + 1);
					++temp;
				}
				else printf(", UE%i", i + 1);
			}
		}
	}
	if (moyP == 0) {
		printf("Aucune\n");
		++temp;
	}
	if (temp == 0) printf("\n");
	
	//Devenir
	printf("Devenir"); for (unsigned int i = 0; i < (strlen("Moyennes annuelles")) - (strlen("Devenir") - 1); ++i) printf(" ");
	if (moyP > f->nbUE/2) printf("Passage\n");
	else printf("Redoublement\n");

}
int main() {

	//Declaration de structure
	Formation f;

	//Initialisation des valeurs à initialiser
	init(&f);


	//Declarations de variables
	CH30 cmd = "";
	unsigned int tempUE = 0;
	unsigned int nSemestre = 0;
	CH30 nomMatiere;
	CH30 nomEpreuve;
	float coeffUE[MAX_UE] = { 0,0,0,0,0,0 };
	CH30 nomEtudiant;
	float note;
	unsigned int nbEtuTemp = 0;
	

	while (1) {
		scanf("%s", cmd);

		//Commande 1
		if (!strcmp(cmd, "exit")) {
			return 0;
		}//Fin C1

		//Commande 2
		else if (!strcmp(cmd, "formation")) {
			//Initialise le nombre d'UE de la formation
			cmdFormation(&f, tempUE);
		}// Fin C2

		//Commande 3
		else if (!strcmp(cmd, "epreuve")) {

			//Ajoute des epreuves
			//Precondition : nSemestre doit être egal à 1 ou 2
			
			//Lecture parametres
			scanf("%i %s %s", &nSemestre, nomMatiere, nomEpreuve);
			for (unsigned int i = 0; i < f.nbUE; ++i) scanf("%f", &coeffUE[i]);

			if (f.nbUE == 0) printf("Le nombre d'UE n'est pas defini\n");
			else {
				//Verification si le numero de semestre entré est correct 
				if (nSemestre == 1 || nSemestre == 2) {

					//Verification si tout les coefficients entrés sont null
					unsigned int vCoefftab = 0; //valeur non null de la table coeff
					for (unsigned int i = 0; i < f.nbUE; ++i) {
						if (coeffUE[i] < 0) {
							vCoefftab = -1;
							break;
						}
						else if (coeffUE[i] > 0) vCoefftab++;
					}

					if (vCoefftab == -1) printf("Au moins un des coefficients est incorrect\n");
					else if (vCoefftab >= 0) {
						for (unsigned int e = 0; e < MAX_ETUDIANTS; ++e) {
							++nbEtuTemp;
							unsigned int m = 0, n = 0;
							//Recherche matiere connu
							while (m < f.etudiants[e].semestres[nSemestre - 1].nbMatieres && strcmp(nomMatiere, f.etudiants[e].semestres[nSemestre - 1].matieres[m].nom) != 0) ++m;
							if (m < f.etudiants[e].semestres[nSemestre - 1].nbMatieres) { //trouvé

								//Recherche epreuve connu
								while (n < f.etudiants[e].semestres[nSemestre - 1].matieres[m].nbEpreuves && strcmp(nomEpreuve, f.etudiants[e].semestres[nSemestre - 1].matieres[m].epreuves[n].nom) != 0) ++n;
								if (n < f.etudiants[e].semestres[nSemestre - 1].matieres[m].nbEpreuves) { //trouvé
									if (nbEtuTemp > 1); //Test pour afficher le message qu'une fois
									else printf("Une meme epreuve existe deja\n");
								}
								else { // pas trouvé, on mémorise l'epreuve et les coeff
									if (vCoefftab == 0) {
										if (nbEtuTemp > 1); //Test pour afficher le message qu'une fois
										else printf("Au moins un des coefficients est incorrect\n");
									}
									else {
										strcpy(f.etudiants[e].semestres[nSemestre - 1].matieres[m].epreuves[n].nom, nomEpreuve);
										++f.etudiants[e].semestres[nSemestre - 1].matieres[m].nbEpreuves;

										if (nbEtuTemp > 1); //Test pour afficher le message qu'une fois
										else printf("Epreuve ajoutee a la formation\n");

										//memorise les coeff
										for (unsigned int i = 0; i < f.nbUE; ++i) f.etudiants[e].semestres[nSemestre - 1].matieres[m].epreuves[n].coef[i] = coeffUE[i];
									}

								}
							}
							else { //matiere pas connu
								if (vCoefftab == 0) {
									if (nbEtuTemp > 1); //Test pour afficher le message qu'une fois
									else printf("Au moins un des coefficients est incorrect\n");
								}
								else {
									//memorise matiere
									strcpy(f.etudiants[e].semestres[nSemestre - 1].matieres[m].nom, nomMatiere);
									++f.etudiants[e].semestres[nSemestre - 1].nbMatieres;
									if (nbEtuTemp > 1); //Test pour afficher le message qu'une fois
									else printf("Matiere ajoutee a la formation\n");

									//memorise l'epreuve d'une matiere inconnu
									strcpy(f.etudiants[e].semestres[nSemestre - 1].matieres[m].epreuves[n].nom, nomEpreuve);
									++f.etudiants[e].semestres[nSemestre - 1].matieres[m].nbEpreuves;
									if (nbEtuTemp > 1); //Test pour afficher le message qu'une fois
									else printf("Epreuve ajoutee a la formation\n");

									//memorise les coeff
									for (unsigned int i = 0; i < f.nbUE; ++i) f.etudiants[e].semestres[nSemestre - 1].matieres[m].epreuves[n].coef[i] = coeffUE[i];
								}

							}
						}//Fin boucle etudiant
						nbEtuTemp = 0;
					}
				}
				else printf("Le numero de semestre est incorrect\n");
			}//Fin test UE existe
		}//Fin C3

		//Commande 4
		else if (!strcmp(cmd, "coefficients")) {

			//Verifie les coefficients
			//Precondition : nSemestre doit être egal à 1 ou 2

			scanf("%i", &nSemestre);

			if (f.nbUE == 0) printf("Le nombre d'UE n'est pas defini\n");
			else {
				//Test verification numero de semestre
				if (nSemestre == 1 || nSemestre == 2) {
					for (unsigned int e = 0; e < MAX_ETUDIANTS; ++e) {
						++nbEtuTemp;
						//Test si le semestre contient au moins une epreuve
						if (f.etudiants[e].semestres[nSemestre - 1].nbMatieres != 0) {
							if (verifCoef(&f, nSemestre, e, cmd)) {	
								if (nbEtuTemp > 1); //Test pour afficher le message qu'une fois
								else printf("Coefficients corrects\n");
							}
							else {
								if (nbEtuTemp > 1); //Test pour afficher le message qu'une fois
								else printf("Les coefficients d'au moins une UE de ce semestre sont tous nuls\n");
							}
						}
						else {
							if (nbEtuTemp > 1); //Test pour afficher le message qu'une fois
							else printf("Le semestre ne contient aucune epreuve\n");
						}//Fin Test contient epreuve
					}//Fin boucle etu
					nbEtuTemp = 0;
				}//Fin Test semestre
				else printf("Le numero de semestre est incorrect\n");
			}//Fin test UE existe 

		}//Fin C4

		//Commande 5
		else if (!strcmp(cmd, "note")) {

			//Ajoute les notes de l'étudiant
			//Precondition : nSemestre doit être egal à 1 ou 2, note doit être compris entre 0 et 20 inclus, nomMatiere et nomEpreuve doivent être connus


			scanf("%i %s %s %s %f", &nSemestre, nomEtudiant, nomMatiere, nomEpreuve, &note);

			if (f.nbUE == 0) printf("Le nombre d'UE n'est pas defini\n");
			else {
				//Verification si le numero de semestre entré est correct 
				if (nSemestre == 1 || nSemestre == 2) {

					//Declaration de variable temporaire
					unsigned int m = 0, ep = 0, e = 0, n = 0;

					//Recherche matiere connue
					while (m < f.etudiants[e].semestres[nSemestre - 1].nbMatieres && strcmp(nomMatiere, f.etudiants[e].semestres[nSemestre - 1].matieres[m].nom) != 0) ++m;
					if (m < f.etudiants[e].semestres[nSemestre - 1].nbMatieres) { //trouvé

						//Recherche epreuve connue
						while (ep < f.etudiants[e].semestres[nSemestre - 1].matieres[m].nbEpreuves && strcmp(nomEpreuve, f.etudiants[e].semestres[nSemestre - 1].matieres[m].epreuves[ep].nom) != 0) ++ep;
						if (ep < f.etudiants[e].semestres[nSemestre - 1].matieres[m].nbEpreuves) { //trouvé

							//Verification si la note entrée est correct
							if (note >= MIN_NOTE && note <= MAX_NOTE) {
								//Recherche etudiant connu
								while (e < f.nbEtudiant && strcmp(nomEtudiant, f.etudiants[e].nom) != 0) ++e;
								if (e < f.nbEtudiant) { //trouvé

									//recherche note connu
									if (f.etudiants[e].semestres[nSemestre - 1].matieres[m].epreuves[ep].note[n] > -1) { //trouvé
										printf("Une note est deja definie pour cet etudiant\n");
									}
									else {
										for (; n < f.nbUE; ++n) f.etudiants[e].semestres[nSemestre - 1].matieres[m].epreuves[ep].note[n] = note;
										++f.etudiants[e].semestres[nSemestre - 1].matieres[m].nbNotes;
										printf("Note ajoutee a l'etudiant\n");
									}
								}
								else { // pas trouvé, on mémorise l'etudiant et sa note
									strcpy(f.etudiants[e].nom, nomEtudiant);
									++f.nbEtudiant;
									printf("Etudiant ajoute a la formation\n");

									for (; n < f.nbUE; ++n) f.etudiants[e].semestres[nSemestre - 1].matieres[m].epreuves[ep].note[n] = note;
									++f.etudiants[e].semestres[nSemestre - 1].matieres[m].nbNotes;
									printf("Note ajoutee a l'etudiant\n");
								}
							}
							else printf("Note incorrecte\n");
						}
						else printf("Epreuve inconnue\n");
					}
					else printf("Matiere inconnue\n");
				}
				else printf("Le numero de semestre est incorrect\n");
			}//Fin test UE defini

		}//Fin C5

		//Commande 6
		else if (!strcmp(cmd, "notes")) {

			//Verifie les notes entrée
			//Precondition : nSemestre doit être egal à 1 ou 2, nomEtudiant doit être connu

			scanf("%i %s", &nSemestre, nomEtudiant);

			if (f.nbUE == 0) printf("Le nombre d'UE n'est pas defini\n");
			else {
				//Verification si le numero de semestre entrée est correct 
				if (nSemestre == 1 || nSemestre == 2) {
					unsigned int e = 0, n = 0;
					//Recherche etudiant connu
					while (e < f.nbEtudiant && strcmp(nomEtudiant, f.etudiants[e].nom) != 0) ++e;
					if (e < f.nbEtudiant) { //trouvé
						for (unsigned int m = 0; m < MAX_MATIERES; ++m) {
							if (f.etudiants[e].semestres[nSemestre - 1].matieres[m].nbNotes == f.etudiants[e].semestres[nSemestre - 1].matieres[m].nbEpreuves) n++;
							else n--;
						}
						if (n == 10) printf("Notes correctes\n");
						else printf("Il manque au moins une note pour cet etudiant\n");
					}
					else printf("Etudiant inconnu\n"); // pas trouvé
				}
				else printf("Le numero de semestre est incorrect\n");
			}//Fin test UE defini

		}//Fin C6

		//Commande 7
		else if (!strcmp(cmd, "releve")) {

			//Affichage du relevé
			//Precondition : nSemestre doit être egal à 1 ou 2, nomEtudiant doit être connu

			scanf("%i %s", &nSemestre, nomEtudiant);

			if (f.nbUE == 0) printf("Le nombre d'UE n'est pas defini\n");
			else {
				//Verification si le numero de semestre entré est correct 
				if (nSemestre == 1 || nSemestre == 2) {

					unsigned int e = 0,n = 0;
					//Recherche etudiant connu
					while (e < f.nbEtudiant && strcmp(nomEtudiant, f.etudiants[e].nom) != 0) ++e;
					if (e < f.nbEtudiant) { //trouvé

						if (verifCoef(&f, nSemestre, e,cmd)) {
							for (unsigned int m = 0; m < MAX_MATIERES; ++m) {
								if (f.etudiants[e].semestres[nSemestre - 1].matieres[m].nbNotes == f.etudiants[e].semestres[nSemestre - 1].matieres[m].nbEpreuves) n++;
								else n--;
							}
							if (n == 10) {
							//Affichage du relevé
								//Affiche les UE
								afficheUE(&f,cmd);

								//Affiche le nom de la matière et ses moyennes par UE
								afficheNomMatAvecMoyenne(&f, nSemestre, e);
								printf("--\n");

								//Affiche les moyennes par UE
								afficheMoyenneUEparSemestre(&f, nSemestre, e, cmd);
							}
							else printf("Il manque au moins une note pour cet etudiant\n");
						}
						else printf("Les coefficients de ce semestre sont incorrects\n");
					}
					else printf("Etudiant inconnu\n");
				}
				else printf("Le numero de semestre est incorrect\n");
			}//Fin test UE defini

		}//Fin C7

		//Commande 8
		else if (!strcmp(cmd, "decision")) {

			//Affiche le relevé des notes annuelles avec les UE acquises et la decision du jury
			//Precondition : nSemestre doit être egal à 1 ou 2, nomEtudiant doit être connu

			scanf("%s",nomEtudiant);
			int nbrEtuTemp = 0;
			if (f.nbUE == 0) printf("Le nombre d'UE n'est pas defini\n");
			else {
				unsigned int e = 0, n = 0;
				//Recherche etudiant connu
				while (e < f.nbEtudiant && strcmp(nomEtudiant, f.etudiants[e].nom) != 0) ++e;
				if (e < f.nbEtudiant) { //trouvé
					for (unsigned int s = 0; s < NB_SEMESTRES; ++s) {
						if (nbrEtuTemp != 0);
						else {
							if (verifCoef(&f, s, e, cmd)) {
								for (unsigned int s = 0; s < NB_SEMESTRES; ++s) {
									for (unsigned int m = 0; m < MAX_MATIERES; ++m) {
										if (f.etudiants[e].semestres[s].matieres[m].nbNotes == f.etudiants[e].semestres[s].matieres[m].nbEpreuves) n++;
										else n--;
									}
								}
								if (n == 20) {

									//Affichage du relevé et decision
									
										//Affiche les UE
										afficheUE(&f, cmd);

										//Affiche les moyennes par UE des semestres
										for (unsigned int sem = 0; sem < NB_SEMESTRES; ++sem) afficheMoyenneUEparSemestre(&f, sem, e, cmd);
										printf("--\n");

										//Affiche les moyennes annuelles de chaque UE, les acquisitions et la decision 
										afficheMoyenneAnnuelleAcqDev(&f,e);

								}
								else printf("Il manque au moins une note pour cet etudiant\n");
							}
							else printf("Les coefficients d'au moins un semestre sont incorrects\n");
						}
					++nbrEtuTemp;
					}
				}
				else printf("Etudiant inconnu\n");
			}//Fin test UE defini

		}//Fin C8

	}//Fin de la boucle infini

}//Fin du main