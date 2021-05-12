#include <iostream>
#include <fstream>
#include <vector>

#define WHITE -1
#define BLACK -2

void addArc(std::vector<std::pair<int, int>>  *adj, int u, int v, int weight) {
    adj[u].push_back(std::make_pair(weight, v));
}

void swap(int* Heap, int* InHeap, int i, int j) {
    int temp = Heap[i];
    Heap[i] = Heap[j];
    Heap[j] = temp;
    InHeap[Heap[i]] = i;
    InHeap[Heap[j]] = j;
}

void setItem(int* Heap, int* InHeap, int u, int i) {
    Heap[i] = u;
    InHeap[u] = i;
}

bool less(int* D, int* Heap, int i, int j) {
    return D[Heap[i]] < D[Heap[j]];
}

void goUp(int* D, int* Heap, int* InHeap, int i) {
    if (i < 2)
        return;
    int j = i / 2;
    while (j > 0 && less(D, Heap, i, j)) {
        swap(Heap, InHeap, i, j);
        i = j;
        j = i / 2;
    }
}

void goDown(int* D, int* Heap, int* InHeap, int HeapCnt, int i) {
    while (2 * i <= HeapCnt) {
        int minIdx = i;
        if (less(D, Heap, 2 * i, minIdx)) {
            minIdx = 2 * i;
        }
        if ((2 * i + 1) <= HeapCnt && less(D, Heap, 2 * i + 1, minIdx)) {
            minIdx = 2 * i + 1;
        }
        if (minIdx == 1) {
            break;
        }
        swap(Heap, InHeap, i, minIdx);
    }
}

void relax(int* D, int* Heap, int* InHeap, int &HeapCnt,int* Parent, int u, int v, int weight) {
    if (D[v] <= D[u] + weight)
        return;
    D[v] = D[u] + weight;
    Parent[v] = u;
    if (InHeap[v] == WHITE) {
        HeapCnt++;
        setItem(Heap, InHeap, v, HeapCnt);
    }
    goUp(D, Heap, InHeap, InHeap[v]);
}

int getMinimal(int* D, int* Heap, int* InHeap, int &HeapCnt){
    int result = Heap[1];
    swap(Heap, InHeap, 1, HeapCnt);
    HeapCnt--;
    InHeap[result] = BLACK;
    goDown(D, Heap, InHeap, HeapCnt, 1);
    return result;
}

void dijkstra(std::vector<std::pair<int, int>>  *adj, int n, int s, int INFINITE, int* D, int* Parent) {
    int* Heap = new int[n + 1];
    int *InHeap = new int[n];
    for (int i = 0; i < n; i++) {
        InHeap[i] = WHITE;
        D[i] = INFINITE;
    }
    D[s] = 0;
    int HeapCnt = 1;
    setItem(Heap, InHeap, s, 1);
    while(HeapCnt > 0) {
        int u = getMinimal(D, Heap, InHeap, HeapCnt);
        for (int i = 0; i < adj[u].size(); i++) {
            int v = adj[u][i].second;
            int weight = adj[u][i].first;
            relax(D, Heap, InHeap, HeapCnt, Parent, u, v, weight);
        }
    }
}
void printPath(int* Parent, int s, int t) {
    if (s != t) {
        printPath(Parent, s, Parent[t]);
    }
    std::cout << (char) (t + 'a') << ' ';
}
void reconstructAllPaths(std::vector<std::pair<int, int>>  *adj, int* D, int* Path,
                         int s, int t, int i) {
    Path[i] = t;
    if (s == t) {
        for (int j = i; j >= 0; j--) {
            std::cout << (char)(Path[j] + 'a') << ' ';
        }
        std::cout << std::endl;
        return;
    }
    for (int j = 0; j < adj[t].size(); j++) {
        int v = adj[t][j].second;
        int w = adj[t][j].first;
        if (D[t] == (D[v] + w)) {
            reconstructAllPaths(adj, D, Path, s, v, i + 1);
        }
    }
}


int main() {
    std::ifstream input("input.txt");
    if (!input) {
        exit(1);
    }
    int n, m;
    char S;
    input  >> n >> m >> S;
    int s = S - 'a';
    auto *adj = new std::vector<std::pair<int, int>>[n];
    for (int i = 0; i < m; i++) {
        char U, V;
        int w;
        input >> U >> V >> w;
        int u = U - 'a';
        int v = V - 'a';
        addArc(adj, u, v, w);
        addArc(adj, v, u, w);
    }
    input.close();

    int infinite = INT32_MAX / 2 - 1;
    int *D = new int[n];
    int *Parent = new int[n];
    dijkstra(adj, n, s, infinite, D, Parent);
    for (int i = 0; i < n; i++) {
        if ((D[i] != 0) && (D[i] < infinite)) {
            std::cout << (char)(s + 'a') << '-' << (char)(i + 'a') << '(' << D[i] << "): ";
            printPath(Parent, s, i);
            std::cout << std::endl;
        }
    }
    int *Path = new int[n];
    std::cout << "\nAll Paths:\n";
    for (int i = 0; i < n; i++) {
        if ((D[i] != 0) && (D[i] < infinite)) {
            std::cout << (char)(s + 'a') << '-' << (char)(i + 'a') << '(' << D[i] << "):\n";
            reconstructAllPaths(adj, D, Path, s, i, 0);
        }
    }

    return 0;
}
