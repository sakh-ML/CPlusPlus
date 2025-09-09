/*
* Projekt: Quadratwurzelberechnung mit Heron-Verfahren
* Beschreibung:
* Dieses Programm berechnet die Quadratwurzel einer gegebenen Zahl
* mittels des Heron-Verfahrens (auch bekannt als Babylonisches Verfahren).
*
* Funktionsweise:
* - Startwerte a = x und b = 1.
* - Iterative Annäherung an die Quadratwurzel durch Mittelwertbildung von a und b.
* - Abbruch nach maximal 100 Iterationen oder wenn die Genauigkeit von 0.00001 erreicht ist.
*
* Beispiel:
* Für x = 23194514 gibt das Programm die approximative Quadratwurzel aus.
*/


#include <cstdio>

float heron(float x) {
  float a = x;
  float b = 1;

  // setze wiederholend a auf das arithmetische Mittel von a und b
  // (und b so, dass a*b=x bleibt) -- maximal 100 Wiederholungen und
  // bis die Genauigkeit von .00001 erreicht ist:
  int zaehler = 0;  // Zaehler fuer die #Wiederholungen
  while ((a - b) > .00001 && zaehler < 100) {
    a = (a + b) / 2;  // arithmetische Mittel
    b = x / a;        // setze b so, dass a*x==b bleibt
    ++zaehler;
  }
  return a;
}

int main() {
  float x = 23194514;
  printf("Die Quadratwurzel von %f ist %f\n", x, heron(x));
}
