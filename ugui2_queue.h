#pragma once

// from https://gist.github.com/kroggen/5fc7380d30615b2e70fcf9c7b69997b6
typedef struct node {
    void* data;
    struct node* next;
} node_t;

void enqueue(node_t** head, void* data);
void* dequeue(node_t** head);
