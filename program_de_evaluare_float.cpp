#include <iostream>
#include <chrono>
#include <algorithm>
#include <cstdlib>
#include <functional>
#include <cmath>
#include <cstring> // Pentru memcpy

using namespace std;
using namespace std::chrono;

union FloatInt {
    float f;
    unsigned int i;
};

const int min_run=32;
int calcMinRun(int n)
{
    int r=0;
    while(n>min_run)
    {
        r|=n&1;
        n>>=1;
    }
    return n+r;
}
void insertSort(float *a, int st, int dr)
{
    int i,j;
    for(i=st+1;i<=dr;i++)
        for(j=i;j>st&&a[j]<a[j-1];j--)
            swap(a[j],a[j-1]);
}
void mergeArrays(float *arr, int l, int m, int r) {
    int len1 = m - l + 1, len2 = r - m;
    float* left = new float[len1];
    float* right = new float[len2];

    for (int i = 0; i < len1; i++)
        left[i] = arr[l + i];
    for (int i = 0; i < len2; i++)
        right[i] = arr[m + 1 + i];

    int i = 0;
    int j = 0;
    int k = l;

    while (i < len1 && j < len2) {
        if (left[i] <= right[j]) {
            arr[k] = left[i];
            i++;
        } else {
            arr[k] = right[j];
            j++;
        }
        k++;
    }

    while (i < len1) {
        arr[k] = left[i];
        k++;
        i++;
    }

    while (j < len2) {
        arr[k] = right[j];
        k++;
        j++;
    }

    delete[] left;
    delete[] right;
}


void timSort(float *a,int n)
{
    int i,j,st,dr,run,mij;
    run=calcMinRun(n);
    for(i=0;i<n;i+=run)
        insertSort(a,i,min(i+run-1,n));
    for(i=1;i<n;i*=2)
        for(j=0;j<n;j+=2*i)
    {
        st=j;
        mij=j+i-1;
        dr=min(j+2*i-1,n);
        if(mij<dr) mergeArrays(a,st,mij,dr);
    }
}
void mergeSort(float *a,int st,int dr)
{
    if(st<dr)
    {
        int mij=(st+dr)>>1;
        mergeSort(a,st,mij);
        mergeSort(a,mij+1,dr);
        mergeArrays(a,st,mij,dr);
    }
}


unsigned int floatToUint(float f) {
    FloatInt conv;
    conv.f = f;
    // Inversăm ordinea pentru numerele negative prin utilizarea complementului față de 1 pentru partea de magnitudine.
    if (conv.i & 0x80000000) { // Verificăm dacă este negativ
        return ~conv.i; // Inversăm toți biții pentru numerele negative
    } else {
        return conv.i | 0x80000000; // Asigurăm că numerele pozitive sunt tratate ca fiind mai mari decât cele negative
    }
}

float uintToFloat(unsigned int i) {
    if (i & 0x80000000) { // Dacă a fost pozitiv (după conversia noastră)
        i &= 0x7FFFFFFF; // Înlăturăm bitul de semn adăugat
    } else { // Dacă a fost negativ
        i = ~i; // Inversăm înapoi pentru a obține reprezentarea corectă
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
	
    for (int i = 1; i < n; i++) {

        if (arr[i - 1] > arr[i]) {
					cout << arr[i-1] << " " << arr[i]<<" ";
            return false;
        }
    }
    return true;
}

void shellSort(float* arr, int n) {
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            float temp = arr[i];
            int j;
            for (j = i; j >= gap && arr[j - gap] > temp; j -= gap) {
                arr[j] = arr[j - gap];
            }
            arr[j] = temp;
        }
    }
}
void heapify(float* arr, int n, int i) {
    int largest = i; 
    int left = 2 * i + 1; 
    int right = 2 * i + 2; 

    if (left < n && arr[left] > arr[largest])
        largest = left;

    if (right < n && arr[right] > arr[largest])
        largest = right;

    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(float* arr, int n) {
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    for (int i=n-1; i>0; i--) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}


int main() {
    int tests[] = {10000000, 100000000, 500000000};
    int numTests = sizeof(tests) / sizeof(tests[0]);

    pair<string, function<void(float*, int)>> sorts[] = {
        {"Radix Sort Base 10", [](float* arr, int n){ radixSort(arr, n, 10); }},
        {"Radix Sort Base 2^16", [](float* arr, int n){ radixSort(arr, n, 1<<16); }},
        {"Merge Sort ", [](float* arr, int n){ mergeSort(arr, 0,n-1); }},
        {"Tim Sort ", [](float* arr, int n){ timSort(arr, n); }},
        {"std::sort", [](float* arr, int n){ sort(arr, arr + n); }},
        {"Shell Sort", [](float* arr, int n){ shellSort(arr, n); }},
		{"Heap Sort", [](float* arr, int n){ heapSort(arr, n); }},
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
