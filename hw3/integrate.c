#include <stdio.h>
#include <math.h>
#include <mpi.h>

#define PI 3.1415926535

int main(int argc, char **argv) 
{
    double message[1];
    MPI_Status status;
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank( MPI_COMM_WORLD, &rank);
    MPI_Comm_size( MPI_COMM_WORLD, &size);
    long long i, num_intervals;
    double rect_width, area, sum, x_middle; 

    sscanf(argv[1],"%llu", &num_intervals);
    printf("Starting. Numbers to be scanned= %lld\n", num_intervals);

    rect_width = PI / num_intervals;

    sum = 0;
    long long st = 1 + num_intervals/size*rank, ed = num_intervals/size*(rank+1);
    //printf("rank:%d st:%d ed:%d\n", rank, st, ed);
    for(i = st; i <= ed ; i++) {

        /* find the middle of the interval on the X-axis. */ 
        x_middle = (i - 0.5) * rect_width;
        area = sin(x_middle) * rect_width; 
        sum = sum + area;
    } 

    if (rank != 0) {
        message[0] = sum;
        MPI_Send(message, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }
    else {
        int source;
        for (source = 1; source < size; ++source) {
            MPI_Recv(message, 1, MPI_DOUBLE, source, 0, MPI_COMM_WORLD, &status);
            sum += message[0];
        }
        for (i = num_intervals/size*size + 1; i <= num_intervals; ++i) {
            x_middle = (i - 0.5) * rect_width;
            area = sin(x_middle) * rect_width; 
            sum = sum + area;
        }
        printf("The total area is: %f\n", (float)sum);
    }
    MPI_Finalize();

    return 0;
}   
