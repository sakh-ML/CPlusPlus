/*
Dieses Programm implementiert eine Prioritätswarteschlange (PQ) mit einer
benutzerdefinierten Vergleichsfunktion, die die Quersumme (Digitensumme)
von Zahlen vergleicht.

Teil 1: Die Klasse DigitSumLess definiert einen Vergleichsoperator,
der zwei Zahlen nach ihrer Quersumme vergleicht.

Teil 2: Die Funktion k_smallest sucht das k-kleinste Element im Array,
basierend auf der Quersumme als Ordnungsmaß.

Das Programm demonstriert die Funktionalität mit Beispielzahlen und
gibt die k-kleinsten Elemente gemäß Quersumme aus.
*/


#include <cstdio>
#include "PQ.hpp"

int digit_sum(int number) {
  int sum = 0;
  while (number) {
    sum += number % 10;
    number /= 10;
  }
  return sum;
}

/* ======= Teil 1: Implementieren Sie eine Klasse DigitSumLess */
class DigitSumLess {
 public:
  bool operator()(int a, int b) {  //
    return digit_sum(a) < digit_sum(b);
  };
};

/* ======= Teil 2: Implementieren Sie eine Funktion k-smallest */
int k_smallest(int const *arr, int k, int n) {
  PQ<int, DigitSumLess> pq(n);
  for (int i = 0; i < n; ++i) pq.push(arr[i]);
  for (int i = 1; i < k; ++i) pq.extract_min();
  return pq.extract_min();
}

void print(int const *arr, int size) {
  if (size) {
    printf("[%i", arr[0]);
    for (int i = 1; i < size; i++) printf(" %i", arr[i]);
    printf("]\n");
  } else
    printf("[]\n");
}

int main() {
  // Teil 1: Klasse DigitSumLess
  int a = 165;  // cross sum: 1 + 6 + 5 = 12
  int b = 911;  // cross sum: 9 + 1 + 1 = 11
  DigitSumLess cmp;
  if (cmp(a, b))
    printf("Cross sum of %i is smaller than cross sum of %i\n", a, b);
  else
    printf("Cross sum of %i is larger than cross sum of %i\n", a, b);

  // Teil 2: k-smallest
  int const test[] = {13,  111, 5,   1, 8,  32, 6,  4,
                      547, 94,  625, 3, 45, 8,  76, 45};
  int n = sizeof(test) / sizeof(int);
  print(test, n);
  for (int i = 0; i < n; ++i) {
    printf("%i ", k_smallest(test, i + 1, n));
  }
  printf("\n");
}
