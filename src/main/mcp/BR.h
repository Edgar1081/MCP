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
  bool prints;
  std::shared_ptr<std::string> log;

public:

  BR(int _seed, int _n_players, int _hp,
     std::shared_ptr<Graph> _graph,
     double _eps, int _cycles, double _prob_step, bool _prints) :
    seed(_seed), max_vertices(_graph->getVertices()),
    n_players(_n_players),hp(_hp), graph(_graph), eps(_eps),
    cycles(_cycles), prob_step(_prob_step), prints(_prints){
    gen.seed(seed);
    log = std::make_shared<std::string>("");
  }

  void solve(){
    std::uniform_int_distribution<int> seedsgen;
    int start = max_vertices;
    int d = std::sqrt(1+(8*graph->get_edges()));
    int n = 1 + d;
    start = n/2;

    for(int vertices = start; vertices > 0; vertices--){
      if(prints)
        std::cout << "Seraching for: K" << vertices << std::endl;
      int rseed = seedsgen(gen);
      Game match (rseed, vertices, n_players, hp, graph, eps, cycles, prob_step);
      auto winner_match = match.play(false);
      w = winner_match;

      if(vertices == start)
        best = w;

      double r1 =static_cast<double> (winner_match->get_cost())/vertices;
      double r2 = static_cast<double>(best->get_cost() )/best->get_vertices();

      if(r1 < r2){
        best = winner_match;
      }

      double ratio = best->get_cost()/vertices;

      if(prints){

        std::cout
          << "| RATIO: " << ratio
          << " | Winner of game: " << max_vertices-vertices
          << " | COST: " << winner_match->get_cost()
          << " | VERTICES: " << vertices
          << std::endl;


        std::cout << "BEST: " << best->get_vertices()
                  << " COST " << best->get_cost() << std::endl;
      }

      if(best->get_cost() == 0){
        if(prints){
          std::cout << "WINNER    : " << vertices << std::endl
                    << "GAME SEED : " << rseed << std::endl
                    << "N_PLAYERS : " << n_players << std::endl
                    << "HP        : " << hp << std::endl
                    << "Eps       : " << eps << std::endl
                    << "Cycles    : " << cycles << std::endl
                    << "Prob_step : " << prob_step << std::endl
                    << std::endl;
        }
        return;
      }
    }

  }

  std::shared_ptr<std::string> get_log(){
    std::string c = std::to_string(best->get_cost());
    return std::make_shared<std::string>(c);
  }


};
