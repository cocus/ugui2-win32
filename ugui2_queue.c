#include <stdlib.h> 
#include "ugui2_queue.h"

// from https://gist.github.com/kroggen/5fc7380d30615b2e70fcf9c7b69997b6

void enqueue(node_t** head, void* data) {
    node_t* new_node = malloc(sizeof(node_t));
    if (!new_node) return;

    new_node->data = data;
    new_node->next = *head;

    *head = new_node;
}

void* dequeue(node_t** head) {
    node_t* current, * prev = NULL;
    void* retval = NULL;

    if (*head == NULL) return NULL;

    current = *head;
    while (current->next != NULL) {
        prev = current;
        current = current->next;
    }

    retval = current->data;
    free(current);

    if (prev)
        prev->next = NULL;
    else
        *head = NULL;

    return retval;
}