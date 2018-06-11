// Coded by ScratchyCode
// Compile in gcc with options -lm -pthread
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <unistd.h>

// arguments passed to thread
struct arguments{
    unsigned long long int dim;
    unsigned long long int lim;
    unsigned long long int *vect2;
    unsigned long long int index;
};

void *threadFunction(void *parameters);
int test(unsigned long long int num);

int main(){
    unsigned long long int i, lim, index=0;
    unsigned long long int *vect1, *vect2;
    char log[] = "Primes.txt";
    pthread_t thread;
    
    printf("\nEnter a maximum number: ");
    scanf("%llu",&lim);
    
    unsigned long long int dim = (unsigned long long int)(lim/2.);
    vect1 = malloc(dim * sizeof(unsigned long long int));
    vect2 = malloc(dim * sizeof(unsigned long long int));
    if(vect1 == NULL || vect2 == NULL){
        perror("\nError");
        exit(1);
    }
    
    printf("\aCalculation in progress, please wait...\n");
    
    // copying the thread arguments into the structure
    struct arguments thread_argument;
    thread_argument.dim = dim+1;
    thread_argument.lim = lim;
    thread_argument.vect2 = vect2;
    
    // running threadFunction parallel to the main
    pthread_create(&thread,NULL,threadFunction,&thread_argument);
    
    // perform the primality checks
    for(i=3; i<=dim; i++){
        if(i%2 == 0){
            // exclusion of even numbers
            continue;
        }else{
            if(test(i)){
                vect1[index] = i;
                index++;
            }
        }
    }
    
    // wait for thread end
    pthread_join(thread,NULL);
    
    printf("Completed.\n");
    
    // writing results
    FILE *fp;
    fp = fopen(log,"w");
    if(fp == NULL){
	    perror("\nError");
        exit(1);
    }
    
    fprintf(fp,"1\n2\n3\n");
    
    for(i=0; i<index; i++){
        fprintf(fp,"%llu\n",vect1[i]);
    }
    index = thread_argument.index;
    for(i=0; i<index; i++){
        fprintf(fp,"%llu\n",vect2[i]);
    }
    
    fclose(fp);
    free(vect1);
    free(vect2);
    
    return 0;
}

void *threadFunction(void *parameters){
    unsigned long long int i, num, lim, index=0, *vect2;
    struct arguments *data = (struct arguments *) parameters;
    
    num = data->dim;
    lim = data->lim;
    vect2 = data->vect2;
    
    // perform the primality checks
    for(i=num; i<=lim; i++){
	    if(i%2 == 0){
            // exclusion of even numbers
            continue;
        }else{
            if(test(i)){
                vect2[index] = i;
                index++;
            }
        }
    }
    
    data->index = index;
    
    return NULL;
}

int test(unsigned long long int num){
    unsigned long long int i;
    unsigned long long int lim;
    
    lim = (unsigned long long int)(sqrtl((long double)num));
    
    unsigned long long int tmp = 1;
    do{
        i = 2*tmp+1;
        if(num%i == 0){
            return 0;
        }
        tmp++;
    }while(i <= lim);
    
    return 1;
}
