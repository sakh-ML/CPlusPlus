/* 
Dieses Programm generiert einen sehr großen Zufallstext (1 GB) und sucht 
darin nach einem bestimmten Suchmuster (hier "abcd"). 
Es gibt zwei Suchfunktionen: eine serielle und eine parallele mit OpenMP.
Ziel ist, die Leistung der parallelen Suche mit mehreren Threads zu messen.
*/

#include <omp.h>
#include <iostream>
#include <string>
using namespace std;


#define STRING_LENGTH (1024UL * 1024UL * 1024UL * 1UL)  // 1GB
#define SEARCH_PATTERN_LENGTH 4

// Text init function in C++
void initText(string& s) {
    static const char alpha[] = "abcdefghijklmnopqrstuvwxyz";
    for (int i = 0; i < STRING_LENGTH; i++) {
        s[i] = alpha[rand() % (sizeof(alpha) - 1)];
    }
}

unsigned long simple_string_search(string& random_text, string& search_pattern) {
    unsigned long matches = 0;
    for (unsigned long i = 0; i < random_text.size() - search_pattern.size(); i++) {
        bool match = true;
        for (int x = 0; x < SEARCH_PATTERN_LENGTH; x++) {
            if (search_pattern[x] != random_text[x + i]) {
                match = false;
            }
        }
        if (match) {
            matches++;
        }
    }
    return matches;
}

// unsigned long simple_string_search_p(string& random_text, string& search_pattern) {
//     unsigned long matches = 0;
// #pragma omp parallel for
//     for (unsigned long i = 0; i < STRING_LENGTH - SEARCH_PATTERN_LENGTH; i++) {
//         bool match = true;
//         for (int x = 0; x < SEARCH_PATTERN_LENGTH; x++) {
//             if (search_pattern[x] != random_text[x + i]) {
//                 match = false;
//             }
//         }
//         if (match) {
// #pragma omp atomic
//             matches++;
//         }
//     }
//     return matches;
// }

// unsigned long simple_string_search_p(string& random_text, string& search_pattern) {
//     unsigned long matches = 0;
// #pragma omp parallel for
//     for (unsigned long i = 0; i < STRING_LENGTH - SEARCH_PATTERN_LENGTH; i++) {
//         bool match = true;
//         for (int x = 0; x < SEARCH_PATTERN_LENGTH; x++) {
//             if (search_pattern[x] != random_text[x + i]) {
//                 match = false;
//             }
//         }
//         if (match) {
// #pragma omp critical
// {
//             matches++;
// }
//         }
//     }
//     return matches;
// }

unsigned long simple_string_search_p(string& random_text, string& search_pattern) {
    unsigned long matches = 0;
#pragma omp parallel for reduction(+ : matches) num_threads(64)
    for (unsigned long i = 0; i < STRING_LENGTH - SEARCH_PATTERN_LENGTH; i++) {
        bool match = true;
        for (int x = 0; x < SEARCH_PATTERN_LENGTH; x++) {
            if (search_pattern[x] != random_text[x + i]) {
                match = false;
            }
        }
        if (match) {
            matches++;
        }
    }
    return matches;
}

int main() {
    cout << "Generating random text" << endl;
    string random_text(STRING_LENGTH, ' ');
    initText(random_text);
    cout << "Random Text created" << endl;

    cout << "Searching in string" << endl;
    string compare = "abcd";
    double start = omp_get_wtime();
    unsigned long res;
    // res = simple_string_search(random_text, compare);
    double end = omp_get_wtime();
    cout << "String search found " << res << " matches and took " << (end - start) << "s" << endl;

    start = omp_get_wtime();
    res = simple_string_search_p(random_text, compare);
    end = omp_get_wtime();
    cout << "Parallel String search found " << res << " matches and took " << end - start << "s" << endl;
}

