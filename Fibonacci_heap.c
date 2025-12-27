#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

/* Fibonacci Heap Node */
typedef struct FibNode {
    int key;
    int degree;
    int mark;
    struct FibNode *parent;
    struct FibNode *child;
    struct FibNode *left;
    struct FibNode *right;
} FibNode;

/* Fibonacci Heap */
typedef struct FibHeap {
    int n;
    FibNode *min;
} FibHeap;

/* Create a node */
FibNode* createNode(int key) {
    FibNode *node = (FibNode*)malloc(sizeof(FibNode));
    node->key = key;
    node->degree = 0;
    node->mark = 0;
    node->parent = NULL;
    node->child = NULL;
    node->left = node;
    node->right = node;
    return node;
}

/* Create heap */
FibHeap* createHeap() {
    FibHeap *heap = (FibHeap*)malloc(sizeof(FibHeap));
    heap->n = 0;
    heap->min = NULL;
    return heap;
}

/* Insert node into heap */
void insert(FibHeap *heap, int key) {
    FibNode *node = createNode(key);

    if (heap->min == NULL) {
        heap->min = node;
    } else {
        node->right = heap->min->right;
        node->left = heap->min;
        heap->min->right->left = node;
        heap->min->right = node;

        if (node->key < heap->min->key)
            heap->min = node;
    }
    heap->n++;
}

/* Merge two heaps */
FibHeap* unionHeap(FibHeap *h1, FibHeap *h2) {
    if (!h1->min) return h2;
    if (!h2->min) return h1;

    FibNode *temp = h1->min->right;
    h1->min->right = h2->min->right;
    h2->min->right->left = h1->min;
    h2->min->right = temp;
    temp->left = h2->min;

    if (h2->min->key < h1->min->key)
        h1->min = h2->min;

    h1->n += h2->n;
    return h1;
}

/* Link two trees */
void fibLink(FibHeap *heap, FibNode *y, FibNode *x) {
    y->left->right = y->right;
    y->right->left = y->left;

    y->parent = x;

    if (!x->child) {
        x->child = y;
        y->left = y;
        y->right = y;
    } else {
        y->right = x->child->right;
        y->left = x->child;
        x->child->right->left = y;
        x->child->right = y;
    }

    x->degree++;
    y->mark = 0;
}

/* Consolidate heap */
void consolidate(FibHeap *heap) {
    int D = (int)(log(heap->n) / log(2)) + 1;
    FibNode *A[D];
    for (int i = 0; i < D; i++)
        A[i] = NULL;

    FibNode *w = heap->min;
    int roots = 0;
    FibNode *x = w;
    do {
        roots++;
        x = x->right;
    } while (x != w);

    while (roots--) {
        x = w;
        w = w->right;
        int d = x->degree;

        while (A[d]) {
            FibNode *y = A[d];
            if (x->key > y->key) {
                FibNode *temp = x;
                x = y;
                y = temp;
            }
            fibLink(heap, y, x);
            A[d] = NULL;
            d++;
        }
        A[d] = x;
    }

    heap->min = NULL;

    for (int i = 0; i < D; i++) {
        if (A[i]) {
            if (!heap->min) {
                heap->min = A[i];
                heap->min->left = heap->min;
                heap->min->right = heap->min;
            } else {
                A[i]->right = heap->min->right;
                A[i]->left = heap->min;
                heap->min->right->left = A[i];
                heap->min->right = A[i];

                if (A[i]->key < heap->min->key)
                    heap->min = A[i];
            }
        }
    }
}

/* Extract minimum */
int extractMin(FibHeap *heap) {
    FibNode *z = heap->min;
    if (!z) return INT_MAX;

    if (z->child) {
        FibNode *c = z->child;
        do {
            FibNode *next = c->right;
            c->parent = NULL;

            c->right = heap->min->right;
            c->left = heap->min;
            heap->min->right->left = c;
            heap->min->right = c;

            c = next;
        } while (c != z->child);
    }

    z->left->right = z->right;
    z->right->left = z->left;

    if (z == z->right)
        heap->min = NULL;
    else {
        heap->min = z->right;
        consolidate(heap);
    }

    heap->n--;
    int minKey = z->key;
    free(z);
    return minKey;
}

/* Cut node */
void cut(FibHeap *heap, FibNode *x, FibNode *y) {
    if (x->right == x)
        y->child = NULL;
    else {
        x->right->left = x->left;
        x->left->right = x->right;
        if (y->child == x)
            y->child = x->right;
    }

    y->degree--;
    x->parent = NULL;
    x->mark = 0;

    x->right = heap->min->right;
    x->left = heap->min;
    heap->min->right->left = x;
    heap->min->right = x;
}

/* Cascading cut */
void cascadingCut(FibHeap *heap, FibNode *y) {
    FibNode *z = y->parent;
    if (z) {
        if (!y->mark)
            y->mark = 1;
        else {
            cut(heap, y, z);
            cascadingCut(heap, z);
        }
    }
}

/* Search node */
FibNode* search(FibNode *root, int key) {
    if (!root) return NULL;
    FibNode *x = root;
    do {
        if (x->key == key)
            return x;
        FibNode *res = search(x->child, key);
        if (res) return res;
        x = x->right;
    } while (x != root);
    return NULL;
}

/* Decrease key */
void decreaseKey(FibHeap *heap, int oldKey, int newKey) {
    FibNode *x = search(heap->min, oldKey);
    if (!x || newKey > x->key) return;

    x->key = newKey;
    FibNode *y = x->parent;

    if (y && x->key < y->key) {
        cut(heap, x, y);
        cascadingCut(heap, y);
    }

    if (x->key < heap->min->key)
        heap->min = x;
}

/* Delete key */
void deleteKey(FibHeap *heap, int key) {
    decreaseKey(heap, key, INT_MIN);
    extractMin(heap);
}

/* Display heap */
void display(FibNode *node) {
    if (!node) return;
    FibNode *start = node;
    do {
        printf("%d ", node->key);
        display(node->child);
        node = node->right;
    } while (node != start);
}

/* Main */
int main() {
    FibHeap *heap = createHeap();
    int choice, key, newKey;

    while (1) {
        printf("\n--- Fibonacci Heap Menu ---\n");
        printf("1. Insert\n2. Find Min\n3. Extract Min\n4. Decrease Key\n5. Delete Key\n6. Display\n7. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter key: ");
                scanf("%d", &key);
                insert(heap, key);
                break;

            case 2:
                if (heap->min)
                    printf("Minimum: %d\n", heap->min->key);
                else
                    printf("Heap is empty\n");
                break;

            case 3:
                printf("Extracted Min: %d\n", extractMin(heap));
                break;

            case 4:
                printf("Enter old key and new key: ");
                scanf("%d %d", &key, &newKey);
                decreaseKey(heap, key, newKey);
                break;

            case 5:
                printf("Enter key to delete: ");
                scanf("%d", &key);
                deleteKey(heap, key);
                break;

            case 6:
                printf("Heap elements: ");
                display(heap->min);
                printf("\n");
                break;

            case 7:
                exit(0);
        }
    }
}
