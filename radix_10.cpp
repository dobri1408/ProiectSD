#include <iostream>
#include <algorithm>

const int MAX_SIZE = 1000; // Dimensiunea maximă a array-ului

// Funcție pentru a obține valoarea maximă din array
int getMax(int arr[], int n) {
    int mx = arr[0];
    for (int i = 1; i < n; i++)
        if (arr[i] > mx)
            mx = arr[i];
    return mx;
}

// Funcție care face sortarea efectivă pe baza cifrei curente
void countSort(int arr[], int n, int exp) {
    int output[MAX_SIZE]; // Array-ul de output
    int i, count[10] = {0};

    // Numărăm aparițiile în count[]
    for (i = 0; i < n; i++)
        count[(arr[i] / exp) % 10]++;

    // Schimbăm count[i] astfel încât count[i] să conțină acum poziția
    // acestei cifre în output[]
    for (i = 1; i < 10; i++)
        count[i] += count[i - 1];

    // Construim array-ul de output
    for (i = n - 1; i >= 0; i--) {
        output[count[(arr[i] / exp) % 10] - 1] = arr[i];
        count[(arr[i] / exp) % 10]--;
    }

    // Copiem array-ul de output în arr[], astfel încât arr[] să fie acum sortat
    for (i = 0; i < n; i++)
        arr[i] = output[i];
}

// Funcția principală pentru a rula Radix Sort
void radixSort(int arr[], int n) {
    // Găsește numărul maxim pentru a ști numărul de cifre
    int m = getMax(arr, n);

    // Facem countSort pentru fiecare cifră. În loc să trecem cifra,
    // exp este trecut. exp este 10^i unde i este cifra curentă
    for (int exp = 1; m / exp > 0; exp *= 10)
        countSort(arr, n, exp);
}

int main() {
    int arr[MAX_SIZE] = {170, 45, 75, 90, 802, 24, 2, 66};
    int n = 8; // Numărul de elemente din array
    radixSort(arr, n);
    for (int i = 0; i < n; i++)
        std::cout << arr[i] << " ";
    std::cout << std::endl;
    return 0;
}

