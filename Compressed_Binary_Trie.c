#include <stdio.h>
#include <stdlib.h>

#define BITS 8

typedef struct CTrieNode {
    int key;
    int bit;
    struct CTrieNode *left;
    struct CTrieNode *right;
} CTrieNode;

/* Create node */
CTrieNode* createCNode(int key, int bit) {
    CTrieNode *node = (CTrieNode*)malloc(sizeof(CTrieNode));
    node->key = key;
    node->bit = bit;
    node->left = node->right = NULL;
    return node;
}

/* Get bit */
int getBit(int key, int bit) {
    return (key >> bit) & 1;
}

/* Insert */
CTrieNode* insert(CTrieNode *root, int key) {
    if (!root)
        return createCNode(key, -1);

    CTrieNode *curr = root;
    CTrieNode *parent = NULL;

    while (curr->bit > (parent ? parent->bit : -1)) {
        parent = curr;
        curr = getBit(key, curr->bit) ? curr->right : curr->left;
    }

    if (curr->key == key)
        return root;

    int i;
    for (i = BITS - 1; i >= 0; i--) {
        if (getBit(curr->key, i) != getBit(key, i))
            break;
    }

    CTrieNode *newNode = createCNode(key, i);
    int bit = getBit(key, i);

    newNode->left = bit ? curr : newNode;
    newNode->right = bit ? newNode : curr;

    if (!parent)
        return newNode;

    if (getBit(key, parent->bit))
        parent->right = newNode;
    else
        parent->left = newNode;

    return root;
}

/* Search */
int search(CTrieNode *root, int key) {
    if (!root) return 0;
    CTrieNode *curr = root;
    CTrieNode *parent = NULL;

    while (curr->bit > (parent ? parent->bit : -1)) {
        parent = curr;
        curr = getBit(key, curr->bit) ? curr->right : curr->left;
    }
    return curr->key == key;
}

/* Delete (simplified, exam-ready) */
CTrieNode* deleteKey(CTrieNode *root, int key) {
    if (!root) return NULL;

    CTrieNode *curr = root, *parent = NULL, *grand = NULL;

    while (curr->bit > (parent ? parent->bit : -1)) {
        grand = parent;
        parent = curr;
        curr = getBit(key, curr->bit) ? curr->right : curr->left;
    }

    if (curr->key != key) return root;

    CTrieNode *child = (parent->left == curr) ? parent->right : parent->left;

    if (!grand)
        root = child;
    else if (grand->left == parent)
        grand->left = child;
    else
        grand->right = child;

    free(curr);
    free(parent);
    return root;
}

/* Display */
void display(CTrieNode *node) {
    if (!node) return;
    if (node->left == node && node->right == node)
        printf("%d ", node->key);
    if (node->left != node)
        display(node->left);
    if (node->right != node)
        display(node->right);
}
