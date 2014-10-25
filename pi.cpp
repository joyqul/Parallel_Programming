#include <cstdio>
#include <cstdlib>
#include <ctime>

using namespace std;

double fRand(double fMin, double fMax) {
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

int main (int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: ./hw1_pthread <number of tosses>\n");
        return 0;
    }

    srand(time(NULL));

    int number_of_tosses = atoi(argv[1]);
    int number_in_circle = 0;
    for (int toss = 0; toss < number_of_tosses; ++toss) {
        double x = fRand(-1, 1);
        double y = fRand(-1, 1);
        double distance_squared = x*x + y*y;
        if (distance_squared <= 1) {
            ++number_in_circle;
        }
    }
    double pi_estimate = 4*number_in_circle/((double) number_of_tosses);
    printf("%f\n", pi_estimate);
    return 0;
}
