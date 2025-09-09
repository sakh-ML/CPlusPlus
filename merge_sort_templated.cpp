/*
Template-Version des Merge-Sort-Algorithmus:
- merge<T>: Mergt zwei sortierte Arrays (a und b) in ein drittes Array (c).
- merge_sort<T>: Sortiert ein Array iterativ mittels Merge-Sort.
Dabei werden Teilarrays schrittweise verdoppelt und gemerged.
- Funktioniert mit beliebigen Datentypen, die Vergleichsoperator '<' unterstützen.
- Beispielhaft im main: Sortierung von int, float und char Arrays.
*/


#include <cstdio>

// ================ Ändern Sie diese Funktion ab ================
// merge 2 sortierte Arrays a[0,la-1] und b[0,lb-1]
// und schreibe das Ergebnis in c[0,la+lb-1]
template <typename T>
void merge(T const a[], T const b[], T c[], int la, int lb) {
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

// ================ Ändern Sie diese Funktion ab ================
// sortiere das array[0,n-1] aufsteigend mit mergesort:
template <typename T>
void merge_sort(T array[], int n) {
  T* tmp = new T[n];  // allokiere temporären Platz
  for (int l = 1; l < n; l = l * 2) {
    // an dieser Stelle sind alle Teilarrays array[0,l-1], array[l,2l-1],
    // array[2l,3l-1], usw sortiert!
    for (int s = 0; s + l < n; s = s + 2 * l) {
      // um nicht aus dem Array rauszufallen
      int length_b = l;
      if (n - s - l < length_b) length_b = n - s - l;

      // array+s bezeichnet array[s,...]
      merge(array + s, array + s + l, tmp, l, length_b);
      // jetzt enthält tmp die Werte aus array[s,s+l-1] und array[s+l,
      // s+l+length_b-1]

      // kopiere diese zurück in array:
      for (int i = 0; i < l + length_b; ++i) array[s + i] = tmp[i];
      // jetzt sind alle Teilarrays array[0,2l-1], array[2l,4l-1] usw. sortiert
    }
  }
  delete[] tmp;
}

int main() {
  // integer
  int n1 = 5;
  int array1[] = {1, 6, 4, 9, 2};
  merge_sort(array1, n1);

  // float
  int n2 = 5;
  float array2[] = {1.9, 6.25, 4.2, 9.125, 2.543};
  merge_sort(array2, n2);

  // char
  int n3 = 4;
  char array3[] = {'D', 'A', 'P', '1'};
  merge_sort(array3, n3);
}
