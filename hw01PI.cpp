#include <iostream>
#include <cstdlib>

using namespace std;

int main (int argc, char** argv) {
    if (argc != 2) {
        cout << "Usage: ./hw1_pthread <number of tosses>" << endl;
        return 0;
    }
    int number_of_tosses = atoi(argv[1]);
    cout << number_of_tosses << endl;
    return 0;
}
