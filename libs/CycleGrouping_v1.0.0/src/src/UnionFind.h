#ifndef _UNIONFIND_H_
#define _UNIONFIND_H_

#include <vector>

using namespace std;

class UnionFind{
public:
	vector<int> parent;
	vector<int> rank;

	UnionFind(int nEle);
	int Find(int x);
	int Union(int x, int y);

	UnionFind(const UnionFind &_uf);
	UnionFind & operator= (const UnionFind &_uf);
};

#endif  //_UNIONFIND_H_
