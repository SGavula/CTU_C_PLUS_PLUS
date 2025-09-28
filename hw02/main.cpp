#include <iostream>
#include "crusoe.hpp"  // Include if you need the Crusoe class

void print_vector(std::vector<size_t>& vec) {
    for(size_t i = 0; i < vec.size(); i++) {
        std::cout << vec[i] << " ";
    }
    std::cout << std::endl;
}

void print_graph(std::vector<vertex> vec) {
    for(size_t i = 0; i < vec.size(); i++) {
        std::cout << vec[i].get_name() << " | ";
        std::vector<std::pair<size_t, std::string>> neighbours = vec[i].get_neighbour();
        for(size_t j = 0; j < neighbours.size(); j++) {
            std::cout << neighbours[j].first << " ";
        }
        std::cout << std::endl;
    }
}

int main() {
    // Here you can write your main program logic
    std::cout << "Hello from Crusoe program!" << std::endl;

    graph g;
    g.add_vertex(20, 20);
    g.add_vertex(60, 20);
    g.add_vertex(100, 20);
    g.add_vertex(20, 60);
    g.add_vertex(60, 60);
    g.add_vertex(100, 60);
    g.add_vertex(20, 100);
    g.add_vertex(60, 100);
    g.add_vertex(100, 100);
    g.add_edge(3, 0);
    g.add_edge(3, 4);
    g.add_edge(3, 6);
    g.add_edge(1, 2);
    g.add_edge(2, 5);
    g.add_edge(8, 5);
    g.add_edge(7, 8);

    std::cout << "Printing vertices of graph g: " << std::endl;
    print_graph(g.get_vertices());

    // std::vector<size_t> achieved;
    // g.is_achievable(0, achieved);
    // std::cout << "Achieved vertexes: " << std::endl;
    // print_vector(achieved);

    std::cout << "Componenets: " << std::endl;
    graph::graph_comp cmp{g};
    for(auto item : cmp.get_components()) {
        print_vector(item);
        std::cout << std::endl;
    }

    cmp.color_componennts();

    for(size_t i = 0; i < g.size(); i++) {
        std::cout << i << " : " << g.get_vertex(i).get_color() << std::endl;
    }

    std::vector<size_t> path = g.path(1, 7);

    print_vector(path);

    return 0;
}