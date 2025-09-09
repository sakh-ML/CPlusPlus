#include <stdio.h>


template <typename T, typename U>
bool binary_search(T const array[], int length, U x) {
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


int main(){


	return 0;
}
