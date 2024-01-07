#include <iostream>
#include <memory>
#include <bitset>
#include <algorithm>
#include <random>
#include "../Graph.h"
#include "../IO.h"
#include "../BRO.h"
int main(int argc, char* argv[]) {
    double probabilities[] = {0.1, 0.1, 0.1, 0.1, 0.1,0.1,0.1,0.1,0.1,0.1};

    size_t size = sizeof(probabilities) / sizeof(probabilities[0]);

    probabilities[9] = 0.9;
    double sum = std::accumulate(probabilities, probabilities + size, 0.0);
    // Normalize probabilities in a single pass
    for (size_t i = 0; i < size; ++i) {
        probabilities[i] /= sum;
    }

    std::mt19937 generator(1);
    std::discrete_distribution<int> distribution(probabilities, probabilities + size);

    distribution.param(std::discrete_distribution<int>::param_type
                       (probabilities, probabilities+size));
    // Generate and print random elements based on their updated probabilities
    for (int i = 0; i < 100; ++i) {
        int randomElement = distribution(generator);
        std::cout << "Random Element: " << randomElement << std::endl;
    }

    for (size_t i = 0; i < size; ++i) {
        std::cout << probabilities[i] << ",";
    }

    std::shared_ptr<double []> pp(new double[10]{0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1});

    std::mt19937 gen(1);
    std::discrete_distribution<int> dist(pp.get(), pp.get()+10);
    pp[0] = .9;
    dist.param(std::discrete_distribution<int>::param_type
                       (pp.get(), pp.get()+10));

    return 0;
}
