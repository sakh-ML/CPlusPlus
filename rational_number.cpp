/*
* Dieses Programm implementiert eine Klasse für rationale Zahlen (Brüche).
* Es erlaubt Addition, Subtraktion, Multiplikation und Division von rationalen Zahlen.
* Intern werden Brüche automatisch gekürzt.
* Außerdem kann man den Bruch in eine Fließkommazahl umwandeln (to_float)
* und den Kehrwert berechnen (invert).
* Der Zugriff auf Zähler und Nenner ist gekapselt (private),
* daher können sie von außen nicht direkt verändert werden.
*/


#include <cstdio>


int gcd(int a, int b) {
  while (b != a) {
    if (a < b)
      b -= a;
    else
      a -= b;
  }
  return a;
}

/* Hier die Klasse RationalNumber implementieren! */
class RationalNumber {
 private:
  int numerator_;
  int denominator_;

  void reduce() {
    if (numerator_ != 0) {
      int number = gcd(numerator_, denominator_);
      numerator_ /= number;
      denominator_ /= number;
    } else
      denominator_ = 1;
  }

 public:
  RationalNumber(int num = 0, int denom = 1)
      : numerator_{num}, denominator_{denom} {
    reduce();
  }

  float to_float() const {
    float const num = numerator_;
    return num / denominator_;
  }

  RationalNumber invert() const {
    RationalNumber result = RationalNumber(denominator_, numerator_);
    return result;
  }

  void add(RationalNumber const other) {
    numerator_ =
        numerator_ * other.denominator_ + other.numerator_ * denominator_;
    denominator_ *= other.denominator_;
    reduce();
  }

  void sub(RationalNumber other) {
    other.numerator_ = -other.numerator_;
    add(other);
  }

  void mul(RationalNumber const other) {
    numerator_ *= other.numerator_;
    denominator_ *= other.denominator_;
    reduce();
  }

  void div(RationalNumber const other) { mul(other.invert()); }
};

int main() {
  RationalNumber number(7, 2);
  printf("%g\n", number.to_float());  // Ausgabe: 3.5

  RationalNumber number2(8, 2);
  RationalNumber inverted = number2.invert();
  printf("%g\n", inverted.to_float());  // Ausgabe: 0.25

  RationalNumber threeEigth(3, 8);
  RationalNumber oneQuarter(1, 4);
  oneQuarter.add(threeEigth);
  printf("%g\n", oneQuarter.to_float());  // Ausgabe: 0.625
  printf("%g\n", threeEigth.to_float());  // Ausgabe: 0.375

  RationalNumber const fixedNumber(40, 8);
  printf("%g\n", fixedNumber.to_float());  // Ausgabe: 5
  RationalNumber inverted2 = fixedNumber.invert();
  printf("%g\n", inverted2.to_float());  // Ausgabe: 0.2

  // Die folgenden Zeilen sollten zu Fehlern führen:
  // number.reduce();
  // printf("%g\n", number.numerator_);
  // printf("%g\n", number.denominator_);
  // fixedNumber.add(number);
  // fixedNumber.sub(number);
  // fixedNumber.mul(number);
  // fixedNumber.div(number);
}
