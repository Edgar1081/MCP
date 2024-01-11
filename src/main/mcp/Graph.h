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
    std::string ver_code;

public:
    Graph(const std::string& graph6) :
        graph6(graph6), vertices(0),bits(""),
        adjMatrix(nullptr) {
        decode_vertices();
        bits = decode_bits();
        decode_adjMatrix();
    }

    Graph(int _vertices, std::shared_ptr<int[]> _adjMatrix, int _n_edges):
        vertices(_vertices), adjMatrix(_adjMatrix), n_edges(_n_edges){
        ver_code = code_vertices();
        code_adjMatrix();
        std::string letters = code_bits();
        graph6 += ver_code;
        graph6 += letters;
    }

    int bits_len(){
        return bits.size();
    }

    int getVertices() const {
        return vertices;
    }
    std::string get_ver_code(){
        return ver_code;
    }

    std::string get_bits(){
        return bits;
    }

    std::shared_ptr<int[]> getAdjMatrix(){
        return adjMatrix;
    }

    int get_edge(int i, int j){
        return adjMatrix[i*vertices + j];
    }

    int get_edges(){
        return n_edges;
    }

    std::string to_string(){
        return graph6;
    }

    std::shared_ptr<Graph> inverse(){
        std::shared_ptr<int[]> inverseadjMatrix(new int[vertices*vertices]);
        for (int i = 0; i < vertices; ++i) {
            for (int j = i+1; j < vertices; ++j) {
                if(i == j)
                    continue;
                if(adjMatrix[i*vertices+j] == 1){
                    inverseadjMatrix[i * vertices + j] = 0;
                    inverseadjMatrix[j * vertices + i] = 0;
                }else{
                    inverseadjMatrix[i * vertices + j] = 1;
                    inverseadjMatrix[j * vertices + i] = 1;
                }
            }
        }
        int max_edges = vertices*(vertices-1)/2;
        return std::make_shared<Graph>(vertices, inverseadjMatrix, max_edges-n_edges);
    }

private:

    void code_adjMatrix(){
        bits = "";
        for (int j = 1; j < vertices; j++) {
            for (int i = 0; i < j; i++) {
                bits += std::to_string(adjMatrix[i*vertices + j]);
            }
        }

    }

    std::string code_bits(){
        std::string letters = "";
        for(size_t i = 0; i < bits.length()%6; i++)
            bits+="0";
        for (size_t i = 0; i < bits.length(); i+=6){
            std::string sub = bits.substr(i, i+6);
            std::bitset<6> byte(sub);
            char c = static_cast<char>(byte.to_ulong()+63);
            letters += c;
        }
        return letters;
    }

    std::string code_vertices(){
        if(vertices <= 62)
            return code_chiquito();
        return code_grande();
    }

    std::string code_chiquito(){
        char c = (vertices+63);
        return std::string(1, c);
    }

    std::string code_grande(){
        std::bitset<18> binary_vertices(vertices);
        int p1 = static_cast<int>(get_range(12,17, binary_vertices).to_ulong());
        int p2 = static_cast<int>(get_range(6,11, binary_vertices).to_ulong());
        int p3 = static_cast<int>(get_range(0,5, binary_vertices).to_ulong());
        char c1 = (p1+63);
        char c2 = (p2+63);
        char c3 = (p3+63);

        std::string vertices_code = "~";
        vertices_code.append(1, c1);
        vertices_code.append(1, c2);
        vertices_code.append(1, c3);
        return vertices_code;
    }

    std::bitset<6> get_range(int a, int b, std::bitset<18> o){
        std::bitset<6> r;
        for(int i =  0; i < 6; i++)
            r[i] = o[i+a];
        return r;
    }

    void decode_vertices() {
        if (graph6[0] == '~')
            decode_grande();
        else
            decode_chiquito();
    }

    void decode_grande() {
        vertices = 0;
        int actual = 0;
        for (int i = 1; i < 4; i++) {
            actual = static_cast<int>(graph6[i]) - 63;
            actual = actual << (3 - i) * 6;
            vertices += actual;
        }
    }

    void decode_chiquito() {
        vertices = static_cast<int>(graph6[0]) - 63;
    }

    std::string decode_bits() {
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

    void decode_adjMatrix() {
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
