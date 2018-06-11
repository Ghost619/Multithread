// Coded by ScratchyCode
// Test prestazioni
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define C 299792458

long double input(long double min, long double max);
long double motoUnifAcc(long double a, long double v, long double t);
long double motoUnif(long double v, long double t);
long double randExp(long double tau);
long double dilatazioneTempo(long double tau, long double beta);

int main(){
    long long int N=1000000000;
    long double tau0=2.2, beta=0.999, D=60000;
    srand48(time(0));
    
    long double tau = dilatazioneTempo(tau0,beta);
    long double t_vita, Dx;
    long long int contaParticelle = 0;
    long long int i;
    
    printf("Calcolo...\n");
    clock_t inizio = clock();
    for(i=0; i<N; i++){
        t_vita = randExp(tau);
        Dx = motoUnif(beta*C,t_vita);
        if(Dx > D){
            contaParticelle++;
        }
    }

    long double frazione = (long double)contaParticelle / N;
    clock_t fine = clock();
    double tempo = (double)(fine - inizio) / CLOCKS_PER_SEC;
    printf("\nIl %.5LF%% delle particelle hanno superato %.2LF metri.\n",frazione*100,D);
    printf("Calcolo eseguito in %lf s.\n\n",tempo);
    
    return 0;
}

long double input(long double min, long double max){
    /* Prende un numero da tastiera che sia compreso in [min, max[ */
    long double x;
    
    do{
        scanf("%lf",&x);
        if(!(min<=x && x<max)){
            printf("Il numero deve essere compreso tra %.3lf e %.3lf.\nRiprova: ",min,max);
        }
    }while(!(min <= x && x < max));
    
    return x;
}

long double motoUnifAcc(long double a, long double v, long double t){
    return v*t+0.5*a*t*t;
}

long double motoUnif(long double v, long double t){
    return motoUnifAcc(0,v,t);
}

long double randExp(long double tau){
    // Genera un numero casuale secondo la distribuzione e^-(t/tau)
    long double G = ((long double)lrand48()+1)/((long double)RAND_MAX+1);
    return -tau*log(G);
}

long double dilatazioneTempo(long double tau, long double beta){
    return tau/sqrt(1-beta*beta);
}
