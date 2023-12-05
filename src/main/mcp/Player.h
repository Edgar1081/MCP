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

    void initial_cost(){
        return vetices*(vertices-1)/2;
    }


    public:

    Player(int _vertices): vertices(_vertices), damage(0), vic(0) {

    }
    
}
