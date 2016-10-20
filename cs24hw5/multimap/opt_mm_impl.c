#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "multimap.h"


/*============================================================================
 * TYPES
 *
 *   These types are defined in the implementation file so that they can
 *   be kept hidden to code outside this source file.  This is not for any
 *   security reason, but rather just so we can enforce that our testing
 *   programs are generic and don't have any access to implementation details.
 *============================================================================*/

// call malloc to allocate the appropriate number of bytes for the array 
//int * values = (int *)malloc(sizeof(* v_array) * 1000);


/* Represents a key and its associated values in the multimap, as well as
 * pointers to the left and right child nodes in the multimap. */
typedef struct multimap_node {
    /* The key-value that this multimap node represents. */
    int key;

    /* A linked list of the values associated with this key in the multimap. */
    int *values;

    /* The tail of the linked list of values, so that we can retain the order
     * they were added to the list.
     */
    int values_tail;

    /* maxiumum values that can be placed in values */
    int max_values;


    /* The left child of the multimap node.  This will reference nodes that
     * hold keys that are strictly less than this node's key.
     */
    int left_child;

    /* The right child of the multimap node.  This will reference nodes that
     * hold keys that are strictly greater than this node's key.
     */
    int right_child;
} multimap_node;


/* The entry-point of the multimap data structure. */
struct multimap {
    multimap_node *root;
    int nodes;
    int max_nodes;
};


/*============================================================================
 * HELPER FUNCTION DECLARATIONS
 *
 *   Declarations of helper functions that are local to this module.  Again,
 *   these are not visible outside of this module.
 *============================================================================*/

multimap_node * alloc_mm_node();

multimap_node * find_mm_node(multimap *mm, int key,
                             int create_if_not_found);
multimap_node * mm_expandValues(multimap_node *node);
multimap * mm_expandNodes(multimap *mm);

void free_multimap_values(int *values);
void free_multimap_node(multimap *mm, int i);


/*============================================================================
 * FUNCTION IMPLEMENTATIONS
 *============================================================================*/

/* Allocates a multimap node, and zeros out its contents so that we know what
 * the initial value of everything will be.
 */
multimap_node * alloc_mm_node(multimap * mm) {
    bzero(&mm->root[mm->nodes], sizeof(multimap_node));
    mm->root[mm->nodes].values = (int*)malloc(sizeof(* mm->root->values)*1000);
    mm->root[mm->nodes].max_values = 1000;
    mm->root[mm->nodes].values_tail = 0;
    mm->root[mm->nodes].left_child = -1;
    mm->root[mm->nodes].right_child = -1;

    return mm->root+mm->nodes;
}


/* This helper function searches for the multimap node that contains the
 * specified key.  If such a node doesn't exist, the function can initialize
 * a new node and add this into the structure, or it will simply return NULL.
 * The one exception is the root - if the root is NULL then the function will
 * return a new root node.
 */
multimap_node * find_mm_node(multimap *mm, int key,
                             int create_if_not_found) {
    multimap_node *node;

    // expand map if it is full 
    if (mm->nodes == mm->max_nodes) {
        mm = mm_expandNodes(mm);
    } 

    /* If the entire multimap is empty, the root will be NULL. */
    if (mm->nodes == 0) {
        if (create_if_not_found) {
            mm->root = alloc_mm_node(mm);
            mm->root->key = key;
            mm->nodes++; // iterate count of nodes up
        }
        return  mm->root;
    }

    /* Now we know the multimap has at least a root node, so start there. */
    node = mm->root;

    while (1) {
        if (node->key == key)
            break;

        if (node->key > key) {   /* Follow left child */
            if (node->left_child == -1 && create_if_not_found) {
                /* No left child, but caller wants us to create a new node. */
                // set current node's left child index
                node->left_child = mm->nodes;

                // set new node
                multimap_node * temp = alloc_mm_node(mm);
                temp->key = key;
                mm->nodes++;

            }
            if (node->left_child == -1) {
                node = NULL;
            } else {
                node = &mm->root[node->left_child];
            }

        }
        else {                   /* Follow right child */
            if (node->right_child == -1 && create_if_not_found) {
                /* No right child, but caller wants us to create a new node. */
                // set current node's right child index
                node->right_child = mm->nodes;

                // set new node
                multimap_node * temp = alloc_mm_node(mm);
                temp->key = key;
                mm->nodes++;

            }
            if (node->right_child ==  -1) {
                node = NULL;
            } else {
                node = &mm->root[node->right_child];    
            }
        }

        if (node == NULL)
            break;
    }

    return node;
}


/* This helper function frees all values in a multimap node's value-list. */
void free_multimap_values(int *values) {
    if (values != NULL) {
        //multimap_value *next = values->next;
        free(values);
#ifdef DEBUG_ZERO
        /* Clear out what we are about to free, to expose issues quickly. */
        // bzero(values, sizeof(multimap_value));
#endif
    }
}

/* Initialize a multimap data structure. */
multimap * init_multimap() {
    multimap *mm = malloc(sizeof(multimap));
    mm->root = malloc(sizeof(multimap_node) * 1000);
    mm->nodes = 0;
    mm->max_nodes = 1000;
    return mm;
}

// expand the multimap's array of nodes
multimap * mm_expandNodes(multimap *mm){
    mm->root = realloc(mm->root, 2 * mm->max_nodes * sizeof(multimap_node));
    mm->max_nodes *= 2; // increase the max size of array counter
    return mm;
}

/* Release all dynamically allocated memory associated with the multimap
 * data structure.
 */
void clear_multimap(multimap *mm) {
    assert(mm != NULL);
    // free values arrays
    for (int i = 0; i < mm->nodes; ++i) {
        free_multimap_values(mm->root[i].values);
    }
    // free multimap 
    free(mm->root);

    mm->root = NULL;
}


/* Adds the specified (key, value) pair to the multimap. */
void mm_add_value(multimap *mm, int key, int value) {
    multimap_node *node;

    assert(mm != NULL);

    /* Look up the node with the specified key.  Create if not found. */
    node = find_mm_node(mm, key, /* create */ 1);
    if (mm->nodes == 0){
        mm->root = node;
        mm->nodes++;
    }

    assert(node != NULL);
    assert(node->key == key);

    /* Add the new value to the multimap node. */
    // empty
    if (node->values_tail == node->max_values) {
        // values has filled the array completely
        node = mm_expandValues(node);
    } 
    // update new value to values
    node->values[node->values_tail] = value;
    node->values_tail++;

}

// reallocate the array to a larger size  
multimap_node * mm_expandValues(multimap_node *node) {
    node->values = realloc(node->values, 2 * node->max_values
         * sizeof(node->values));
    node->max_values *= 2; // increase the max size of array counter
    return node;
}

/* Returns nonzero if the multimap contains the specified key-value, zero
 * otherwise.
 */
int mm_contains_key(multimap *mm, int key) {
    return find_mm_node(mm, key, /* create */ 0) != NULL;
}


/* Returns nonzero if the multimap contains the specified (key, value) pair,
 * zero otherwise.
 */
int mm_contains_pair(multimap *mm, int key, int value) {
    multimap_node *node;

    node = find_mm_node(mm, key, /* create */ 0);
    if (node == NULL)
        return 0;

    for (int i = 0; i < node->values_tail; ++i) {
        if (node->values[i] == value)
            return 1;
    }
    return 0;
}


/* This helper function is used by mm_traverse() to traverse every pair within
 * the multimap.
 */
void mm_traverse_helper(multimap *mm, multimap_node *node, 
    void (*f)(int key, int value)) {

    if (node->left_child != -1){
        mm_traverse_helper(mm, &mm->root[node->left_child], f);   
    }

    for (int i = 0; i < node->values_tail; ++i) {
        f(node->key, node->values[i]);
    }

    if (node->right_child != -1) {
        mm_traverse_helper(mm, &mm->root[node->right_child], f);
    }
}


/* Performs an in-order traversal of the multimap, passing each (key, value)
 * pair to the specified function.
 */
void mm_traverse(multimap *mm, void (*f)(int key, int value)) {
    mm_traverse_helper(mm, mm->root, f);
}

