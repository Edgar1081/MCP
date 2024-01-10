#include <iostream>
#include <memory>
#include <bitset>
#include <algorithm>
#include <random>
#include <string>
#include <unistd.h>
#include "../Graph.h"
#include "../IO.h"
#include "../BR.h"  // Include the necessary header for the BR class

int main(int argc, char* argv[]) {
    int seed = 0;
    int cycles = 0;
    int numPlayers = 0;
    int hp = 0;
    double e = 0;
    double p = 0;
    std::string filename;

    int opt;
    while ((opt = getopt(argc, argv, "s:c:n:h:e:p:f:")) != -1) {
        switch (opt) {
            case 's':
                seed = std::stoi(optarg);
                break;
            case 'c':
                cycles = std::stoi(optarg);
                break;
            case 'n':
                numPlayers = std::stoi(optarg);
                break;
            case 'h':
                hp = std::stoi(optarg);
                break;
            case 'e':
                e = std::stod(optarg);
                break;
            case 'p':
                p = std::stod(optarg);
                break;
            case 'f':
                filename = optarg;
                break;
            default:
                std::cerr << "Usage: " << argv[0] << " -s <seed> -c <cycles> -n <num_players> -h <hp> -e <epsilon> -p <probability> -f <filename.g6>" << std::endl;
                return 1;
        }
    }

    if (filename.empty()) {
        std::cerr << "Usage: " << argv[0] << " -s <seed> -c <cycles> -n <num_players> -h <hp> -e <epsilon> -p <probability> -f <filename.g6>" << std::endl;
        return 1;
    }

    std::string graph6String = IO::readGraph6File(filename);
    std::cout << graph6String << std::endl;

    if (graph6String.empty()) {
        std::cerr << "Failed to read the graph6 file." << std::endl;
        return 1;
    }

    std::shared_ptr<Graph> graph = std::make_shared<Graph>(graph6String);

    // Create BR class using provided parameters
    BR br(seed, numPlayers, hp, graph, e, cycles, p);

    // Call the solve method
    br.solve();

    return 0;
}
