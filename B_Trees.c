#include <stdio.h>
#include <stdlib.h>

#define T 3   // Minimum degree

/* B-Tree Node */
typedef struct BTreeNode {
    int keys[2*T - 1];
    struct BTreeNode *child[2*T];
    int n;
    int leaf;
} BTreeNode;

/* Create node */
BTreeNode* createNode(int leaf) {
    BTreeNode *node = (BTreeNode*)malloc(sizeof(BTreeNode));
    node->leaf = leaf;
    node->n = 0;
    for (int i = 0; i < 2*T; i++)
        node->child[i] = NULL;
    return node;
}

/* Traverse */
void traverse(BTreeNode *root) {
    if (!root) return;
    int i;
    for (i = 0; i < root->n; i++) {
        if (!root->leaf)
            traverse(root->child[i]);
        printf("%d ", root->keys[i]);
    }
    if (!root->leaf)
        traverse(root->child[i]);
}

/* Search */
BTreeNode* search(BTreeNode *root, int key) {
    int i = 0;
    while (i < root->n && key > root->keys[i])
        i++;
    if (i < root->n && root->keys[i] == key)
        return root;
    if (root->leaf)
        return NULL;
    return search(root->child[i], key);
}

/* Split child */
void splitChild(BTreeNode *parent, int i, BTreeNode *y) {
    BTreeNode *z = createNode(y->leaf);
    z->n = T - 1;

    for (int j = 0; j < T - 1; j++)
        z->keys[j] = y->keys[j + T];

    if (!y->leaf) {
        for (int j = 0; j < T; j++)
            z->child[j] = y->child[j + T];
    }

    y->n = T - 1;

    for (int j = parent->n; j >= i + 1; j--)
        parent->child[j + 1] = parent->child[j];

    parent->child[i + 1] = z;

    for (int j = parent->n - 1; j >= i; j--)
        parent->keys[j + 1] = parent->keys[j];

    parent->keys[i] = y->keys[T - 1];
    parent->n++;
}

/* Insert non-full */
void insertNonFull(BTreeNode *node, int key) {
    int i = node->n - 1;
    if (node->leaf) {
        while (i >= 0 && key < node->keys[i]) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1] = key;
        node->n++;
    } else {
        while (i >= 0 && key < node->keys[i])
            i--;
        i++;
        if (node->child[i]->n == 2*T - 1) {
            splitChild(node, i, node->child[i]);
            if (key > node->keys[i])
                i++;
        }
        insertNonFull(node->child[i], key);
    }
}

/* Insert */
void insert(BTreeNode **root, int key) {
    if (*root == NULL) {
        *root = createNode(1);
        (*root)->keys[0] = key;
        (*root)->n = 1;
        return;
    }

    if ((*root)->n == 2*T - 1) {
        BTreeNode *s = createNode(0);
        s->child[0] = *root;
        splitChild(s, 0, *root);

        int i = 0;
        if (s->keys[0] < key)
            i++;
        insertNonFull(s->child[i], key);
        *root = s;
    } else {
        insertNonFull(*root, key);
    }
}

/* Get predecessor */
int getPred(BTreeNode *node, int idx) {
    BTreeNode *cur = node->child[idx];
    while (!cur->leaf)
        cur = cur->child[cur->n];
    return cur->keys[cur->n - 1];
}

/* Get successor */
int getSucc(BTreeNode *node, int idx) {
    BTreeNode *cur = node->child[idx + 1];
    while (!cur->leaf)
        cur = cur->child[0];
    return cur->keys[0];
}

/* Merge */
void merge(BTreeNode *node, int idx) {
    BTreeNode *child = node->child[idx];
    BTreeNode *sibling = node->child[idx + 1];

    child->keys[T - 1] = node->keys[idx];

    for (int i = 0; i < sibling->n; i++)
        child->keys[i + T] = sibling->keys[i];

    if (!child->leaf) {
        for (int i = 0; i <= sibling->n; i++)
            child->child[i + T] = sibling->child[i];
    }

    for (int i = idx + 1; i < node->n; i++)
        node->keys[i - 1] = node->keys[i];

    for (int i = idx + 2; i <= node->n; i++)
        node->child[i - 1] = node->child[i];

    child->n += sibling->n + 1;
    node->n--;
    free(sibling);
}

/* Borrow from previous */
void borrowFromPrev(BTreeNode *node, int idx) {
    BTreeNode *child = node->child[idx];
    BTreeNode *sibling = node->child[idx - 1];

    for (int i = child->n - 1; i >= 0; i--)
        child->keys[i + 1] = child->keys[i];

    if (!child->leaf) {
        for (int i = child->n; i >= 0; i--)
            child->child[i + 1] = child->child[i];
    }

    child->keys[0] = node->keys[idx - 1];
    if (!child->leaf)
        child->child[0] = sibling->child[sibling->n];

    node->keys[idx - 1] = sibling->keys[sibling->n - 1];

    child->n++;
    sibling->n--;
}

/* Borrow from next */
void borrowFromNext(BTreeNode *node, int idx) {
    BTreeNode *child = node->child[idx];
    BTreeNode *sibling = node->child[idx + 1];

    child->keys[child->n] = node->keys[idx];
    if (!child->leaf)
        child->child[child->n + 1] = sibling->child[0];

    node->keys[idx] = sibling->keys[0];

    for (int i = 1; i < sibling->n; i++)
        sibling->keys[i - 1] = sibling->keys[i];

    if (!sibling->leaf) {
        for (int i = 1; i <= sibling->n; i++)
            sibling->child[i - 1] = sibling->child[i];
    }

    child->n++;
    sibling->n--;
}

/* Fill */
void fill(BTreeNode *node, int idx) {
    if (idx != 0 && node->child[idx - 1]->n >= T)
        borrowFromPrev(node, idx);
    else if (idx != node->n && node->child[idx + 1]->n >= T)
        borrowFromNext(node, idx);
    else {
        if (idx != node->n)
            merge(node, idx);
        else
            merge(node, idx - 1);
    }
}

/* Remove from node */
void removeNode(BTreeNode *node, int key);

void removeFromNonLeaf(BTreeNode *node, int idx) {
    int k = node->keys[idx];

    if (node->child[idx]->n >= T) {
        int pred = getPred(node, idx);
        node->keys[idx] = pred;
        removeNode(node->child[idx], pred);
    } else if (node->child[idx + 1]->n >= T) {
        int succ = getSucc(node, idx);
        node->keys[idx] = succ;
        removeNode(node->child[idx + 1], succ);
    } else {
        merge(node, idx);
        removeNode(node->child[idx], k);
    }
}

void removeNode(BTreeNode *node, int key) {
    int idx = 0;
    while (idx < node->n && node->keys[idx] < key)
        idx++;

    if (idx < node->n && node->keys[idx] == key) {
        if (node->leaf) {
            for (int i = idx + 1; i < node->n; i++)
                node->keys[i - 1] = node->keys[i];
            node->n--;
        } else {
            removeFromNonLeaf(node, idx);
        }
    } else {
        if (node->leaf) {
            printf("Key not found\n");
            return;
        }

        int flag = (idx == node->n);
        if (node->child[idx]->n < T)
            fill(node, idx);

        if (flag && idx > node->n)
            removeNode(node->child[idx - 1], key);
        else
            removeNode(node->child[idx], key);
    }
}

/* Delete key */
void deleteKey(BTreeNode **root, int key) {
    if (!*root) return;

    removeNode(*root, key);

    if ((*root)->n == 0) {
        BTreeNode *temp = *root;
        if ((*root)->leaf)
            *root = NULL;
        else
            *root = (*root)->child[0];
        free(temp);
    }
}

/* Main */
int main() {
    BTreeNode *root = NULL;
    int choice, key;

    while (1) {
        printf("\n--- B-Tree Menu ---\n");
        printf("1. Insert\n2. Delete\n3. Search\n4. Traverse\n5. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter key: ");
                scanf("%d", &key);
                insert(&root, key);
                break;

            case 2:
                printf("Enter key to delete: ");
                scanf("%d", &key);
                deleteKey(&root, key);
                break;

            case 3:
                printf("Enter key to search: ");
                scanf("%d", &key);
                printf(search(root, key) ? "Found\n" : "Not Found\n");
                break;

            case 4:
                traverse(root);
                printf("\n");
                break;

            case 5:
                exit(0);
        }
    }
}
