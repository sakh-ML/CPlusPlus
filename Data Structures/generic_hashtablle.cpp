/*
Dieser Code implementiert eine generische Hashtabelle mit linearem Sondieren, 
die es erlaubt, Zahlen oder Strings effizient zu speichern und wiederzufinden.
*/

#include <cassert>
#include <fstream>
#include <random>

template <typename KeyT>
class Hashtable {
  KeyT* array_;  // die eigentliche Hashtable
  bool* used_;   // markiere alle belegten Zellen in array_ mit true
  int m_;        // Groesse der Hashtabelle
  int overflow_buffer_size_;

 public:
  Hashtable(int m) : m_{m} {
    overflow_buffer_size_ = 0.01 * m_;
    array_ = new KeyT[m_ + overflow_buffer_size_ + 1];
    used_ = new bool[m_ + overflow_buffer_size_ +
                     1]();  // mark all entries as unused
  }

  // gib Einfügeposition zurück (oder -1, falls nicht moeglich)
  int insert(KeyT x) {
    int pos = hash(x);
    while (used_[pos]) ++pos;

    if (pos == m_ + overflow_buffer_size_) return -1;  // insert failed

    // jetzt ist pos_ an einer unbelegten Stelle!
    array_[pos] = x;
    used_[pos] = true;
    return pos;
  }

  bool find(KeyT x) {
    int pos = hash(x);
    assert(pos >= 0);
    assert(pos < m_);
    while (used_[pos]) {
      if (array_[pos] == x) return true;
      ++pos;
    }
    return false;
  }

 private:
  int hash(KeyT const& x) const;
};

// Template-Spezialisierung für KeyT = int:
template <>
int Hashtable<int>::hash(const int& x) const {
  return x % m_;
}

template <>
int Hashtable<std::string>::hash(const std::string& x) const {
  int result = 0;

  for (char c : x) {
    result *= 256 % m_;
    result += c;
    result %= m_;
  }

  return result;
}

int main() {
  // Hashtable<int> ht(110000);

  // std::random_device rd;
  // for (int i = 0; i < 99999; ++i) {
  //   int x = rd() % INT_MAX;
  //   printf("%i,", x);

  //   ht.insert(x);  // füge Zufallszahl ein
  // }
  // printf("\n");

  // for (int i = 0; i < 1000000; ++i) {
  //   if (ht.find(i)) printf("%i,", i);
  // }
  // printf("\n");

  Hashtable<std::string> dictionary(10000 * 1.1);

  std::ifstream myfile;
  myfile.open("wordlist-german.txt");
  for (std::string line; getline(myfile, line);) {
    if (line.length() == 6) {
      dictionary.insert(line);
    }
  }

  if (dictionary.find("sieben")) printf("super!\n");
}
