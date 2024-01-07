#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <bitset>
#include <random>
#include <set>
#include <unordered_set>
#include <algorithm>
#include <random>
#include "Graph.h"

class Player {
private:
    std::shared_ptr<Graph> graph;
    int vertices;
    int seed;
    int hp;
    std::shared_ptr<std::unordered_set<int>> subset;
    std::mt19937 generator;
    std::discrete_distribution<int> avail;
    std::shared_ptr<double []> probs;
    int damage;
    int vic;
    int max_vertices;
    int max_edges;
    int actual_edges;
    double cost;
    std::uniform_int_distribution<int> distribution;

public:
    Player(std::shared_ptr<Graph> _graph, int _vertices, int _seed,
           int _hp,std::discrete_distribution<int> _avail,
           std::shared_ptr<double []> _probs) :
        graph(_graph), vertices(_vertices), seed(_seed), hp(_hp),
        avail(_avail),probs(_probs), damage(0),vic(0),
        max_vertices(_graph->getVertices()),max_edges(maxedges()),
        actual_edges(count_edges()),cost(initial_cost()),
        distribution(0, max_vertices-1) {
        generator.seed(seed);
    }

    double get_prob(int index){
        return probs[index];
    }

    void shoot(){
        vic++;
    }

    int get_wins(){
        return vic;
    }

    int get_cost(){
        return cost;
    }

    void get_injured(std::shared_ptr<Player> best){
        damage++;
        if(damage > hp){
            respawn(best);
            return;
        }
        int percent = 1;
        closer(percent, best);
        actual_edges = count_edges();
    }

    std::shared_ptr<std::unordered_set<int>> get_subset(){
        return subset;
    }

    int get_random_vetex(){
        int i = -1;
        do {
            i = distribution(generator);
        } while (subset->find(i) != subset->end());
        return -1;
    }

private:

    //respawn a player closer to the best in the match
    void respawn(std::shared_ptr<Player> best){
        vic = 0;
        damage = 0;
        std::uniform_int_distribution<int> clone(0,99);
        int percent = clone(generator)*vertices/100;
        init_subset();
        closer(percent, best);
        actual_edges = count_edges();
    }

    void closer(int n, std::shared_ptr<Player> best){
        std::shared_ptr<std::unordered_set<int>> inserted;
        std::shared_ptr<std::unordered_set<int>> best_subset
            = best->get_subset();

        int new_elements = insert(n, best_subset, inserted);
        erase_new(new_elements, inserted);
    }

    int insert(int p, std::shared_ptr<std::unordered_set<int>> bss,
               std::shared_ptr<std::unordered_set<int>> inserted){
        int new_elements = 0;
        auto it = bss->begin();
        for (int i = 0; i < p && it != bss->end(); ++i, ++it) {
            auto itt = subset->find(*it);
            if(itt == subset->end())
                new_elements++;
            subset->insert(*it);
            inserted->insert(*it);
        }
        return new_elements;
    }

    void erase_new(int new_elements,
                   std::shared_ptr<std::unordered_set<int>> inserted){
        auto it = subset->begin();
        int erased = 0;
        while(erased < new_elements){
            int e = *it;
            auto itt = inserted->find(e);

            if (itt != inserted->end()){
                ++it;
                continue;
            }
            subset->erase(e);
            erased++;
        }
    }

    //Return the max posible number of edges given the number of vertices
    int maxedges() {
        return vertices * (vertices - 1) / 2;
    }

    //return the initial cost, the difference between the max posible number of edges and
    //the actual edges in the subgraph
    double initial_cost() {
        return max_edges - actual_edges;
    }

    void wipe(){
        subset->clear();
    }

    //count the actual number of edges in the subgraph
    int count_edges(){
        int e = 0;
        for (std::unordered_set<int>::iterator it = subset->begin(); it != subset->end(); ++it) {
            for (std::unordered_set<int>::iterator it2 = it; it2 != subset->end(); ++it) {
                if(graph->get_edge(*it, *it2) == 1)
                    e++;
            }
        }
        return e;
    }


    void init_subset() {
        int c = 0;
        while(c < vertices){
            int i = -1;
            i = avail(generator);

            if(subset->find(i) == subset->end()){
                c++;
                subset->insert(i);
            }

        }
    }
};
