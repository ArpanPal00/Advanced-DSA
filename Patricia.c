#include <stdio.h>
#include <stdlib.h>

#define MAX_BITS 32   // For 32-bit integers

typedef struct PatriciaNode {
    int key;
    int bit;
    struct PatriciaNode *left;
    struct PatriciaNode *right;
} PatriciaNode;

PatriciaNode *root = NULL;
int getBit(int key, int bit) {
    return (key >> (MAX_BITS - bit - 1)) & 1;
}

int firstDifferentBit(int k1, int k2) {
    for (int i = 0; i < MAX_BITS; i++) {
        if (getBit(k1, i) != getBit(k2, i))
            return i;
    }
    return -1;
}
PatriciaNode* createNode(int key, int bit) {
    PatriciaNode *node = (PatriciaNode*)malloc(sizeof(PatriciaNode));
    node->key = key;
    node->bit = bit;
    node->left = node;
    node->right = node;
    return node;
}
PatriciaNode* search(int key) {
    if (!root) return NULL;

    PatriciaNode *curr = root;
    PatriciaNode *prev = NULL;

    while (curr->bit > (prev ? prev->bit : -1)) {
        prev = curr;
        curr = getBit(key, curr->bit) ? curr->right : curr->left;
    }

    return (curr->key == key) ? curr : NULL;
}
void insert(int key) {
    if (!root) {
        root = createNode(key, 0);
        root->left = root;
        root->right = root;
        return;
    }

    PatriciaNode *curr = root;
    PatriciaNode *prev = NULL;

    while (curr->bit > (prev ? prev->bit : -1)) {
        prev = curr;
        curr = getBit(key, curr->bit) ? curr->right : curr->left;
    }

    if (curr->key == key) {
        printf("Duplicate key\n");
        return;
    }

    int diffBit = firstDifferentBit(key, curr->key);
    PatriciaNode *newNode = createNode(key, diffBit);

    PatriciaNode *parent = root;
    PatriciaNode *parentPrev = NULL;

    while (parent->bit < diffBit && parent->bit > (parentPrev ? parentPrev->bit : -1)) {
        parentPrev = parent;
        parent = getBit(key, parent->bit) ? parent->right : parent->left;
    }

    if (getBit(key, diffBit)) {
        newNode->right = newNode;
        newNode->left = parent;
    } else {
        newNode->left = newNode;
        newNode->right = parent;
    }

    if (!parentPrev)
        root = newNode;
    else if (getBit(key, parentPrev->bit))
        parentPrev->right = newNode;
    else
        parentPrev->left = newNode;
}
void deleteKey(int key) {
    if (!root) return;

    PatriciaNode *curr = root;
    PatriciaNode *parent = NULL;
    PatriciaNode *grand = NULL;

    while (curr->bit > (parent ? parent->bit : -1)) {
        grand = parent;
        parent = curr;
        curr = getBit(key, curr->bit) ? curr->right : curr->left;
    }

    if (curr->key != key) {
        printf("Key not found\n");
        return;
    }

    if (curr == parent) {
        root = NULL;
        free(curr);
        return;
    }

    PatriciaNode *child = (getBit(key, parent->bit)) ? parent->left : parent->right;

    if (!grand)
        root = child;
    else if (parent == grand->left)
        grand->left = child;
    else
        grand->right = child;

    free(parent);
}
void traverse(PatriciaNode *node, int prevBit) {
    if (!node || node->bit <= prevBit)
        return;

    traverse(node->left, node->bit);
    printf("%d ", node->key);
    traverse(node->right, node->bit);
}
int main() {
    int choice, key;

    while (1) {
        printf("\n--- PATRICIA TREE MENU ---\n");
        printf("1. Insert\n2. Search\n3. Delete\n4. Traverse\n5. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter key: ");
                scanf("%d", &key);
                insert(key);
                break;

            case 2:
                printf("Enter key: ");
                scanf("%d", &key);
                printf(search(key) ? "Found\n" : "Not Found\n");
                break;

            case 3:
                printf("Enter key: ");
                scanf("%d", &key);
                deleteKey(key);
                break;

            case 4:
                printf("Patricia Tree: ");
                traverse(root, -1);
                printf("\n");
                break;

            case 5:
                exit(0);
        }
    }
}
