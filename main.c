
#include "9cc.h"

char *user_input;
int pos = 0;
Vector *token_vec;

int main(int argc, char **argv) {
    if (strstr(argv[1], "-test")) {
        runtest();

        return 0;
    }

    if (argc != 2) {
        fprintf(stderr, "引数の個数が正しくありません\n");
        return 1;
    }

    user_input = argv[1];
    tokenize();
    Node *node = expr();

    printf(".intel_syntax noprefix\n");
    printf(".global main\n");
    printf("main:\n");

    gen(node);

    printf("    pop rax\n");
    printf("    ret\n");

    return 0;
}
