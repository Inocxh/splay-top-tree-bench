#include <cassert>
#include <chrono>
#include <iostream>
#include <vector>
#include <unordered_map>

#include "parse_input.h"

#ifndef VARIANT
#define VARIANT 13
#endif

/** Variants
 * 1-6: MTR
 * 10: Naive greedy from paper
 * 11: Semi-naive greedy from rust impl
 * 12: Slightly improved 11
 * 13: Improved 12 with NST
 */

#if ( VARIANT >= 1 && VARIANT <= 3 ) || ( VARIANT >= 10 && VARIANT <= 12 )
#define ROT_IMPROVED
#elif VARIANT >= 4 || VARIANT >= 13
#define ROT_NST
#endif

static size_t num_rotations = 0;

namespace stt {
	enum NodeSepType {
		NOSEP, DSEP, ISEP
	};
	
	struct Node {
		Node* parent;
		Node* dsep_child;
		Node* isep_child;
	
		void attach( Node* p ) {
			assert( this->parent == nullptr );
			this->parent = p;
		}
	
		void detach() {
			assert( this->parent != nullptr && !this->is_separator_hint( this->parent ) );
			this->parent = nullptr;
		}
		
		[[nodiscard]] NodeSepType get_sep_type() {
			Node* p = this->parent;
			if( p ) {
				if( p->dsep_child == this ) { return DSEP; }
				else if( p->isep_child == this ) { return ISEP; }
			}
			return NOSEP;
		}
		
		[[nodiscard]] NodeSepType get_sep_type_hint( Node* p ) {
			if( p->dsep_child == this ) { return DSEP; }
			else if( p->isep_child == this ) { return ISEP; }
			else { return NOSEP; }
		}

#ifdef ROT_IMPROVED
		inline bool rotate() { // Returns whether this is a separator after the rotation
			num_rotations++;
			
			assert( this->parent != nullptr );
			assert( this->is_separator() || !this->parent->is_separator() );
			
			Node* v = this;
			Node* p = v->parent;
			Node* g = p->parent;
			Node* c = v->dsep_child;
			
			// Change parents
			v->parent = g;
			p->parent = v;
			
			// Changes related to c
			if( c ) {
				c->parent = p;
				std::swap( c->dsep_child, c->isep_child );
			}
			
			bool p_was_sep = false;
			// Change separator information for children of v and g
			if( g ) { // p was not root
				Node* old_p_dsep_child = p->dsep_child;
				
				// Change isep_child of p (stays null if p is the root)
				if( old_p_dsep_child && old_p_dsep_child != v ) {
					p->isep_child = old_p_dsep_child;
				}
				else if( p->isep_child == v ) {
					p->isep_child = nullptr;
				}
				
				if( p == g->dsep_child ) {
					p_was_sep = true;
					g->dsep_child = v;
				}
				else if( p == g->isep_child ) {
					p_was_sep = true;
					g->isep_child = v;
				}
				
				if( old_p_dsep_child != v ) {
					// p separates v and g
					v->dsep_child = p;
				}
				else {
					// v separates p and g
					v->dsep_child = v->isep_child;
					if( p_was_sep ) {
						v->isep_child = p;
					}
					else {
						v->isep_child = nullptr;
					}
				}
			}
			else { // p was root
				v->dsep_child = nullptr;
			}
			
			// Change dsep child of p
			p->dsep_child = c;
			
			return p_was_sep;
		}
		
#elif defined( ROT_NST )
		inline NodeSepType rotate() { // Returns separator type after the rotation
			num_rotations++;
			
			assert( this->parent != nullptr );
			assert( this->is_separator() || !this->parent->is_separator() );
			
			Node* v = this;
			Node* p = v->parent;
			Node* g = p->parent;
			Node* c = v->dsep_child;
			
			// Change parents
			v->parent = g;
			p->parent = v;
			
			// Changes related to c
			if( c ) {
				c->parent = p;
				std::swap( c->dsep_child, c->isep_child );
			}
			
			NodeSepType p_type = NOSEP;
			// Change separator information for children of v and g
			if( g ) { // p was not root
				Node* old_p_dsep_child = p->dsep_child;
				
				// Change isep_child of p (stays null if p is the root)
				if( old_p_dsep_child && old_p_dsep_child != v ) {
					p->isep_child = old_p_dsep_child;
				}
				else if( p->isep_child == v ) {
					p->isep_child = nullptr;
				}
				
				if( p == g->dsep_child ) {
					p_type = DSEP;
					g->dsep_child = v;
				}
				else if( p == g->isep_child ) {
					p_type = ISEP;
					g->isep_child = v;
				}
				
				if( old_p_dsep_child != v ) {
					// p separates v and g
					v->dsep_child = p;
				}
				else {
					// v separates p and g
					v->dsep_child = v->isep_child;
					if( p_type != NOSEP ) {
						v->isep_child = p;
					}
					else {
						v->isep_child = nullptr;
					}
				}
			}
			else { // p was root
				v->dsep_child = nullptr;
			}
			
			// Change dsep child of p
			p->dsep_child = c;
			
			return p_type;
		}
		
		inline NodeSepType rotate_dsep() { // Returns separator type after the rotation
			num_rotations++;
			
			assert( this->parent != nullptr );
			assert( this->parent->dsep_child == this );
			
			Node* v = this;
			Node* p = v->parent;
			Node* g = p->parent;
			Node* c = v->dsep_child;
			
			// Change parents
			v->parent = g;
			p->parent = v;
			
			// Changes related to c
			if( c ) {
				c->parent = p;
				std::swap( c->dsep_child, c->isep_child );
			}
			
			NodeSepType p_type = NOSEP;
			// Change separator information for children of v and g
			if( g ) { // p was not root
				if( p == g->dsep_child ) {
					p_type = DSEP;
					g->dsep_child = v;
				}
				else if( p == g->isep_child ) {
					p_type = ISEP;
					g->isep_child = v;
				}
				
				// v separates p and g
				v->dsep_child = v->isep_child;
				if( p_type != NOSEP ) {
					v->isep_child = p;
				}
				else {
					v->isep_child = nullptr;
				}
			}
			else { // p was root
				v->dsep_child = nullptr;
			}
			
			// Change dsep child of p
			p->dsep_child = c;
			
			return p_type;
		}
		
		inline NodeSepType rotate_isep() { // Returns separator type after the rotation
			num_rotations++;
			
			assert( this->parent != nullptr );
			assert( this->parent->isep_child == this );
			
			Node* v = this;
			Node* p = v->parent;
			Node* g = p->parent;
			Node* c = v->dsep_child;
			
			// Change parents
			v->parent = g;
			p->parent = v;
			
			// Changes related to c
			if( c ) {
				c->parent = p;
				std::swap( c->dsep_child, c->isep_child );
			}
			
			NodeSepType p_type = NOSEP;
			// Change separator information for children of v and g
			if( g ) { // p was not root
				Node* old_p_dsep_child = p->dsep_child;
				
				// Change isep_child of p (stays null if p is the root)
				p->isep_child = old_p_dsep_child;
				
				if( p == g->dsep_child ) {
					p_type = DSEP;
					g->dsep_child = v;
				}
				else if( p == g->isep_child ) {
					p_type = ISEP;
					g->isep_child = v;
				}
				
				// We now that p separates v and g
				v->dsep_child = p;
			}
			else { // p was root
				v->dsep_child = nullptr;
			}
			
			// Change dsep child of p
			p->dsep_child = c;
			
			return p_type;
		}
		
		inline NodeSepType rotate_nosep() { // Returns separator type after the rotation
			num_rotations++;
			
			assert( this->parent != nullptr );
			assert( !this->is_separator() && !this->parent->is_separator() );
			assert( this->isep_child == nullptr );
			
			Node* v = this;
			Node* p = v->parent;
			Node* g = p->parent;
			Node* c = v->dsep_child;
			
			// Change parents
			v->parent = g;
			p->parent = v;
			
			// Changes related to c
			if( c ) {
				c->parent = p;
				std::swap( c->dsep_child, c->isep_child );
			}
			
			// Change separator information for children of v and g
			if( g ) { // p was not root
				Node* old_p_dsep_child = p->dsep_child;
				
				// Change isep_child of p (stays null if p is the root)
				if( old_p_dsep_child ) {
					p->isep_child = old_p_dsep_child;
				}
				
				// p cannot be a separator of g (otherwise rotation wouldn't be valid)
				
				// We know that p separates v and g
				v->dsep_child = p;
			}
			else { // p was root
				v->dsep_child = nullptr;
			}
			
			// Change dsep child of p
			p->dsep_child = c;
			
			return NOSEP;
		}
		
		inline NodeSepType rotate_type_hint( const NodeSepType type ) {
			if( type == DSEP ) {
				return rotate_dsep();
			}
			else if( type == ISEP ) {
				return rotate_isep();
			}
			else {
				return rotate_nosep();
			}
		}
#else
		void rotate() {
			num_rotations++;
			
			assert( this->parent != nullptr );
			assert( this->is_separator() || !this->parent->is_separator() );
			
			Node* v = this;
			Node* p = v->parent;
			Node* g = p->parent;
			Node* c = v->dsep_child;
			
			// Change parents
			v->parent = g;
			p->parent = v;
			if( c ) { c->parent = p; }
			
			// Change separator information for children of gp
			bool p_was_sep = false;
			if( g ) {
				if( p == g->dsep_child ) {
					p_was_sep = true;
					g->dsep_child = v;
				}
				else if( p == g->isep_child ) {
					p_was_sep = true;
					g->isep_child = v;
				}
			}
			
			// Change separator information for children of p
			Node* old_p_dsep_child = p->dsep_child;
			p->dsep_child = c;
			if( old_p_dsep_child && old_p_dsep_child != v ) {
				p->isep_child = old_p_dsep_child;
			}
			else if( p->isep_child == v ) {
				p->isep_child = nullptr;
			}
			
			// Change separator information for children of v
			if( g ) { // p was not root
				if( old_p_dsep_child != v ) {
					// p separates v and g
					v->dsep_child = p;
				}
				else {
					// v separates p and g
					v->dsep_child = v->isep_child;
					if( p_was_sep ) {
						v->isep_child = p;
					}
					else {
						v->isep_child = nullptr;
					}
				}
			}
			else { // p was root
				v->dsep_child = nullptr;
			}
			
			// Change separator information for children of c (not affected by the rotation otherwise)
			if( c ) {
				std::swap( c->dsep_child, c->isep_child );
			}
		}
#endif
		
		bool is_separator_hint( Node* p ) {
			return p->dsep_child == this || p->isep_child == this;
		}
		
		bool is_separator() {
			return this->parent && this->is_separator_hint( this->parent );
		}
		
		Node* get_stt_root() {
			Node* v = this;
			while( v->parent ) {
				v = v->parent;
			}
			return v;
		}
	};
	
	class STF {
	public :
		explicit STF( size_t n ) : nodes( n ) {}
		
		inline Node* get_node( size_t idx ) { return &nodes[idx]; }
		
		[[nodiscard]] inline size_t num_nodes() const { return nodes.size(); }

#if VARIANT >= 10
	// Splay-related stuff
	static inline void splay_step( Node* v ) {
		Node* p = v->parent;
		if( p->dsep_child == v ) {
			v->rotate();
		}
		else if( p->parent ) {
			p->rotate();
		}
		v->rotate();
	}
	
	static inline void splay_step_full( Node* v, Node* p ) {
		if( p->dsep_child == v ) {
			v->rotate();
		}
		else {
			p->rotate();
		}
		v->rotate();
	}
	
#ifdef ROT_NST
	static inline void splay_step_type_hint( Node* v, const NodeSepType v_type, Node* p, const NodeSepType p_type ) {
		if( v_type == DSEP ) {
			v->rotate_dsep();
			v->rotate_type_hint( p_type );
		}
		else {
			p->rotate_type_hint( p_type );
			v->rotate();
		}
	}
#endif
	
	static inline bool can_splay_step( Node* v ) {
		Node* p = v->parent;
		assert( p );
		Node* g = p->parent;
		return !g || !g->is_separator() || ( v->is_separator_hint( p ) && p->is_separator_hint( g ) );
	}
#endif
		
#if VARIANT == 1
		inline void access( Node* v ) {
			while( Node* p = v->parent ) {
				if( !v->is_separator_hint( p ) ) {
					// Rotate at p as long as p is a separator
					if( Node* g = p->parent ) {
						bool is_p_sep = p->is_separator_hint( g );
						while( is_p_sep ) {
							is_p_sep = p->rotate();
						}
					}
				}
				// Now either v is a separator, or p is not, meaning we are allowed to rotate at v.
				v->rotate();
			}
		}
#elif VARIANT == 2
		inline void access( Node* v ) {
			Node* p = v->parent;
			
			if( p == nullptr ) {
				return;
			}
			
			bool is_v_sep = v->is_separator_hint( p );
			while( true ) {
				if( !is_v_sep ) {
					// Rotate at p as long as p is a separator
					if( Node* g = p->parent ) {
						bool is_p_sep = p->is_separator_hint( g );
						while( is_p_sep ) {
							is_p_sep = p->rotate();
						}
					}
				}
				// Now either v is a separator, or p is not, meaning we are allowed to rotate at v.
				is_v_sep = v->rotate();
				// Now v may not be a separator anymore
				p = v->parent;
				if( p == nullptr ) {
					return;
				}
			}
		}
#elif VARIANT == 3
		static inline void access( Node* v ) {
			bool is_v_sep = v->is_separator();
			while( Node* p = v->parent ) {
				if( !is_v_sep ) {
					// Rotate at p as long as p is a separator
					if( Node* g = p->parent ) {
						bool is_p_sep = p->is_separator_hint( g );
						while( is_p_sep ) {
							is_p_sep = p->rotate();
						}
					}
				}
				// Now either v is a separator, or p is not, meaning we are allowed to rotate at v.
				is_v_sep = v->rotate();
			}
		}
#elif VARIANT == 4
		static inline void access( Node* v ) {
			NodeSepType v_sep_type = v->get_sep_type();
			while( Node* p = v->parent ) {
				if( v_sep_type == NOSEP ) {
					// Rotate at p as long as p is a separator
					auto p_sep_type = p->get_sep_type();
					while( p_sep_type != NOSEP ) {
						if( p_sep_type == DSEP ) {
							p_sep_type = p->rotate();
						}
						else { // p_sep_type == ISEP
							assert( p_sep_type == ISEP );
							p_sep_type = p->rotate();
						}
						assert( p_sep_type == p->get_sep_type() );
					}
					assert( !p->is_separator() );
					
					// Now both v and p are NOSEP
					v_sep_type = v->rotate();
				}
				else if( v_sep_type == DSEP ) {
					v_sep_type = v->rotate();
				}
				else {
					assert( v_sep_type == ISEP );
					v_sep_type = v->rotate();
				}
			}
		}
#elif VARIANT == 5
		static inline void access( Node* v ) {
			NodeSepType v_sep_type = v->get_sep_type();
			while( Node* p = v->parent ) {
				if( v_sep_type == NOSEP ) {
					// Rotate at p as long as p is a separator
					auto p_sep_type = p->get_sep_type();
					while( p_sep_type != NOSEP ) {
						if( p_sep_type == DSEP ) {
							p_sep_type = p->rotate_dsep();
						}
						else { // p_sep_type == ISEP
							assert( p_sep_type == ISEP );
							p_sep_type = p->rotate_isep();
						}
						assert( p_sep_type == p->get_sep_type() );
					}
					assert( !p->is_separator() );
					
					// Now both v and p are NOSEP
					v_sep_type = v->rotate_nosep();
					// Note: from here on, v_sep_type can never become anything else than nosep
				}
				else if( v_sep_type == DSEP ) {
					v_sep_type = v->rotate_dsep();
				}
				else {
					assert( v_sep_type == ISEP );
					v_sep_type = v->rotate_isep();
				}
			}
		}
#elif VARIANT == 6
		static inline void access( Node* v ) {
			NodeSepType v_sep_type = v->get_sep_type();
			while( v_sep_type != NOSEP ) {
				if( v_sep_type == DSEP ) {
					v_sep_type = v->rotate_dsep();
				}
				else {
					assert( v_sep_type == ISEP );
					v_sep_type = v->rotate_isep();
				}
			}
			
			while( Node* p = v->parent) {
				assert( !v->is_separator() );
				
				// Rotate at p as long as p is a separator
				auto p_sep_type = p->get_sep_type();
				while( p_sep_type != NOSEP ) {
					if( p_sep_type == DSEP ) {
						p_sep_type = p->rotate_dsep();
					}
					else { // p_sep_type == ISEP
						assert( p_sep_type == ISEP );
						p_sep_type = p->rotate_isep();
					}
					assert( p_sep_type == p->get_sep_type() );
				}
				assert( !p->is_separator() );
				
				// Now both v and p are NOSEP
				v->rotate_nosep();
			}
		}
#elif VARIANT == 10
		// Very naive Greedy impl from paper
		static inline void access( Node* v ) {
			while( v->parent ) {
				if( can_splay_step( v ) ) {
					splay_step( v );
				}
				else if( can_splay_step( v->parent ) ) {
					splay_step( v->parent );
				}
				else {
					assert( can_splay_step( v->parent->parent ) );
					splay_step( v->parent->parent );
				}
			}
		}
#elif VARIANT == 11
		// Naive Greedy impl from Rust lib
		static inline void access( Node* v ) {
			while( Node* p = v->parent ) {
				if( Node* g = p->parent ) {
					if( Node* gg = g->parent ) {
						bool v_sep = v->is_separator_hint( p );
						bool p_sep = p->is_separator_hint( g );
						bool g_sep = g->is_separator_hint( gg );
						if( ( v_sep && p_sep ) || !g_sep ) { // Can splay at v
							splay_step_full( v, p );
						}
						else { // Cannot splay at v
							if( Node* ggg = gg->parent ) {
								bool gg_sep = gg->is_separator_hint( ggg );
								if( ( p_sep && g_sep ) || !gg_sep ) { // Can splay at p
									splay_step_full( p, g );
								}
								else { // Cannot splay at p, so splaying at g must be allowed
									splay_step_full( g, gg );
								}
							}
							else { // ggg is root, splaying at p must be allowed
								splay_step_full( p, g );
							}
						}
					}
					else { // g is root, splaying at v must be allowed
						splay_step_full( v, p );
					}
				}
				else { // p is root
					v->rotate();
				}
			}
		}
#elif VARIANT == 12
		// Improved Greedy impl from Rust lib
		static inline void access( Node* v ) {
			while( Node* p = v->parent ) {
				if( Node* g = p->parent ) {
					bool v_sep = v->is_separator_hint( p );
					bool p_sep = p->is_separator_hint( g );
					if( v_sep && p_sep ) { // Can splay at v
						splay_step_full( v, p );
					}
					else if( Node* gg = g->parent ) { // !v_sep or !p_sep
						bool g_sep = g->is_separator_hint( gg );
						if( !g_sep ) { // Can splay at v
							splay_step_full( v, p );
						}
						else if( p_sep ) { // g_sep and p_sep => can splay at p
							splay_step_full( p, g );
						}
						else { // Cannot splay at v and g_sep and !p_sep
							if( Node* ggg = gg->parent ) {
								bool gg_sep = gg->is_separator_hint( ggg );
								if( !gg_sep ) { // Can splay at p
									splay_step_full( p, g );
								}
								else { // Cannot splay at p, so splaying at g must be allowed
									splay_step_full( g, gg );
								}
							}
							else { // ggg is root, splaying at p must be allowed
								splay_step_full( p, g );
							}
						}
					}
					else { // g is root, splaying at v must be allowed
						splay_step_full( v, p );
					}
				}
				else { // p is root
					v->rotate();
				}
			}
		}
#elif VARIANT == 13
		// Improved Greedy impl from Rust lib, using NodeSepType
		static inline void access( Node* v ) {
			while( Node* p = v->parent ) {
				if( Node* g = p->parent ) {
					NodeSepType v_sep = v->get_sep_type_hint( p );
					NodeSepType p_sep = p->get_sep_type_hint( g );
					
					// Try splaying at v without information about g's NodeSepType.
					if( v_sep != NOSEP && p_sep != NOSEP ) {
						splay_step_type_hint( v, v_sep, p, p_sep );
					}
					// Either v or p is not a separator
					else if( Node* gg = g->parent ) {
						NodeSepType g_sep = g->get_sep_type_hint( gg );
						if( g_sep == NOSEP ) { // Can splay at v
							splay_step_type_hint( v, v_sep, p, p_sep );
						}
						else if( p_sep != NOSEP ) { // g_sep and p_sep => can splay at p
							splay_step_type_hint( p, p_sep, g, g_sep );
						}
						else { // Cannot splay at v and g_sep and !p_sep
							Node* ggg = gg->parent; // Must exist, since g_sep
							assert( gg->parent );
							NodeSepType gg_sep = gg->get_sep_type_hint( ggg );
							if( gg_sep == NOSEP ) { // Can splay at p
								splay_step_type_hint( p, p_sep, g, g_sep );
							}
							else { // Cannot splay at p, so splaying at g must be allowed
								splay_step_type_hint( g, g_sep, gg, gg_sep );
							}
						}
					}
					else { // g is root, splaying at v must be allowed
						splay_step_type_hint( v, v_sep, p, p_sep );
					}
				}
				else { // p is root
					v->rotate();
				}
			}
		}
#else
		// Naive MTR impl
		static inline void access( Node* v ) {
			while( Node* p = v->parent ) {
				if( !v->is_separator_hint( p ) ) {
					while( Node* g = p->parent ) {
						if( p->is_separator_hint( g ) ) {
							p->rotate();
//							std::cout << *this << "\n";
							continue;
						}
						break;
					}
				}
				// Now either v is a separator, or p is not, meaning we are allowed to rotate at v.
				v->rotate();
//				std::cout << *this << "\n";
			}
		}
#endif
		
		void link( size_t u_idx, size_t v_idx ) {
			Node* u = get_node( u_idx );
			Node* v = get_node( v_idx );
			access( u );
			access( v );
			u->attach( v );
		}
		
		void cut( size_t u_idx, size_t v_idx ) {
			Node* u = get_node( u_idx );
			Node* v = get_node( v_idx );
			access( u );
			access( v );
			u->detach();
		}
		
		bool is_connected( size_t u_idx, size_t v_idx ) {
			Node* u = get_node( u_idx );
			Node* v = get_node( v_idx );
			access( u );
			access( v );
			return u->get_stt_root() == v;
		}
		
		friend std::ostream& operator<<( std::ostream& os, stt::STF& f );
		
	private :
		std::vector<Node> nodes;
	};

	void _write_tree( std::ostream& os, STF& f, size_t v_idx, const std::vector<std::vector<size_t>>& node_children, const std::string& indent = "" ) {
		if( indent.length() >= 1000 ) {
			std::cerr << "Refusing to write tree of depth >= 1000\n";
			exit( -1 );
		}
		auto v = f.get_node( v_idx );
		os << indent << v_idx;
		// os << "[" << v << "]";
		if( v->parent && v->parent->dsep_child == v ) { os << "d"; }
		else if( v->parent && v->parent->isep_child == v ) { os << "i"; }
		os << "\n";
		for( const size_t c_idx : node_children[v_idx] ) {
			_write_tree( os, f, c_idx, node_children, indent + "  " );
		}
	}

	std::ostream& operator<<( std::ostream& os, STF& f ) {
		std::unordered_map<Node*, size_t> node_indices;
		for( size_t i = 0; i < f.num_nodes(); i++ ) {
			node_indices[f.get_node( i )] = i;
		}
		std::vector<std::vector<size_t>> node_children( f.num_nodes() );
		for( size_t i = 0; i < f.num_nodes(); i++ ) {
			Node* v = f.get_node( i );
			if( v->parent ) {
				node_children[node_indices[v->parent]].push_back( i );
			}
		}
		for( size_t i = 0; i < f.num_nodes(); i++ ) {
			if( f.get_node( i )->parent == nullptr ) {
				_write_tree( os, f, i, node_children );
			}
		}
		return os;
	}
}

void test() {
	std::cout << "Starting test" << std::endl;
	stt::STF f( 10 );
	f.link( 1, 2 );
	f.link( 2, 3 );
	f.link( 3, 4 );
	f.link( 0, 2 );
//	std::cout << f << std::endl;
//	f.get_node( 0 )->access();
//	std::cout << f << std::endl;
	assert( f.is_connected( 0, 2 ) );
	assert( f.is_connected( 0, 1 ) );
	assert( f.is_connected( 0, 3 ) );
	assert( f.is_connected( 0, 4 ) );
	assert( !f.is_connected( 0, 5 ) );
//	std::cout << f << std::endl;
	std::cout << "----- CUT(1,2) -----\n";
	f.cut( 1, 2 );
	std::cout << f << std::endl;
	assert( f.is_connected( 0, 2 ) );
	assert( !f.is_connected( 0, 1 ) );
	assert( f.is_connected( 0, 3 ) );
	assert( f.is_connected( 0, 4 ) );
	assert( !f.is_connected( 0, 5 ) );
	std::cout << "Done.\n";
}

int main( int argc, const char** argv ) {
	main_connectivity<stt::STF>( argc, argv );
}
