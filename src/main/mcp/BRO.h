#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <bitset>
#include <random>
#include <set>
#include "Graph.h"

class BRO{
private:
    int seed;
    int vertices;
    int max_vertices;
    int n_players;
    std::shared_ptr<Graph> graph;
    std::shared_ptr<int []> players;
    std::mt19937 generator;
    std::mt19937 gen;
    int best_index;
    int worst_index;

public:

    BRO(int _seed, int _vertices, int _max_vertices, int _n_players,
        std::shared_ptr<Graph> _graph):
        seed(_seed), vertices(_vertices), max_vertices(_max_vertices),
        n_players(_n_players), graph(_graph), players(new int[n_players]){
    }

private:
    void init_match(){
        for(int i = 0; i < n_players; i++){

        }
    }
};
