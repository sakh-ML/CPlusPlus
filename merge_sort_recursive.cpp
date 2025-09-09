/*
Implementierung von Merge-Sort mit Rekursion:
- Die Funktion `mergesort` sortiert ein Array zwischen zwei Indizes.
- Die Hilfsfunktion `merge` kombiniert zwei bereits sortierte Teilarrays.
- `print_all` gibt ein Array auf der Konsole aus.
- Im `main` werden zwei Beispiel-Arrays sortiert und vor/nach der Sortierung ausgegeben.
*/


#include <cstdio>

// merge 2 sortierte Arrays a[0,la-1] und b[0,lb-1]
// und schreibe das Ergebnis in c[0,la+lb-1]
void merge(int const a[], int const b[], int c[], int la, int lb) {
  int left = 0;   // Index in a[0,la-1]
  int right = 0;  // Index in b[0,lb-1]

  int idx;  // Index in c[0,la+lb-1]

  for (idx = 0; left < la && right < lb; ++idx) {
    if (a[left] < b[right]) {
      c[idx] = a[left];  // Minimum ist in a
      ++left;            // gehe 1 Position weiter in a
    } else {
      c[idx] = b[right];  // Minimum ist in b
      ++right;            // gehe 1 Position weiter in b
    }
  }

  if (left < la) {  // es gibt noch Werte in a
    while (idx < la + lb) {
      c[idx] = a[left];
      ++idx;
      ++left;
    }
  } else {
    while (idx < la + lb) c[idx++] = b[right++];  // Kurzschreibweise
  }
}

void mergesort(int values[], int first_index, int last_index) {
  // Intervalle der Länge 1 sind immer sortiert!
  if (first_index == last_index) return;

  // Sortiere linke und rechte Hälfte rekursiv
  int m = (first_index + last_index) / 2;
  mergesort(values, first_index, m);
  mergesort(values, m + 1, last_index);

  // Jetzt sind beide Hälften sortiert. Merge!
  int *c = new int[last_index - first_index + 1];  // Buffer anlegen
  merge(&values[first_index], &values[m + 1], c,   // In Buffer mergen
        m - first_index + 1, last_index - m);
  for (int i = first_index; i <= last_index; i++)  // Aus Buffer zurück kopieren
    values[i] = c[i - first_index];
}

void print_all(int const values[], int length) {
  for (int i = 0; i < length; ++i) printf("%i, ", values[i]);
  printf("\n");
}

int main() {
  printf("======== Test 1 ========\n");
  int values1[] = {9,  30, 18, 27, 14, 26, 3, 5,  13, 28, 16,
                   25, 22, 1,  12, 8,  17, 2, 23, 15, 24, 31,
                   6,  20, 19, 21, 32, 4,  7, 10, 29, 11};
  print_all(values1, 32);
  mergesort(values1, 0, 31);
  print_all(values1, 32);

  printf("======== Test 2 ========\n");
  int values2[] = {9,  30, 18, 27, 14, 26, 3, 5,  13, 28, 16, 25, 22, 1,  12, 8,
                   17, 2,  23, 15, 24, 31, 6, 20, 19, 21, 4,  7,  10, 29, 11};
  print_all(values2, 31);
  mergesort(values2, 0, 30);
  print_all(values2, 31);

  return 0;
}
