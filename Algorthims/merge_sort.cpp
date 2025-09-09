/*
Implementierung des Merge-Sort Algorithmus:
- Sortiert ein int-Array mit divide-and-conquer.
- Enthält Hilfsfunktionen zum Mergen sortierter Teilarrays.
- Erzeugt ein großes Array mit Zufallszahlen und sortiert es.
- Prüft und gibt das Ergebnis aus.
*/


#include <random>
#include <iostream>

int min(int x, int y){

  if(x < y){
    return x;
  }
  
  else{
  return y;
  }
  
}

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

// sortiere das array[0,n-1] aufsteigend mit mergesort:
void merge_sort(int array[], int n) {
  int* tmp = new int[n];  // allokiere temporären Platz
  for (int l = 1; l < n; l = l * 2) {
    // an dieser Stelle sind alle Teilarrays array[0,l-1], array[l,2l-1],
    // array[2l,3l-1], usw sortiert!
    for (int s = 0; s + l < n; s = s + 2 * l) {
      int length_b = min(l, n - s - l);  // um nicht aus dem Array rauszufallen

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

// true gdw array[0,n-1] sortiert ist:
bool is_sorted(int array[], int n) {
  for (int i = 1; i < n; ++i)
    if (array[i - 1] > array[i]) return false;
  return true;
}

int main() {
  int n = 100000;
  int* array = new int[n];
  std::random_device r;  // erzeuge Zufallszahlengenerator

  // fülle array mit zufälligen Zahlen aus [0,999]:
  for (int i = 0; i < n; ++i) array[i] = r() % 1000;

  // sortiere array:
  merge_sort(array, n);

  // prüfe, ob sortiert:
  if (!is_sorted(array, n)) std::cout << "Mist" << std::endl;

  else{
	  std::cout << "Sorted !! " << std::endl;
	  for(int i = 0; i < n; ++i){
		  std::cout << array[i] << " "; 
	  }
  }
}
