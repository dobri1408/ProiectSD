	#include <iostream>
	#include <chrono>
	#include <algorithm>
	#include <cstdlib>
	#include <functional>
	#include <cmath>

	using namespace std;
	using namespace std::chrono;

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
	void insertSort(int *a, int st, int dr)
	{
		int i,j;
		for(i=st+1;i<=dr;i++)
			for(j=i;j>st&&a[j]<a[j-1];j--)
				swap(a[j],a[j-1]);
	}
	void mergeArrays(int *arr, int l, int m, int r) {
		int len1 = m - l + 1, len2 = r - m;
		int* left = new int[len1];
		int* right = new int[len2];

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


	void timSort(int *a,int n)
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
	void mergeSort(int *a,int st,int dr)
	{
		if(st<dr)
		{
			int mij=(st+dr)>>1;
			mergeSort(a,st,mij);
			mergeSort(a,mij+1,dr);
			mergeArrays(a,st,mij,dr);
		}
	}

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

	void countingSort(int arr[], int n) {
	
		int maxVal = arr[0];
		int minVal = arr[0];
		for (int i = 1; i < n; i++) {
			if (arr[i] > maxVal) maxVal = arr[i];
			if (arr[i] < minVal) minVal = arr[i];
		}
		
		int range = maxVal - minVal + 1;

		
		int* count = new int[range]();
		
	
		for (int i = 0; i < n; i++) {
			count[arr[i] - minVal]++;
		}

		int index = 0;
		for (int i = 0; i < range; i++) {
			while (count[i] > 0) {
				arr[index++] = i + minVal;
				count[i]--;
			}
		}
		

		delete[] count;
	}
	
	void shellSort(int* arr, int n) {
    for (int gap = n/2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            int temp = arr[i];
            int j;
            for (j = i; j >= gap && arr[j - gap] > temp; j -= gap) {
                arr[j] = arr[j - gap];
            }
            arr[j] = temp;
        }
    }
}
void heapify(int* arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest]) {
        largest = left;
    }

    if (right < n && arr[right] > arr[largest]) {
        largest = right;
    }

    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(int* arr, int n) {
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }

    for (int i = n - 1; i > 0; i--) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}


	int main() {
	   int tests[] = {100000, 1000000, 10000000, 100000000, 500000000};
		int numTests = sizeof(tests)/sizeof(tests[0]);

		pair<string, function<void(int*, int)>> sorts[] = {
			{"Radix Sort Base 10", [](int* arr, int n){ radixSort(arr, n, 10); }},
			{"Radix Sort Base 2^16", [](int* arr, int n){ radixSort(arr, n, 1<<16); }},
			{"Counting Sort", [](int* arr, int n){ countingSort(arr, n); }},
			{"Merge Sort", [](int* arr, int n){ mergeSort(arr, 0,n-1); }},
			{"Tim Sort", [](int* arr, int n){ timSort(arr, n); }},
			{"Shell Sort", [](int* arr, int n){ shellSort(arr, n); }},
			{"Heap Sort", [](int* arr, int n){ heapSort(arr, n); }},
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
