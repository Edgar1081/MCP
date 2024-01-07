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

    int vertices = graph->getVertices();
    std::shared_ptr<double []> probs(new double [vertices]);
    for (int i = 0; i < vertices; ++i) {
        probs[i] = 1.0/vertices;
    }
    std::shared_ptr<BRO> bro = std::make_shared<BRO>(123,25,15,3,graph, probs);



    std::cout << bro->get_cost() << std::endl;
     bro->print_sets();

    std::cout << "PLAY" << std::endl;
    bro->play();

    std::cout << bro->get_cost() << std::endl;
    bro->print_sets();
    return 0;
}
