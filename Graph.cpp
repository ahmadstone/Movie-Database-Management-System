#include "Graph.h"
#include <iostream>
using namespace std;

Graph::Graph(int vertices) {
    V = vertices;
    adj = new LinkedList[V];
}

//undirected graph
void Graph::addEdge(int src, int dest) {
    if (src >= V || dest >= V) return;

    adj[src].insert(dest);
    adj[dest].insert(src);
}

void Graph::display() {
    for (int i = 0; i < V; i++) {
        cout << "Movie " << i << ": ";

        ListNode* temp = adj[i].head;
        while (temp != nullptr) {
            cout << temp->vertex << " ";
            temp = temp->next;
        }
        cout << endl;
    }
}

//BFS (uses queue)
void Graph::BFS(int start) {
    if (start >= V) return;

    int* visited = new int[V];
    for (int i = 0; i < V; i++) visited[i] = 0;

    int* queue = new int[V];
    int front = 0, rear = 0;

    visited[start] = 1;
    queue[rear++] = start;

    cout << "BFS: ";

    while (front < rear) {
        int movie = queue[front++];

        //printing movie titles
        cout << movieArray[movie]->movie_title << "  |  ";

        ListNode* temp = adj[movie].head;
        while (temp != nullptr) {
            if (!visited[temp->vertex]) {
                visited[temp->vertex] = 1;
                queue[rear++] = temp->vertex;
            }
            temp = temp->next;
        }
    }
    cout << endl;

    delete[] visited;
    delete[] queue;
}


//DFS helper
void Graph::DFSUtil(int v, int visited[]) {
    visited[v] = 1;

	//printing movie titles
    cout << movieArray[v]->movie_title << "  |  ";

    ListNode* temp = adj[v].head;
    while (temp != nullptr) {
        if (!visited[temp->vertex]) {
            DFSUtil(temp->vertex, visited);
        }
        temp = temp->next;
    }
}

//DFS
void Graph::DFS(int start) {
    if (start >= V) return;

    int* visited = new int[V];
    for (int i = 0; i < V; i++) visited[i] = 0;

    cout << "DFS: ";
    DFSUtil(start, visited);
    cout << endl;

    delete[] visited;
}

void Graph::BFS_ShortestPath(int start, int end) {
    if (start >= V || end >= V) {
        cout << "Invalid movie indices.\n";
        return;
    }

    int* visited = new int[V];
    int* parent = new int[V];  // store path
    for (int i = 0; i < V; i++) {
        visited[i] = 0;
        parent[i] = -1;
    }

    int* queue = new int[V];
    int front = 0, rear = 0;

    visited[start] = 1;
    queue[rear++] = start;

    bool found = false;

    while (front < rear) {
        int u = queue[front++];
        if (u == end) {
            found = true;
            break;
        }

        ListNode* temp = adj[u].head;
        while (temp != nullptr) {
            int v = temp->vertex;
            if (!visited[v]) {
                visited[v] = 1;
                parent[v] = u;
                queue[rear++] = v;
            }
            temp = temp->next;
        }
    }

    if (!found) {
        cout << "No connection between movies found.\n";
    }
    else {
        // reconstruct path
        int* path=new int [V];
        int count = 0;
        int crawl = end;
        while (crawl != -1) {
            path[count++] = crawl;
            crawl = parent[crawl];
        }

        cout << "Shortest path (" << count << " movies):\n";
        for (int i = count - 1; i >= 0; i--) {
            cout << movieArray[path[i]]->movie_title;
            if (i != 0) cout << "  ->  ";
        }
        cout << endl;
    }

    delete[] visited;
    delete[] parent;
    delete[] queue;
}

void Graph::recommendMovies(int start, int maxRecommendations) {
    if (start >= V) {
        cout << "Invalid movie index.\n";
        return;
    }

    int* visited = new int[V];
    for (int i = 0; i < V; i++) visited[i] = 0;

    int* queue = new int[V];
    int front = 0, rear = 0;
    int* recommendations = new int[maxRecommendations];
    int recCount = 0;

    visited[start] = 1;
    queue[rear++] = start;

    cout << "\n=== Movie Recommendations (based on matching genre or actors) ===\n";
    cout << "Starting from: " << movieArray[start]->movie_title << "\n\n";

    while (front < rear && recCount < maxRecommendations) {
        int movie = queue[front++];

        // Skip the starting movie itself
        if (movie != start) {
            recommendations[recCount++] = movie;
        }

        ListNode* temp = adj[movie].head;
        while (temp != nullptr && recCount < maxRecommendations) {
            if (!visited[temp->vertex]) {
                visited[temp->vertex] = 1;
                queue[rear++] = temp->vertex;
            }
            temp = temp->next;
        }
    }

    if (recCount == 0) {
        cout << "No recommendations found.\n";
    } else {
        cout << "Found " << recCount << " recommendations:\n\n";
        for (int i = 0; i < recCount; i++) {
            cout << (i + 1) << ". ";
            movieArray[recommendations[i]]->display();
            cout << endl;
        }
    }

    delete[] visited;
    delete[] queue;
    delete[] recommendations;
}

