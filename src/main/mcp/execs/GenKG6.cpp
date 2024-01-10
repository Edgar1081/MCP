#include <iostream>
#include <random>
#include <string>
#include <memory>
#include <unistd.h>
#include <set>
#include "../Graph.h"

int main(int argc, char* argv[]) {
  int seed = 0;
  int n = 0;
  double edge_prob = 0.5;
  int k_value = 0;

  int opt;
  while ((opt = getopt(argc, argv, "s:n:p:k:")) != -1) {
    switch (opt) {
    case 's':
      seed = std::atoi(optarg);
      break;
    case 'n':
      n = std::atoi(optarg);
      break;
    case 'p':
      edge_prob = std::atof(optarg);
      break;
    case 'k':
      k_value = std::atoi(optarg);
      break;
    default:
      std::cerr << "Usage: " << argv[0] << " -s <seed> -n <size> [-p <probability>] [-k <value>]" << std::endl;
      return 1;
    }
  }

  if (n <= 0) {
    std::cerr << "Size of the graph must be a positive integer." << std::endl;
    return 1;
  }

  std::mt19937 generator(seed);

  if (k_value > n || k_value < 0) {
    std::cerr << "Error: k invalid." << std::endl;
    return 1;
  }

  std::bernoulli_distribution distribution(edge_prob);
  std::shared_ptr<int[]> adjMatrix(new int[n * n]);


  std::set<int> komplete;
  std::uniform_int_distribution<int> d(0, n-1);
  std::mt19937 gen(seed);

  size_t k = k_value;

  while(komplete.size() < k){
    komplete.insert(d(gen));
  }


  int edges = 0;

  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      if (i == j)
        continue;
      int edge = distribution(generator);
      if (edge == 1)
        edges++;
      adjMatrix[i * n + j] = edge;
      adjMatrix[j * n + i] = edge;
    }
  }

  for(int e : komplete)
    for(int i: komplete){
      adjMatrix[e*n + i] = 1;
      adjMatrix[i*n+e] = 1;
    }

  edges += k_value*(k_value-1)/2;

  std::shared_ptr<Graph> g = std::make_shared<Graph>(n, adjMatrix, edges);
  std::cout << ">>graph6<<" << std::endl;
  std::cout << g->to_string();

  return 0;
}
