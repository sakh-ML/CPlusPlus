#pragma once

#include <chrono>
#include <algorithm>
#include <random>
#include <stdexcept>
#include <vector>
#include "cacheline.h"

inline void prefetch(cacheLine &addr, int level = 3) {
    // level = 0: L1 (default)
    // level = 1: L2
    // level = 2: L3
    // level = 3 NTA (Non-temporal: Only L1 )
    // rw = 0: read
    switch (level) {
        case 0:  // L1 (+L2+L3)
            __builtin_prefetch(&addr, 0, 3);
            break;
        case 1:  // L2 (not L1) (+L3)
            __builtin_prefetch(&addr, 0, 2);
            break;
        case 2:  // L3 (not L1 or L2)
            __builtin_prefetch(&addr, 0, 1);
            break;
        default: // No locality
            __builtin_prefetch(&addr, 0, 0);
            break;
    }
}

void prepare_benchmark(cacheLine *data_array, std::vector<uint64_t> &indices, size_t size){
    for (auto i = 0U; i < size; ++i){
        data_array[i] = i;
        indices[i] = i;
    }
}

void shuffle_indices(std::vector<uint64_t>& indices) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(indices.begin(), indices.end(), gen);
}

class Timer {
public:
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::time_point<Clock>;

    void start() {
        m_start = Clock::now();
    }

    void stop() {
        m_end = Clock::now();
    }

    double elapsedSeconds() const {
        return std::chrono::duration<double>(m_end - m_start).count();
    }

private:
    TimePoint m_start{};
    TimePoint m_end{};
};
