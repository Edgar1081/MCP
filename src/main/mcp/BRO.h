#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <bitset>
#include <random>
#include <set>
#include "Graph.h"
#include "Player.h"

class BRO{
private:
    int seed;
    int vertices;
    int max_vertices;
    int n_players;
    int hp;
    std::shared_ptr<Graph> graph;
    std::discrete_distribution<int> avail;
    std::shared_ptr<double []> probs;
    std::shared_ptr<Player>* players;
    std::mt19937 gen;
    int best_index;
    int worst_index;

public:
    BRO(int _seed, int _vertices, int _n_players, int _hp,
        std::shared_ptr<Graph> _graph, std::discrete_distribution<int> _avail,
        std::shared_ptr<double[]> _probs) :
        seed(_seed), vertices(_vertices), max_vertices(_graph->getVertices()),
        n_players(_n_players),hp(_hp), graph(_graph), avail(_avail), probs(_probs),
        players(new std::shared_ptr<Player>[n_players]){
        gen.seed(seed);
        init_match();
    }

private:
    void init_match(){
        for(int i = 0; i < n_players; i++){
            players[i] = std::make_shared<Player>
                (graph, vertices, i, hp, avail, probs);
        }
    }

    void update_state(){
        best_index = -1;
        for(int i = 0; i < n_players; i++){
            for(int j = i+1; j < n_players; j++){
                int d = distance(i,j);
            }
        }

    }

    double distance(int a, int b){
        std::shared_ptr<std::unordered_set<int>> setA
            = players[a]->get_subset();
        std::shared_ptr<std::unordered_set<int>> setB
            = players[b]->get_subset();
        return jaccard_distance(setA,setB);
        //return hamming_distance(setA,setB);
    }

    int hamming_distance(
        std::shared_ptr<std::unordered_set<int>>& A,
        std::shared_ptr<std::unordered_set<int>>& B){

        std::bitset<500> bitset1, bitset2;

        for (int elem : *A) {
            bitset1.set(elem);
        }

        for (int elem : *B) {
            bitset2.set(elem);
        }

        std::bitset<500> xorResult = bitset1 ^ bitset2;
        return xorResult.count();
    }

    double jaccard_distance(
        std::shared_ptr<std::unordered_set<int>> A,
        std::shared_ptr<std::unordered_set<int>> B){
        std::unordered_set<int> uni, inter;
        std::set_union(A->begin(), A->end(), B->begin(), B->end(),
                       std::inserter(uni, uni.begin()));

        std::set_intersection(A->begin(), A->end(), B->begin(), B->end()
                              , std::inserter(inter, inter.begin()));

        double distance = 1.0 - static_cast<double>(inter.size()) / uni.size();
        return distance;
    }


    void update_probs(int index, double new_prob){
        probs[index] = new_prob;
        avail.param(std::discrete_distribution<int>::param_type
                           (probs.get(), probs.get()+max_vertices));
    }

};
