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
#define SIZE (MAXPOINTS+2)*sizeof(float)

void check_param(void);
void init_line(void);
void update (void);
void printfinal (void);

int nsteps,                 	/* number of time steps */
    tpoints, 	     		/* total points along string */
    rcode;                  	/* generic return code */
float values[MAXPOINTS+2], 	/* values at time t */
      oldval[MAXPOINTS+2], 	/* values at time (t-dt) */
      newval[MAXPOINTS+2]; 	/* values at time (t+dt) */
float *gvalues, *goldval, *gnewval;


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
    for (int i = 1; i <= n; i++) {
        A[i] = sin(2.0 * PI * ((float)i - 1.0) / (float)(n - 1));
    } 
}

/**********************************************************************
 *     Initialize points on line
 *********************************************************************/
void init_line(void)
{
    /* Calculate initial values based on sine curve */
    VecAdd<<<1, 1>>>(gvalues, tpoints);

    /* Initialize old values array */
    cudaMemcpy(goldval, gvalues, SIZE, cudaMemcpyDeviceToDevice);

    cudaMemcpy(values, gvalues, SIZE, cudaMemcpyDeviceToHost);
    cudaMemcpy(oldval, gvalues, SIZE, cudaMemcpyDeviceToHost);

}

/**********************************************************************
 *      Calculate new values using wave equation
 *********************************************************************/
void do_math(int i)
{
    float dtime, c, dx, tau, sqtau;

    dtime = 0.3;
    c = 1.0;
    dx = 1.0;
    tau = (c * dtime / dx);
    sqtau = tau * tau;
    newval[i] = (2.0 * values[i]) - oldval[i] + (sqtau *  (-2.0)*values[i]);
}

/**********************************************************************
 *     Update all values along line a specified number of times
 *********************************************************************/
void update()
{
    int i, j;

    /* Update values for each time step */
    for (i = 1; i<= nsteps; i++) {
        /* Update points along line for this time step */
        for (j = 1; j <= tpoints; j++) {
            /* global endpoints */
            if ((j == 1) || (j  == tpoints))
                newval[j] = 0.0;
            else
                do_math(j);
        }

        /* Update old values with new values */
        for (j = 1; j <= tpoints; j++) {
            oldval[j] = values[j];
            values[j] = newval[j];
        }
    }
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

    // allocate GPU memory
    cudaMalloc((float**)&gvalues, SIZE);
    cudaMalloc((float**)&goldval, SIZE);
    cudaMalloc((float**)&gnewval, SIZE);

    printf("Initializing points on the line...\n");
    init_line();
    printf("Updating all points for all time steps...\n");
    update();
    printf("Printing final results...\n");
    printfinal();
    printf("\nDone.\n\n");

    // Free GPU memory
    cudaFree(gnewval);
    cudaFree(goldval);
    cudaFree(gvalues);

    return 0;
}
