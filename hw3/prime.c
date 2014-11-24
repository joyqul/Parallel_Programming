#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

int isprime(unsigned long long int n) {
    unsigned long long int i, squareroot;
    if (n>10) {
        squareroot = (unsigned long long int) sqrt(n);
        for (i=3; i<=squareroot; i=i+2)
            if ((n%i)==0)
                return 0;
        return 1;
    }
    else
        return 0;
}

int main(int argc, char *argv[])
{
    unsigned long long int message[2];
    MPI_Status status;
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank( MPI_COMM_WORLD, &rank);
    MPI_Comm_size( MPI_COMM_WORLD, &size);
    unsigned long long int pc,       /* prime counter */
        foundone; /* most recent prime found */
    unsigned long long int n, limit;

    sscanf(argv[1], "%llu", &limit);	
    printf("Starting. Numbers to be scanned= %llu\n", limit);

    pc = 0;     /* Assume (2,3,5,7) are counted here */

    unsigned long long int st = 11 + (limit-11)/(size*2)*2*rank, ed = st + (limit-11)/(size*2)*2 - 1;
    //printf("st: %d ed: %d\n", st, ed);
    for (n = st; n <= ed; n = n+2) {
        if (isprime(n)) {
            pc++;
            foundone = n;
        }			
    }
    if (rank != 0) {
        message[0] = pc;
        message[1] = foundone;
        MPI_Send(message, 2, MPI_UNSIGNED_LONG, 0, 0, MPI_COMM_WORLD);
    }
    else {
        int source;
        for (source = 1; source < size; ++source) {
            MPI_Recv(message, 2, MPI_UNSIGNED_LONG, source, 0, MPI_COMM_WORLD, &status);
            pc += message[0];
            if (message[1] > foundone) foundone = message[1];
        }
        for (n = 11 + (limit-11)/(size*2)*2*(size); n <= limit; n+=2) {
            if (isprime(n)) {
                pc++;
                foundone = n;
            }
        }
        pc += 4;
        printf("Done. Largest prime is %llu Total primes %llu\n", foundone, pc);
    }
    MPI_Finalize();

    return 0;
} 
