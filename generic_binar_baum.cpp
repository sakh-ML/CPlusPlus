/*
* Generischer Binary Search Tree (BST):
* Implementiert einen binären Suchbaum für beliebige, vergleichbare Datentypen (z. B. int, string, double).
* Unterstützt Operationen wie Einfügen, Suchen, Höhe berechnen und Traversierungen (Inorder/Preorder).
*
* Im Beispielprogramm (main) wird der BST mit Strings verwendet:
* - Wörter mit genau 3 Buchstaben werden aus einer Datei eingelesen.
* - Diese werden in den BST eingefügt.
* - Anschließend werden die Anzahl, die Höhe und die sortierte Liste der Wörter ausgegeben.
*/


#include <cstdio>
#include <fstream>
#include <string>  // aka MyString
#include <vector>  // aka ResizableArray
using namespace std;


template <class T>
struct BSTNode {  // struct = class, nur per default alles public
  BSTNode(T const& x) : left_{nullptr}, cont_{x}, right_{nullptr} {}

  ~BSTNode() {
    if (left_ != nullptr) delete left_;
    if (right_) delete right_;
  }

  BSTNode* left_;
  T cont_;
  BSTNode* right_;
};

template <class T>
class BST {
  BSTNode<T>* root_;  // the root_ of the tree
  int size_;          // stores number of items
 public:
  BST() : root_{nullptr}, size_{0} {}

  ~BST() { delete root_; }

  int size() const { return size_; }

  // return height of the tree:
  int get_height() { return get_height(root_); }

  // suche x im Baum:
  bool find(T x) { return find(x, root_); }

  // eigentlich inorder:
  vector<T> get_sorted() {
    vector<T> result;
    result.reserve(size_);
    inorder(root_, result);
    return result;
  }

  vector<T> preorder() {
    vector<T> result;
    result.reserve(size_);
    preorder(root_, result);
    return result;
  }

  // insert new element x into this tree as a new leaf
  void insert(T const& x) {
    root_ = insert(x, root_);
    ++size_;
  }

 private:
  void inorder(BSTNode<T>* node, vector<T>& v) {
    if (node == nullptr) return;
    inorder(node->left_, v);
    v.push_back(node->cont_);
    inorder(node->right_, v);
  }

  void preorder(BSTNode<T>* node, vector<T>& v) {
    if (node == nullptr) return;
    v.push_back(node->cont_);
    preorder(node->left_, v);
    preorder(node->right_, v);
  }

  int get_height(BSTNode<T>* node) {
    if (node == nullptr) return 0;  // leerer (Teil)baum

    int linke_TB_Hoehe = get_height(node->left_);
    int rechte_TB_Hoehe = get_height(node->right_);

    // gib Maximum der beiden Teilbaumenhoehen (+1) zurueck:
    return (linke_TB_Hoehe > rechte_TB_Hoehe ? linke_TB_Hoehe
                                             : rechte_TB_Hoehe) +
           1;
  }

  bool find(T x, BSTNode<T>* current) {
    if (current == nullptr) return false;
    if (current->cont_ == x) return true;
    if (x < current->cont_)
      return find(x, current->left_);
    else
      return find(x, current->right_);
  }

  // recursive insert into BST; return ptr to either new node or current
  BSTNode<T>* insert(T const& x, BSTNode<T>* current) {
    if (current == nullptr) {
      // base case: create node and return pointer to it
      return new BSTNode<T>(x);
    }

    // recursive case: compare x with current->cont and go left/right
    if (x < current->cont_)
      current->left_ = insert(x, current->left_);
    else
      current->right_ = insert(x, current->right_);
    return current;  // return pointer to passed pointer
  }
};

int main() {
  BST<string> dictionary;

  ifstream myfile;
  myfile.open("wordlist-german_shuffled.txt");
  for (std::string line; getline(myfile, line);) {
    if (line.length() == 3) {
      dictionary.insert(line);
    }
  }
  printf("# Woerter mit Laenge 3 ist %i\n", dictionary.size());
  printf("Hoehe des Baums ist %i\n", dictionary.get_height());

  if (dictionary.find("C++"))
    printf("C++ gefunden");
  else
    dictionary.insert("C++");

  if (dictionary.find("C++")) printf("C++ jetzt gefunden\n");

  vector<string> sortierte_woerter = dictionary.get_sorted();
  for (string s : sortierte_woerter) {
    printf("%s,", s.c_str());
  }
  printf("\n");

  return 0;
}
