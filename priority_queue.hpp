/*
* Implementiert eine generische Prioritätswarteschlange (Heap) mit
* anpassbarem Vergleich. Unterstützt die Operationen push und extract_min
* mittels sift-up und sift-down. Intern wird ein Heap als 1-basiertes
* Array verwendet. Der Vergleichsoperator ist standardmäßig std::less<T>
* für eine Min-Heap-Implementierung.
*/


#include <functional>

template <typename T, typename CmpT = std::less<T>>
class PQ {
 public:
  PQ(int max_size, CmpT cmp = CmpT()) : n_{0}, cmp_{cmp} {
    array_ = new T[max_size + 1];
  }

 private:
  int n_;
  CmpT cmp_;
  T* array_;  // size of heap; function object for comparisons; array holding
              // the data

 public:
  void push(T x) {
    array_[++n_] = x;  // place x at next free leaf
    sift_up(n_);       // restore heap property
  }

  T extract_min() {       // "min" not necessarily meaningful
    swap_elts(1, n_);     // swap first with last elt
    sift_down(1);         // restore heap property
    return array_[n_--];  // return "minimum" elt.
  }

 private:
  void sift_up(int k) {
    while (k > 1 && cmp_(array_[k], array_[k / 2])) {
      swap_elts(k / 2, k);
      k /= 2;
    }
  }

  void sift_down(int k) {
    while (2 * k < n_) {
      int j = 2 * k;  // left child of k
      if (j < n_ - 1 && cmp_(array_[j + 1], array_[j]))
        ++j;                                  // goto right child if smaller
      if (cmp_(array_[k], array_[j])) break;  // exit loop
      swap_elts(k, j);
      k = j;  // continue at child
    }
  }

  // swap array_[i] with array_[j]
  void swap_elts(int i, int j) {
    T tmp = array_[i];
    array_[i] = array_[j];
    array_[j] = tmp;
  }
};
