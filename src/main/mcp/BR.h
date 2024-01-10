#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <bitset>
#include <random>
#include <set>
#include "Graph.h"
#include "Player.h"
#include "Game.h"

class BR{
private:
  int seed;
  int max_vertices;
  int n_players;
  int hp;
  std::shared_ptr<Graph> graph;
  std::shared_ptr<Player> best;
  std::shared_ptr<Player> w;
  std::mt19937 gen;
  double eps;
  int cycles;
  double prob_step;

public:

  BR(int _seed, int _n_players, int _hp,
     std::shared_ptr<Graph> _graph,
     double _eps, int _cycles, double _prob_step) :
    seed(_seed), max_vertices(_graph->getVertices()),
    n_players(_n_players),hp(_hp), graph(_graph), eps(_eps),
    cycles(_cycles), prob_step(_prob_step){
    gen.seed(seed);
  }

  void solve(){
    std::uniform_int_distribution<int> seedsgen;
    for(int vertices = max_vertices; vertices > 0; vertices--){
      std::cout << "Seraching for: K" << vertices << std::endl;
      int rseed = seedsgen(gen);
      Game match (rseed, vertices, n_players, hp, graph, eps, cycles, prob_step);
      auto winner_match = match.play(false);
      w = winner_match;

      if(vertices == max_vertices)
        best = w;

      if(winner_match->get_cost() < best->get_cost()){
        best = winner_match;
      }

      std::cout << "Winner of game: " << max_vertices-vertices
                << "| COST: " << winner_match->get_cost()
                << "| VERTICES: " << vertices << std::endl;

      std::cout << "BEST: " << best->get_vertices()
                << " COST " << best->get_cost() << std::endl;

      if(best->get_cost() == 0){
        std::cout << "WINNER " << vertices << std::endl;
        return;
      }
    }

  }

};
