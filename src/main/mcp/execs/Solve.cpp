#include <iostream>
#include <memory>
#include <bitset>
#include <algorithm>
#include <random>
#include <string>
#include <unistd.h>
#include "../Graph.h"
#include "../IO.h"
#include "../Game.h"

int main(int argc, char* argv[]) {
    int seed = 0;
    int verticesToSearch = 0;
    int numPlayers = 0;
    int hp = 0;
    std::string filename;

    int opt;
    while ((opt = getopt(argc, argv, "s:a:n:h:f:")) != -1) {
        switch (opt) {
            case 's':
                seed = std::stoi(optarg);
                break;
            case 'a':
                verticesToSearch = std::stoi(optarg);
                break;
            case 'n':
                numPlayers = std::stoi(optarg);
                break;
            case 'h':
                hp = std::stoi(optarg);
                break;
            case 'f':
                filename = optarg;
                break;
            default:
                std::cerr << "Usage: " << argv[0] << " -s <seed> -a <vertices_to_search> -n <num_players> -h <hp> -f <filename.g6>" << std::endl;
                return 1;
        }
    }

    if (filename.empty()) {
        std::cerr << "Usage: " << argv[0] << " -s <seed> -a <vertices_to_search> -n <num_players> -h <hp> -f <filename.g6>" << std::endl;
        return 1;
    }

    std::string graph6String = IO::readGraph6File(filename);
    std::cout << graph6String << std::endl;

    if (graph6String.empty()) {
        std::cerr << "Failed to read the graph6 file." << std::endl;
        return 1;
    }

    std::shared_ptr<Graph> graph = std::make_shared<Graph>(graph6String);

    int max_vertices = graph->getVertices();

    std::shared_ptr<double []> probs(new double [max_vertices]);
    for (int i = 0; i < max_vertices; ++i) {
        probs[i] = 1.0 / max_vertices;
    }

    std::shared_ptr<Game> game =
        std::make_shared<Game>(seed, verticesToSearch, numPlayers, hp, graph, probs);

    std::cout << "BEST: "<< game->get_best_index() << " " <<game->get_cost() << std::endl;

    game->print_distances(0);
    //game->print_sets();
    std::cout << "-------------PLAY------------" << std::endl;

    game->play();

    //game->print_sets();
    std::cout << "BEST: "<< game->get_best_index() << " " <<game->get_cost() << std::endl;
    //game->print_probs();
    game->print_distances(game->get_best_index());
    game->print_probs();
    return 0;
}
