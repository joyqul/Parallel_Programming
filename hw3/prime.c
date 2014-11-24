#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

int isprime(int n) {
    int i, squareroot;
    if (n>10) {
        squareroot = (int) sqrt(n);
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
    int message[2];
    MPI_Status status;
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank( MPI_COMM_WORLD, &rank);
    MPI_Comm_size( MPI_COMM_WORLD, &size);
    int pc,       /* prime counter */
        foundone; /* most recent prime found */
    long long int n, limit;

    sscanf(argv[1], "%llu", &limit);	
    printf("Starting. Numbers to be scanned= %lld\n", limit);

    pc = 0;     /* Assume (2,3,5,7) are counted here */

    int st = 11 + (limit-11)/8*2*rank, ed = st + (limit-11)/8*2 - 1;
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
        MPI_Send(message, 2, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    else {
        int source;
        for (source = 1; source < size; ++source) {
            MPI_Recv(message, 2, MPI_INT, source, 0, MPI_COMM_WORLD, &status);
            pc += message[0];
            if (message[1] > foundone) foundone = message[1];
        }
        for (n = 11 + (limit-11)/8*8; n <= limit; n+=2) {
            if (isprime(n)) {
                pc++;
                foundone = n;
            }
        }
        pc += 4;
        printf("Done. Largest prime is %d Total primes %d\n",foundone,pc);
    }
    MPI_Finalize();

    return 0;
} 
