#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <bitset>

class Graph {
    private:
        std::string graph6;
        int vertices;
        std::string bits;
        std::shared_ptr<int[]> adjMatrix;
        int n_edges;

    public:
        Graph(const std::string& graph6) : graph6(graph6), vertices(0), bits(""), adjMatrix(nullptr) {
            decodeVertices();
            bits = decodeBits();
            decodeAdjMatrix();
        }

        int getVertices() const {
            return vertices;
        }

        const int* getAdjMatrix() const {
            return adjMatrix.get();
        }

        int get_edge(int i, int j){
            return adjMatrix[i*vertices + j];
        }

        int edges_count(){
            return n_edges;
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

        void decodeAdjMatrix() {
            adjMatrix = std::make_unique<int[]>(vertices * vertices);
            n_edges = 0;

            int index = 0;
            for (int j = 1; j < vertices; j++) {
                for (int i = 0; i < j; i++) {
                    int bit = bits[index] - '0';
                    if (bit == 1)
                        n_edges++;
                    adjMatrix[i * vertices + j] = bit;
                    adjMatrix[j * vertices + i] = bit;
                    index++;
                }
            }
        }
};
