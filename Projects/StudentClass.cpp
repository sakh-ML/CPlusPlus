/*
* Klasse Student: Speichert Name, Noten und Matrikelnummer.
* Unterstützt Konstruktoren, Copy-Konstruktor, Zuweisung und Destruktor.
* Verwaltet dynamischen Speicher für Name und Noten.
*/


#include <cstdio>

class Student {
 private:
  char* name_;
  int name_length_;

  float* grades_;
  int grade_count_;

  int matriculation_number_;

 public:
  // Konstruktor, der nur Platz für den Namen und die Noten allokiert
  Student(int name_length, int grade_count, int matriculation_number)
      : name_length_{name_length},
        grade_count_{grade_count},
        matriculation_number_{matriculation_number} {
    name_ = new char[name_length_];
    grades_ = new float[grade_count_];
  }

  // Konstruktor, der Platz für den Namen und die Noten allokiert und den
  // übergebenen Namen kopiert
  Student(char* name, int name_length, int grade_count,
          int matriculation_number)
      : name_length_{name_length},
        grade_count_{grade_count},
        matriculation_number_{matriculation_number} {
    name_ = new char[name_length];
    overwrite_name(name, name_length);
    grades_ = new float[grade_count_];
  }

  // Konstruktor, der Platz für den Namen und die Noten allokiert und den
  // übergebenen Namen und die übergebenen Noten kopiert
  Student(char const* name, int name_length, float const* grades,
          int grade_count, int matriculation_number)
      : name_length_{name_length},
        grade_count_{grade_count},
        matriculation_number_{matriculation_number} {
    name_ = new char[name_length];
    overwrite_name(name, name_length);

    grades_ = new float[grade_count_];
    for (int i = 0; i < grade_count; ++i) grades_[i] = grades[i];
  }

  //======== Implementieren Sie hier den Copy-Konstruktor ========
  Student(Student const& other_student) {
    name_ = nullptr;
    grades_ = nullptr;
    *this = other_student;
  }

  //======== Implementieren Sie hier den Copy-Zuweisungsoperator ========
  Student& operator=(Student const& other_student) {
    delete[] name_;
    delete[] grades_;

    name_length_ = other_student.name_length_;
    grade_count_ = other_student.grade_count_;
    matriculation_number_ = other_student.matriculation_number_;

    name_ = new char[name_length_];
    grades_ = new float[grade_count_];

    for (int i = 0; i < name_length_; ++i) {
      name_[i] = other_student.name_[i];
    }

    for (int i = 0; i < grade_count_; ++i) {
      grades_[i] = other_student.grades_[i];
    }

    return *this;
  }

  // Destruktor
  ~Student() {
    delete[] name_;
    delete[] grades_;
  }

  // Überschreibt den gespeicherten Namen mit new_name
  void overwrite_name(char const* new_name, int new_name_length) {
    delete[] name_;
    name_length_ = new_name_length;
    name_ = new char[name_length_];
    for (int i = 0; i < name_length_; ++i) name_[i] = new_name[i];
  }

  // Gibt einen Pointer auf den Namen zurück
  char* get_name() { return name_; }

  // Gibt die Länge des gespeicherten Namens zurück
  int get_name_length() { return name_length_; }

  // Setzt die Note an Position grade_index auf den Wert grade_value
  void set_grade(int grade_index, float grade_value) {
    grades_[grade_index] = grade_value;
  }

  // Gibt die Note an Position grade_index zurück
  float get_grade(int grade_index) { return grades_[grade_index]; }
};

int main(){


	return 0;
}
