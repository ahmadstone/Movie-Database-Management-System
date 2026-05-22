#ifndef GRAPH_H
#define GRAPH_H
#include "MovieNode.h"
using namespace std;

struct ListNode {
    int vertex;
    ListNode* next;

    ListNode(int v) {
        vertex = v;
        next = nullptr;
    }
};

class LinkedList {
public:
    ListNode* head;

    LinkedList() {
        head = nullptr;
    }

    void insert(int v) {
        ListNode* newNode = new ListNode(v);
        newNode->next = head;
        head = newNode;
    }
};

class Graph {
private:
    int V;                    //number of vertices
    LinkedList* adj;          //adjacency list array
	MovieNode** movieArray;   //array of movie nodes


    void DFSUtil(int v, int visited[]);

public:
    Graph(int vertices);
    void addEdge(int src, int dest);
    void display();

    void BFS(int start);
    void DFS(int start);
    void BFS_ShortestPath(int start, int end);
    void recommendMovies(int start, int maxRecommendations = 20);

    void setMovieArray(MovieNode** arr) {
        movieArray = arr;
    }
};

#endif
