#include <stdio.h>
#include <stdlib.h>

#define MAX 100

// Heap structure
typedef struct {
    int arr[MAX];
    int size;
} Heap;

/* Function prototypes */
void initHeap(Heap *h);
void insert(Heap *h, int value);
void heapifyUp(Heap *h, int index);
void heapifyDown(Heap *h, int index);
int deleteRoot(Heap *h);
int peek(Heap *h);
void display(Heap *h);

/* Initialize heap */
void initHeap(Heap *h) {
    h->size = 0;
}

/* Insert an element into heap */
void insert(Heap *h, int value) {
    if (h->size == MAX) {
        printf("Heap Overflow\n");
        return;
    }

    h->arr[h->size] = value;
    heapifyUp(h, h->size);
    h->size++;
}

/* Heapify from bottom to top */
void heapifyUp(Heap *h, int index) {
    int parent;
    while (index > 0) {
        parent = (index - 1) / 2;
        if (h->arr[index] > h->arr[parent]) {
            int temp = h->arr[index];
            h->arr[index] = h->arr[parent];
            h->arr[parent] = temp;
            index = parent;
        } else {
            break;
        }
    }
}

/* Delete root (maximum element) */
int deleteRoot(Heap *h) {
    if (h->size == 0) {
        printf("Heap Underflow\n");
        return -1;
    }

    int root = h->arr[0];
    h->arr[0] = h->arr[h->size - 1];
    h->size--;
    heapifyDown(h, 0);

    return root;
}

/* Heapify from top to bottom */
void heapifyDown(Heap *h, int index) {
    int largest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < h->size && h->arr[left] > h->arr[largest])
        largest = left;

    if (right < h->size && h->arr[right] > h->arr[largest])
        largest = right;

    if (largest != index) {
        int temp = h->arr[index];
        h->arr[index] = h->arr[largest];
        h->arr[largest] = temp;
        heapifyDown(h, largest);
    }
}

/* Peek root element */
int peek(Heap *h) {
    if (h->size == 0) {
        printf("Heap is empty\n");
        return -1;
    }
    return h->arr[0];
}

/* Display heap elements */
void display(Heap *h) {
    if (h->size == 0) {
        printf("Heap is empty\n");
        return;
    }

    printf("Heap elements: ");
    for (int i = 0; i < h->size; i++) {
        printf("%d ", h->arr[i]);
    }
    printf("\n");
}

/* Main function */
int main() {
    Heap h;
    initHeap(&h);

    int choice, value;

    while (1) {
        printf("\n--- Binary Heap Menu ---\n");
        printf("1. Insert\n");
        printf("2. Delete Root\n");
        printf("3. Peek\n");
        printf("4. Display\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter value: ");
                scanf("%d", &value);
                insert(&h, value);
                break;

            case 2:
                value = deleteRoot(&h);
                if (value != -1)
                    printf("Deleted root: %d\n", value);
                break;

            case 3:
                value = peek(&h);
                if (value != -1)
                    printf("Root element: %d\n", value);
                break;

            case 4:
                display(&h);
                break;

            case 5:
                exit(0);

            default:
                printf("Invalid choice\n");
        }
    }

    return 0;
}
