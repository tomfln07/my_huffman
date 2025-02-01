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
void print_tree(char_node_t *node, int depth);
#endif