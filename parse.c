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

Token *add_token(int ty, char *input) {
    Token *token = (Token *)calloc(1, sizeof(Token));

    token->ty = ty;
    token->input = input;

    vec_push(token_vec, token);

    return token;
}

void tokenize() {
    char *p = user_input;
    token_vec = new_vector();

    while (*p) {
        if (isspace(*p)) {
            p++;
            continue;
        }

        if (!strncmp(p, "==", 2)) {
            add_token(TK_EQ, p);

            p += strlen("==");

            continue;
        }

        if (!strncmp(p, "!=", 2)) {
            add_token(TK_NE, p);

            p += strlen("!=");

            continue;
        }

        if (!strncmp(p, "<=", 2)) {
            add_token(TK_LE, p);

            p += strlen("<=");

            continue;
        }

        if (!strncmp(p, ">=", 2)) {
            add_token(TK_GE, p);

            p += strlen(">=");

            continue;
        }

        if (strchr("+-*/()<>", *p)) {
            add_token(*p, p);

            p++;

            continue;
        }

        if (isdigit(*p)) {
            Token *t = add_token(TK_NUM, p);
            t->val = strtol(p, &p, 10);

            continue;
        }

        error_at(p, "トークナイズできません");
    }

    add_token(TK_EOF, p);
}


Node *new_node(int ty, Node *lhs, Node *rhs) {
    Node *node = malloc(sizeof(Node));

    node->ty = ty;
    node->lhs = lhs;
    node->rhs = rhs;

    return node;
}

Node *new_node_num(int val) {
    Node *node = malloc(sizeof(Node));

    node->ty = ND_NUM;
    node->val = val;

    return node;
}

int consume(int ty) {
    Token *t = (Token *)token_vec->data[pos];
    if (t->ty != ty)
        return 0;

    pos++;

    return 1;
}

Node *expr() {
    return equality();
}

Node *equality() {
    Node *node = relational();

    for (;;) {
        if (consume(TK_EQ))
            node = new_node(TK_EQ, node, relational());
        else if (consume(TK_NE))
            node = new_node(TK_NE, node, relational());
        else
            return node;
    }
}

Node *relational() {
    Node *node = add();

    for (;;) {
        if (consume('<'))
            node = new_node('<', node, add());
        else if (consume(TK_LE))
            node = new_node(TK_LE, node, add());
        else if (consume('>'))
            node = new_node('<', add(), node);
        else if (consume(TK_GE))
            node = new_node(TK_LE, add(), node);
        else
            return node;
    }
}

Node *add() {
    Node *node = mul();

    for (;;) {
        if (consume('+'))
            node = new_node('+', node, mul());
        else if (consume('-'))
            node = new_node('-', node, mul());
        else
            return node;
    }
}

Node *mul() {
    Node *node = unary();

    for (;;) {
        if (consume('*'))
            node = new_node('*', node, unary());
        else if (consume('/'))
            node = new_node('/', node, unary());
        else
            return node;
    }
}

Node *unary() {
    if (consume('+'))
        return term();
    if (consume('-'))
        return new_node('-', new_node_num(0), term());

    return term();
}

Node *term() {
    Token *t = (Token *)token_vec->data[pos];

    if (consume('(')) {
        Node *node = expr();

        if (!consume(')')) {
            error_at(t->input, "開き括弧に対する閉じ括弧がありません");
        }

        return node;
    }

    if (t->ty != TK_NUM) {
        error_at(t->input, "数値でも開き括弧でもないトークンです");
    }

    pos++;
    return new_node_num(t->val);
}
