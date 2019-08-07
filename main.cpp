#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>
class R {
public:
	std::string id;
	int length;
	bool flag;
};
std::istream &operator>>(std::istream &in, R &r) {
	in >> r.id >> r.length >> r.flag;
	return in;
}
class C {
public:
	std::string id;
	std::string r_in;
	std::string r_out;
};
std::istream &operator>>(std::istream &in, C &c) {
	in >> c.id >> c.r_in >> c.r_out;
	return in;
}
class P {
public:
	std::string id;
	std::string r;
	int dist;
};
std::istream &operator>>(std::istream &in, P &p) {
	in >> p.id >> p.r >> p.dist;
	return in;
}
class D {
public:
	std::string id;
	std::string r;
	int dist;
};
std::istream &operator>>(std::istream &in, D &d) {
	in >> d.id >> d.r >> d.dist;
	return in;
}
class Q {
public:
	bool type;
	std::string x;
	std::string y;
};
std::istream &operator>>(std::istream &in, Q &q) {
	in >> q.type >> q.x >> q.y;
	return in;
}
class Search {
private:
	std::map<std::pair<std::string, std::string>, int> dist;
public:
	std::vector<R> rs;
	std::vector<C> cs;
	std::vector<P> ps;
	std::vector<D> ds;
	std::vector<Q> qs;
	std::vector<std::string> get_all() {
		std::vector<std::string> all;
		std::set<std::string> tmp;
		for (const C &c : this->cs) {
			tmp.emplace(c.id);
		}
		all.resize(tmp.size());
		std::copy(tmp.cbegin(), tmp.cend(), all.begin());
		return all;
	}
	int get_length(std::pair<std::string, std::string> key) {
		std::vector<C> cs_in, cs_out;
		std::copy_if(this->cs.cbegin(), this->cs.cend(), std::back_inserter(cs_in), [&](const C &c) {
			return c.id == key.first ? true : false;
		});
		std::copy_if(this->cs.cbegin(), this->cs.cend(), std::back_inserter(cs_out), [&](const C &c) {
			return c.id == key.second ? true : false;
		});
		for (const C &c_in : cs_in) {
			const std::vector<C>::const_iterator itr = find_if(cs_out.cbegin(), cs_out.cend(), [&](const C &c_out) {
				return c_in.r_out == c_out.r_in ? true : false;
			});
			if (itr != cs_out.cend()) {
				const std::string id = itr->r_in.substr(0, itr->r_in.size() - 2);
				return std::find_if(this->rs.cbegin(), this->rs.cend(), [&](const R &r) {
					return r.id == id;
				})->length;
			}
		}
		return INT_MAX;
	}
	void make_graph() {
		const std::vector<std::string> all = get_all(); 
		for (int i = 0; i < all.size(); ++i) {
			for (int j = 0; j < all.size(); ++j) {
				dist.emplace(std::make_pair(all.at(i), all.at(j)), INT_MAX);
			}
		}
		for (std::map<std::pair<std::string, std::string>, int>::iterator itr = this->dist.begin(); itr != this->dist.end(); ++itr) {
			itr->second = this->get_length(itr->first);
		}
		for (const auto &m : this->dist) {
			std::cout << m.first.first << "," << m.first.second << " = " << m.second << std::endl;
		}
	}
	int search(const Q &q) {
		return 0;
	}
};
std::istream &operator>>(std::istream &in, Search &s) {
	int alpha, beta, gamma, delta, eta;
	in >> alpha >> beta >> gamma >> delta >> eta;
	s.rs.resize(alpha);
	s.cs.resize(beta);
	s.ps.resize(gamma);
	s.ds.resize(delta);
	s.qs.resize(eta);
	for (int i = 0; i < s.rs.size(); ++i) {
		in >> s.rs.at(i);
	}
	for (int i = 0; i < s.cs.size(); ++i) {
		in >> s.cs.at(i);
	}
	for (int i = 0; i < s.ps.size(); ++i) {
		in >> s.ps.at(i);
	}
	for (int i = 0; i < s.ds.size(); ++i) {
		in >> s.ds.at(i);
	}
	for (int i = 0; i < s.qs.size(); ++i) {
		in >> s.qs.at(i);
	}
	return in;
}
int main() {
	Search s;
	std::cin >> s;
	s.make_graph();
	for (const Q &q : s.qs) {
		std::cout << s.search(q) << std::endl;
	}
	return EXIT_SUCCESS;
}
