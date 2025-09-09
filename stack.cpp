/* 
Implementiert einen generischen Stack mit dynamischem Array.
Unterstützt push, pop, top und prüft, ob der Stack leer ist.
Beispiel: Webseiten-Stack mit Ausgaben in umgekehrter Reihenfolge.
*/

#include <cstdio>
#include <string>

#include "ResizableArray.hpp"

template <typename T>
class MyStack {
 public:
  MyStack() : array_(1) {}

  void push(T x) { array_.push_back(x); }
  T pop() { return array_.pop_back(); }
  bool is_empty() { return array_.size() == 0; }

  // gib das oberste Element zurück (aber loesche es nicht):
  T top() {
    T last = array_[array_.size() - 1];
    return last;
  }

 private:
  ResizableArray<T>
      array_;  // die Elemente des Stacks werden in diesem Array gespeichert
};

int main() {
  MyStack<std::string> webseiten;
  webseiten.push(std::string("www.tagesschau.de"));
  webseiten.push(std::string("w3.org"));
  webseiten.push(std::string("dap1.cs.tu-dortmund.de"));
  webseiten.push(std::string("cppreference.com"));

  webseiten.top();

  while (!webseiten.is_empty()) {
    std::string s = webseiten.pop();
    printf("%s\n", s.c_str());
  }
  return 0;
}
