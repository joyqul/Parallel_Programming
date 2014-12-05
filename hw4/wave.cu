/**********************************************************************
 * DESCRIPTION:
 *   Serial Concurrent Wave Equation - C Version
 *   This program implements the concurrent wave equation
 *********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <cuda.h>

#define MAXPOINTS 1000000
#define MAXSTEPS 1000000
#define MINPOINTS 20
#define PI 3.14159265
#define SIZE (MAXPOINTS+22)*sizeof(float)

void check_param(void);
void init_line(void);
void update (void);
void printfinal (void);

int nsteps,                 	/* number of time steps */
    tpoints, 	     		/* total points along string */
    rcode;                  	/* generic return code */
float values[MAXPOINTS+22], 	/* values at time t */
      oldval[MAXPOINTS+22], 	/* values at time (t-dt) */
      newval[MAXPOINTS+22]; 	/* values at time (t+dt) */
float *gvalues, *goldval, *gnewval;

int grid, block;


/**********************************************************************
 *	Checks input values from parameters
 *********************************************************************/
void check_param(void)
{
    char tchar[20];

    /* check number of points, number of iterations */
    while ((tpoints < MINPOINTS) || (tpoints > MAXPOINTS)) {
        printf("Enter number of points along vibrating string [%d-%d]: "
                ,MINPOINTS, MAXPOINTS);
        scanf("%s", tchar);
        tpoints = atoi(tchar);
        if ((tpoints < MINPOINTS) || (tpoints > MAXPOINTS))
            printf("Invalid. Please enter value between %d and %d\n", 
                    MINPOINTS, MAXPOINTS);
    }
    while ((nsteps < 1) || (nsteps > MAXSTEPS)) {
        printf("Enter number of time steps [1-%d]: ", MAXSTEPS);
        scanf("%s", tchar);
        nsteps = atoi(tchar);
        if ((nsteps < 1) || (nsteps > MAXSTEPS))
            printf("Invalid. Please enter value between 1 and %d\n", MAXSTEPS);
    }

    printf("Using points = %d, steps = %d\n", tpoints, nsteps);

}

__global__ void VecAdd(float* A, int n) {
    /* Calculate initial values based on sine curve */
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    A[i] = sin(2.0 * PI * ((float)i - 1.0) / (float)(n - 1));
}

/**********************************************************************
 *     Initialize points on line
 *********************************************************************/
void init_line(void)
{
    /* Calculate initial values based on sine curve */
    VecAdd<<<grid, block>>>(gvalues, tpoints);
    cudaThreadSynchronize();

    /* Initialize old values array */
    cudaMemcpy(goldval, gvalues, SIZE, cudaMemcpyDeviceToDevice);
    //cudaMemcpy(values, gvalues, SIZE, cudaMemcpyDeviceToHost);
    //cudaMemcpy(oldval, gvalues, SIZE, cudaMemcpyDeviceToHost);
}

/**********************************************************************
 *      Calculate new values using wave equation
 *********************************************************************/
__global__ void do_math(float* goldval, float* gvalues, float* gnewval, int n, float sqtau, int step) {

    int i = blockIdx.x * blockDim.x + threadIdx.x;
    for (int k = 0; k < step; ++k) {
        gnewval[i] = (2.0 * (1-sqtau) *gvalues[i]) - goldval[i];
        /* Update old values with new values */
        goldval[i] = gvalues[i];
        gvalues[i] = gnewval[i];
    }
}

/**********************************************************************
 *     Update all values along line a specified number of times
 *********************************************************************/
void update()
{
    float dtime, c, dx, tau, sqtau;
    dtime = 0.3;
    c = 1.0;
    dx = 1.0;
    tau = (c * dtime / dx);
    sqtau = tau * tau;

    /* Update values for each time step */
    /* Update points along line for this time step */
    do_math<<<grid, block>>>(goldval,  gvalues,  gnewval, tpoints, sqtau, nsteps);
    cudaThreadSynchronize();

    cudaMemcpy(values, gvalues, SIZE, cudaMemcpyDeviceToHost);
}

/**********************************************************************
 *     Print final results
 *********************************************************************/
void printfinal()
{
    int i;

    for (i = 1; i <= tpoints; i++) {
        printf("%6.4f ", values[i]);
        if (i%10 == 0)
            printf("\n");
    }
}

/**********************************************************************
 *	Main program
 *********************************************************************/
int main(int argc, char *argv[])
{
    if (argc < 3) {
        printf("Usage: ./wave <tpoints> <nsteps>\n");
        return 0;
    }
    sscanf(argv[1], "%d", &tpoints);
    sscanf(argv[2], "%d", &nsteps);
    check_param();

    // fill it up
    int left = tpoints%20;
    if (left) tpoints += (20 - left);
    grid = 20;
    block = tpoints/20;

    // allocate GPU memory
    cudaMalloc((float**)&gvalues, SIZE);
    cudaMalloc((float**)&goldval, SIZE);
    cudaMalloc((float**)&gnewval, SIZE);

    printf("Initializing points on the line...\n");
    init_line();
    printf("Updating all points for all time steps...\n");
    update();

    // resume tpoints
    if (left) tpoints -= (20 - left);

    // result
    printf("Printing final results...\n");
    printfinal();
    printf("\nDone.\n\n");

    // Free GPU memory
    cudaFree(gnewval);
    cudaFree(goldval);
    cudaFree(gvalues);

    return 0;
}
