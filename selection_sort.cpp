/*
* Dieses Programm sortiert ein Array von Zahlen mit dem Selection-Sort-Verfahren
* und sucht anschließend mit der binären Suche (Binary Search), ob ein bestimmter Wert vorhanden ist.
*
* Funktionen:
* - sort(): sortiert ein Array aufsteigend durch Auswahl (Selection Sort)
* - find_faster(): sucht effizient einen Wert in einem aufsteigend sortierten Array
* - print_all(): gibt alle Werte im Array aus
*/

#include <cstdio>

// Gibt alle Werte eines Arrays aus
template <typename T>
void print_all(T const array[], int length) {
  for (int i = 0; i < length; ++i) {
    printf("%g ", static_cast<double>(array[i]));
  }
  printf("\n");
}

// Gibt Position des Minimums in array[j,length-1] zurück
template <typename T>
int min_pos(T array[], int length, int j) {
  int min_pos_so_far = j;
  for (int i = j + 1; i < length; ++i) {
    if (array[i] < array[min_pos_so_far]) min_pos_so_far = i;
  }
  return min_pos_so_far;
}

// Sortiert das Array mit Selection Sort
template <typename T>
void sort(T array[], int length) {
  for (int i = 0; i < length; ++i) {
    int j = min_pos(array, length, i);
    T tmp = array[i];
    array[i] = array[j];
    array[j] = tmp;
  }
}

// Binäre Suche in einem aufsteigend sortierten Array
template <typename T, typename U>
bool find_faster(T const array[], int length, U x) {
  int left = 0, right = length - 1;

  while (right >= left) {
    int middle = (left + right) / 2;
    if (x == array[middle]) return true;
    if (x < array[middle])
      right = middle - 1;
    else
      left = middle + 1;
  }
  return false;
}

int main() {
  float noten[] = {2.7, 1.0, 1.0, 4.0, 5.0};
  sort(noten, 5);
  print_all(noten, 5);
  if (find_faster(noten, 5, 4.0f))
    printf("die Note 4.0 wurde gefunden\n");
  else
    printf("die Note 4.0 wurde NICHT gefunden\n");
}

