#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>

class Graph {
private:
    std::string graph6;
    int vertices;
    std::string bits;
    std::unique_ptr<std::vector<std::vector<int>>> adjMatrix;

public:
    Graph(const std::string& graph6) : graph6(graph6), vertices(0), bits(""), adjMatrix(nullptr) {
        decodeVertices();
        bits = decodeBits();
        adjMatrix = std::make_unique<std::vector<std::vector<int>>>(decodeAdjMatrix());
    }

    int getVertices() const {
        return vertices;
    }

    const std::vector<std::vector<int>>& getAdjMatrix() const {
        return *adjMatrix;
    }

private:
    void decodeVertices() {
        if (graph6[0] == '~')
            decodeGrande();
        else
            decodeChiquito();
    }

    void decodeGrande() {
        vertices = 0;
        int actual = 0;
        for (int i = 1; i < 4; i++) {
            actual = static_cast<int>(graph6[i]) - 63;
            actual = actual << (3 - i) * 6;
            vertices += actual;
        }
    }

    void decodeChiquito() {
        vertices = static_cast<int>(graph6[0]) - 63;
    }

    std::string decodeBits() {
        int start = 0;
        bits = "";
        if (vertices > 62)
            start = 4;
        else
            start = 1;

        for (size_t i = start; i < graph6.length(); i++)
            bits += std::bitset<6>(graph6[i] - 63).to_string();
        return bits;
    }

    std::vector<std::vector<int>> decodeAdjMatrix() {
        int index = 0;
        std::vector<std::vector<int>> matrix(vertices, std::vector<int>(vertices, 0));
        for (int j = 1; j < vertices; j++) {
            for (int i = 0; i < j; i++) {
                int bit = bits[index] - '0';
                matrix[i][j] = bit;
                matrix[j][i] = bit;
                index++;
            }
        }
        return matrix;
    }
};
