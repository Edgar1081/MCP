#include <iostream>
#include <memory>
#include <bitset>
#include "../Graph.h"
#include "../IO.h"
int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename.g6>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    std::string graph6String = IO::readGraph6File(filename);
    std::cout << graph6String << std::endl;

    if (!graph6String.empty()) {
        Graph graph(graph6String);

        std::cout << "Vertices: " << graph.getVertices() << std::endl;
        std::cout << "Adjacency Matrix:" << std::endl;
        const auto& adjMatrix = graph.getAdjMatrix();
        for (const auto& row : adjMatrix) {
            for (int value : row) {
                std::cout << value << " ";
            }
            std::cout << std::endl;
        }
    } else {
        std::cerr << "Failed to read the graph6 file." << std::endl;
        return 1;
    }

    return 0;
}
