
#include "linked_list.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

int length_list(node_t* head_list) {
    int length = 0;
    node_t* current = head_list;

    while (current != NULL) {
        length++;
        current = current->next;
    }

    return length;
}

void* get_first(node_t* head_list) {
    if (head_list == NULL) {
        return NULL;
    }

    return head_list->data;
}

void* get_last(node_t* head_list) {
    if (head_list == NULL) {
        return NULL;
    }

    node_t* current = head_list;

    while (current->next != NULL) {
        current = current->next;
    }

    return current->data;
}

void* remove_first(node_t** head_list) {
    if (*head_list == NULL) {
        return NULL;
    }

    node_t* curr = *head_list;
    void* data = curr->data;

    *head_list = curr->next;
    free(curr);

    return data;
}

void* remove_last(node_t** head_list) {
    if (*head_list == NULL) {
        return NULL;
    }

    node_t* curr = *head_list;
    node_t* prev = NULL;

    while (curr->next != NULL) {
        prev = curr;
        curr = curr->next;
    }

    void* data = curr->data;

    if (prev == NULL) {
        // Removing the only element
        free(curr);
        *head_list = NULL;
    } else {
        prev->next = NULL;
        free(curr);
    }

    return data;
}

void insert_first(node_t** head_list, void* to_add, size_t size) {
    node_t* new_node = malloc(sizeof(node_t));
    new_node->data = malloc(size);

    // Assuming to_add is a block of memory of 'size'
    // Copy data from to_add to the new_node
    memcpy(new_node->data, to_add, size);

    new_node->next = *head_list;
    *head_list = new_node;
}

void insert_last(node_t** head_list, void* to_add, size_t size) {
    node_t* new_node = malloc(sizeof(node_t));
    new_node->data = malloc(size);

    // Assuming to_add is a block of memory of 'size'
    // Copy data from to_add to the new_node
    memcpy(new_node->data, to_add, size);

    new_node->next = NULL;

    if (*head_list == NULL) {
        *head_list = new_node;
    } else {
        node_t* current = *head_list;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
}

void* get(node_t* head_list, int index) {
    int current_index = 0;
    node_t* current = head_list;

    while (current != NULL && current_index < index) {
        current_index++;
        current = current->next;
    }

    if (current == NULL) {
        return NULL; // Index out of bounds
    }

    return current->data;
}

int remove_element(node_t** head_list, void* to_remove, size_t size) {
    node_t* curr = *head_list;
    node_t* prev = NULL;

    while (curr != NULL) {
        if (memcmp(curr->data, to_remove, size) == 0) {
            if (prev == NULL) {
                // Removing the first element
                *head_list = curr->next;
            } else {
                prev->next = curr->next;
            }
            free(curr->data);
            free(curr);
            return 1; // Element removed
        }

        prev = curr;
        curr = curr->next;
    }

    return 0; // Element not found
}

void reverse_helper(node_t** head_list) {
    node_t* prev = NULL;
    node_t* current = *head_list;
    node_t* next = NULL;

    while (current != NULL) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }

    *head_list = prev;
}

void reverse(node_t** head_list) {
    reverse_helper(head_list);
}
