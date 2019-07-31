#define _GNU_SOURCE
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef enum {
    TK_NUM,
    TK_EOF,
    TK_EQ,
    TK_NE,
    TK_LE,
    TK_GE,
    TK_RESERVED,
    TK_IDENT,
} TokenKind;

typedef struct Token Token;

struct Token {
    TokenKind kind;
    Token *next;
    int val; // Number Literal
    char *input; // Identifier

    // String literal
    int len;
    char *str;
};

extern Token *token;

bool at_eof();

Token *tokenize();
Token *new_token(TokenKind kind, Token *cur, char *str);

typedef struct LVar LVar;
typedef struct LVar {
    LVar *next;
    char *name;
    int len;
    int offset;
} LVar;
LVar *find_lvar(Token *tok);
Token *ident(char *p, Token *cur);

typedef enum {
    ND_NUM = 256,
    ND_ADD,
    ND_SUB,
    ND_MUL,
    ND_DIV,
    ND_ASSIGN,
    ND_LVAR,
    ND_LE,
    ND_LT,
    ND_EQ,
    ND_NE,
} NodeKind;

typedef struct Node {
    NodeKind kind;
    struct Node *lhs;
    struct Node *rhs;
    int val;
    int offset;
} Node;

void error(char *fmt, ...);
void error_at(char *loc, char *msg);
bool consume(char *op);
void expect();
int expect_number();

Node *new_node(NodeKind kind, Node *lhs, Node *rhs);
Node *new_node_num(int val);

void program();
Node *stmt();
Node *expr();
Node *assign();
Node *equality();
Node *relational();
Node *add();
Node *mul();
Node *unary();
Node *term();

void gen(Node *node);

typedef struct Vector {
    void **data;
    int capacity;
    int len;
} Vector;

Vector *new_vector();
void vec_push(Vector *vec, void *elem);

extern Node *code[100];
extern char *user_input;
extern LVar *locals;
