#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <emmintrin.h>

#include "../inc/proto.h"

typedef char * caddr_t;

#ifdef PAPI
#include <papi.h>
#define NUMEVENTS 2
#endif

/**
 *\author François Rué
 *\version 0.1
 *\date 11 avril 2013
 *\file nbody.c
 *\brief Probleme a n-corps multidimensionnel 
 */

/*! \fn int main()
 *  \brief Programme principal initialisant les champs (position, masse)
 *  \param void
 *  \return Les points d'evolution
 */


int main() {

	int i,j,k,l,m;

#ifdef MATRIX
	/* double vitX, vitY, vitZ; */
	double uX, uY, uZ;
	double res, tmp1X, tmp1Y, tmp1Z;
	int tmp2;

	double  rX[npart];
	double  rY[npart];
	double  rZ[npart];
	double  rM[npart];

/* Structure de la Matrice */
        double m_val[2*npart-1] __attribute__ ((aligned (32))) ;
        int m_i[2*npart] __attribute__ ((aligned (8))) ;
        int m_j[npart] __attribute__ ((aligned (8))) ;

#else
	adble  rX[npart];
	adble  rY[npart];
	adble  rZ[npart];
	adble  rM[npart];

	__m128d r1d, r2d, r3d, r4d, r5d, r6d, temp, und, rMd;
	__m128d FXd, FYd, FZd;
	__m128d VXd, VYd, VZd;

	double storedX[2];
	double storedY[2];
	double storedZ[2];

	und = _mm_setr_pd(1.0,1.0);
#endif
	double FX, FY, FZ;
	
        struct timeval start, end;
#ifdef PAPI
	int retval,event_set=PAPI_NULL;
	long_long   values[NUMEVENTS];
	int events[NUMEVENTS] = {PAPI_FP_INS,PAPI_L2_DCM};

	/* Initialize PAPI Library */
	if(PAPI_library_init(PAPI_VER_CURRENT)!=PAPI_VER_CURRENT) exit(1);

	/* Create the eventset */
	if(PAPI_create_eventset(&event_set)!=PAPI_OK) exit(1);

	PAPI_add_events(event_set, events, 2);

#endif

	gettimeofday(&start, NULL);
        for (i=0;i<npart;i++) {
		rM[i]  = 1.89 * ( i + 1 ) ;
	}
        for (i=0;i<npart;i++) {
		rX[i]  = 10.2 * ( i + 1 ) ;
	}
        for (i=0;i<npart;i++) {
		rY[i]  = 10.3 * ( i + 1 ) ;
	}
        for (i=0;i<npart;i++) {
		rZ[i]  = 10.4 * ( i + 1 ) ;
	}

	printf("_________________________________________________________________________\n\n");


#ifdef PAPI
	if(PAPI_start_counters(events,NUMEVENTS)!=PAPI_OK) exit(1);
#endif
	for(k=0;k<niter;k++) {
		for(i=0;i<npart;i++) {
#ifdef MATRIX
		FX = 0.0;
		FY = 0.0;
		FZ = 0.0;
			for(j=0;j<npart;j++) {
				if(j!=i) {

					uX  = rX[i] - rX[j] ; 
					uY  = rY[i] - rY[j] ; 
					uZ  = rZ[i] - rZ[j] ; 

					res = sqrt (uX*uX + uY*uY + uZ*uZ) ;
					res = (double) 1.0 / res ;
/* Remplissage de la matrice creuse CSR => remplissage de (i,j,val)*/
					m_val[2*j]   = res;
					m_val[2*j+1] = -1.0 * res;
					m_i[2*j]     = j;
					m_i[2*j+1]   = i;
					m_j[j]       = 2*j;
				}
				else {
                                        m_val[2*j]   = 0.0 ;
                                        m_val[2*j+1] = 0.0 ;
                                        m_i[2*j]     = j;
                                        m_i[2*j+1]   = i;
                                        m_j[j]       = 2*j;
				}
/* Fin de remplissage de la matrice creuse CSR */
			}
			for(l=0;l<npart;l++) {
				tmp1X = 0.0;
				tmp1Y = 0.0;
				tmp1Z = 0.0;
				for(m=m_j[l];m<m_j[l+1];m++) {
				tmp2 = m_i[l];
				tmp1X = tmp1X + m_val[l] * rX[tmp2];
				tmp1Y = tmp1Y + m_val[l] * rY[tmp2];
				tmp1Z = tmp1Z + m_val[l] * rZ[tmp2];
				}
			}
		rX[i] = rX[i] + deltat * FX / rM[i];
		rY[i] = rY[i] + deltat * FY / rM[i];
		rZ[i] = rZ[i] + deltat * FZ / rM[i];
#else
		FXd = _mm_setzero_pd();
		FYd = _mm_setzero_pd();
		FZd = _mm_setzero_pd();
			for(j=0;j<npart;j+=2) {
				if(j!=i && (j+1)!=i) {
					r1d = _mm_setr_pd(rX[i],rX[i]);
					r2d = _mm_setr_pd(rX[j],rX[j+1]);
					r3d = _mm_setr_pd(rY[i],rY[i]);
					r4d = _mm_setr_pd(rY[j],rY[j+1]);
					r5d = _mm_setr_pd(rZ[i],rZ[i]);
					r6d = _mm_setr_pd(rZ[j],rZ[j+1]);
					temp = _mm_add_pd( _mm_mul_pd( _mm_sub_pd(r1d,r2d), _mm_sub_pd(r1d,r2d) ) , _mm_mul_pd( _mm_sub_pd(r3d,r4d), _mm_sub_pd(r3d,r4d) ));
					temp = _mm_add_pd( temp , _mm_mul_pd( _mm_sub_pd(r5d,r6d), _mm_sub_pd(r5d,r6d) ));
					temp = _mm_div_pd( und,  _mm_sqrt_pd( temp) );
					temp = _mm_mul_pd( _mm_mul_pd (temp,temp), temp);

					FXd  = _mm_add_pd( FXd, _mm_mul_pd( temp, _mm_sub_pd(r1d, r2d)));
					FYd  = _mm_add_pd( FYd, _mm_mul_pd( temp, _mm_sub_pd(r3d, r4d)));
					FZd  = _mm_add_pd( FZd, _mm_mul_pd( temp, _mm_sub_pd(r5d, r6d)));
				}
			}
/*
		rMd = _mm_load1_pd(&rM[i]);
		VXd = _mm_div_pd (FXd ,rMd);
		VYd = _mm_div_pd (FYd ,rMd);
		VZd = _mm_div_pd (FZd ,rMd);
*/
		_mm_store_pd(&storedX[0],FXd);
		_mm_store_pd(&storedY[0],FYd);
		_mm_store_pd(&storedZ[0],FZd);

		FX = storedX[0] + storedX[1];
		FY = storedY[0] + storedY[1];
		FZ = storedZ[0] + storedZ[1];

		rX[i] = rX[i] + deltat * FX / rM[i];
		rY[i] = rY[i] + deltat * FY / rM[i];
		rZ[i] = rZ[i] + deltat * FZ / rM[i];
#endif
		}
	}

#ifdef PAPI
	/* Write the number of cache misses for the Loop */
	if(PAPI_read_counters(values,NUMEVENTS)!=PAPI_OK) exit(1);

	printf("\tPAPI_FP_INS =%lli\n ",values[0]);
	printf("\tPAPI_L2_DCM =%lli\n ",values[1]);
	/* Stop counting */
	if(PAPI_stop_counters(values,NUMEVENTS) != PAPI_OK) exit(1);
	/* reset the PAPI counters */
	if(PAPI_reset(event_set)!=PAPI_OK) exit(1);
#endif
	gettimeofday(&end, NULL);
	printf("duree = %ld\n", ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)));
	printf("rX[%i] = %g \n",npart-1,rX[npart-1]);
	printf("rY[%i] = %g \n",1,rY[1]);
	printf("_________________________________________________________________________\n\n");

	return EXIT_SUCCESS;

}
