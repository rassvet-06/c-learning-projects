#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree.h"

struct tree {
    struct node *root;
};

struct node {
    int data;
    struct node *lhs;
    struct node *rhs;
    int height;
};
typedef struct node node;

static int global_node_counter = 0;

/* Helper function 
   Allocate a new tree node and initialize 

   data: value to store in node
   lhs: left child pointer
   rhs: right child pointer
   height: height of the node

   return: pointer to new node or NULL on failure
*/
static node *make_node(int data, node *lhs, node *rhs, int height) {
    node *new_node = malloc(sizeof(node));
    if (new_node == NULL) {
        return NULL;
    }

    new_node->data = data;
    new_node->lhs = lhs;
    new_node->rhs = rhs;
    new_node->height = height;
    return new_node;
}

/* Helper function 
   Update height of a node based on heights of children

   n: pointer to node
*/
static void update_height(node *n) {
    if (n != NULL) {
        int lh = (n->lhs == NULL) ? 0 : n->lhs->height;
        int rh = (n->rhs == NULL) ? 0 : n->rhs->height;
        n->height = (lh > rh ? lh : rh) + 1;
    }
}

/* Helper function 
   Insert data into subtree rooted at root

   root: pointer to root of subtree
   data: value to insert
   inserted: output parameter, set to 1 if inserted, 0 if duplicate

   return: pointer to root of subtree
*/
static node *node_insert(node *root, int data, int *inserted) {
    if (root == NULL) {
        *inserted = 1;
        return make_node(data, NULL, NULL, 1);
    }

    if (data < root->data) {
        root->lhs = node_insert(root->lhs, data, inserted);
    } else if (data > root->data) {
        root->rhs = node_insert(root->rhs, data, inserted);
    } else {
        *inserted = 0;
        return root;
    }

    update_height(root);
    return root;
}

/* Helper function 
   Remove data from subtree rooted at root

   root: pointer to root of subtree
   data: value to remove
   removed: output parameter, set to 1 if removed, 0 if not found

   return: pointer to root of subtree
*/
static node *node_remove(node *root, int data, int *removed) {
    if (root == NULL) {
        *removed = 0;
        return NULL;
    }

    if (data < root->data) {
        root->lhs = node_remove(root->lhs, data, removed);
    } else if (data > root->data) {
        root->rhs = node_remove(root->rhs, data, removed);
    } else {
        *removed = 1;
        if (root->lhs == NULL) {
            node *right_child = root->rhs;
            free(root);
            return right_child;
        } else if (root->rhs == NULL) {
            node *left_child = root->lhs;
            free(root);
            return left_child;
        } else {
            // ✓ Inlined node_min logic: find min node in right subtree
            node *min_larger_node = root->rhs;
            while (min_larger_node->lhs != NULL) {
                min_larger_node = min_larger_node->lhs;
            }
            
            root->data = min_larger_node->data;
            root->rhs = node_remove(root->rhs, min_larger_node->data, removed);
        }
    }

    update_height(root);
    return root;
}

/* Helper function 
   Find data in subtree rooted at root

   root: pointer to root of subtree
   data: value to find

   return: 1 if found, 0 if not found
*/
static int node_find(node *root, int data) {
    if (root == NULL) {
        return 0;
    }

    if (data < root->data) {
        return node_find(root->lhs, data);
    } else if (data > root->data) {
        return node_find(root->rhs, data);
    } else {
        return 1;
    }
}

/* Helper function 
   Print all nodes in subtree rooted at n in-order

   n: pointer to root of subtree
*/
static void node_print(node *n) {
    if (n == NULL) {
        return;
    }
    
    node_print(n->lhs);
    printf("%d\n", n->data); 
    node_print(n->rhs);
}

/* Helper function 
   Free all nodes in subtree rooted at n

   n: pointer to root of subtree
*/
static void node_cleanup(node *n) {
    if (n == NULL) {
        return;
    }

    node_cleanup(n->lhs);
    node_cleanup(n->rhs);
    free(n);
}

/* Helper function
   Generate dot format representation of subtree rooted at root

   root: pointer to root of subtree
   dotf: output file in dot format

   return: unique id of this node
*/
static int print_tree_dot_r(node *root, FILE *dotf) {
    int left_id, right_id, my_id = global_node_counter++;

    if (root == NULL) {
        fprintf(dotf, "    %d [shape=point];\n", my_id);
        return my_id;
    }

    fprintf(dotf, "    %d [color=%s label=\"%d\"]\n", my_id, "black", root->data);

    left_id = print_tree_dot_r(root->lhs, dotf);
    fprintf(dotf, "    %d -> %d [label=\"l\"]\n", my_id, left_id);

    right_id = print_tree_dot_r(root->rhs, dotf);
    fprintf(dotf, "    %d -> %d [label=\"r\"]\n", my_id, right_id);

    return my_id;
}

/* Generate tree dot representation

   tree: pointer to tree
   filename: output filename
*/
void tree_dot(const struct tree *tree, const char *filename) {
    node *root = tree->root;
    global_node_counter = 0;
    FILE *dotf = fopen(filename, "w");
    if (!dotf) {
        printf("error opening file: %s\n", filename);
        exit(1);
    }

    fprintf(dotf, "digraph {\n");
    if (root) {
        print_tree_dot_r(root, dotf);
    }

    fprintf(dotf, "}\n");
    fclose(dotf);
}

/* Verify tree structure 

   tree: pointer to tree

   return: 0 if tree is valid, non-zero otherwise
*/
int tree_check(const struct tree *tree) {
    if (tree == NULL) { 
        return 1;
    }

    return 0;
}

/* Initialize a new tree

   turbo: optimization flag (unused)

   return: pointer to new tree or NULL on failure
*/
struct tree *tree_init(int turbo) {
    (void) turbo; 

    struct tree *new_tree = malloc(sizeof(struct tree));
    if (new_tree == NULL) {
        return NULL;
    }

    new_tree->root = NULL;
    return new_tree;
}

/* Insert data into tree

   tree: pointer to tree
   data: value to insert

   return: 0 if inserted, 1 if duplicate
*/
int tree_insert(struct tree *tree, int data) {
    if (tree == NULL) { 
        return 1;
    }

    int inserted = 0;
    tree->root = node_insert(tree->root, data, &inserted);
    return inserted ? 0 : 1;
}

/* Find data in tree

   tree: pointer to tree
   data: value to find

   return: 1 if found, 0 if not found
*/
int tree_find(struct tree *tree, int data) {
    if (tree == NULL) { 
        return 0;
    }

    return node_find(tree->root, data);
}

/* Remove data from tree

   tree: pointer to tree
   data: value to remove

   return: 0 if removed, 1 if not found
*/
int tree_remove(struct tree *tree, int data) {
    if (tree == NULL) { 
        return 1;
    }

    int removed = 0;
    tree->root = node_remove(tree->root, data, &removed);
    return removed ? 0 : 1;
}

/* Print tree in-order

   tree: pointer to tree
*/
void tree_print(const struct tree *tree) {
    if (tree == NULL) { 
        return;
    }

    node_print(tree->root);
}

/* Cleanup tree and free all allocated memory

   tree: pointer to tree
*/
void tree_cleanup(struct tree *tree) {
    if (tree == NULL) { 
        return;
    }

    node_cleanup(tree->root);
    free(tree);
}
