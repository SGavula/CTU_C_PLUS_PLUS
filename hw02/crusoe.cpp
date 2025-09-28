#include "crusoe.hpp"
#include <stack>
#include <queue>

using namespace std;

// VERTEX CLASS
// Default constructor
vertex::vertex() : xy{0, 0}, name("0"), c_forward("#000000") {}

// Parametrized constructor
vertex::vertex(string str, int x, int y, string col) : xy{x, y}, name{str}, c_forward{col} {}

bool vertex::add_neighbour(size_t vv, const string &col) {
    // Check if vertex with id vv is already in the neighbour list
    auto it = find_if(neighbours.begin(), neighbours.end(), [vv](const pair<size_t, string> el) {
        return el.first == vv;
    });

    if(it == neighbours.end()) {
        neighbours.emplace_back(vv, col);
        return true;
    }

    return false;
}

vector<pair<size_t, string>> vertex::get_neighbour() const {
    return neighbours;
}

pair<int, int> vertex::get_xy() const {
    return xy;
}

void vertex::set_color(const std::string& col) {
    c_forward = col;
}

string vertex::get_color() const {
    return c_forward;
}

void vertex::set_edge_color(size_t vv, const string& col) {
    auto it = find_if(neighbours.begin(), neighbours.end(), [vv](const pair<size_t, string> el) {
        return el.first == vv;
    });

    if(it != neighbours.end()) {
        it->second = col;
    }
}

string vertex::get_edge_color(size_t vv) {
    string default_color = "#FFFFFF";

    auto it = find_if(neighbours.begin(), neighbours.end(), [vv](pair<size_t, string> el) {
        return el.first == vv;
    });

    if(it == neighbours.end()) {
        return default_color;
    }

    return it->second;
}

// Graph class
void graph::add_vertex(int x, int y, const std::string &col) {
    string name = to_string(num_elem);
    vertex v(name, x, y, col);
    vertices.push_back(v);
    num_elem++;
}

void graph::add_edge(size_t v1, size_t v2, const string& col) {
    if(v1 >= 0 && v2 >= 0 && v1 < num_elem && v2 < num_elem) {
        vertices[v1].add_neighbour(v2, col);
        vertices[v2].add_neighbour(v1, col);
    }
}

bool graph::is_edge(size_t v1, size_t v2) const {
    if(v1 < 0 || v1 > num_elem || v2 < 0 || v2 > num_elem) {
        return false;
    }

    vector<std::pair<size_t, std::string>> neighbours = vertices[v1].get_neighbour();
    auto it = find_if(neighbours.begin(), neighbours.end(), [v2](const pair<size_t, string> el) {
        return el.first == v2;
    });

    if(it == neighbours.end()) {
        return false;
    }

    return true;
}

string graph::edge_color(size_t v1, size_t v2) const {
    string default_color = "#FFFFFF";

    vector<pair<size_t, std::string>> neighbours = vertices[v1].get_neighbour();

    auto it = find_if(neighbours.begin(), neighbours.end(), [v2](const pair<size_t, string> el) {
        return el.first == v2;
    });

    if(it == neighbours.end()) {
        return default_color;
    }

    return it->second;
}

string graph::vertex_color(size_t v1) const {
    if(v1 < num_elem) {
        return vertices[v1].get_color();
    }

    return "#FFFFFF";
}

void graph::set_vertex_color(size_t v1, const std::string& col) {
    if(v1 >= 0 && v1 < num_elem) {
        vertices[v1].set_color(col);
    }
}

void graph::set_edge_color(size_t v1, size_t v2, const std::string& col) {
    if(v1 >= 0 && v2 >= 0 && v1 < num_elem && v2 < num_elem) {
        vertices[v1].set_edge_color(v2, col);
        vertices[v2].set_edge_color(v1, col);
    }
}

bool graph::empty() const {
    if(num_elem == 0) {
        return true;
    }

    return false;
}

size_t graph::size() const {
    return num_elem;
}

size_t graph::num_edge() const {
    size_t sum = 0;

    for(const auto v : vertices) {
        sum = sum + v.get_neighbour().size();
    }

    return sum / 2;
}

// Stage 2
vertex graph::get_vertex(size_t num) const {
    if(num >= 0 && num < num_elem) {
        return vertices[num];
    }
}

void graph::is_achievable(size_t from, std::vector<size_t>& achieved) {
    vector<bool> visited(vertices.size(), false);
    std::stack<size_t> stack;

    achieved.push_back(from);
    visited[from] = true;
    stack.push(from);

    while(!stack.empty()) {
        size_t vertex_id = stack.top();
        stack.pop();

        vertex vertex = vertices[vertex_id];

        for(auto neighbour : vertex.get_neighbour()) {
            size_t neighbour_id = neighbour.first;
            if(visited[neighbour_id]) {
                continue;
            }

            achieved.push_back(neighbour_id);
            // Add to stack neighbour
            stack.push(neighbour_id);
            // Mark neighbour as visited
            visited[neighbour_id] = true;
        }
    }
}

void graph::color_component(std::vector<size_t> cmp, const std::string& col) {
    for(size_t i = 0; i < cmp.size(); i++) {
        set_vertex_color(cmp[i], col);

        for(size_t j = 0; j < cmp.size(); j++) {
            if(is_edge(cmp[i], cmp[j])) {
                set_edge_color(cmp[i], cmp[j], col);
            }
        }
    }
}

std::vector<size_t> graph::path(size_t v1, size_t v2) {
    // Create vector for visited vertices
    std::vector<bool> visited(vertices.size(), false);
    // Create vector where each node has its own predecessor
    std::vector<size_t> prev(vertices.size(), -1);
    // Create queue for bfs
    std::queue<size_t> queue;

    // Add first node to queue
    queue.push(v1);
    visited[v1] = true;

    // Execute bfs
    while(!queue.empty()) {
        size_t vertex_id = queue.front();
        queue.pop();

        for(auto neighbour : vertices[vertex_id].get_neighbour()) {
            if(!visited[neighbour.first]) {
                queue.push(neighbour.first);
                visited[neighbour.first] = true;
                prev[neighbour.first] = vertex_id;
            }
        }
    }

    std::vector<size_t> path;

    for(size_t at = v2; at != -1; at = prev[at]) {
        path.push_back(at);
    }

    std::reverse(path.begin(), path.end());

    if(path[0] == v1) {
        return path;
    }

    return std::vector<size_t>();
}

void graph::color_path(std::vector<size_t> pth, const std::string& col) {
    for(size_t i = 0; i < pth.size(); i++) {
        if(i+1 == pth.size()) {
            break;
        }

        if(is_edge(pth[i], pth[i+1])) {
            set_edge_color(pth[i], pth[i+1], col);
        }
    }
}

// Stage 3
graph::graph_comp::graph_comp(graph& g) : gg(g), components() {
    std::vector<bool> visited(gg.num_elem, false);
    for(size_t v = 0; v < gg.size(); v++) {
        if(visited[v]) {
            continue;
        }
        std::vector<size_t> achieved = std::vector<size_t>();
        gg.is_achievable(v, achieved);
        components.push_back(achieved);
        for(size_t comp_v : achieved) {
            visited[comp_v] = true;
        }
    }
}

void graph::graph_comp::color_componennts() {
    std::vector<std::string> colors{"red", "olive", "orange", "lightblue", "yellow", "pink", "cyan", "purple", "brown", "magenta"};

    size_t col_size = colors.size();

    for(size_t i = 0; i < components.size(); i++) {
        if(components[i].size() == 1) {
            continue;
        }
        gg.color_component(components[i], colors[i % col_size]);
    }
}

size_t graph::graph_comp::count() const {
    return components.size();
}

size_t graph::graph_comp::count_without_one() const {
    size_t sum = 0;

    for(auto comp : components) {
        if(comp.size() == 1) {
            continue;
        }
        sum++;
    }

    return sum;
}

size_t graph::graph_comp::max_comp() const {
    std::pair<size_t, size_t> max = {0, components[0].size()};

    for(size_t i = 1; i < components.size(); i++) {
        size_t curr_size = components[i].size();
        if(curr_size > max.second) {
            max.first = i;
            max.second = curr_size;
        }
    }

    return max.first;
}

size_t graph::graph_comp::size_of_comp(size_t i) const {
    return components[i].size();
}

std::vector<size_t> graph::graph_comp::get_component(size_t i) const {
    return components[i];
}

bool graph::graph_comp::same_comp(size_t v1, size_t v2) const {
    for (auto component : components) {
        bool found_v1 = false, found_v2 = false;
        for (size_t vertex : component) {
            if (vertex == v1) found_v1 = true;
            if (vertex == v2) found_v2 = true;
            if (found_v1 && found_v2) return true;
        }
    }
    return false;
}

graph::graph_fence::graph_fence(graph& gg, size_t vv, size_t distance) : gg(gg), fence() {
    std::vector<bool> visited(gg.num_elem, false);
    // pair<vertex, distance>
    std::queue<std::pair<size_t, size_t>> queue;

    queue.push({vv, 0});
    visited[vv] = true;
    fence.push_back(vv);

    while(!queue.empty()) {
        std::pair<size_t, size_t> front = queue.front();
        queue.pop();

        size_t vertex = front.first;
        size_t curr_distance = front.second;

        if(curr_distance >= distance) {
            continue;
        }

        for(auto neighbour : gg.get_vertex(vertex).get_neighbour()) {
            size_t neighbour_idx = neighbour.first;
            if(!visited[neighbour_idx]) {
                queue.push({neighbour_idx, (curr_distance + 1)});
                visited[neighbour_idx] = true;
                fence.push_back(neighbour_idx);
            }
        }
    }
}

void graph::graph_fence::color_fence(const std::string& col) {
    for(size_t vertex_id : fence) {
        gg.get_vertex(vertex_id).set_color(col);
    }
}

size_t graph::graph_fence::count_stake() const {
    return fence.size();
}

size_t graph::graph_fence::get_stake(size_t i) const {
    if(i >= fence.size()) {
        return -1;
    }

    return fence[i];
}