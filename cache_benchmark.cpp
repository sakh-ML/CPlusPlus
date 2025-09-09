#include <iostream>

#include "cacheline.h"
#include "helper.h"

#define ELEMENTS 50000000

int main(int argc, char **argv) {
    Timer timer;
    std::vector<uint64_t> indices(ELEMENTS);
    cacheLine *data_array = new cacheLine[ELEMENTS];
    uint64_t result = 0;

    // Erzeuge Daten für den Benchmark und fülle indices mit aufsteigenden Werten
    prepare_benchmark(data_array, indices, ELEMENTS);

    // Starte den sequentiellen Durchlauf
    timer.start();
    for (auto i = 0U; i < ELEMENTS; ++i) {
        result += data_array[indices[i]].compute();
    }
    timer.stop();
    std::cout << "Die Ausführung mit sequentiellen Zugriffen hat " << timer.elapsedSeconds() << " Sekunden benötigt (check: " << result << ")!" << std::endl;

    // Shuffle nun die Indizes, damit die Daten im Datenarray nun zufällig zugegriffen werden
    shuffle_indices(indices);
    result = 0;
    timer.start();
    for (auto i = 0U; i < ELEMENTS; ++i) {
        result += data_array[indices[i]].compute();
    }
    timer.stop();
    std::cout << "Die Ausführung mit zufälligen Zugriffen hat " << timer.elapsedSeconds() << " Sekunden benötigt (check: " << result << ")!" << std::endl;

    // Füge nun Prefetch-Instruktionen hinzu
    result = 0;
    //auto prefetch_distance = 0U;
    auto prefetch_distance = 1500U;
    timer.start();

    for(auto i = 0U; i < ELEMENTS; ++i){
        if(i + prefetch_distance < ELEMENTS){
            //Einer davon auswählen !! Level element {0,1,2,3}
            prefetch(data_array[indices[i + prefetch_distance]], 3);
            //prefetch(data_array[i + prefetch_distance], 3);
            //prefetch(data_array[i + prefetch_distance], 2);
            //prefetch(data_array[i + prefetch_distance], 1);
            //prefetch(data_array[i + prefetch_distance], 0);

        }
        result += data_array[indices[i]].compute();
    }

    timer.stop();
    std::cout << "Die Ausführung mit zufälligen Zugriffen und SW-Prefetching hat " << timer.elapsedSeconds() << " Sekunden benötigt (check: " << result << ")!" << std::endl;
}
