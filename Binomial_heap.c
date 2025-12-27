#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/* Binomial Heap Node */
struct Node {
    int key;
    int degree;
    struct Node *parent;
    struct Node *child;
    struct Node *sibling;
};

/* Create a new node */
struct Node* createNode(int key) {
    struct Node *newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->key = key;
    newNode->degree = 0;
    newNode->parent = NULL;
    newNode->child = NULL;
    newNode->sibling = NULL;
    return newNode;
}

/* Create empty heap */
struct Node* createHeap() {
    return NULL;
}

/* Link two binomial trees */
void binomialLink(struct Node *y, struct Node *z) {
    y->parent = z;
    y->sibling = z->child;
    z->child = y;
    z->degree++;
}

/* Merge root lists */
struct Node* mergeHeaps(struct Node *h1, struct Node *h2) {
    if (!h1) return h2;
    if (!h2) return h1;

    struct Node *head = NULL, *tail = NULL;
    struct Node *a = h1, *b = h2;

    if (a->degree <= b->degree) {
        head = tail = a;
        a = a->sibling;
    } else {
        head = tail = b;
        b = b->sibling;
    }

    while (a && b) {
        if (a->degree <= b->degree) {
            tail->sibling = a;
            a = a->sibling;
        } else {
            tail->sibling = b;
            b = b->sibling;
        }
        tail = tail->sibling;
    }

    tail->sibling = (a) ? a : b;
    return head;
}

/* Union of two binomial heaps */
struct Node* unionHeaps(struct Node *h1, struct Node *h2) {
    struct Node *newHeap = mergeHeaps(h1, h2);
    if (!newHeap) return NULL;

    struct Node *prev = NULL;
    struct Node *curr = newHeap;
    struct Node *next = curr->sibling;

    while (next) {
        if ((curr->degree != next->degree) ||
            (next->sibling && next->sibling->degree == curr->degree)) {
            prev = curr;
            curr = next;
        } else {
            if (curr->key <= next->key) {
                curr->sibling = next->sibling;
                binomialLink(next, curr);
            } else {
                if (!prev)
                    newHeap = next;
                else
                    prev->sibling = next;
                binomialLink(curr, next);
                curr = next;
            }
        }
        next = curr->sibling;
    }

    return newHeap;
}

/* Insert key */
struct Node* insert(struct Node *heap, int key) {
    struct Node *temp = createNode(key);
    return unionHeaps(heap, temp);
}

/* Find minimum */
struct Node* findMin(struct Node *heap) {
    if (!heap) return NULL;

    struct Node *minNode = heap;
    int min = heap->key;

    struct Node *curr = heap->sibling;
    while (curr) {
        if (curr->key < min) {
            min = curr->key;
            minNode = curr;
        }
        curr = curr->sibling;
    }
    return minNode;
}

/* Reverse linked list */
struct Node* reverseList(struct Node *node) {
    struct Node *prev = NULL;
    struct Node *curr = node;
    struct Node *next;

    while (curr) {
        curr->parent = NULL;
        next = curr->sibling;
        curr->sibling = prev;
        prev = curr;
        curr = next;
    }
    return prev;
}

/* Extract minimum */
struct Node* extractMin(struct Node *heap) {
    if (!heap) return NULL;

    struct Node *minNode = heap;
    struct Node *minPrev = NULL;
    struct Node *prev = NULL;
    struct Node *curr = heap;
    int min = heap->key;

    while (curr) {
        if (curr->key < min) {
            min = curr->key;
            minNode = curr;
            minPrev = prev;
        }
        prev = curr;
        curr = curr->sibling;
    }

    if (minPrev)
        minPrev->sibling = minNode->sibling;
    else
        heap = minNode->sibling;

    struct Node *childHeap = reverseList(minNode->child);
    heap = unionHeaps(heap, childHeap);

    free(minNode);
    return heap;
}

/* Decrease key */
void decreaseKey(struct Node *heap, struct Node *node, int newKey) {
    if (newKey > node->key) {
        printf("New key is greater than current key\n");
        return;
    }

    node->key = newKey;
    struct Node *parent = node->parent;

    while (parent && node->key < parent->key) {
        int temp = node->key;
        node->key = parent->key;
        parent->key = temp;

        node = parent;
        parent = parent->parent;
    }
}

/* Search key */
struct Node* search(struct Node *heap, int key) {
    if (!heap) return NULL;
    if (heap->key == key) return heap;

    struct Node *res = search(heap->child, key);
    if (res) return res;

    return search(heap->sibling, key);
}

/* Delete key */
struct Node* deleteKey(struct Node *heap, int key) {
    struct Node *node = search(heap, key);
    if (!node) {
        printf("Key not found\n");
        return heap;
    }
    decreaseKey(heap, node, INT_MIN);
    return extractMin(heap);
}

/* Display heap */
void display(struct Node *heap) {
    if (!heap) return;

    printf("%d ", heap->key);
    display(heap->child);
    display(heap->sibling);
}

/* Main function */
int main() {
    struct Node *heap = createHeap();
    int choice, key;

    while (1) {
        printf("\n--- Binomial Heap Menu ---\n");
        printf("1. Insert\n");
        printf("2. Find Min\n");
        printf("3. Extract Min\n");
        printf("4. Delete Key\n");
        printf("5. Display\n");
        printf("6. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter key: ");
                scanf("%d", &key);
                heap = insert(heap, key);
                break;

            case 2: {
                struct Node *min = findMin(heap);
                if (min)
                    printf("Minimum key: %d\n", min->key);
                else
                    printf("Heap is empty\n");
                break;
            }

            case 3:
                heap = extractMin(heap);
                printf("Minimum extracted\n");
                break;

            case 4:
                printf("Enter key to delete: ");
                scanf("%d", &key);
                heap = deleteKey(heap, key);
                break;

            case 5:
                printf("Heap elements: ");
                display(heap);
                printf("\n");
                break;

            case 6:
                exit(0);

            default:
                printf("Invalid choice\n");
        }
    }
}
