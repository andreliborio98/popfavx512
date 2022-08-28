#include "OPF.h"
#include <stdio.h>
#include <immintrin.h>
#include <time.h>
#include <sys/time.h>

float euclAvx512(float *f1, float *f2, int n){
    int i;
    float dist=0.0f;
    //n=48;
    int nRest=n;

#ifdef VECT_AVX
  __m512 R_f1, R_f2;
  __m256 R_f12, R_f22;
  __m512 R_acc = _mm512_setzero_ps();
  __m256 R_acc2 = _mm256_setzero_ps();

	if (nRest>=16){
   	  nRest-=16;
	  for (i=0; i < n-16; i+=16){
	    R_f1 = _mm512_load_ps(&f1[i]);
	    R_f2 = _mm512_load_ps(&f2[i]);
	    R_acc = R_acc + ((R_f1 - R_f2) * (R_f1 - R_f2));
	  }
	  __m256 low  = _mm512_castps512_ps256(R_acc);
	  __m256 high = _mm256_castpd_ps(_mm512_extractf64x4_pd(_mm512_castps_pd(R_acc),1));
	  R_acc2 = _mm256_add_ps(low, high);
	  R_acc2 = _mm256_hadd_ps(R_acc2, R_acc2);
	  R_acc2 = _mm256_hadd_ps(R_acc2, R_acc2);

		float tmp2[8] __attribute__((aligned(32)));
	  _mm256_store_ps(tmp2, R_acc2);
	  dist += tmp2[0];
	}
	
	if (nRest>=8 && nRest<16){
	  nRest-=8;
	  for (i=0; i < n-8; i+=8){
	    R_f12 = _mm256_load_ps(&f1[i]);
	    R_f22 = _mm256_load_ps(&f2[i]);
	    R_acc2 = R_acc2 + ((R_f12 - R_f22) * (R_f12 - R_f22));
	  }
	  __m256 tmp = _mm256_permute2f128_ps(R_acc2,R_acc2, 0x1);
	  R_acc2 = _mm256_add_ps(R_acc2, tmp);
	  R_acc2 = _mm256_hadd_ps(R_acc2, R_acc2);
	  R_acc2 = _mm256_hadd_ps(R_acc2, R_acc2);

	  float tmp2[8] __attribute__((aligned(32)));
	  _mm256_store_ps(tmp2, R_acc2);
	  dist += tmp2[0];
	}

  for (; i < n; i++)
    dist += (f1[i]-f2[i])*(f1[i]-f2[i]);
#else
#ifdef VECT_OMP_SIMD
#pragma omp simd reduction(+:dist) aligned(f1, f2:32)
#endif /* VECT_OMP_SIMD */
  for (i=0; i < n; i++)
    dist += (f1[i]-f2[i])*(f1[i]-f2[i]);
#endif /* NO VECT */
    //printf ("%f\n", dist);
  return (dist);
}

float euclAvx512u(float *f1, float *f2, int n){
    int i;
    float dist=0.0f;
    //n=48;
    int nRest=n;

#ifdef VECT_AVX
  __m512 R_f1, R_f2;
  __m256 R_f12, R_f22;
  __m512 R_acc = _mm512_setzero_ps();
  __m256 R_acc2 = _mm256_setzero_ps();

	if (nRest>=16){
	  nRest-=16;
	  for (i=0; i < n-16; i+=16){
	    R_f1 = _mm512_loadu_ps(&f1[i]);
	    R_f2 = _mm512_loadu_ps(&f2[i]);
	    R_acc = R_acc + ((R_f1 - R_f2) * (R_f1 - R_f2));
	  }
	  __m256 low  = _mm512_castps512_ps256(R_acc);
	  __m256 high = _mm256_castpd_ps(_mm512_extractf64x4_pd(_mm512_castps_pd(R_acc),1));
	  R_acc2 = _mm256_add_ps(low, high);
	  R_acc2 = _mm256_hadd_ps(R_acc2, R_acc2);
	  R_acc2 = _mm256_hadd_ps(R_acc2, R_acc2);

		float tmp2[8] __attribute__((aligned(32)));
	  _mm256_store_ps(tmp2, R_acc2);
	  dist += tmp2[0];
	}
	
	if (nRest>=8 && nRest<16){
      nRest-=8;
	  for (i=0; i < n-8; i+=8){
	    R_f12 = _mm256_loadu_ps(&f1[i]);
	    R_f22 = _mm256_loadu_ps(&f2[i]);
	    R_acc2 = R_acc2 + ((R_f12 - R_f22) * (R_f12 - R_f22));
	  }
	  __m256 tmp = _mm256_permute2f128_ps(R_acc2,R_acc2, 0x1);
	  R_acc2 = _mm256_add_ps(R_acc2, tmp);
	  R_acc2 = _mm256_hadd_ps(R_acc2, R_acc2);
	  R_acc2 = _mm256_hadd_ps(R_acc2, R_acc2);

	  float tmp2[8] __attribute__((aligned(32)));
	  _mm256_store_ps(tmp2, R_acc2);
	  dist += tmp2[0];
	}

  for (; i < n; i++)
    dist += (f1[i]-f2[i])*(f1[i]-f2[i]);
#else
#ifdef VECT_OMP_SIMD
#pragma omp simd reduction(+:dist) aligned(f1, f2:32)
#endif /* VECT_OMP_SIMD */
  for (i=0; i < n; i++)
    dist += (f1[i]-f2[i])*(f1[i]-f2[i]);
#endif /* NO VECT */
    //printf ("%f\n", dist);
  return (dist);
}

int main(int argc, char **argv){
	fflush(stdout);
	fprintf(stdout, "\nProgram that generates the precomputed distance file for the OPF classifier\n");
	fprintf(stdout, "\nIf you have any problem, please contact: ");
	fprintf(stdout, "\n- alexandre.falcao@gmail.com");
	fprintf(stdout, "\n- papa.joaopaulo@gmail.com\n");
	fprintf(stdout, "\nLibOPF version 2.0 (2009)\n");
	fprintf(stdout, "\n"); fflush(stdout);

	if(argc != 4){
		fprintf(stderr, "\nusage opf_distance <P1> <P2> <P3>");
		fprintf(stderr, "\nP1: Dataset in the OPF file format");
		fprintf(stderr, "\nP2: Distance ID\n");
		fprintf(stderr, "\n	1 - Euclidean");
		fprintf(stderr, "\n	2 - Chi-Square");
		fprintf(stderr, "\n	3 - Manhattan (L1)");
		fprintf(stderr, "\n	4 - Canberra");
		fprintf(stderr, "\n	5 - Squared Chord");
		fprintf(stderr,"\n	6 - Squared Chi-Squared");
		fprintf(stderr,"\n	7 - BrayCurtis");
		fprintf(stderr, "\nP3: Distance normalization? 1- yes 0 - no");
		exit(-1);
	}

	Subgraph *sg = ReadSubgraph(argv[1]);
	FILE *fp = fopen("distances.dat", "wb");
	int i, j, distance = atoi(argv[2]), normalize = atoi(argv[3]);
	float **Distances = NULL, max = FLT_MIN;
	struct timeval start, end; //gettimeofday
	double t;

	fwrite(&sg->nnodes, sizeof(int), 1, fp);

	Distances  = (float **)malloc(sg->nnodes*sizeof(float *));
	for (i = 0; i < sg->nnodes; i++)
		Distances[i] = (float *)malloc(sg->nnodes*sizeof(int));

	switch(distance){
		case 1:
			fprintf(stdout, "\n	Computing euclidean distance ...");
			gettimeofday(&start, NULL);
			for (i = 0; i < sg->nnodes; i++){
				for (j = 0; j < sg->nnodes; j++){
					if(i == j) Distances[i][j] = 0.0;
					else {
						Distances[sg->node[i].position][sg->node[j].position] = opf_EuclDist(sg->node[i].feat, sg->node[j].feat, sg->nfeats);
						//printf ("%f\n", opf_EuclDist(sg->node[i].feat, sg->node[j].feat, sg->nfeats));
					}
					if(Distances[sg->node[i].position][sg->node[j].position] > max) max = Distances[sg->node[i].position][sg->node[j].position];
				}
			}
			gettimeofday(&end, NULL);
			t = (double) ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)) / 1000000.0;
			printf ("\nt %f\n", t);
		break;
		case 2:
			fprintf(stdout, "\n	Computing chi-square distance ...\n");
			for (i = 0; i < sg->nnodes; i++){
				for (j = 0; j < sg->nnodes; j++){
					if(i == j) Distances[i][j] = 0.0;
					else Distances[sg->node[i].position][sg->node[j].position] = opf_ChiSquaredDist(sg->node[i].feat, sg->node[j].feat, sg->nfeats);
					if(Distances[sg->node[i].position][sg->node[j].position] > max) max = Distances[sg->node[i].position][sg->node[j].position];
				}
			}
		break;
		case 3:
			fprintf(stdout, "\n	Computing Manhattan distance ...\n");
			for (i = 0; i < sg->nnodes; i++){
				for (j = 0; j < sg->nnodes; j++){
					if(i == j) Distances[i][j] = 0.0;
					else Distances[sg->node[i].position][sg->node[j].position] = opf_ManhattanDist(sg->node[i].feat, sg->node[j].feat, sg->nfeats);
					if(Distances[sg->node[i].position][sg->node[j].position] > max) max = Distances[sg->node[i].position][sg->node[j].position];
				}
			}
		break;
		case 4:
			fprintf(stdout, "\n	Computing Canberra distance ...\n");
			for (i = 0; i < sg->nnodes; i++){
				for (j = 0; j < sg->nnodes; j++){
					if(i == j) Distances[i][j] = 0.0;
					else Distances[sg->node[i].position][sg->node[j].position] = opf_CanberraDist(sg->node[i].feat, sg->node[j].feat, sg->nfeats);
					if(Distances[sg->node[i].position][sg->node[j].position] > max) max = Distances[sg->node[i].position][sg->node[j].position];
				}
			}
		break;
		case 5:
			fprintf(stdout, "\n	Computing Squared Chord distance ...\n");
			for (i = 0; i < sg->nnodes; i++){
				for (j = 0; j < sg->nnodes; j++){
					if(i == j) Distances[i][j] = 0.0;
					else Distances[sg->node[i].position][sg->node[j].position] = opf_SquaredChordDist(sg->node[i].feat, sg->node[j].feat, sg->nfeats);
					if(Distances[sg->node[i].position][sg->node[j].position] > max) max = Distances[sg->node[i].position][sg->node[j].position];
				}
			}
		break;
		case 6:
			fprintf(stdout, "\n	Computing Squared Chi-squared distance ...\n");
			for (i = 0; i < sg->nnodes; i++){
				for (j = 0; j < sg->nnodes; j++){
					if(i == j) Distances[i][j] = 0.0;
					else Distances[sg->node[i].position][sg->node[j].position] = opf_SquaredChiSquaredDist(sg->node[i].feat, sg->node[j].feat, sg->nfeats);
					if(Distances[sg->node[i].position][sg->node[j].position] > max) max = Distances[sg->node[i].position][sg->node[j].position];
				}
			}
		break;
		case 7:
			fprintf(stdout, "\n	Computing Bray Curtis distance ...\n");
			for (i = 0; i < sg->nnodes; i++){
				for (j = 0; j < sg->nnodes; j++){
					if(i == j) Distances[i][j] = 0.0;
					else Distances[sg->node[i].position][sg->node[j].position] = opf_BrayCurtisDist(sg->node[i].feat, sg->node[j].feat, sg->nfeats);
					if(Distances[sg->node[i].position][sg->node[j].position] > max) max = Distances[sg->node[i].position][sg->node[j].position];
				}
			}
		break;
		case 8: //8 (extra)
			fprintf(stdout, "\n	Computing avx512 euclidean distance ...");
			gettimeofday(&start, NULL);
			for (i = 0; i < sg->nnodes; i++){
				for (j = 0; j < sg->nnodes; j++){
					if(i == j) Distances[i][j] = 0.0;
					else{
						Distances[sg->node[i].position][sg->node[j].position] = euclAvx512(sg->node[i].feat, sg->node[j].feat, sg->nfeats);
						} 
					if(Distances[sg->node[i].position][sg->node[j].position] > max) max = Distances[sg->node[i].position][sg->node[j].position];
				}
			}
			gettimeofday(&end, NULL);
			t = (double) ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)) / 1000000.0;
			printf ("\nt %f\n", t);
		break; 
		case 9: //8+u (extra loadu)
			fprintf(stdout, "\n	Computing avx512 euclidean distance loadu...");
			gettimeofday(&start, NULL);
			for (i = 0; i < sg->nnodes; i++){
				for (j = 0; j < sg->nnodes; j++){
					if(i == j) Distances[i][j] = 0.0;
					else{
						Distances[sg->node[i].position][sg->node[j].position] = euclAvx512u(sg->node[i].feat, sg->node[j].feat, sg->nfeats);
						} 
					if(Distances[sg->node[i].position][sg->node[j].position] > max) max = Distances[sg->node[i].position][sg->node[j].position];
				}
			}
			gettimeofday(&end, NULL);
			t = (double) ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)) / 1000000.0;
			printf ("\nt %f\n", t);
		break; 

		default:
			fprintf(stderr, "\nInvalid distance ID ...\n");
	}

	if (!normalize) max = 1.0;
	for (i = 0; i < sg->nnodes; i++){
		for (j = 0; j < sg->nnodes; j++){
			Distances[i][j]/=max;
			fwrite(&Distances[i][j], sizeof(float), 1, fp);
		}
	}

	fprintf(stdout, "\n\nDistances generated ...\n"); fflush(stdout);
	fprintf(stdout, "\n\nDeallocating memory ...\n");
	for (i = 0; i < sg->nnodes; i++)
		free(Distances[i]);
	free(Distances);

	DestroySubgraph(&sg);
	fclose(fp);


	return 0;
}
