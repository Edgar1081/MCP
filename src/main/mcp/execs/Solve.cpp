#include <iostream>
#include <memory>
#include <bitset>
#include <algorithm>
#include <random>
#include "../Graph.h"
#include "../IO.h"
#include "../BRO.h"
int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename.g6>" << std::endl;
        return 1;
    }
    std::string filename = argv[1];
    std::string graph6String = IO::readGraph6File(filename);
    std::cout << graph6String << std::endl;
    if (graph6String.empty()) {
        std::cerr << "Failed to read the graph6 file." << std::endl;
        return 1;
    }

    std::shared_ptr<Graph> graph = std::make_shared<Graph>(graph6String);

    std::cout << "Vertices: " << graph->getVertices() << std::endl;
    std::cout << "Adjacency Matrix:" << std::endl;

    std::shared_ptr<int[]> adjMatrix = graph->getAdjMatrix();
    for (int i = 0; i < graph->getVertices(); i++) {
        for (int j = 0; j < graph->getVertices(); j++) {
            std::cout << adjMatrix[i * graph->getVertices() + j] << " ";
        }
        std::cout << std::endl;
    }

    int vertices = graph->getVertices();
    std::shared_ptr<double []> probs(new double [vertices]);
    for (int i = 0; i < vertices; ++i) {
        probs[i] = 100.0/vertices;
    }
    std::shared_ptr<std::discrete_distribution<int>> avail(
                                                           new std::discrete_distribution<int>(probs.get(), probs.get() + vertices));
    std::shared_ptr<BRO> bro = std::make_shared<BRO>(123,vertices-50,50,3,graph, avail, probs);

    bro->print_sets();

    bro->play();
    bro->print_sets();

    return 0;
}
