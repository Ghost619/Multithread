// Compile with -lgmp
#include <gmp.h>
#include <stdio.h>
#include <stdlib.h>

void factorial(long long int n);

int main(int argc, char * argv[]){
    long long int n=100000;
    
    printf("Calculating...\n");
    factorial(n);
    printf("Done.\n");
    
    return 0;
}

void factorial(long long int n){
    int i;
    mpz_t p;
    
    mpz_init_set_ui(p,1);       /* p = 1 */
    
    for(i=1; i<=n; i++){
        mpz_mul_ui(p,p,i);      /* p = p * i */
    }
    
    FILE *write = fopen("Factorial.txt","w");
    if(write == NULL){
        perror("\nError");
        exit(1);
    }
    
    fprintf(write,"%d! = ",n);
    mpz_out_str(write,10,p);
    fprintf(write,"\n");
    
    fclose(write);
    mpz_clear(p);
    
    return ;
}
