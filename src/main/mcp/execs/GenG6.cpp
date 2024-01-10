#include <iostream>
#include <random>
#include <string>
#include <memory>
#include "../Graph.h"
int main(int argn, char* argv[]){
  int seed = std::atoi(argv[1]);
  int n = std::atoi(argv[2]);
  double edge_prob = std::atof(argv[3]);
  std::bernoulli_distribution distribution(edge_prob);

  std::mt19937 generator(seed);

  std::shared_ptr<int[]> adjMatrix(new int[n*n]);

  int edges = 0;

  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      if(i == j)
        continue;
      int edge = distribution(generator);
      if(edge == 1)
        edges++;
      adjMatrix[i * n + j] = edge;
      adjMatrix[j * n + i] = edge;
    }
  }

  std::shared_ptr<Graph> g = std::make_shared<Graph> (n, adjMatrix, edges);
  std::cout << g->to_string();
  std::cout << ">>graph6<<" << std::endl;
  return 0;
}
