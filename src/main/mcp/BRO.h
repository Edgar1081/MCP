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
    std::shared_ptr<std::discrete_distribution<int>> avail;
    std::shared_ptr<double []> probs;
    std::shared_ptr<Player>* players;
    std::mt19937 gen;
    int best_index;
    int worst_index;

public:
    BRO(int _seed, int _vertices, int _n_players, int _hp,
        std::shared_ptr<Graph> _graph, std::shared_ptr<std::discrete_distribution<int>> _avail,
        std::shared_ptr<double[]> _probs) :
        seed(_seed), vertices(_vertices), max_vertices(_graph->getVertices()),
        n_players(_n_players),hp(_hp), graph(_graph), avail(_avail), probs(_probs){
        gen.seed(seed);
        players = new std::shared_ptr<Player>[n_players];
        init_match();
        set_best();
    }

    void print_sets(){
        for(int i = 0; i< n_players; i++){
            std::cout << "Player: " << i << std::endl;
            players[i]->print_set();
            std::cout << std::endl;
        }
    }

    void play(){
        if(vertices == max_vertices){
            int etarget = vertices*(vertices-1)/2;
            int act = players[best_index]->get_cost();
            if(etarget == act)
                return;
            else
                return;
        }

        int c = 0;
        //while(players[best_index]->get_cost() != 0){
        while(c < 100000){
            // std::cout << "~~~" << std::endl;
            // std::cout << "DAMAGE: " <<
            //     players[best_index]->get_damage() << std::endl;
            shoot_closer();
            set_best();
            int i = players[best_index]->get_random_vertex();
            int j = i;
            while(i != j){
                j = players[best_index]->get_random_vertex();
            }
            update_probs(i, probs[i]+0.1);
            update_probs(j, probs[i]+0.1);
            // std::cout << best_index << ": " << players[best_index]->get_cost() << std::endl;
            // std::cout << "last" << ": " << players[n_players-1]->get_cost() << std::endl;
            c++;
        }
            std::cout << "~~~" << std::endl;
    }

private:

    void init_match(){
        for(int i = 0; i < n_players; i++){
            players[i] =
                std::make_shared<Player>(graph, vertices, i, hp, avail, probs);
        }
    }

    void set_best(){
        int best_index_i = -1;
        int c = vertices*(vertices-1)/2;
        for(int i =0; i < n_players; i++){
            double f = players[i]->get_cost();
            if(f < c){
                best_index_i = i;
                c = f;
            }
        }
        best_index = best_index_i;
    }

    void shoot_closer(){
        for(int i = 0; i < n_players; i++){
            double d = std::numeric_limits<double>::max();
            int closer = -1;
            for(int j = 0; j < n_players; j++){
                if(i == j)
                    continue;
                double dmin = distance(i,j);
                if(dmin < d){
                    d = dmin;
                    closer = j;
                }
            }
            // std::cout << "I: " << i << " C: " << closer
            //           << " D: " << d << std::endl;
            players[i]->shoot();
            players[closer]->get_injured(players[best_index]);
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

        double distance =1.0 - static_cast<double>(inter.size()) / uni.size();
        return distance;
    }

    void update_probs(int index, double new_prob){
        probs[index] = new_prob;
        (*avail).param(std::discrete_distribution<int>::param_type
                           (probs.get(), probs.get()+max_vertices));
    }

};
