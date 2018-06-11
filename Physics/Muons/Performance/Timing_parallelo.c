// Coded by ScratchyCode
// test prestazioni (corretto)
/*
VALORI DI RIFERIMENTO:
Muoni:
    tempo decadimento:      2.2 (10^-6 s)
    spessore atmosfera:     60000 (metri)
    velocità:               0.999c
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

#define CORE 2
#define C 299792458
#define MICRO 1E-6

struct struttura{
    long long int N;
    long double tau;
    long double beta;
    long double D;
};

long long int contaParticelle = 0;

void *parallela(void *parametri);
long double input(long double min, long double max);
long double motoUnifAcc(long double a, long double v, long double t);
long double motoUnif(long double v, long double t);
long double randExp(long double tau);
long double dilatazioneTempo(long double tau, long double beta);

int main(){
    long long int N=1000000000, i;
    long double tau0=2.2, beta=0.999, D=60000, tau;
    long double frazione;
    
    printf("\nCalcolo...\n");
    clock_t inizio = clock();
    
    tau = dilatazioneTempo(tau0,beta);
    
    // passo gli argomenti dei thread alla struttura
    struct struttura argomenti_thread;
    argomenti_thread.N = N;
    argomenti_thread.tau = tau;
    argomenti_thread.beta = beta;
    argomenti_thread.D = D;
    
    // inizializzo i thread i li eseguo parallelamente al main
    pthread_t *thread = malloc(CORE * sizeof(pthread_t));
    for(i=0; i<CORE; i++){
        pthread_create(&thread[i],NULL,parallela,&argomenti_thread);
    }

    // aspetto la fine di tutti i thread
    for(i=0; i<CORE; i++){
        pthread_join(thread[i],NULL);
    }
    
    frazione = (long double)(contaParticelle) / (long double)(N);
    clock_t fine = clock();
    double tempo = (double)(fine - inizio) / CLOCKS_PER_SEC;
    printf("\nIl %.5LF%% delle particelle hanno superato %.2LF metri.\n",frazione*100,D);
    printf("Calcolo eseguito in %lf s.\n\n",tempo);
    
    return 0;
}

void *parallela(void *parametri){
    long long int i;
    long double t_vita, Dx;
    
    srand48(time(NULL));
    
    struct struttura *dati = (struct struttura *) parametri;
    
    for(i=0; i<(long long int)((dati->N)/CORE); i++){
        t_vita = randExp(dati->tau);
        Dx = motoUnif(dati->beta*C,t_vita);
        if(Dx > dati->D){
            contaParticelle++;
        }
    }
    
    return NULL;
}

long double input(long double min, long double max){
    // prende un numero x da tastiera che sia compreso in [min, max[
    long double x;
    
    do{
        scanf("%LF",&x);
        if(!(min <= x && x < max)){
            printf("Il numero deve essere compreso tra %.3LF e %.3LF.\nRiprova: ",min,max);
        }
    }while(!(min <= x && x < max));
    
    return x;
}

long double motoUnifAcc(long double a, long double v, long double t){
    return v*t + 0.5*a*t*t;
}

long double motoUnif(long double v, long double t){
    return motoUnifAcc(0,v,t);
}

long double randExp(long double tau){
    // genera un numero casuale secondo la distribuzione e^-(t/tau)
    long double G = ((long double)lrand48()+1)/((long double)RAND_MAX+1);
    return -tau*log(G);
}

long double dilatazioneTempo(long double tau, long double beta){
    return tau/sqrt(1-beta*beta);
}
