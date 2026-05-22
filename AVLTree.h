#ifndef AVLTREE_H
#define AVLTREE_H

#include "AVLNode.h"
#include "MovieNode.h"
using namespace std;

class AVLTree {
private:
    AVLNode* root;
    
    // Helper functions
    int getHeight(AVLNode* node);
    int getBalance(AVLNode* node);
    AVLNode* rightRotate(AVLNode* y);
    AVLNode* leftRotate(AVLNode* x);
    AVLNode* insert(AVLNode* node, MovieNode* movie);
    void inorder(AVLNode* node);
    void searchByScoreHelper(AVLNode* node, float minScore, MovieNode** results, int& count, int maxSize);
    int countByScore(AVLNode* node, float minScore);
    void destroyTree(AVLNode* node);
    
    // Helper for max function
    int max(int a, int b);
    
public:
    // Constructor and Destructor
    AVLTree();
    ~AVLTree();
    
    // Public interface
    void insert(MovieNode* movie);
    void displaySorted();
    int searchByScore(float minScore, MovieNode** results, int maxResults);
    int countByScore(float minScore);
    int getTreeHeight();
    bool compareMovies(MovieNode* a, MovieNode* b);
};

#endif




