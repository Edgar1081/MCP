#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>

class Player {
    private:

    int vertices;
    int damage;
    int vic;
    double cost;
    int max_edges;
    int actual_edges;

    void edges(){
        max_edges vetices*(vertices-1)/2;
    }

    void initial_cost(){
        return max_edges-actual_edges;
    }

    public:

    Player(int _vertices): vertices(_vertices), damage(0), vic(0) {

    }
    
}
