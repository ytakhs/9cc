#define _GNU_SOURCE
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void error(char *fmt, ...);
void error_at(char *loc, char *msg);

enum {
    TK_NUM = 256,
    TK_EOF,
    TK_EQ,
    TK_NE,
    TK_LE,
    TK_GE,
};

typedef struct {
    int ty;
    int val;
    char *input;
} Token;

Token *add_token(int ty, char *input);
void tokenize();

enum {
    ND_NUM = 256,
};

typedef struct Node {
    int ty;
    struct Node *lhs;
    struct Node *rhs;
    int val;
} Node;

Node *new_node(int ty, Node *lhs, Node *rhs);
Node *new_node_num(int val);
Node *expr();
Node *equality();
Node *relational();
Node *add();
Node *mul();
Node *unary();
Node *term();
int consume(int ty);
void gen(Node *node);

typedef struct Vector {
    void **data;
    int capacity;
    int len;
} Vector;

Vector *new_vector();
void vec_push(Vector *vec, void *elem);
