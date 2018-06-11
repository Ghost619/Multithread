// Coded by ScratchyCode
// Compile in gcc with options -lm -pthread
/*
*REFERENCE VALUES*
Muons:
    decay time:             2.2 (10^-6 s)
    atmosphere thickness:   60000 (meters)
    speed:                  0.999c
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

#define CORE 2          // CPU core number
#define C 299792458     // speed of light
#define MICRO 1E-6

struct arguments{
    long long int N;
    long double tau;
    long double beta;
    long double D;
};

long long int particlesCount = 0;

void *threadFunction(void *parameters);
long double input(long double min, long double max);
long double unifAccelMot(long double a, long double v, long double t);
long double unifMot(long double v, long double t);
long double randExp(long double tau);
long double timeDilation(long double tau, long double beta);

int main(){
    long long int N, i;
    long double tau0, beta, D, tau, t_life, Dx;
    long double fraction;
    
    printf("\nThis program simulates the decay of N particles moving at relativistic speeds.\n\n");
    
    printf("Enter the particles number: ");
    N = input(1,1E15);
    // N divisible exactly
    while((N%CORE) != 0){
        N--;
    }
    
    printf("Enter the particles average life (in microseconds): ");
    tau0 = input(1,1E12) * MICRO;
    
    printf("Enter the particle velocity (fraction of c): ");
    beta = input(0,1);
    
    printf("Enter the distance that the particles must travel (in meters): ");
    D = input(0,1E12);
    
    printf("\nComputing...\n");
    
    tau = timeDilation(tau0,beta);
    
    // pass thread arguments to structure
    struct arguments thread_arguments;
    thread_arguments.N = N;
    thread_arguments.tau = tau;
    thread_arguments.beta = beta;
    thread_arguments.D = D;
    
    // initialize threads and run them parallel to the main
    pthread_t *thread = malloc((CORE-1) * sizeof(pthread_t));
    
    for(i=0; i<(CORE-1); i++){
        pthread_create(&thread[i],NULL,threadFunction,&thread_arguments);
    }
    
    // main execution
    srand48(time(0));
    
    for(i=0; i<(long long int)((long double)N/(long double)CORE); i++){
    	t_life = randExp(tau);
    	Dx = unifMot(beta*C,t_life);
    	if(Dx > D){
            particlesCount++;
        }
    }
    
    // wait for the end of all the threads
    for(i=0; i<(CORE-1); i++){
        pthread_join(thread[i],NULL);
    }
    
    fraction = (long double)(particlesCount) / (long double)(N);
    
    printf("\nThe %.5llf%% of particles have exceeded %.2llf meters.\n\n",fraction*100,D);
    
    return 0;
}

void *threadFunction(void *parameters){
    long long int i;
    long double t_life, Dx;
    
    srand48(time(0));
    
    struct arguments *data = (struct arguments *) parameters;
    
    for(i=0; i<(long long int)((data->N)/CORE); i++){
        t_life = randExp(data->tau);
        Dx = unifMot(data->beta*C,t_life);
        if(Dx > data->D){
            particlesCount++;
        }
    }
}

long double input(long double min, long double max){
    // takes an x number from the keyboard between [min, max[
    long double x;
    
    do{
        scanf("%llf",&x);
        if(!(min <= x && x < max)){
            printf("Il numero deve essere compreso tra %.3llf e %.3llf.\nRiprova: ",min,max);
        }
    }while(!(min <= x && x < max));
    
    return x;
}

long double unifAccelMot(long double a, long double v, long double t){
    return v*t + 0.5*a*t*t;
}

long double unifMot(long double v, long double t){
    return unifAccelMot(0,v,t);
}

long double randExp(long double tau){
    // generates a random number according to the distribution: e^-(t/tau)
    long double G = ((long double)lrand48()+1)/((long double)RAND_MAX+1);
    return -tau*log(G);
}

long double timeDilation(long double tau, long double beta){
    return tau/sqrt(1-beta*beta);
}
