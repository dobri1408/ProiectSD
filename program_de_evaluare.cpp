#include <iostream>
#include <chrono>
#include <algorithm>
#include <cstdlib>
#include <functional>
#include <cmath>

using namespace std;
using namespace std::chrono;

int getMax(int arr[], int n) {
    int mx = arr[0];
    for (int i = 1; i < n; i++)
        if (arr[i] > mx)
            mx = arr[i];
    return mx;
}

void countSort(int arr[], int n, int exp, int base) {
    int* output = new int[n];
    int* count = new int[base](); // Alocare dinamică pentru count cu inițializare la 0

    for (int i = 0; i < n; i++)
        count[(arr[i] / exp) % base]++;

    for (int i = 1; i < base; i++)
        count[i] += count[i - 1];

    for (int i = n - 1; i >= 0; i--) {
        output[count[(arr[i] / exp) % base] - 1] = arr[i];
        count[(arr[i] / exp) % base]--;
    }

    for (int i = 0; i < n; i++)
        arr[i] = output[i];

    delete[] output; // Eliberăm memoria alocată pentru output
    delete[] count;  // Eliberăm memoria alocată pentru count
}

void radixSort(int arr[], int n, int base) {
    int m = getMax(arr, n);

    for (int exp = 1; m / exp > 0; exp *= base)
        countSort(arr, n, exp, base);
}

bool isSorted(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        if (arr[i-1] > arr[i]) {
            return false;
        }
    }
    return true;
}

int main() {
    int tests[] = {10000, 100000, 1000000,10000000,100000000,1000000000};
    int numTests = sizeof(tests)/sizeof(tests[0]);

    pair<string, function<void(int*, int)>> sorts[] = {
        {"Radix Sort Base 10", [](int* arr, int n){ radixSort(arr, n, 10); }},
        {"Radix Sort Base 16", [](int* arr, int n){ radixSort(arr, n, 16); }},
        {"std::sort", [](int* arr, int n) { sort(arr, arr + n); }}
    };
    int numSorts = sizeof(sorts)/sizeof(sorts[0]);

    for (int i = 0; i < numTests; i++) {
        int n = tests[i];
        int* numbers = new int[n];

        cout << "N=" << n << endl;

        for (int j = 0; j < numSorts; j++) {
            for (int k = 0; k < n; k++) {
                numbers[k] = rand() % 10000; // Generăm numere aleatorii
            }

            auto t_start = high_resolution_clock::now();
            sorts[j].second(numbers, n);
            auto t_end = high_resolution_clock::now();

            auto duration = duration_cast<milliseconds>(t_end - t_start).count();

            cout << sorts[j].first << " a durat " << duration << " milisecunde. ";
            cout << "Sortat corect? " << (isSorted(numbers, n) ? "Da" : "Nu") << endl;
        }
        cout << endl;

        delete[] numbers; // Eliberăm memoria alocată pentru numbers
    }

    return 0;
}
