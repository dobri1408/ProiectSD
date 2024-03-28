#include <iostream>
#include <chrono>
#include <algorithm>
#include <cstdlib>
#include <functional>
#include <cmath>
#include <cstring> // Pentru memcpy

using namespace std;
using namespace std::chrono;
const float eps = 0.001;
union FloatInt {
    float f;
    unsigned int i;
};

unsigned int floatToUint(float f) {
    FloatInt conv;
    conv.f = f;
    unsigned int sign = conv.i & 0x80000000 ? 0x80000000 : 0;
    unsigned int magnitude = conv.i & 0x7fffffff;
    // Numerele pozitive sunt tratate normal, numerele negative sunt codate în complement față de 2
    return sign ? (~magnitude + 1) : (conv.i | 0x80000000);
}

float uintToFloat(unsigned int i) {
    if (i & 0x80000000) {
        i &= 0x7fffffff;
    } else {
        i = (~i + 1) | 0x80000000;
    }
    FloatInt conv;
    conv.i = i;
    return conv.f;
}

void countSort(float* arr, int n, int exp, int base) {
    auto output = new float[n];
    auto count = new int[base]();
    int index;

    for (int i = 0; i < n; i++) {
        index = (floatToUint(arr[i]) / exp) % base;
        count[index]++;
    }

    for (int i = 1; i < base; i++) {
        count[i] += count[i - 1];
    }

    for (int i = n - 1; i >= 0; i--) {
        index = (floatToUint(arr[i]) / exp) % base;
        output[count[index] - 1] = arr[i];
        count[index]--;
    }

    for (int i = 0; i < n; i++) {
        arr[i] = output[i];
    }

    delete[] output;
    delete[] count;
}

void radixSort(float* arr, int n, int base) {
    unsigned int max = floatToUint(arr[0]);
    for (int i = 1; i < n; i++) {
        unsigned int ai = floatToUint(arr[i]);
        if (ai > max) max = ai;
    }

    for (unsigned int exp = 1; max / exp > 0; exp *= base) {
        countSort(arr, n, exp, base);
    }
}
bool isSorted(float arr[], int n) {
	cout << arr[0] << " ";
    for (int i = 1; i < n; i++) {
		cout << arr[i]<<" ";
        if (arr[i - 1] > arr[i]) {
            return false;
        }
    }
    return true;
}


int main() {
    int tests[] = {5, 10, 11};
    int numTests = sizeof(tests) / sizeof(tests[0]);

    pair<string, function<void(float*, int)>> sorts[] = {
        {"Radix Sort Base 10", [](float* arr, int n){ radixSort(arr, n, 10); }},
        {"Radix Sort Base 16", [](float* arr, int n){ radixSort(arr, n, 16); }},
        {"std::sort", [](float* arr, int n){ sort(arr, arr + n); }}
    };
    int numSorts = sizeof(sorts) / sizeof(sorts[0]);

    for (int i = 0; i < numTests; i++) {
        int n = tests[i];
        float* numbers = new float[n];

        cout << "N=" << n << " (Floats)" << endl;

        for (int j = 0; j < numSorts; j++) {
            // Generăm numere float aleatorii
            for (int k = 0; k < n; k++) {
                numbers[k] = static_cast<float>(rand()) / RAND_MAX * 10000.0f - 5000.0f; // Numere între -5000 și 5000
            }

            auto t_start = high_resolution_clock::now();
            sorts[j].second(numbers, n);
            auto t_end = high_resolution_clock::now();

            auto duration = duration_cast<milliseconds>(t_end - t_start).count();

            cout << sorts[j].first << " a durat " << duration << " milisecunde. ";
            cout << "Sortat corect? " << (isSorted(numbers, n) ? "Da" : "Nu") << endl;
        }
        cout << endl;

        delete[] numbers;
    }

    return 0;
}
