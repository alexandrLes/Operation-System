#include <iostream>
#include <windows.h>
#include <cmath>
#include <math.h>
#include <string>
#include <ctime>
#include <chrono>
#include <omp.h>

#define N 100000000
#define ITERATIONS_PER_BLOCK 30830

// calculating the number of pi with the OpenMP library

using namespace std;


// directly the function itself
double f(int i) {
    double x_i = (double)(i + 0.5) / N;
    return (double)4 / (1 + pow(x_i,2));
}


int main() {

    int threads_num;
    cout << "number of threads is ";
    cin >> threads_num;

    while (threads_num != 0) {
        double pi = (double) 0; // global total amount
        auto startTime = chrono::system_clock::now();

        #pragma omp parallel num_threads(threads_num) reduction(+:pi)
        {
            #pragma omp for schedule(dynamic, ITERATIONS_PER_BLOCK)
            for (int i = 0; i < N; ++i) {
                pi += f(i);
            }
        }

        auto finishTime = chrono::system_clock::now();
        chrono::duration<double> total_time = (finishTime - startTime) * 1000;

        cout << "time is " << (int)total_time.count() << endl;
        cout << "pi is " << (double)result_pi / N << endl;

        system("pause");
    }

    return 0;
}
