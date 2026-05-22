#ifndef AVLNODE_H
#define AVLNODE_H

using namespace std;

// Forward declaration
class MovieNode;

class AVLNode {
public:
    MovieNode* movie;
    AVLNode* left;
    AVLNode* right;
    int height;
    
    // Constructor
    AVLNode(MovieNode* m);
};

#endif