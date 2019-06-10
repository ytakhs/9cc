#include "9cc.h"

char *user_input;
int pos = 0;
Token tokens[100];

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

void tokenize() {
    char *p = user_input;
    int i = 0;

    while (*p) {
        if (isspace(*p)) {
            p++;
            continue;
        }

        if (*p == '+' || *p == '-') {
            tokens[i].ty = *p;
            tokens[i].input = p;

            i++;
            p++;

            continue;
        }

        if (isdigit(*p)) {
            tokens[i].ty = TK_NUM;
            tokens[i].input = p;
            tokens[i].val = strtol(p, &p, 10);

            i++;

            continue;
        }

        error_at(p, "トークナイズできません");
    }

    tokens[i].ty = TK_EOF;
    tokens[i].input = p;
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
    if (tokens[pos].ty != ty)
        return 0;

    pos++;

    return 1;
}

Node *expr() {
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
    Node *node = term();

    for (;;) {
        if (consume('*'))
            node = new_node('*', node, term());
        else if (consume('/'))
            node = new_node('/', node, term());
        else
            return node;
    }
}

Node *term() {
    if (consume('(')) {
        Node *node = expr();

        if (!consume(')'))
            error_at(tokens[pos].input, "開き括弧に対する閉じ括弧がありません");

        return node;
    }

    if (tokens[pos].ty == TK_NUM)
        return new_node_num(tokens[pos++].val);

    error_at(tokens[pos].input, "数値でも開き括弧でもないトークンです");
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "引数の個数が正しくありません\n");
        return 1;
    }

    user_input = argv[1];
    tokenize();

    printf(".intel_syntax noprefix\n");
    printf(".global main\n");
    printf("main:\n");

    if (tokens[0].ty != TK_NUM)
        error_at(tokens[0].input, "数ではありません");

    printf("    mov rax, %d\n", tokens[0].val);

    int i = 1;
    while (tokens[i].ty != TK_EOF) {
        if (tokens[i].ty == '+') {
            i++;

            if (tokens[i].ty != TK_NUM)
                error_at(tokens[i].input, "数ではありません");
            printf("    add rax, %d\n", tokens[i].val);

            i++;

            continue;
        }

        if (tokens[i].ty == '-') {
            i++;

            if (tokens[i].ty != TK_NUM)
                error_at(tokens[i].input, "数ではありません");

            printf("    sub rax, %d\n", tokens[i].val);

            i++;

            continue;
        }

        error_at(tokens[i].input, "予期しないトークンです");
    }

    printf("    ret\n");

    return 0;
}
