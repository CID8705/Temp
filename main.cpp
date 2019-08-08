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
class Info {
public:
	std::string id;
	int dist;
};
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
				const std::string tmp = itr->r_in.substr(0, itr->r_in.size() - 2);
				return std::find_if(this->rs.cbegin(), this->rs.cend(), [&](const R &r) {
					return r.id == tmp;
				})->length;
			}
		}
		return INT_MAX;
	}
	void make_graph() {
		const std::vector<std::string> all = get_all(); 
		for (int i = 0; i < all.size(); ++i) {
			for (int j = 0; j < all.size(); ++j) {
				if (i == j) {
					dist.emplace(std::make_pair(all.at(i), all.at(j)), 0);
				}
				else {
					std::pair<std::string, std::string> key = std::make_pair(all.at(i), all.at(j));
					dist.emplace(key, this->get_length(key));
				}
			}
		}
		for(int k = 0; k < all.size(); ++k) {
			for(int i = 0; i < all.size(); ++i) {
				for(int j = 0; j < all.size(); ++j) {
					int dist_ik = dist.at(std::make_pair(all.at(i), all.at(k)));
					int dist_kj = dist.at(std::make_pair(all.at(k), all.at(j)));
					if (dist_ik < INT_MAX && dist_kj < INT_MAX) {
						dist.at(std::make_pair(all.at(i), all.at(j))) = std::min(dist.at(std::make_pair(all.at(i), all.at(j))), dist_ik + dist_kj);
					}
				}
			}
		}
	}
	std::vector<Info> get_nearest(const std::string &id, const int &flag) {
		std::vector<Info> nearest;
		if (id.front() == 'D') {
			const std::vector<D>::const_iterator x = find_if(this->ds.cbegin(), this->ds.cend(), [&](const D &d) {
				return d.id == id ? true : false;
			});
			const std::vector<C>::const_iterator c_in = find_if(cs.cbegin(), cs.cend(), [&](const C &c) {
				return c.r_in.substr(0, c.r_in.size() - 2) == x->r ? true : false;
			});
			const std::vector<C>::const_iterator c_out = find_if(cs.cbegin(), cs.cend(), [&](const C &c) {
				return c.r_out.substr(0, c.r_out.size() - 2) == x->r && c.id != c_in->id ? true : false;
			});
			Info info;
			info.id = c_in->id;
			if (c_in->r_in.substr(c_in->r_in.size() - 2) == "SE") {
				std::string tmp = c_in->r_in.substr(0, c_in->r_in.size() - 2);
				info.dist = find_if(rs.cbegin(), rs.cend(), [&](const R &r) {
					return r.id == tmp ? true : false;
				})->length - x->dist;
			}
			else {
				info.dist = x->dist;
			}
			nearest.emplace_back(info);
			info.id = c_out->id;
			if (c_out->r_out.substr(c_out->r_out.size() - 2) == "SE") {
				info.dist = x->dist;
			}
			else {
				std::string tmp = c_out->r_out.substr(0, c_out->r_out.size() - 2);
				info.dist = find_if(rs.cbegin(), rs.cend(), [&](const R &r) {
					return r.id == tmp ? true : false;
				})->length - x->dist;
			}
			nearest.emplace_back(info);
		}
		else {
			const std::vector<P>::const_iterator x = find_if(this->ps.cbegin(), this->ps.cend(), [&](const P &p) {
				return p.id == id ? true : false;
			});
			if (flag <= 0) {
				Info info;
				const std::vector<C>::const_iterator c_in = find_if(cs.cbegin(), cs.cend(), [&](const C &c) {
					return c.r_in == x->r + x->id.substr(x->id.size() - 2) ? true : false;
				});
				info.id = c_in->id;
				if (c_in->r_in.substr(c_in->r_in.size() - 2) == "SE") {
					std::string tmp = c_in->r_in.substr(0, c_in->r_in.size() - 2);
					info.dist = find_if(rs.cbegin(), rs.cend(), [&](const R &r) {
						return r.id == tmp ? true : false;
					})->length - x->dist;
				}
				else {
					info.dist = x->dist;
				}
				nearest.emplace_back(info);
			}
			if (flag >= 0) {
				Info info;
				const std::vector<C>::const_iterator c_out = find_if(cs.cbegin(), cs.cend(), [&](const C &c) {
					return c.r_out == x->r + x->id.substr(x->id.size() - 2) ? true : false;
				});
				info.id = c_out->id;
				if (c_out->r_out.substr(c_out->r_out.size() - 2) == "SE") {
					info.dist = x->dist;
				}
				else {
					std::string tmp = c_out->r_out.substr(0, c_out->r_out.size() - 2);
					info.dist = find_if(rs.cbegin(), rs.cend(), [&](const R &r) {
						return r.id == tmp ? true : false;
					})->length - x->dist;
				}
				nearest.emplace_back(info);
			}
		}
		return nearest;
	}
	int search(const Q &q) {
		int min = INT_MAX;
		const std::vector<Info> ss = this->get_nearest(q.x, q.type ? 0 : -1);
		const std::vector<Info> es = this->get_nearest(q.y, q.type ? 0 : 1);
		for (const Info &s : ss) {
			for (const Info &e : es) {
				int tmp = s.dist + this->dist.at(std::make_pair(s.id, e.id)) + e.dist;
				std::cout << s.id << " + " << this->dist.at(std::make_pair(s.id, e.id)) << " + " << e.id << std::endl;
				if (tmp < min) {
					min = tmp;
				}
			}
		}
		return min < INT_MAX ? min : -1;
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
