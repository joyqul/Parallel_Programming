#include <cstdio>
#include <cstdlib>

using namespace std;

int main (int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: ./hw1_pthread <number of tosses>\n");
        return 0;
    }
    int number_of_tosses = atoi(argv[1]);
    return 0;
}
