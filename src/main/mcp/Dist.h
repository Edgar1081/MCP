#pragma once
#include <memory>
#include <vector>
#include <random>
#include <iostream>

class Dist {
private:
    std::shared_ptr<double[]> probabilities;
    size_t size;
    std::shared_ptr<std::discrete_distribution<int>> distribution;

public:
    // Constructor
    Dist(std::shared_ptr<double[]> probabilities, size_t size)
        : probabilities(probabilities), size(size) {
        distribution = std::make_shared<std::discrete_distribution<int>>(probabilities.get(), probabilities.get() + size);
    }

    // Update probability at a specific index
    void update(int index, double newProbability) {
        probabilities[index] = newProbability;
        distribution =
            std::make_shared<std::discrete_distribution<int>>(probabilities.get(), probabilities.get() + size);
    }

    void print_probs(){
        double sum = 0;
        for (size_t i = 0; i < size; ++i) {
            sum += probabilities[i];
        }

        std::cout << "SUM " << sum << std::endl;
        for(size_t i = 0; i < size; i++)
            if(probabilities[i] > 0.01)
                std::cout << i << "(" << probabilities[i]/sum << ")" << " ";
        std::cout << std::endl;
    }

    // Sample from the distribution
    int operator()(std::mt19937& generator) {
        return (*distribution)(generator);
    }
};
