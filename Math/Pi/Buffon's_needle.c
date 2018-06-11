// Coded by ScratchyCode
// Approximates the pi value with the simulation of Buffon's needle launch, generating events in parallel with threads.
// Compile with -lm -pthread
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <pthread.h>

struct structure{
    long long int LIM;
    double L;
    double d;
    int threadID;
    double **matrix;
};

#define CORE 2
#define SIMULATIONS 1000
#define MAX_THETA (M_PI/2)

void *parallel(void *parameters);
double input1();
int input2();
double realRand(double min, double max);
double mean(double **matrix, int length);
double maxim(double **matrix, int dim);
double minim(double **matrix, int dim);
double **createMatrix(long long int row, long long int columns);
void zeroMatrix(long long int row, long long int columns, double **M);
void printMatrix(long long int row, long long int columns, double **M);
void freeMatrix(long long int row, double **M);
void statistics(double **matrix, int length, double *average, double *min, double *max);

int main(){
    double L, d, theta, x, pi;
    int N, S, i, j;
    srand48(time(NULL));
    
    do{
        printf("Insert the length 'L' of the Buffon needle (0cm < L): ");
        L = input1();
        
        printf("Enter the distance 'd' between the parallel lines (0cm < d): ");
        d = input1();
        
        if(L > d){
            printf("\nThe length of the needle must be less than the distance between the lines.\nTry again.\n");
        }
    }while(L > d);
    
    printf("Enter the number of launches to simulate: ");
    N = input2();
	
	long long int LIM = N / CORE;
    
    double **matrix = createMatrix(CORE,SIMULATIONS);
    zeroMatrix(CORE,SIMULATIONS,matrix);
    
    struct structure threadsMem[CORE];
    
    for(i=0; i<CORE-1; i++){
        threadsMem[i].LIM = LIM;
        threadsMem[i].L = L;
        threadsMem[i].d = d;
        threadsMem[i].threadID = i;
        threadsMem[i].matrix = matrix;
    }
	
	pthread_t *thread = malloc(CORE * sizeof(pthread_t));
	for(i=0; i<CORE-1; i++){
	    pthread_create(&thread[i],NULL,parallel,&threadsMem[i]);
    }
    
	for(i=0; i<CORE-1; i++){
	    pthread_join(thread[i],NULL);
	}
        
    double average, min, max;
    statistics(matrix,SIMULATIONS,&average,&min,&max);
        
    printf("\nMean:\t%lf",average);
    printf("\nMin:\t%lf",min);
    printf("\nMax:\t%lf\n",max);
    
    FILE *write = fopen("pi.dat","w");
    if(write == NULL){
        perror("\nError");
        exit(1);
    }
    
    for(i=0; i<CORE-1; i++){
        for(j=0; j<SIMULATIONS; j++){
            fprintf(write,"%lf\n",matrix[i][j]);
        }
    }
        
    fclose(write);
    
    freeMatrix(CORE,matrix);
    
    return 0;
}

void *parallel(void *parameters){
    struct structure *dati = (struct structure *) parameters;
    long long int i, j, S, ID=dati->threadID, N=dati->LIM;
    double L=dati->L, d=dati->d;
    double theta, x, pi;
    double **matrix=dati->matrix;
    
    for(i=0; i<SIMULATIONS; i++){
        S = 0;
        for(j=0; j<N; j++){
            theta = realRand(0,MAX_THETA);
            x = realRand(0,d/2);
            if(x < (((L)/2) * sin(theta))){
                S++;
            }
        }
        
        pi = (2*L*N) / (S*d);
        matrix[ID][i] = pi;
    }
	
	return NULL;
}

double input1(){
    double value;
    
    do{
        scanf("%lf",&value);
        if(value <= 0){
            printf("\nThe value must be greater than 0!\nTry again: ");
        }
    }while(value <= 0);
    
    return value;
}

int input2(){
    int value;
    
    do{
        scanf("%d",&value);
        if(value <= 0 || value % CORE != 0){
            printf("\nThe number of launches must be greater than 0 and multiple of the number of cores!\nTry again: ");
        }
    }while(value <= 0 || value % CORE != 0);
    
    return value;
}

double realRand(double min, double max){
    if(min >= max){
        printf("\nError: max value less than or equal to the min value entered!\n");
        exit(1);
    }
    
    double range = max - min;
    double denom = RAND_MAX / range;
    
    return min + lrand48() / denom;   
}

double mean(double **matrix, int length){
    int i, j, nElem=0;
    double sum=0;
    
    for(i=0; i<CORE-1; i++){
        for(j=0; j<SIMULATIONS; j++){
            sum += matrix[i][j];
            nElem++;
        }
    }
    
    sum /= nElem;
    
    return sum;
}

double maxim(double **matrix, int dim){
    int i, j;
    double max=-1E20;
    
    for(i=0; i<CORE-1; i++){
        for(j=0; j<SIMULATIONS; j++){
            if(max < matrix[i][j]){
                max = matrix[i][j];
            }
        }
    }
    
    return max;
}

double minim(double **matrix, int dim){
    int i, j;
    double min=1E20;
    
    for(i=0; i<CORE-1; i++){
        for(j=0; j<SIMULATIONS; j++){
            if(min > matrix[i][j]){
                min = matrix[i][j];
            }
        }
    }
    
    return min;
}

double **createMatrix(long long int row, long long int columns){
    long long int i;
    
    double **M = (double **) malloc(row * sizeof(double *));
    if(M == NULL){
        perror("\nError");
        printf("\n");
        exit(2);
    }
    
    for(i=0; i<row; i++){
        M[i] = (double *) malloc(columns * sizeof(double));
        if(M == NULL){
            perror("\nError");
            printf("\n");
            exit(3);
        }
    }
    
    return M;
}

void zeroMatrix(long long int row, long long int columns, double **M){
    long long int i, j;
    
    for(i=0; i<row; i++)
        for(j=0; j<columns; j++)
            M[i][j] = 0;
            
    return ;
}

void printMatrix(long long int row, long long int columns, double **M){
    long long int i, j;
    
    printf("\n");
    
    for(i=0; i<row; i++){
        for(j=0; j<columns; j++){
            printf("%lf ",M[i][j]);
        }
        printf("\n");
    }

    return ;
}

void freeMatrix(long long int row, double **M){

    while(--row > -1){
        free(M[row]);
    }
    
    free(M);
    
    return ;
}

void statistics(double **matrix, int length, double *average, double *min, double *max){
    
    *average = mean(matrix,length);
    *min = minim(matrix,length);
    *max = maxim(matrix,length);
    
    return ;
}
