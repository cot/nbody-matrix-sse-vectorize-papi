/**
 *\author François Rué
 *\version 0.1
 *\date 15 avril 2013
 *\file proto.c
 *\brief Probleme a n-corps multidimensionnel pour le cas d'effet gravitationnel
 */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "../inc/proto.h"

/*! \fn double Distance(double v, double w)
 *  \brief Permet d'evaluer la distance entre deux points: cette fonction permet d'évaluer chaque entitée dans chaque direction
 *  \param[in] "v" première valeur d'entrée
 *  \param[in] "w" seconde valeur d'entrée
 *  \return toutes les fonctions de proto.c sont créées pour n'effectuer que des retours de type double
 */
double Distance(double v, double w) {
	return( v - w );
}

/*! \fn double Carre(double v)
 *  \brief Permet d'evaluer le carre d'un nombre
 *  \param[in] "v" première valeur d'entrée
 *  \return toutes les fonctions de proto.c sont créées pour n'effectuer que des retours de type double
 */
double Carre(double v) {
	return( v * v );
}

/*! \fn double Norme(double A[3], double B[3])
 *  \brief Permet d'evaluer la norme L2 de la différence de deux vecteurs A et B, représentés en coordonnées cartésiennes (x,y,z)
 *  \param[in] "a" premiere coordonnee suivant x
 *  \param[in] "b" deuxieme coordonnee suivant x
 *  \param[in] "c" premiere coordonnee suivant y
 *  \param[in] "d" deuxieme coordonnee suivant y
 *  \param[in] "e" premiere coordonnee suivant z
 *  \param[in] "f" deuxieme coordonnee suivant z
 *  \return toutes les fonctions de proto.c sont créées pour n'effectuer que des retours de type double
 */
double Norme(double a,double b,double c,double d,double e,double f) {
	return(sqrt( Carre(Distance(a,b)) + Carre(Distance(c,d)) + Carre(Distance(e,f)) ) ) ;
}

/*! \fn double UnCube(double v)
 *  \brief Permet d'evaluer l'inverse d'une valeur élevée au cube
 *  \param[in] "v" valeur à évaluer au cube et à inverser (dans le cas n-body: la distance entre deux entitées)
 *  \return toutes les fonctions de proto.c sont créées pour n'effectuer que des retours de type double
 */
double UnCube(double v) {
	return(1.0 / (v*v*v));
}

