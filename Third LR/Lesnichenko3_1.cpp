#include <iostream>
#include <windows.h>
#include <cmath>
#include <math.h>
#include <string>
#include <ctime>
#include <mmsystem.h>
#include <ctime>
#include <chrono>
#include <synchapi.h>

#define N 100000000 //
#define ITERATIONS_PER_BLOCK 5


using namespace std;

// the number of the first unoccupied iteration from which the next thread starts calculating the next block
int start_iteration = 0;

// the counter of completed threads, we will use as an increment
int finish_counter = 0;

// global total amount
double result_pi = (double)0;

// critical section, to get rid of mutual exclusion when accessing global variables
CRITICAL_SECTION Crit_Section;


// directly the function itself
double f(int i) {
    double x_i = (double)(i + 0.5) / N;
    return (double)4 / (1 + pow(x_i,2));
}

// calculates the sum of terms with numbers from the number of the first unoccupied iteration to the last inclusive
double block_func(int from_iter, int to_iter) {
    double result = (double)0;

    for (int i = from_iter; i <= to_iter; ++i) {
        result += f(i);
    }

    return result;
}

// func for thread
DWORD threadFuction(LPVOID lpParameter) {
    int from_iter;
    double part_pi = (double)0; // sum for iteration
    EnterCriticalSection(&Crit_Section);
        from_iter = start_iteration;
        start_iteration += ITERATIONS_PER_BLOCK;
    LeaveCriticalSection(&Crit_Section);

    while (from_iter < N) {

        if ((from_iter + ITERATIONS_PER_BLOCK) < N) {
            part_pi += block_func(from_iter, from_iter + ITERATIONS_PER_BLOCK - 1);
        }
        else {
            part_pi += block_func(from_iter, N - 1);
        }

        EnterCriticalSection(&Crit_Section);
            from_iter = start_iteration;
            start_iteration += ITERATIONS_PER_BLOCK;
        LeaveCriticalSection(&Crit_Section);

    }

    EnterCriticalSection(&Crit_Section);
        result_pi += part_pi;
        ++finish_counter;
    LeaveCriticalSection(&Crit_Section);

    return 0;
}

int main() {
    int threads_num;
    cout << "number of threads is ";
    cin >> threads_num;

    InitializeCriticalSection(&Crit_Section);

    while (threads_num > 0) {
        HANDLE* descriptorsList = new HANDLE[threads_num];
        for (int i = 0; i < threads_num; ++i) {
            descriptorsList[i] = CreateThread(
                NULL, // LPSECURITY_ATTRIBUTES lpThreadAttributes
                0, // SIZE_T dwStackSize
                threadFuction, // LPTHREAD_START_ROUTINE lpStartAddress;
                NULL, // LPVOID lpParameter;
                CREATE_SUSPENDED, // DWORD dwCreationFlags;
                NULL // LPDWORD lpThreadId
            );
        }

        auto startTime = chrono::system_clock::now();

        for (int i = 0; i < threads_num; ++i) {
            ResumeThread(descriptorsList[i]);
        }

        while (finish_counter < threads_num) {
            Sleep(1);
        }

        auto finishTime = chrono::system_clock::now();
        chrono::duration<double> total_time = (finishTime - startTime) * 1000;

        cout << "time is " << (int)total_time.count() << endl;
        cout << "pi is " << (double)result_pi / N << endl;

        for (int i = 0; i < threads_num; ++i) {
            CloseHandle(descriptorsList[i]);
        }

        delete[] descriptorsList;

        system("pause");

    }

    DeleteCriticalSection(&Crit_Section);

    return 0;
}
