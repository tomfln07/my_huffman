#ifndef HUFFMAN_TREE_H
    #define HUFFMAN_TREE_H

enum node_type {
    INTERNAL,
    LEAF
};

typedef struct char_node_s char_node_t;
typedef struct char_node_s {
    unsigned char c;
    int freq;
    char_node_t *l;
    char_node_t *r;
    enum node_type type;
} char_node_t;

char_node_t *gen_tree(char_node_t **occurr);
int get_tree_depth(char_node_t *tree);
void free_tree(char_node_t *tree);
void free_nodes_list(char_node_t **nodes, int len);
void print_tree(char_node_t *node, int depth);
char_node_t *new_internal_node(char_node_t *node1, char_node_t *node2);
int occurr_cpy(char_node_t **src, char_node_t ***dest, int len);
#endif