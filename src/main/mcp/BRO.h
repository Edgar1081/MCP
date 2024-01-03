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
        std::shared_ptr<Graph> graph;
        int vertices;
        int n_players;
        std::shared_ptr<int []> players;

    public:

        BRO(std::shared_ptr<Graph> _graph, int _vertices, int _n_players):
        graph(_graph), vertices(_vertices), n_players(_n_players){
            init_match();
        }

    private:

        void init_match(){
            for(int i = 0; i < n_players; i++){

            }
        }
};
