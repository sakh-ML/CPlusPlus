/*
* Projekt: Huffman-Komprimierung & Dekomprimierung
* Beschreibung:
* Dieses Programm implementiert den Huffman-Algorithmus zur verlustfreien Textkomprimierung.
* Es liest eine Datei ein, analysiert die Häufigkeit der Zeichen, baut daraus einen Huffman-Baum
* und erzeugt Bitcodes für jedes Zeichen. Damit kann die Datei komprimiert und später wieder
* dekomprimiert werden.
*
* Funktionen:
* - Analyse der Zeichenhäufigkeit (Frequenztabelle)
* - Aufbau des Huffman-Baums
* - Erzeugung von Bit-Codes für jedes Zeichen
* - Komprimierung einer Datei auf Bit-Ebene
* - Dekomprimierung (auf Grundlage des Baumes)
*
* Beispiel:
* Die Datei "wordlist-german.txt" wird mit Huffman-Kompression auf "wordlist-german.hfm" komprimiert
* und anschließend in "test.txt" dekomprimiert.
*/


#include <bitset>
#include <cstdio>
using std::bitset;
#include "heap.hpp"


struct Code {
  Code(int l = 0, bitset<64> c = {}) : length_{l}, code_{c} {}
  int length_;
  bitset<64> code_;
};

class HTNode {
 public:
  HTNode(unsigned char letter, int freq) : letter_{letter}, freq_{freq} {}

  ~HTNode() {
    if (left_) delete left_;
    if (right_) delete right_;
  }

  HTNode* left_ = nullptr;
  unsigned char letter_;
  int freq_;
  HTNode* right_ = nullptr;
};

struct HTNodeCompare {
  bool operator()(HTNode const* lhs, HTNode const* rhs) const {
    return lhs->freq_ < rhs->freq_;
  }
};

HTNodeCompare ht_node_cmp;

class HuffmanTree {
 public:
  HuffmanTree(char const* filename) {
    FILE* in_file = fopen(filename, "r");
    compute_frequencies(in_file);

    fclose(in_file);  // close file

    build_ht();  // baue Huffman-Baum

    compute_codes(root_, 1, bitset<64>{});
  }

  ~HuffmanTree() { delete root_; }

  Code get_code(unsigned char x) { return codetable_[x]; }

  // given a bitset, decode first code and return pair consisting of the letter
  // and the actually read code length:
  std::pair<unsigned char, int> get_letter(bitset<64> buffer) const {
    HTNode *current = root_, *last_node;
    int read_bits = 0;
    while (current) {
      last_node = current;
      if (buffer[64 - 1])  // always read msb from buffer
        current = current->right_;
      else
        current = current->left_;
      if (current) {
        buffer <<= 1;  // shift one bit to the left
        ++read_bits;
      }
    }

    // now at a leaf in HT => return its letter:
    return std::make_pair(last_node->letter_, read_bits);
  }

  int text_length() { return text_length_; }

 private:
  int text_length_ = 0;
  int frequencytable_[256] = {};
  Code codetable_[256] = {};  // soll die Codes für Buchstaben enthalten
  HTNode* root_;

  void compute_frequencies(FILE* file) {
    rewind(file);
    for (unsigned char ch = fgetc(file); !feof(file); ch = fgetc(file)) {
      ++frequencytable_[ch];
      ++text_length_;
    }
  }

  void build_ht() {
    PQ<HTNode*, HTNodeCompare> pq(512, ht_node_cmp);
    for (int i = 0; i < 256; ++i) {
      if (frequencytable_[i] > 0) {
        HTNode* new_node = new HTNode((unsigned char)i, frequencytable_[i]);
        pq.push(new_node);
      }
    }

    while (pq.size() > 1) {
      // holen die 2 Knoten mit der niedrigsten Frequency aus der PQ:
      HTNode* left_node = pq.extract_min();
      HTNode* right_node = pq.extract_min();

      HTNode* new_node = new HTNode('_', left_node->freq_ + right_node->freq_);
      new_node->left_ = left_node;
      new_node->right_ = right_node;
      pq.push(new_node);
    }

    root_ = pq.extract_min();
  }

  void compute_codes(HTNode const* node, int depth, bitset<64> code_so_far) {
    if (node->left_ == nullptr && node->right_ == nullptr) {
      // wir sind an einem Blatt
      codetable_[node->letter_] = Code(depth - 1, code_so_far);
    } else {
      // wir sind an einem internen Knoten
      if (node->left_) {
        bitset<64> left_code = code_so_far;
        left_code[64 - depth] = 0;
        compute_codes(node->left_, depth + 1, left_code);
      }
      if (node->right_) {
        bitset<64> right_code = code_so_far;
        right_code[64 - depth] = 1;
        compute_codes(node->right_, depth + 1, right_code);
      }
    }
  }
};

class HuffmanCompressor {
  HuffmanTree* ht_;

 public:
  HuffmanCompressor(HuffmanTree* ht) : ht_{ht} {}

  void uncompress(char const* in_filename, char const* out_filename);

  void compress(char const* in_file_name, char const* out_file_name) {
    FILE* in_file = fopen(in_file_name, "r");
    rewind(in_file);
    bw_.out_file_ = fopen(out_file_name, "w");

    for (unsigned char ch = fgetc(in_file); !feof(in_file);
         ch = fgetc(in_file)) {
      Code ch_code = ht_->get_code(ch);
      for (int j = 0; j < ch_code.length_; ++j) {
        bw_.write_bit(ch_code.code_[63 - j]);
      }
    }

    // flush buffer:
    while (bw_.current_bit_ > 0) bw_.write_bit(0);

    fclose(in_file);
    fclose(bw_.out_file_);
  }

 private:
  class {  // for writing bit-wise
   public:
    int current_bit_ = 0;
    unsigned char bit_buffer_ = 0;
    FILE* out_file_;

    void write_bit(bool bit) {
      bit_buffer_ <<= 1;   // make room
      bit_buffer_ += bit;  // write new bit
      if (++current_bit_ == 8) {
        fputc(bit_buffer_, out_file_);
        current_bit_ = 0;
        bit_buffer_ = 0;
      }
    }
  } bw_;
};

// test decoding: (lazy - only works if text size is known -- should be stored
// in file)
void HuffmanCompressor::uncompress(char const* in_filename,
                                   char const* out_filename) {
  FILE* in_file = fopen(in_filename, "r");
  rewind(in_file);
  FILE* out_file = fopen(out_filename, "w");

  bitset<64> buffer = 0;
  unsigned char ch;              // for reading from file
  int free_bits_in_buffer = 64;  // buffer empty

  // now decompress and refill buffer if there is room:
  int written_bytes = 0;
  while (written_bytes < ht_->text_length()) {
    if (free_bits_in_buffer >= 8 && !feof(in_file)) {
      // read a byte from file to buffer
      ch = fgetc(in_file);
      bitset<64> new_bits{ch};
      buffer |= (new_bits <<= (free_bits_in_buffer -= 8));
      // free_bits_in_buffer -= 8;  // filled 8 bits
    } else {
      // byte buffer full - decode one character and continue:
      std::pair<unsigned char, int> new_char = ht_->get_letter(buffer);

      // shift read bits in buffer:
      buffer <<= new_char.second;
      free_bits_in_buffer += new_char.second;

      // write to file:
      fputc(new_char.first, out_file);
      ++written_bytes;
    }
  }
  fclose(in_file);
  fclose(out_file);
}

int main() {
  //lets say the file to compress name wordlist-german.txt
  HuffmanTree ht("wordlist-german.txt");
  HuffmanCompressor hc(&ht);
  // hc.compress("wordlist-german.txt", "wordlist-german.hfm");

  hc.uncompress("wordlist-german.hfm", "test.txt");
}
