/*
* Implementiert eine generische doppelt verkettete Liste mit
* grundlegenden Operationen wie Einfügen vorne/hinten, Löschen,
* Suchen, Einfügen nach einem bestimmten Knoten und Zugriff per Index.
* Enthält außerdem Copy-Konstruktor und Zuweisungsoperator zur sicheren
* Kopie der Liste.
*/

template <typename U>
class DLLNode {
 public:
  DLLNode(U const& x) : cont_{x} {}
  DLLNode* prev_ = nullptr;  // Zeiger auf das vorherige Element
  U cont_;                   // speichert das Datenelement
  DLLNode* next_ = nullptr;  // zeiger auf das nächste Element
};

template <typename T>
class DoublyLinkedList {
  DLLNode<T>* head_;  // Beginn der Liste
  DLLNode<T>* last_;  // Ende der Liste
  int size_;          // #der Elemente in der Liste sein

 public:
  // Konstruktor:
  DoublyLinkedList() : head_{nullptr}, last_{nullptr}, size_{0} {}

  // copy constructor: s. Quiz VL 20 Folie 18
  DoublyLinkedList(DoublyLinkedList const& other)
      : head_{nullptr}, size_{0}, last_{nullptr} {
    for (DLLNode<T>* current = other.head_; current != nullptr;
         current = current->next_) {
      push_back(current->cont_);  // pushing to back to remain order
    }
  }

  // Kopierzuweisungsoperator -- not covered in lecture:
  DoublyLinkedList& operator=(DoublyLinkedList const& other) {
    while (!is_empty()) pop_front();  // empty myself
    for (DLLNode<T>* current = other.head_; current != nullptr;
         current = current->next_) {
      push_back(current->cont_);  // pushing to back to remain order
    }
    return *this;
  }

  // wahr gdw. die Liste leer ist
  bool is_empty() const { return head_ == nullptr; }

  // fügt x am Anfang der Liste an:
  void push_front(T const& x) {
    DLLNode<T>* new_node =
        new DLLNode<T>(x);    // erzeuge neue Knoten mit Inhalt x
    new_node->next_ = head_;  // der neue Knoten zeigt auf den bisher ersten...
    new_node->prev_ = nullptr;  // neue Knoten hat keinen Vorgänger
    if (is_empty())
      last_ = new_node;  // falls in leere Liste eingefügt, dann last_ setzen
    else
      head_->prev_ = new_node;
    head_ = new_node;  // ... und ist der neue erste Knoten
    ++size_;
  }

  // loescht das erste Element aus der DoublyLinkedList:
  T pop_front() {
    if (!is_empty()) {
      DLLNode<T>* tmp = head_->next_;  // temporäre Kopie des Zeigers
      T first_element = head_->cont_;  // erzeugt eine Kopie
      delete head_;  // loescht das Knotenobjekt (das irgendwann mal in
                     // push_front angelegt wurde)

      head_ = tmp;
      if (is_empty())
        last_ = nullptr;
      else
        head_->prev_ = nullptr;  // das vormals 2. Element ist nun das erste
      --size_;
      return first_element;  // erzeugt Kopie, da keine Referenz zurückgegeben
    }
    return T();  // damit irgendwas zurückgegeben wird
  }

  // fügt x am Ende der Liste an:
  void push_back(T x) {
    DLLNode<T>* new_node =
        new DLLNode<T>(x);  // erzeuge neue Knoten mit Inhalt x
    new_node->next_ =
        nullptr;  // der neue Knoten zeigt mit next_ ins nichts, da am Ende..
    new_node->prev_ = last_;  // .. aber mit prev_ auf den vormals letzten
    if (is_empty())
      head_ = new_node;  // falls in leere Liste eingefügt, dann head_ setzen
    else
      last_->next_ = new_node;
    last_ = new_node;  // ... und ist der neue erste Knoten
    ++size_;
  }

  // gibt das letzte Element der Liste zurück und loescht es aus der Liste:
  T pop_back() {
    if (!is_empty()) {
      T last_elt(last_->cont_);  // erzeuge Kopie vom letzten Eintrag
      DLLNode<T>* tmp_node = last_->prev_;
      delete last_;
      last_ = tmp_node;
      if (!last_)         // list is now empty
        head_ = nullptr;  // for push_back/queue
      else
        last_->next_ = nullptr;  // new head is now first
      --size_;
      return last_elt;
    }
    return T();  // ruft Standard-Konstruktor für T auf
  }

  // insert s after where -- assume where exists:
  void insert_after(DLLNode<T>* where, T const& x) {
    DLLNode<T>* nxt_node = where->next_;
    //                                 situation:  WHERE     <->     NXT
    DLLNode<T>* new_node = new DLLNode<T>(x);
    //                                      want:  WHERE <-> NEW <-> NXT
    where->next_ = new_node;                   //  WHERE  -> NEW
    new_node->prev_ = where;                   //  WHERE <-  NEW
    new_node->next_ = nxt_node;                //            NEW  -> NXT
    if (nxt_node) nxt_node->prev_ = new_node;  //            NEW <-  NXT
    if (!nxt_node) last_ = new_node;           // we got a new last node!
    ++size_;
  }

  // gibt Zeiger auf den Knoten node mit node.cont_ == s zurück:
  DLLNode<T>* find(T const& s) const {
    for (DLLNode<T>* current = head_; current != nullptr;
         current = current->next_) {
      if (current->cont_ == s) return current;
    }
    return nullptr;
  }

  // zum Iterieren über die Liste:
  DLLNode<T> const* begin() const { return head_; }
  DLLNode<T> const* end() const { return last_; }

  // Index-Operator als Referenz:
  T& operator[](int i) {
    DLLNode<T>* current = head_;  // kopiere Zeiger auf head_
    for (int j = 0; j < i; ++j) current = current->next_;  // folge i-mal next_
    return current->cont_;
  }

  // gibt die #Elemente in der Liste zurück:
  int size() { return size_; }

  // Destruktor (nicht effizient, da bei pop_front() unnoetige Kopien erzeugt
  // werden):
  ~DoublyLinkedList() {
    while (!is_empty()) pop_front();  // loescht alle Knotenobjekte aus der LL
  }
};
