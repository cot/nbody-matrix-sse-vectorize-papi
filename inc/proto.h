/**
 * \author François Rué
 * \version 0.1
 * \date 11 avril 2013
 * \file proto.h
 * \brief fichier d'entête pour les prototypes de fonctions contenant les variables globales utiles au problème et quelques operateurs
 * \param npart nombre de particules
 * \param niter nombre d'iterations
 * \param deltat pas de temps du probleme (pour l'evaluation de la vitesse puis du deplacement)
 * \param dim dimension spatiale du probleme
 */
#include <emmintrin.h>

#define npart 100000
#define niter 1
#define deltat 0.001 

#define dim 3 

typedef double adble __attribute__ ((__aligned__(8)));

double Carre(double v);
double Norme(double a,double b,double c,double d,double e,double f);
double UnCube(double v);
double Distance(double v, double w);

