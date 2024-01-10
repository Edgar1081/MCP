#pragma once
#include <stdexcept>
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
#include "Dist.h"

class Player {
private:
    int index;
    std::shared_ptr<Graph> graph;
    int vertices;
    int seed;
    int hp;
    std::shared_ptr<std::unordered_set<int>> prev_subset;
    std::shared_ptr<std::unordered_set<int>> subset;
    std::shared_ptr<Dist> avail;
    std::shared_ptr<double []> probs;
    std::mt19937 generator;
    int damage;
    int vic;
    int max_vertices;
    int max_edges;
    int actual_edges;
    double cost;
    std::uniform_int_distribution<int> distribution;

public:
    Player(int _index, std::shared_ptr<Graph> _graph, int _vertices, int _seed,
           int _hp, std::shared_ptr<Dist> _avail,
           std::shared_ptr<double []> _probs) :
        index(_index),graph(_graph), vertices(_vertices), seed(_seed), hp(_hp),
        avail(_avail),probs(_probs), generator(seed){
        damage = 0;
        vic = 0;
        max_vertices = graph->getVertices();
        max_edges = maxedges();
        init_subset();
        prev_subset = std::make_shared<std::unordered_set<int>>(*subset);
        actual_edges = count_edges();
        cost = initial_cost();
        distribution = std::uniform_int_distribution<int>(0, max_vertices-1);
    }

    ~Player() {
        subset.reset();
    }

    std::shared_ptr<Player> clone() const {
        auto clonedPlayer = std::make_shared<Player>(*this);  // Use copy constructor
        clonedPlayer->subset = std::make_shared<std::unordered_set<int>>(*subset);
        return clonedPlayer;
    }

    void update_distribution(const std::shared_ptr<Dist>& distWrapper) {
        avail = distWrapper;
    }

    int get_damage(){
        return damage;
    }

    int get_vertices(){
        return vertices;
    }

    void print_set(){
        std::cout << "Player: " << index << " COST: " << cost <<
            " V: " << subset->size() << " v: " << vertices << std::endl;
        std::vector<int> orderedSubset(subset->begin(), subset->end());

        std::sort(orderedSubset.begin(), orderedSubset.end());

        for (const auto& element : orderedSubset) {
            std::cout << element << " ";
        }
        std::cout << std::endl;
    }

    double get_prob(int index){
        return probs[index];
    }

    void shoot(){
        vic++;
        damage = 0;
    }

    int get_wins(){
        return vic;
    }

    int get_cost(){
        return max_edges-actual_edges;
    }

    int get_injured(std::shared_ptr<Player> best){
        damage++;
        if(damage > hp){
            respawn(best);
            return 0;
        }
        int percent = 1;
        closer(percent, best);
        return 1;
    }

    std::shared_ptr<std::unordered_set<int>> get_subset(){
        return subset;
    }

    int get_random_vertex() {
        std::uniform_int_distribution<int> p(0, subset->size() - 1);
        int steps = p(generator);

        auto set_it = subset->begin();
        std::advance(set_it, steps);

        return *set_it;
    }

    int erase_one(int r, int m){
        if(subset->find(r) == subset->end())
            std::cout << r << " NOT IN" << std::endl;
        subset->erase(r);
        subset->erase(m);
        std::shared_ptr<std::unordered_set<int>> erased =
            std::make_shared<std::unordered_set<int>>();
        erased->insert(r);
        erased->insert(m);
        prev_subset = std::make_shared<std::unordered_set<int>>(*subset);
        recal_rem(erased);
        return r;
    }

    int insert_one(int i, int j){
        subset->insert(i);
        subset->insert(j);
        std::shared_ptr<std::unordered_set<int>> inserted =
            std::make_shared<std::unordered_set<int>>();
        inserted->insert(i);
        inserted->insert(j);
        recal_ins(inserted);
        prev_subset = std::make_shared<std::unordered_set<int>>(*subset);
        return i;
    }

    int erase_one(){
        auto it = subset->begin();
        int r = *it;
        subset->erase(r);
        std::shared_ptr<std::unordered_set<int>> erased =
            std::make_shared<std::unordered_set<int>>();
        erased->insert(r);
        prev_subset = std::make_shared<std::unordered_set<int>>(*subset);
        recal_rem(erased);
        actual_edges = count_edges();
        return r;
    }
private:

    void closer(int n, std::shared_ptr<Player> best){

        std::shared_ptr<std::unordered_set<int>> inserted =
            std::make_shared<std::unordered_set<int>>();
        std::shared_ptr<std::unordered_set<int>> erased =
            std::make_shared<std::unordered_set<int>>();

        int new_elements = insert(n, best, inserted);
        recal_ins(inserted);
        prev_subset = std::make_shared<std::unordered_set<int>>(*subset);

        erase_new(new_elements, inserted, erased);
        prev_subset = std::make_shared<std::unordered_set<int>>(*subset);
        recal_rem(erased);

        cost = initial_cost();
    }

    void recal_ins(std::shared_ptr<std::unordered_set<int>> inserted){
        int total_added = 0;
        for(auto it = inserted->begin(); it != inserted->end(); ++it)
            total_added += edges_of(*it);

        for(auto it = inserted->begin(); it != inserted->end(); ++it)
            for(auto itt = std::next(it); itt != inserted->end(); ++itt)
                if(graph->get_edge(*it, *itt) == 1)
                    total_added++;
        actual_edges += total_added;
    }

    void recal_rem(std::shared_ptr<std::unordered_set<int>> erased){
        int total_removed = 0;
        for(auto it = erased->begin(); it != erased->end(); ++it)
            total_removed += edges_of(*it);

        for(auto it = erased->begin(); it != erased->end(); ++it)
            for(auto itt = std::next(it); itt != erased->end(); ++itt)
                if(graph->get_edge(*it, *itt) == 1)
                    total_removed++;

        actual_edges -= total_removed;
    }

    int edges_of(int v){
        int e = 0;
        for(auto it = prev_subset->begin(); it != prev_subset->end(); ++it)
            if(graph->get_edge(*it, v) == 1)
                e++;
        return e;
    }

    int insert(int p, std::shared_ptr<Player> bss,
               std::shared_ptr<std::unordered_set<int>> inserted) {
        int new_elements = 0;

        for (int i = 0; i < p; i++) {
            int n = bss->get_random_vertex();
            auto itt = subset->find(n);
            if (itt == subset->end()) {
                new_elements++;
                subset->insert(n);
                inserted->insert(n);
            }
        }
        return new_elements;
    }

    void erase_new(int new_elements,
                   std::shared_ptr<std::unordered_set<int>> inserted,
                   std::shared_ptr<std::unordered_set<int>> erased) {
        auto it = subset->begin();
        int erase = 0;
        while (erase < new_elements && it != subset->end()) {
            int e = *it;
            auto itt = inserted->find(e);

            if (itt != inserted->end()) {
                ++it;
                continue;
            }

            erased->insert(e);
            it = subset->erase(it);
            erase++;
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
        for (auto it = subset->begin(); it != subset->end(); ++it) {
            for (auto it2 = std::next(it); it2 != subset->end(); ++it2) {
                if(*it == *it2)
                    continue;
                if(graph->get_edge(*it, *it2) == 1)
                    e++;
            }
        }
        return e;
    }

    void init_subset() {
        subset = std::make_shared<std::unordered_set<int>>();
        int c = 0;
        while(c < vertices){
            int sample = -1;
            sample = (*avail)(generator);
            if(subset->find(sample) == subset->end()){
                c++;
                subset->insert(sample);
            }
        }
    }
    //respawn a player closer to the best in the match
    void respawn(std::shared_ptr<Player> best){
        vic = 0;
        damage = 0;
        // std::uniform_int_distribution<int> clone(0,99);
        //int percent = clone(generator)*vertices/100;
        init_subset();
        //land(best, percent);
        prev_subset = std::make_shared<std::unordered_set<int>>(*subset);
        actual_edges = count_edges();
        cost = initial_cost();
        // closer(percent, best);
    }

};
