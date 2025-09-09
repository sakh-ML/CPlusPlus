/*
Template-Klasse für ein dynamisch vergrößerbares Array.
Unterstützt Hinzufügen, Entfernen, Kopieren und Indexzugriff.
Kapazität passt sich automatisch an die Anzahl der Elemente an.
*/


#include <cstdio>

// todo: cpy & assgn
template <typename T>
class ResizableArray {
 protected:       // wie private, nur erbende Klassen haben Zugriff
  int capacity_;  // maximale Kapazität
  T* array_;      // Zeiger auf das C-Array an sich
  int n_;         // # Elemente im Array
 public:
  int size() const { return n_; }  // getter-Methode

  ResizableArray(int initial_capacity = 1)
      : capacity_{initial_capacity}, n_{0} {
    printf("allocate memory of size %i\n.", capacity_);
    array_ = new T[capacity_];  // allokiere den Speicher für das Array
  }

  // Kopierkonstruktor:
  ResizableArray(ResizableArray const& other) {
    n_ = other.n_;                // kopiert member-Variable n_ aus other
    capacity_ = other.capacity_;  // kopiert capacity_ aus other
    array_ = new T[capacity_];    // allokiere den Speicher für das Array
    for (int i = 0; i < n_; ++i) {
      array_[i] = other.array_[i];  // kopiere Elemente aus other.array_
    }
  }

  // Kopierzuweisungsoperator:
  ResizableArray& operator=(ResizableArray const& other) {
    delete[] array_;              // gib Speicher frei
    n_ = other.n_;                // kopiert member-Variable n_ aus other
    capacity_ = other.capacity_;  // kopiert capacity_ aus other
    array_ = new T[capacity_];    // allokiere den Speicher für das Array
    for (int i = 0; i < n_; ++i) {
      array_[i] = other.array_[i];  // kopiere Elemente aus other.array_
    }
    return *this;
  }

  ~ResizableArray() { delete[] array_; }

  // füge x am Ende des Arrays ein (verdoppelt evtl. die Kapazität)
  void push_back(T x) {
    if (n_ == capacity_) {
      capacity_ *= 2;                   // verdoppele capacity_
      T* new_array = new T[capacity_];  // fordere neuen Speicherbereich an
                                        // (doppelt so gross)
      for (int i = 0; i < n_; ++i)
        new_array[i] = array_[i];  // kopiere Elemente
      delete[] array_;     // loesche alten Speicher, da nicht mehr beoetigt
      array_ = new_array;  // dies kopiert NUR den Zeiger!
    }
    array_[n_++] = x;  // speichert den Wert x am Ende des Arrays
  }

  T pop_back() {
    if (n_ < capacity_ / 4 && capacity_ > 4) {
      // need less space
      capacity_ /= 2;  // halve capacity
      T* tmp_array = new T[capacity_];
      for (int i = 0; i < n_; ++i) {
        // copy old array to new one:
        tmp_array[i] = array_[i];
      }
      delete[] array_;
      array_ = tmp_array;
    }
    --n_;
    return array_[n_];
  }

  T& operator[](int i) { return array_[i]; }  // Indexoperator als Referenz
  T const& operator[](int i) const {
    return array_[i];
  }  // Indexoperator als Const-Referenz

  T read(int i) const {
    if (i < 0 || i > n_ - 1) return 0;  // index out of bounds
    return array_[i];
  }

  void write(int i, T x) {
    if (i < 0 || i > n_ - 1) return;
    array_[i] = x;  // speichere an der Stelle i den Wert x
  }
};
