#include <stdio.h>
#include <stdlib.h>

#define M 4   // Order of B+ Tree

/* B+ Tree Node */
typedef struct BPlusNode {
    int keys[M];
    struct BPlusNode *child[M + 1];
    int isLeaf;
    int n;
    struct BPlusNode *next; // For leaf nodes
} BPlusNode;

BPlusNode *root = NULL;

/* Create node */
BPlusNode* createNode(int isLeaf) {
    BPlusNode *node = (BPlusNode*)malloc(sizeof(BPlusNode));
    node->isLeaf = isLeaf;
    node->n = 0;
    node->next = NULL;
    for (int i = 0; i < M + 1; i++)
        node->child[i] = NULL;
    return node;
}

/* Search */
BPlusNode* search(int key) {
    BPlusNode *curr = root;
    if (!curr) return NULL;

    while (!curr->isLeaf) {
        int i = 0;
        while (i < curr->n && key >= curr->keys[i])
            i++;
        curr = curr->child[i];
    }

    for (int i = 0; i < curr->n; i++)
        if (curr->keys[i] == key)
            return curr;

    return NULL;
}

/* Insert in leaf */
void insertIntoLeaf(BPlusNode *leaf, int key) {
    int i = leaf->n - 1;
    while (i >= 0 && leaf->keys[i] > key) {
        leaf->keys[i + 1] = leaf->keys[i];
        i--;
    }
    leaf->keys[i + 1] = key;
    leaf->n++;
}

/* Split leaf */
void splitLeaf(BPlusNode *leaf, int *upKey, BPlusNode **newLeaf) {
    int mid = (M) / 2;

    *newLeaf = createNode(1);
    (*newLeaf)->n = leaf->n - mid;

    for (int i = 0; i < (*newLeaf)->n; i++)
        (*newLeaf)->keys[i] = leaf->keys[mid + i];

    leaf->n = mid;
    (*newLeaf)->next = leaf->next;
    leaf->next = *newLeaf;
    *upKey = (*newLeaf)->keys[0];
}

/* Insert into internal node */
void insertIntoInternal(BPlusNode *node, int key, BPlusNode *child) {
    int i = node->n - 1;
    while (i >= 0 && node->keys[i] > key) {
        node->keys[i + 1] = node->keys[i];
        node->child[i + 2] = node->child[i + 1];
        i--;
    }
    node->keys[i + 1] = key;
    node->child[i + 2] = child;
    node->n++;
}

/* Split internal node */
void splitInternal(BPlusNode *node, int *upKey, BPlusNode **newNode) {
    int mid = node->n / 2;

    *newNode = createNode(0);
    *upKey = node->keys[mid];

    (*newNode)->n = node->n - mid - 1;

    for (int i = 0; i < (*newNode)->n; i++)
        (*newNode)->keys[i] = node->keys[mid + 1 + i];

    for (int i = 0; i <= (*newNode)->n; i++)
        (*newNode)->child[i] = node->child[mid + 1 + i];

    node->n = mid;
}

/* Recursive insert */
int insertRecursive(BPlusNode *node, int key, int *upKey, BPlusNode **newChild) {
    if (node->isLeaf) {
        insertIntoLeaf(node, key);
        if (node->n < M)
            return 0;
        splitLeaf(node, upKey, newChild);
        return 1;
    }

    int i = 0;
    while (i < node->n && key >= node->keys[i])
        i++;

    int split = insertRecursive(node->child[i], key, upKey, newChild);

    if (!split)
        return 0;

    insertIntoInternal(node, *upKey, *newChild);

    if (node->n < M)
        return 0;

    splitInternal(node, upKey, newChild);
    return 1;
}

/* Insert key */
void insert(int key) {
    if (!root) {
        root = createNode(1);
        root->keys[0] = key;
        root->n = 1;
        return;
    }

    int upKey;
    BPlusNode *newChild;

    int split = insertRecursive(root, key, &upKey, &newChild);

    if (split) {
        BPlusNode *newRoot = createNode(0);
        newRoot->keys[0] = upKey;
        newRoot->child[0] = root;
        newRoot->child[1] = newChild;
        newRoot->n = 1;
        root = newRoot;
    }
}

/* Traverse (print leaves) */
void traverse() {
    if (!root) return;
    BPlusNode *curr = root;
    while (!curr->isLeaf)
        curr = curr->child[0];

    while (curr) {
        for (int i = 0; i < curr->n; i++)
            printf("%d ", curr->keys[i]);
        curr = curr->next;
    }
}

/* Delete from leaf (simple version) */
void deleteKey(int key) {
    BPlusNode *leaf = search(key);
    if (!leaf) {
        printf("Key not found\n");
        return;
    }

    int i;
    for (i = 0; i < leaf->n && leaf->keys[i] != key; i++);

    for (; i < leaf->n - 1; i++)
        leaf->keys[i] = leaf->keys[i + 1];

    leaf->n--;
}

/* Main */
int main() {
    int choice, key;

    while (1) {
        printf("\n--- B+ Tree Menu ---\n");
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
                traverse();
                printf("\n");
                break;

            case 5:
                exit(0);
        }
    }
}
