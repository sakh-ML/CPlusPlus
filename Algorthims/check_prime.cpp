/*
* Projekt: Primzahl-Prüfer
* Beschreibung:
* Dieses Programm prüft, ob eine vom Benutzer eingegebene ganze Zahl eine Primzahl ist.
* Es verwendet eine effiziente Methode:
* - Zahlen kleiner als 2 werden sofort ausgeschlossen.
* - Gerade Zahlen (außer 2) sind keine Primzahlen.
* - Danach wird nur mit ungeraden Zahlen bis zur Quadratwurzel getestet,
*   ob die Zahl durch einen dieser Teiler teilbar ist.
* Am Ende wird dem Benutzer angezeigt, ob es sich um eine Primzahl handelt oder nicht.
*/

#include <cstdio>
#include <iostream>

bool is_prime(int x) {

	if(x < 2){
		return false;
	}

	if(x == 2){
		return true;
	}

	if(x % 2 == 0){
		return false;
	}

	int i = 3;
	while(i * i <= x){
		if(x % i == 0){
			return false;
		}
		else{
			i += 2;
		}
	}

	return true;
}

int main() {
		
	int x;
	std::cout << "Enter a number, u want to check if it is a prime :" << std::endl;
	std::cin >> x;
	if(is_prime(x) == true){
		std::cout << x << " is a prime number !!" << std::endl;
	}

	else{
		std::cout << x << " it is not a prime mumber !!" << std::endl;
	}
}
