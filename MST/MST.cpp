#include <iostream>
#include <cstring>
#include <vector>
#include <algorithm>

using namespace std;

// The structure to hold vertex's pair for the edge
typedef pair<int, int> vertexPair;

class ISet {
public:
	virtual ~ISet() = default;
	virtual int find(int u) = 0;
	virtual void merge(int x, int y) = 0;
};

class NamedArray : public ISet {
public:
	NamedArray(int n)
	{
		// Allocate memory 
		this->n = n;
		Name = new int[n + 1];

		for (int i = 0; i <= n; i++)
		{
			//every element is parent of itself and size is 0
			Name[i] = i;
		}
	}

	~NamedArray() {
		delete Name;
	}

	// Find the parent of a node 'u' 
	int find(int u) {
		return Name[u];
	}

	void merge(int x, int y)
	{
		x = find(x), y = find(y);
		// Assign all the names with y to x to merge
		for (int i = 0; i< n; i++) {
			if (Name[i] == y)
				Name[i] = x;
		}
	}

private:
	int *Name;
	int n;
};

class InvertedForrest : public ISet {
public:
	InvertedForrest(int n)
	{
		// Allocate memory 
		this->n = n;
		parent = new int[n + 1];
		size = new int[n + 1];

		for (int i = 0; i <= n; i++)
		{
			//every element is parent of itself and size is 0
			parent[i] = i;
			size[i] = 1;
		}
	}

	~InvertedForrest() {
		delete parent;
		delete size;
	}

	// Find the parent of a node 'u' 
	int find(int u) {
		int y = parent[u];
		while (y != parent[y])
			y = parent[y];
		return y;
	}

	void merge(int x, int y)
	{
		x = find(x), y = find(y);

		/* Make tree with smaller height a subtree of the other tree */
		if (size[x] >= size[y]) {
			parent[y] = x;
			size[x] = size[x] + size[y];
			size[y] = 0;
		}
		else {
			parent[x] = y;
			size[y] = size[x] + size[y];
			size[x] = 0;
		}
	}
private:
	int *parent, *size;
	int n;

};


class PathCompress : public ISet {
public:
	PathCompress(int n)
	{
		// Allocate memory 
		this->n = n;
		parent = new int[n + 1];
		size = new int[n + 1];

		for (int i = 0; i <= n; i++)
		{
			size[i] = 0;
			//every element is parent of itself
			parent[i] = i;
		}
	}

	~PathCompress() {
		delete parent;
		delete size;
	}

	// Find the parent of a node 'u' along with perform Path Compression
	int find(int u)
	{
		if (u != parent[u])
			parent[u] = find(parent[u]);
		return parent[u];
	}

	// Union by rank
	void merge(int x, int y)
	{
		x = find(x), y = find(y);

		/* Make tree with smaller height a subtree of the other tree */
		if (size[x] > size[y])
			parent[y] = x;
		else // If rnk[x] <= rnk[y] 
			parent[x] = y;

		if (size[x] == size[y])
			size[y]++;
	}
private:
	int *parent, *size;
	int n;
};

// Structure to represent a graph
class Graph
{
public:
	Graph(int V, int E) {
		this->nVertex = V;
		this->nEdges = E;
	}

	~Graph() {
		delete ds;
	}

	// Utility function to add an edge
	void addEdge(int u, int v, int w)
	{
		edges.push_back({ w,{ u, v } });
	}

	// Method to find MST and its weight
	int MST(int op);

private:
	int nVertex, nEdges;
	vector< pair<int, vertexPair> > edges;
	ISet *ds = NULL;

};


int Graph::MST(int op)
{
	int mst_wt = 0; // Init

					// Sort edges in increasing order on basis of cost
	sort(edges.begin(), edges.end());

	// Create disjoint sets 
	switch (op) {
	case 1: {
		ds = new NamedArray(nVertex);
	}
			break;
	case 2: {
		ds = new InvertedForrest(nVertex);
	}
			break;
	case 3:
	default: {
		ds = new PathCompress(nVertex);
	}

			 break;
	}

	vector< pair<int, vertexPair> >::iterator it;
	for (it = edges.begin(); it != edges.end(); it++)
	{
		int u = it->second.first;
		int v = it->second.second;

		int set_u = ds->find(u);
		int set_v = ds->find(v);

		// Check if the selected edge is creating a cycle or not
		// It forms cycle if both are in same set
		if (set_u != set_v)
		{
			cout << u << " - " << v << endl;
			mst_wt += it->first;

			// Merge two sets
			ds->merge(set_u, set_v);
		}
	}

	return mst_wt;
}

int main()
{

	int option;

	cout << "Choose the Desired ADT to be used" << endl;
	cout << "1. Named Array " << endl;
	cout << "2. Inverted Forrest" << endl;
	cout << "3. Inverted Forrest with Path Compression" << endl;
	cout << "Please input your option:";
	cin >> option;

#if 1
	int V, E;
	int w, e1, e2;

	cout << "Input number of vertices : ";
	cin >> V;
	cout << "Input number of edges for the graph: ";
	cin >> E;

	Graph g(V, E);

	for (int i = 0; i< E; i++) {
		cout << "Enter weight of Edge" << i << ":";
		cin >> w;
		cout << "Enter its vertex1:";
		cin >> e1;
		cout << "Enter its vertex2:";
		cin >> e2;
		g.addEdge(w, e1, e2);
	}

#else
	// Test Sample
	int V = 9, E = 14;
	Graph g(V, E);
	g.addEdge(0, 1, 4);
	g.addEdge(0, 7, 8);
	g.addEdge(1, 2, 8);
	g.addEdge(1, 7, 11);
	g.addEdge(2, 3, 7);
	g.addEdge(2, 8, 2);
	g.addEdge(2, 5, 4);
	g.addEdge(3, 4, 9);
	g.addEdge(3, 5, 14);
	g.addEdge(4, 5, 10);
	g.addEdge(5, 6, 2);
	g.addEdge(6, 7, 1);
	g.addEdge(6, 8, 6);
	g.addEdge(7, 8, 7);
#endif

	cout << "Edges of MST are \n";
	int mst_wt = g.MST(option);

	cout << "\nWeight of MST is " << mst_wt;

	return 0;
}


