#pragma once
#include <iostream>
#include <fstream>
#include <string>

class IO {
public:
    static std::string readGraph6File(const std::string& filename) {
        std::ifstream file(filename);
        std::string graph6String;

        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                if (line.find(">>graph6<<") != std::string::npos) {
                    std::getline(file, graph6String);
                    break;
                }
            }

            file.close();
        } else {
            std::cerr << "Unable to open file: " << filename << std::endl;
        }

        return graph6String;
    }
};
