#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>

using namespace std;

int main() {
	std::cout << "====================================" << endl;
	std::cout << "For Loop Parallel" << endl;
	std::cout << "====================================" << endl << endl;
	//unsigned long long int n = 77953283800643;
	// Number to factorize
	unsigned long long int n;
	std::cout << "n: ";
	std::cin >> n;
	// The smallest factor (prime number) must be less than the sqrt(n)
	unsigned long int searchLimit = floor(sqrt(n));
	//std::cout << "n: " << n << endl;
	//std::cout << "Search Limit: " << searchLimit << endl;
	//Some numbers to test
	// Factores primos de 18446726951719762537 son: 4972900021 y 3709450597
	//77953283800643 son 9343261 y 8343263
	//Factores primos de 52037048131;
	//Factores primos de 5202881462819;
	//781957524492779 son 23823259 y 32823281 APROX 53 seg

	long long int i;
	long long int j;
	bool isPrime;
	int tid;
	unsigned long coFactor;
	bool factorsFound = false;

	printf("Num of CPU: %d\n", omp_get_num_procs());
	printf("Num max threads: %d\n", omp_get_max_threads());

#pragma omp parallel shared(searchLimit, n, factorsFound) private (i, j, isPrime, tid)
	{
		tid = omp_get_thread_num();
//dynamic
#pragma omp for schedule(dynamic)  
		for (i = 2; i <= searchLimit; i++) {
			if (!factorsFound) {
				isPrime = true;
				if (i == 2)
					isPrime = true;
				else if (i == 3)
					isPrime = true;
				else {
					for (j = 2; j <= floor(sqrt(i)); j++) {
						isPrime = !((i % j) == 0);
						if (!isPrime)
							j = (int)floor(sqrt(i)) + 1;
					}
				}
				if (isPrime)
					if (n % i == 0) {
						coFactor = n / i;
						std::cout << "From thread (" << tid << "), Factores primos de " << n << ": " << i << ", " << coFactor << "." << endl;
						factorsFound = true;
					}
			}	
		}
	}
	std::cout << "============" << endl;
	std::cout << "All threads done" << endl;
	std::cout << "============" << endl;
	system("pause");
	std::cin.get();
	return 0;
}
