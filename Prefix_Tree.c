#include <stdio.h>
#include <stdlib.h>

typedef struct TrieNode {
    int isLeaf;              // 1 if leaf node (stores data)
    int data;                // valid if isLeaf == 1
    struct TrieNode *left;   // represents bit 0
    struct TrieNode *right;  // represents bit 1
} TrieNode;

/* Create a new node */
TrieNode* createNode() {
    TrieNode *node = (TrieNode*)malloc(sizeof(TrieNode));
    node->isLeaf = 0;
    node->data = -1;
    node->left = node->right = NULL;
    return node;
}

/* Get ith bit of number from MSB */
int getBit(int num, int i) {
    int totalBits = sizeof(int) * 8;
    return (num >> (totalBits - i - 1)) & 1;
}

/* Insert element */
void insert(TrieNode **root, int num) {
    if (!*root) *root = createNode();
    TrieNode *curr = *root;
    int totalBits = sizeof(int) * 8;

    for (int i = 0; i < totalBits; i++) {
        int bit = getBit(num, i);
        if (bit == 0) {
            if (!curr->left) curr->left = createNode();
            curr = curr->left;
        } else {
            if (!curr->right) curr->right = createNode();
            curr = curr->right;
        }
    }
    curr->isLeaf = 1;
    curr->data = num;
}

/* Search element */
int search(TrieNode *root, int num) {
    if (!root) return 0;
    TrieNode *curr = root;
    int totalBits = sizeof(int) * 8;

    for (int i = 0; i < totalBits; i++) {
        int bit = getBit(num, i);
        if (bit == 0) curr = curr->left;
        else curr = curr->right;
        if (!curr) return 0;
    }
    return (curr && curr->isLeaf && curr->data == num);
}

/* Check if node is empty */
int isEmpty(TrieNode *node) {
    return node && !node->left && !node->right && !node->isLeaf;
}

/* Delete element */
TrieNode* deleteNode(TrieNode *root, int num, int depth) {
    if (!root) return NULL;

    int totalBits = sizeof(int) * 8;

    if (depth == totalBits) {
        if (root->isLeaf && root->data == num) {
            free(root);
            return NULL;
        }
        return root;
    }

    int bit = getBit(num, depth);
    if (bit == 0) root->left = deleteNode(root->left, num, depth + 1);
    else root->right = deleteNode(root->right, num, depth + 1);

    if (isEmpty(root)) {
        free(root);
        root = NULL;
    }

    return root;
}

/* Traverse and display elements */
void traverse(TrieNode *root) {
    if (!root) return;
    if (root->isLeaf) {
        printf("%d ", root->data);
        return;
    }
    traverse(root->left);
    traverse(root->right);
}

/* Main function */
int main() {
    TrieNode *root = NULL;
    int choice, num;

    while (1) {
        printf("\n--- Prefix Tree Menu ---\n");
        printf("1. Insert\n2. Search\n3. Delete\n4. Display\n5. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter number to insert: ");
                scanf("%d", &num);
                insert(&root, num);
                break;

            case 2:
                printf("Enter number to search: ");
                scanf("%d", &num);
                printf(search(root, num) ? "Found\n" : "Not Found\n");
                break;

            case 3:
                printf("Enter number to delete: ");
                scanf("%d", &num);
                root = deleteNode(root, num, 0);
                break;

            case 4:
                printf("Elements in Prefix Tree: ");
                traverse(root);
                printf("\n");
                break;

            case 5:
                exit(0);

            default:
                printf("Invalid choice\n");
        }
    }
}
