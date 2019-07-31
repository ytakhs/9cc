#include "9cc.h"

void error(char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");

    exit(1);
}

void error_at(char *loc, char *msg) {
    int pos = loc - user_input;

    fprintf(stderr, "%s\n", user_input);
    fprintf(stderr, "%*s", pos, "");
    fprintf(stderr, "^ %s\n", msg);

    exit(1);
}

bool consume(char *op) {
    if (token->kind != TK_RESERVED || (int)strlen(op) != token->len || memcmp(token->str, op, token->len) != 0)
        return false;

    token = token->next;

    return true;
}

LVar *find_lvar(Token *tok) {
    for (LVar *var = locals; var; var = var->next)
        if (var->len == tok->len && !memcmp(tok->str, var->name, var->len))
            return var;

    return NULL;
}


Token *consume_ident() {
    if (token->kind != TK_IDENT)
        return NULL;

    Token *tok = token;
    token = token->next;

    return tok;
}

void expect(char *op) {
    if (token->kind != TK_RESERVED || (int)strlen(op) != token->len || memcmp(token->str, op, token->len) != 0) {
        error("not '%s', but '%s'", op, token->str);
    }

    token = token->next;
}

int expect_number() {
    if (token->kind != TK_NUM) {
        error("not a number");
    }

    int val = token->val;
    token = token->next;

    return val;
}

Node *new_node(NodeKind kind, Node *lhs, Node *rhs) {
    Node *node = calloc(1, sizeof(Node));

    node->kind = kind;
    node->lhs = lhs;
    node->rhs = rhs;

    return node;
}

Node *new_node_num(int val) {
    Node *node = calloc(1, sizeof(Node));

    node->kind = ND_NUM;
    node->val = val;

    return node;
}

void program() {
    int i = 0;

    locals = calloc(1, sizeof(LVar));
    locals->offset = 0;

    while(!at_eof())
        code[i++] = stmt();

    code[i] = NULL;
}

Node *stmt() {
    Node *node = expr();

    expect(";");

    return node;
}

Node *expr() {
    return assign();
}

Node *assign() {
    Node *node = equality();
    if (consume("="))
        node = new_node(ND_ASSIGN, node, assign());

    return node;
}

Node *equality() {
    Node *node = relational();

    for (;;) {
        if (consume("=="))
            node = new_node(ND_EQ, node, relational());
        else if (consume("!="))
            node = new_node(ND_NE, node, relational());
        else
            return node;
    }
}

Node *relational() {
    Node *node = add();

    for (;;) {
        if (consume("<"))
            node = new_node(ND_LT, node, add());
        else if (consume("<="))
            node = new_node(ND_LE, node, add());
        else if (consume(">"))
            node = new_node(ND_LT, add(), node);
        else if (consume(">="))
            node = new_node(ND_LE, add(), node);
        else
            return node;
    }
}

Node *add() {
    Node *node = mul();

    for (;;) {
        if (consume("+"))
            node = new_node(ND_ADD, node, mul());
        else if (consume("-"))
            node = new_node(ND_SUB, node, mul());
        else
            return node;
    }
}

Node *mul() {
    Node *node = unary();

    for (;;) {
        if (consume("*"))
            node = new_node(ND_MUL, node, unary());
        else if (consume("/"))
            node = new_node(ND_DIV, node, unary());
        else
            return node;
    }
}

Node *unary() {
    if (consume("+"))
        return term();
    if (consume("-"))
        return new_node(ND_SUB, new_node_num(0), term());

    return term();
}

Node *term() {
    if (consume("(")) {
        Node *node = expr();

        expect(")");

        return node;
    }

    Token *tok = consume_ident();
    if (tok) {
        Node *node = calloc(1, sizeof(Node));
        node->kind = ND_LVAR;

        LVar *lvar = find_lvar(tok);
        if (lvar) {
            node->offset = lvar->offset;
        } else {
            lvar = calloc(1, sizeof(LVar));
            lvar->next = locals;
            lvar->name = tok->str;
            lvar->len = tok->len;
            lvar->offset = locals->offset + 8;
            node->offset = lvar->offset;
            locals = lvar;
        }

        return node;
    }

    return new_node_num(expect_number());
}
