#include <min_spanning_forest.h>
#include <MST.h>

#include <stdio.h>
#include <fstream>
#include <cassert>
#include <chrono>
#include <string>

struct MSF_Query {
    int u;
    int v;
    int weight;
};

int main(int argc, char *argv[]) {

    int warmups = std::atol(argv[1]);
    int iterations = std::atol(argv[2]);
    std::ifstream data_file(argv[3]);

    std::string name;
    std::getline(data_file, name);

    std::string problem;
    int n;
    int m;
    data_file >> problem >> n >> m;

    std::vector<MSF_Query> queries;

    for (int i = 0; i < m; i++) {
        char query;
        int u;
        int v;
        int weight;
        data_file >> query >> u >> v >> weight;
        assert(query == 'e');
        queries.push_back(MSF_Query{u,v,weight});
    }

    // Warmup
    for (int i = 0; i < warmups; i++) {
        //Run measurement
        MinSpanForest tree(n);
        for (MSF_Query q : queries) {
            tree.insert(q.u,q.v,q.weight);
        }    
    }

    std::vector<double> times;
    // Run measurement
    for (int i = 0; i < iterations; i++) {
        auto start = std::chrono::high_resolution_clock::now();
        MinSpanForest tree(n);
        for (MSF_Query q : queries) {
            tree.insert(q.u,q.v,q.weight);
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::nano> time_taken = end - start;
        times.push_back(time_taken.count());
    }
    std::sort(times.begin(), times.end());



    //Return elapsed time
    std::cout << "{ \"num_vertices\":" << n << ",\"num_edges\":" << m << ",\"name\":\"" << argv[3] << "\",\"time_ns\":" << times[times.size() / 2] << "}\n";
    return 0;
}

