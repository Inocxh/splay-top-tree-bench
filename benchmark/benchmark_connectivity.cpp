#include <min_spanning_forest.h>
#include <top_tree.h>
#include "add_weight_cluster.hpp"

#include <stdio.h>
#include <fstream>
#include <bits/stdc++.h>
#include <string>
#include <cassert>
#include <chrono>
#include <unordered_map> 


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


struct ConnectivityTopTree {
    TopTree<AddWeightCluster, int, None> top_tree;
    std::unordered_map<std::pair<int,int>, Edge<AddWeightCluster, int, None> *, pair_hash> edge_map;

    ConnectivityTopTree(int n) {
        this->top_tree = TopTree<AddWeightCluster, int, None>(n);
    }

    void insert(int u, int v) {
        if (u > v) {
            std::swap(u,v);
        } 
        auto edge = top_tree.link_ptr(u, v, 0);
        this->edge_map.insert({std::make_pair(u, v), edge});
    };
    void remove(int u, int v) {
        if (u > v) {
            std::swap(u,v);
        } 
        auto edge = this->edge_map.extract(std::make_pair(u, v));
        top_tree.cut_ptr(edge.mapped());
    };
    bool path(int u, int v) {
        return top_tree.connected(u, v);
    };
};

int main(int argc, char *argv[]) {
    int warmups = std::atol(argv[1]);
    int iterations = std::atol(argv[2]);
    std::ifstream data_file(argv[3]);
    std::string name = "";
    //std::getline(data_file, name);
    std::string problem;
    int n;
    int m;
    data_file >> problem >> n >> m;
    std::vector<Connectivity_Query> queries;
    for (int i = 0; i < m; i++) {
        char query;
        int u;
        int v;
        int weight;
        data_file >> query >> u >> v;
        queries.push_back(Connectivity_Query{query, u, v});
    }

    std::vector<double> warmup_times;
    // Warmup
    for (int i = 0; i < warmups; i++) {
        auto start = std::chrono::high_resolution_clock::now();
        int total_cons = 0;
        ConnectivityTopTree top_tree = ConnectivityTopTree(n);
        for (Connectivity_Query q : queries) {
            if (q.op == 'i') {
                top_tree.insert(q.u, q.v);
            } else if (q.op == 'd') {
                top_tree.remove(q.u, q.v);
            } else if (q.op == 'p') { 
                if (top_tree.path(q.u,q.v))
                    total_cons += 1;          
            }
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::micro> time_taken = end - start;
        warmup_times.push_back(time_taken.count());
    }
    std::vector<double> times;

    // Run measurement
    for (int i = 0; i < iterations; i++) {
        auto start = std::chrono::high_resolution_clock::now();
        int total_cons = 0;

        ConnectivityTopTree top_tree = ConnectivityTopTree(n);

        for (Connectivity_Query q : queries) {
            if (q.op == 'i') {
                top_tree.insert(q.u, q.v);
            } else if (q.op == 'd') {
                top_tree.remove(q.u, q.v);
            } else if (q.op == 'p') { 
                if (top_tree.path(q.u,q.v))
                    total_cons += 1;      
            }
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::micro> time_taken = end - start;
        times.push_back(time_taken.count());
    }
    std::vector<double> median = times;
	std::sort(median.begin(), median.end());
	std::cout << "{ \"num_vertices\":" << n << ",\"num_edges\":" << queries.size() << ",\"name\":\"splay top tree con\",\"median\":" << median[median.size() / 2] << ",\"warmup_times\":[";
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

