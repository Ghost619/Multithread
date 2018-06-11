// Coded by ScratchyCode
// Big number factorial with multithread
// Compile with -lgmp -pthread
#include <gmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// data structure passed to the thread
struct structure{
    long long int n;
    long long int lim;
};
// variables that will contain the splitted factorial
mpz_t prod1, prod2;

void *threadFunction(void *parameters);
void mainFactorial(long long int lim);
void threadFactorial(long long int lim, long long int n);

int main(){
    long long int n;
    
    printf("Calculate the factorial of number n.\nEnter a positive number: ");
    scanf("%lld",&n);
    
    printf("Calculating...\n");
    
    // split limit of the numbers to be calculated from threads
    long long int lim = n / 2;
    
    // copy the thread arguments in the structure
    struct structure threadArgs;
    threadArgs.n = n;
    threadArgs.lim = lim;
    
    // initialize and run the thread parallel to the main
    pthread_t thread2;
    pthread_create(&thread2,NULL,threadFunction,&threadArgs);
    
    // running main
    mainFactorial(lim);
    
    // waiting for the thread end
    pthread_join(thread2,NULL);
    
    // combine the two results using the distributive property
    mpz_mul(prod1,prod1,prod2);
    
    // writing result
    FILE *write = fopen("Factorial.txt","w");
    if(write == NULL){
        perror("\nError");
        exit(1);
    }
    
    fprintf(write,"%d! = ",n);
    mpz_out_str(write,10,prod1);
    fprintf(write,"\n");
    
    // close the result file and free the memory
    fclose(write);
    mpz_clear(prod1);
    mpz_clear(prod2);
    
    printf("Done.\n");
    
    return 0;
}

void *threadFunction(void *parameters){
    
    struct structure *dati = (struct structure *)parameters;
    
    threadFactorial(dati->lim,dati->n);
    
}

// factorial from 1 to lim = n/2
void mainFactorial(long long int lim){
    long long int i;
    
    mpz_init_set_ui(prod1,1);
    
    for(i=1; i<lim; i++){
        mpz_mul_ui(prod1,prod1,i);
    }
    
    return ;
}

// factorial from lim to n
void threadFactorial(long long int lim, long long int n){
    long long int i;
    
    mpz_init_set_ui(prod2,1);
    
    for(i=lim; i<=n; i++){
        mpz_mul_ui(prod2,prod2,i);
    }
    
    return ;
}
