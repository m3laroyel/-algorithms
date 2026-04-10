#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main() {
    int n,p,i,count;
    
    printf("Enter n: ");
    scanf("%d", &n);
    
    if (n < 2) {
        printf("No prime numbers less than 2\n");
        return 0;
    }
    
    bool* isPrime = (bool*)malloc((n + 1) * sizeof(bool));
    
    for (i = 2; i <= n; i++) {
        isPrime[i] = true;
    }
    
    for (p = 2; p * p <= n; p++) {
        if (isPrime[p]) {
            for (i = p * p; i <= n; i += p) {
                isPrime[i] = false;
            }
        }
    }
    
    printf("Prime numbers up to %d:\n", n);
    count = 0;
    
    for (i = 2; i <= n; i++) {
        if (isPrime[i]) {
            printf("%d ", i);
            count++;
        }
    }
    printf("\n");
    printf("Found %d prime numbers\n", count);
    
    free(isPrime);
    
    return 0;
}
