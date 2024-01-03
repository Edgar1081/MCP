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
        std::set<int> subset;
        int damage;
        int vic;
        std::mt19937 generator;
        int max_vertices;
        int max_edges;
        int actual_edges;
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
            int percent = clone(generator)/vertices;
            int c = 0;
            std::shared_ptr<std::set<int>> iused;
            while(c < percent){
                int indexE = -1;
                int indexI = -1;

                do {
                    indexE = distribution(generator);
                } while (subset.find(indexE) == subset.end());

                do {
                    indexI = distribution(generator);
                } while (subset.find(indexI) != subset.end());

                subset.erase(indexE);
                subset.insert(indexI);
            }

            actual_edges = count_edges();
        }


    private:
        int maxedges() {
            return vertices * (vertices - 1) / 2;
        }

        double initial_cost() {
            return max_edges - actual_edges;
        }

        void respawn(std::shared_ptr<Player> best){
            init_subset();
            actual_edges = count_edges();
        }

        int count_edges(){
            int e = 0;
            for (std::set<int>::iterator it = subset.begin(); it != subset.end(); ++it) {
                for (std::set<int>::iterator it2 = it; it2 != subset.end(); ++it) {
                    if(graph->get_edge(*it, *it2) == 1)
                        e++;
                }
            }
            return e;
        }

        void init_subset() {
            int c = 0;
            while(c < vertices){
                int i = distribution(generator);
                if(subset.find(i) == subset.end()){
                    c++;
                    subset.insert(i);
                }

            }
        }
};
