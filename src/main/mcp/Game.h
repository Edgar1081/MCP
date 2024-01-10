#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <bitset>
#include <random>
#include <set>
#include "Graph.h"
#include "Player.h"
#include "Dist.h"

class Game{
private:
    int seed;
    int vertices;
    int max_vertices;
    int n_players;
    int hp;
    std::shared_ptr<Graph> graph;
    double eps;
    int cycles;
    double prob_step;
    std::shared_ptr<Dist> avail;
    std::shared_ptr<Player>* players;
    std::mt19937 gen;
    int best_index;
    int area;
    double radio;
    std::shared_ptr<double []> probs;

public:
    Game(int _seed, int _vertices, int _n_players, int _hp,
         std::shared_ptr<Graph> _graph, double _eps, int _cycles, double _prob_step) :
        seed(_seed), vertices(_vertices), max_vertices(_graph->getVertices()),
        n_players(_n_players), hp(_hp), graph(_graph), eps(_eps),
        cycles(_cycles), prob_step(_prob_step),
        avail(nullptr),  // Initialize avail to avoid issues
        players(new std::shared_ptr<Player>[n_players]), gen(_seed),
        best_index(0), area(_vertices), radio(1),
        probs(new double[max_vertices]) {

        for (int i = 0; i < max_vertices; ++i) {
            probs[i] = 1.0 / max_vertices;
        }

        avail = std::make_shared<Dist>(probs, max_vertices);
        init_match();
        set_best();
    }


    ~Game() {
        delete[] players;
    }

    void print_sets(){
        for(int i = 0; i< n_players; i++){
            players[i]->print_set();
            std::cout << std::endl;
        }
    }

    int get_cost(){
        return players[best_index]->get_cost();
    }

    std::shared_ptr<Player> play(bool prints){
        if(vertices == max_vertices)
            return players[best_index]->clone();

        int c = 0;
        while(c < cycles && players[best_index]->get_cost() != 0){
            shoot_closer();
            if(c % 50 == 0){
                new_probs();
                radio*=eps;
            }
            c++;
            if(prints)
                std::cout << players[best_index]->get_cost() << std::endl;
        }
        return players[best_index]->clone();
    }

    void print_probs(){
        for(int i = 0; i < max_vertices; i++)
                std::cout << i << "(" << probs[i] << ")" << " ";
        std::cout << std::endl;
        avail->print_probs();
    }

    void print_distances(int i){
        for(int j = 0; j < n_players; j++){
            double d = distance(i,j);
            std::cout << "D: "
                      << i << "---" << j << " " << d << std::endl;
        }
    }

    void atack(int i, int j){
        players[i]->shoot();
        players[j]->get_injured(players[best_index]);
    }

    int get_best_index(){
        return best_index;
    }

private:

    void shoot_closer(){
        for(int i = 0; i < n_players; i++){
            double d_best = distance(best_index, i);
            if(d_best > radio)
                players[i]->get_injured(players[best_index]);
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
            if(players[i]->get_cost() < players[closer]->get_cost())
                atack(i,closer);
            else
                atack(closer,i);

            if(players[closer]->get_cost() < players[best_index]->get_cost()){
                int prev = best_index;
                best_index = closer;
                std::cout << "BEST_UPDATED: " << prev << "--------->" << best_index<< std::endl;
            }
            if(players[i]->get_cost() < players[best_index]->get_cost()){
                int prev = best_index;
                best_index = i;
                std::cout << "BEST_UPDATED: " << prev << "---------->" << best_index<< std::endl;
            }


        }
    }

    void set_best(){
        int prev = best_index;
        int best_index_i_ = best_index;
        int c = players[best_index]->get_cost();
        for(int i =0; i < n_players; i++){
            double f = players[i]->get_cost();
            if(f < c){
                best_index_i_ = i;
                c = f;
        }
            }
        best_index = best_index_i_;
        if(prev != best_index)
            std::cout << "BEST_UPDATED: " << prev << "-->" << best_index<< std::endl;
    }

    void new_probs() {
        int c = 0;
        while (c < area) {
            int i = players[best_index]->get_random_vertex();
            probs[i] += prob_step;
            c++;
        }

        avail = std::make_shared<Dist>(probs, max_vertices);
        for (int i = 0; i < n_players; ++i) {
            players[i]->update_distribution(avail);
        }
        area *=eps;
    }

    void init_match(){
        std::uniform_int_distribution<int> seedsgen;
        for(int i = 0; i < n_players; i++){
            int s = seedsgen(gen);
            players[i] =
                std::make_shared<Player>(i,graph, vertices, s, hp, avail, probs);
        }
    }

    double distance(int a, int b){
        std::shared_ptr<std::unordered_set<int>> setA
            = players[a]->get_subset();
        std::shared_ptr<std::unordered_set<int>> setB
            = players[b]->get_subset();
        return jaccard_distance(setA,setB);

    }
    double jaccard_distance(
                            std::shared_ptr<std::unordered_set<int>> A,
                            std::shared_ptr<std::unordered_set<int>> B) {

        std::set<int> OA(A->begin(), A->end());
        std::set<int> OB(B->begin(), B->end());
        std::set<int> uni;
        std::set<int>inter;
        //1 - |(A ^ B)|/|(A u B)|
        std::set_union(OA.begin(), OA.end(), OB.begin(), OB.end(),
                       std::inserter(uni, uni.begin()));
        std::set_intersection(OA.begin(), OA.end(), OB.begin(), OB.end(),
                              std::inserter(inter, inter.begin()));
        double distance = 1.0 - static_cast<double>(inter.size()) / uni.size();
        return distance;
    }

};
