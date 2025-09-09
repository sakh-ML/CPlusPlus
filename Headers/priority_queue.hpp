/*
* Implementiert eine generische Prioritätswarteschlange (Heap) mit einem
* Vergleichsobjekt (Comparator). Unterstützt Einfügen (push),
* Entfernen des minimalen Elements (extract_min), sowie Abfragen
* von Größe und Leere. Die Heap-Eigenschaft wird über sift_up und sift_down
* gewahrt.
*/


#ifndef HEAP_HPP
#define HEAP_HPP

#include <functional>

template <typename T, typename CmpT = std::less<T>>
class PQ {
  T* array_;  // the array for actually storing the heap
  int n_;     // #elements in PQ
  CmpT cmp_;  // comparator object for comparing elements of type T

 public:
  PQ(int max_size = 1, CmpT cmp = CmpT()) : n_{0}, cmp_{cmp} {
    array_ = new T[max_size + 1];
  }

  void push(T x) {
    ++n_;
    array_[n_] = x;
    sift_up(n_);
  }

  int size() const { return n_; }

  bool is_empty() const { return n_ == 0; }

  T extract_min() {       // "min" not necessarily meaningful
    swap(1, n_);          // swap first with last elt
    sift_down(1);         // restore heap property
    return array_[n_--];  // return "minimum" elt.
  }

 private:
  void sift_down(int k) {
    while (2 * k < n_) {
      int j = 2 * k;  // left child of k
      if (j < n_ - 1 && cmp_(array_[j + 1], array_[j]))
        ++j;                                  // goto right child if smaller
      if (cmp_(array_[k], array_[j])) break;  // exit loop
      swap(k, j);
      k = j;  // continue at child
    }
  }

  void sift_up(int k) {
    while (k > 1 && cmp_(array_[k], array_[k / 2])) {
      // Heap-Eigenschaft gilt nicht => vertausche mit Elternknoten:
      swap(k, k / 2);
      k = k / 2;
    }
  }

  // swap array_[i] and array_[j]:
  void swap(int i, int j) {
    T tmp = array_[i];
    array_[i] = array_[j];
    array_[j] = tmp;
  }
};

#endif  // HEAP_HPP
