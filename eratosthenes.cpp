/*
Dieses Programm implementiert den "Sieb des Eratosthenes" Algorithmus,
um alle Primzahlen kleiner als eine eingegebene Zahl n auszugeben.

Es verwendet ein dynamisch alloziertes boolesches Array, um zu markieren,
welche Zahlen Primzahlen sind, und filtert die Vielfachen systematisch heraus.

Am Ende werden alle gefundenen Primzahlen ausgegeben.
*/

#include <cassert>
#include <cstdio>

// print all primes less than n:
void print_all_primes(int n) {
  // allokiere den Platz für n bools (ein Array von n bools):
  bool* prime = new bool[n];  // dynamische Speicherallokation

  if (prime == nullptr) {  // teste, ob genügend Speicher vorhanden war
    printf("out of memory\n");
    return;
  }

  // // 1. Schritt: initialisiere alle Einträge mit true
  for (int i = 2; i < n; ++i) prime[i] = true;

  // 2. Schritt: besuche die Primzahlen der Reihe nach und
  // setze ihre Vielfachen auf false
  for (int i = 2; i < n; ++i) {
    if (prime[i] == true) {
      // gib die Primzahl aus:
      printf("%i ", i);
      // besuche alle Vielfachen von i, solange sie kleiner als n sind:
      for (int j = 2; i * j < n; ++j) {
        prime[i * j] =
            false;  // i*j kann keine Primzahl sein, da Vielfaches von i
      }
    }
  }
  printf("\n");

  delete[] prime;  // gib Speicher frei
}

int main() {
  int n;
  printf("Bis zu welcher Zahl sollen alle Primzahlen ausgegeben werden?\n");
  scanf("%i", &n);

  print_all_primes(n + 1);
}
