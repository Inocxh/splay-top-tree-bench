#include <chrono>
#include <fstream>
#include <iostream>
#include <vector>
#include <bits/stdc++.h>
#include <string>

#ifndef PARSE_INPUT_H

// TODO: Rename

enum QueryType { LINK, CUT, CUT_FROM_PARENT, LCA, PATH };

struct Query {
	QueryType type;
	long arg1;
	long arg2;
	long arg3;
	
	Query( QueryType type, long arg1, long arg2 = -1, long arg3=-1 );
};

std::ostream& operator<<( std::ostream& out, const Query& query );

bool read_query_file( const char* filename, size_t& num_vertices, std::vector<Query>& queries );


/* Requires class with the following methods:
void link( size_t u, size_t v );
void cut( size_t u, size_t v );
bool is_connected( size_t u, size_t v );
*/

template<typename T>
bool bench_queries( size_t num_vertices, const std::vector<Query>& queries, size_t repeat, int warmups, int iterations ) {
	
	std::vector<double> warmup_times;
	// Warmups
	for (int i = 0; i < warmups; i++) {
		auto start = std::chrono::high_resolution_clock::now();
		int total_cons = 0; // To avoid optimizing away path queries.
		T t( num_vertices );
		for( const auto& query : queries ) {
			if( query.type == LINK ) {
				t.link( query.arg1, query.arg2 );
			}
			else if( query.type == CUT ) {
				t.cut( query.arg1, query.arg2 );
			}
			else if( query.type == PATH ) {
				total_cons += t.is_connected( query.arg1, query.arg2 );
			}
			else {
				std::cerr << "Cannot execute query '" << query << "'\n";
				return false;
			}
		}
		auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>( std::chrono::high_resolution_clock::now() - start );
		warmup_times.push_back(duration.count());
	}
	
	
	// Run measurement
	std::vector<double> times;
	for (int i = 0; i < iterations; i++) {
		auto start = std::chrono::high_resolution_clock::now();
		int total_cons = 0; // To avoid optimizing away path queries.
		T t( num_vertices );
		for( const auto& query : queries ) {
			if( query.type == LINK ) {
				t.link( query.arg1, query.arg2 );
			}
			else if( query.type == CUT ) {
				t.cut( query.arg1, query.arg2 );
			}
			else if( query.type == PATH ) {
				total_cons += t.is_connected( query.arg1, query.arg2 );
			}
			else {
				std::cerr << "Cannot execute query '" << query << "'\n";
				return false;
			}
		}
	
		auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>( std::chrono::high_resolution_clock::now() - start );
		times.push_back(duration.count());
	}
	std::vector<double> median = times;
	std::sort(median.begin(), median.end());
	std::cout << "{ \"num_vertices\":" << num_vertices << ",\"num_edges\":" << queries.size() << ",\"name\":\"stt-c\",\"median\":" << median[median.size() / 2] << ",\"warmup_times\":[";
	std::cout << std::accumulate(std::next(warmup_times.begin()), warmup_times.end(), std::to_string(warmup_times[0]), [](std::string a, double b) {
		return a + ',' + std::to_string(b);
	});
	std::cout << "], \"times\":[";
	std::cout << std::accumulate(std::next(times.begin()), times.end(), std::to_string(times[0]), [](std::string a, double b) {
		return a + ',' + std::to_string(b);
	});
	std::cout << "]}\n";
	
	
	
	/*std::cout << "Total yes-anwers: " << total_cons / repeat << "\n";
    std::cout << "Rotations: " << num_rotations / repeat << "/run – " << num_rotations / repeat / queries.size() << "/query\n";
	std::cout << duration.count() << " us total\n";
	std::cout << duration.count() / repeat << " us/run\n";
	std::cout << duration.count() * 1. / repeat / queries.size() << " us/query\n";*/
	return true;
}

template<typename T>
bool compute_queries( size_t num_vertices, const std::vector<Query>& queries ) {
	T t( num_vertices );
	for( const auto& query : queries ) {
		if( query.type == LINK ) {
			t.link( query.arg1, query.arg2 );
		}
		else if( query.type == CUT ) {
			t.cut( query.arg1, query.arg2 );
		}
		else if( query.type == PATH ) {
			std::cout << (int) t.is_connected( query.arg1, query.arg2 ) << "\n";
		}
		else {
			std::cerr << "Cannot execute query '" << query << "'\n";
			return false;
		}
	}
	
	return true;
}

template<typename T>
int main_connectivity( int argc, const char** argv ) {
	if( argc < 3 ) {
		std::cout << "usage: " << argv[0] << " <bench|compute> <...> <query-file>\n";
		return 1;
	}
	
	std::string cmd( argv[1] );
	if( cmd == "bench" ) {
		if( argc != 6 ) {
			std::cout << "usage: " << argv[0] << " bench <repeat> <warmups> <iterations> <query-file>\n";
			return 1;
		}
		size_t repeat = std::atol( argv[2] );
		int warmups = std::atol(argv[3]);
		int iterations = std::atol(argv[4]);

		size_t num_vertices;
		std::vector<Query> queries;
		if( !read_query_file( argv[5], num_vertices, queries ) ) {
			std::cerr << "Failed parsing file' " << argv[3] << "'\n";
			return 2;
		}
		
		//std::cout << "Successfully parsed file. Now executing " << queries.size() << " queries on " << num_vertices << " vertices " << repeat << " times." << std::endl;
		if( ! bench_queries<T>( num_vertices, queries, repeat, warmups, iterations ) ) {
			return 3;
		}
	}
	else if( cmd == "compute" ) {
		size_t num_vertices;
		std::vector<Query> queries;
		if( !read_query_file( argv[2], num_vertices, queries ) ) {
			std::cerr << "Failed parsing file' " << argv[2] << "'\n";
			return 2;
		}
		if( ! compute_queries<T>( num_vertices, queries ) ) {
			return 3;
		}
	}
	else {
		std::cout << "usage: " << argv[0] << " <bench|compute> <...> <query-file>\n";
		return 1;
	}
	
	
	return 0;
}

#endif
