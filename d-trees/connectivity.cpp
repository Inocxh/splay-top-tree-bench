#include "dtree/tree.h"
#include "dtree/tree-inl.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <unordered_map> 
#include <bits/stdc++.h>

class Forest
{
public:
    // don't change Cost to an unsigned type; that breaks the Min aggregate
    // read the appendix before changing Cost to a floating-point type
    typedef int Cost;

private:
    typedef dtree::WithAncAggr<dtree::Min<Cost>,
                               dtree::WithAncValue<dtree::Add<Cost>,
                                                   dtree::Begin<>>>
        C;
    typedef dtree::WithEvert<C> E;
    typedef dtree::EndTree<E> Node;

public:
    typedef size_t NodeId;
    Forest();
    ~Forest();
    // creates a forest with node ids 0..size-1
    void Initialize(size_t size);
    // as in [ST85]
    NodeId FindRoot(NodeId v) { return Root(&node_[v]) - node_; }
    void Link(NodeId v, NodeId w) { dtree::Link(&node_[v], &node_[w]); }
    void Cut(NodeId v) { dtree::Cut(&node_[v]); }
    void Evert(NodeId v) { E::Evert(&node_[v]); }

    Node *node_;
private:
    // disallows the copy constructor and the assignment operator
    Forest(const Forest &);
    void operator=(const Forest &);
};

Forest::Forest() : node_(NULL)
{
}

Forest::~Forest()
{
    delete[] node_;
}

void Forest::Initialize(size_t size)
{
    Node *old_node = node_;
    // avoids a double delete[] if new[] throws
    node_ = NULL;
    delete[] old_node;
    node_ = new Node[size];
}

struct Connectivity_Query {
    char op;
    int u;
    int v;
    
};
struct pair_hash {
    template <class T1, class T2>
    std::size_t operator () (const std::pair<T1,T2> &p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second); 

        // Mainly for demonstration purposes, i.e. works but is overly simple
        // In the real world, use sth. like boost.hash_combine
        return h1 ^ h2;  
    }
};

struct Connectivity {
    Forest f;
    std::unordered_map<std::pair<int, int>, int, pair_hash> edge_map;

    Connectivity(int n) {
        this->f.Initialize(n);
    }

    void insert(int u, int v) {
        if (u > v)
            std::swap(u, v);
        f.Evert(u);
        f.Evert(v);

        f.Link(u, v);
        
    }
    void remove(int u, int v) {
        if (u > v)
            std::swap(u, v);    
        f.Evert(v);
        f.Cut(u);
    }

    bool connected(int u, int v) {
        return dtree::SameTree(&f.node_[u], &f.node_[v]);
    }
};


int main(int argc, char *argv[]) {
    int warmups = std::atol(argv[1]);
    int iterations = std::atol(argv[2]);
    std::ifstream data_file(argv[3]);
    if (!data_file) {
        std::cerr << "file not found" << std::endl;
    }
    std::string problem;
    int n;
    int m;
    data_file >> problem >> n >> m;
    std::vector<Connectivity_Query> queries;
    for (int i = 0; i < m; i++) {
        char query;
        int u;
        int v;
        data_file >> query >> u >> v;
        queries.push_back(Connectivity_Query{query, u, v});
    }

    std::vector<double> warmup_times;
    // Warmup
    for (int i = 0; i < warmups; i++) {
        auto start = std::chrono::high_resolution_clock::now();
        int total_cons = 0;
        Connectivity con = Connectivity(n);
        for (Connectivity_Query q : queries) {
            if (q.op == 'i') {
                con.insert(q.u, q.v);
            } else if (q.op == 'd') {
                con.remove(q.u, q.v);
            } else if (q.op == 'p') { 
                if (con.connected(q.u, q.v))
                    total_cons += 1;          
            }
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::nano> time_taken = end - start;
        warmup_times.push_back(time_taken.count());
    }
    std::vector<double> times;
    
    // Run measurement
    for (int i = 0; i < iterations; i++) {
        auto start = std::chrono::high_resolution_clock::now();
        int total_cons = 0;
        Connectivity con = Connectivity(n);
        for (Connectivity_Query q : queries) {
            if (q.op == 'i') {
                con.insert(q.u, q.v);
            } else if (q.op == 'd') {
                con.remove(q.u, q.v);
            } else if (q.op == 'p') { 
                if (con.connected(q.u, q.v))
                    total_cons += 1;          
            }
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::nano> time_taken = end - start;
        times.push_back(time_taken.count());
    }
    std::vector<double> median = times;
	std::sort(median.begin(), median.end());
	std::cout << "{ \"num_vertices\":" << n << ",\"num_edges\":" << queries.size() << ",\"name\":\"D-trees\",\"median\":" << median[median.size() / 2] << ",\"warmup_times\":[";
	std::cout << std::accumulate(std::next(warmup_times.begin()), warmup_times.end(), std::to_string(warmup_times[0]), [](std::string a, double b) {
		return a + ',' + std::to_string(b);
	});
	std::cout << "], \"times\":[";
	std::cout << std::accumulate(std::next(times.begin()), times.end(), std::to_string(times[0]), [](std::string a, double b) {
		return a + ',' + std::to_string(b);
	});
	std::cout << "]}\n";
    return 0;
}

