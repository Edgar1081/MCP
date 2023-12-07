#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <bitset>
#include <random>
#include <set>
#include "Graph.h"

class Player {
    private:
        std::shared_ptr<Graph> graph;
        int vertices;
        int seed;
        int hp;
        int damage;
        int vic;
        std::mt19937 generator;
        int max_vertices;
        int max_edges;
        int actual_edges;
        std::bitset<1000> subset;
        double cost;
        std::uniform_int_distribution<int> distribution;

    public:
        Player(std::shared_ptr<Graph> _graph, int _vertices, int _seed, int _hp) :
            graph(_graph), vertices(_vertices), seed(_seed), hp(_hp), damage(0),
            vic(0), generator(_seed),max_vertices(_graph->getVertices()),
            max_edges(maxedges()), actual_edges(count_edges()),
            cost(initial_cost()), distribution(0, max_vertices) {
        }

        void get_injured(std::shared_ptr<Player> best){

            damage++;
            if(damage > hp){
                respawn(best);
                return;
            }
            std::uniform_int_distribution<int> clone(0,99);
            int percent = clone(generator);
            int c = 0;
            std::set<int> selectedIndices;
            while(c < percent){
                std::uniform_int_distribution<int> index1(0, max_vertices);
                std::uniform_int_distribution<int> index0(0, max_vertices);

                if(index1(generator) == 0){

                }


            }
        }

    private:
        int maxedges() {
            return vertices * (vertices - 1) / 2;
        }

        double initial_cost() {
            return max_edges - actual_edges;
        }

        void respawn(std::shared_ptr<Player> best){}



        void recal_cost(){

        }

        int count_edges(){
            int c = 0;
            for(int i = 0; i < max_vertices; i++){
                if(subset[i] == 0)
                    continue;
                for(int j = i+1; j < max_edges; j++){
                    if(subset[j] == 0)
                        continue;
                    c += graph->get_edge(i,j);
                }
            }
            return c;
        }

        void init_subset() {
            std::set<int> selectedIndices;

            while (subset.count() < static_cast<std::size_t>(vertices)) {
                int index = distribution(generator);

                if (selectedIndices.find(index) == selectedIndices.end()) {
                    subset.set(index);
                    selectedIndices.insert(index);
                }
            }
        }
};
