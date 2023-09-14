#include <tree_sitter/api.h>
#include <stdio.h>
#include <string.h>

extern TSLanguage *tree_sitter_c();

void print_tree(TSNode n, int level) {
    printf("%*s%s\n", level * 2, "", ts_node_type(n));

    for (uint32_t i = 0; i < ts_node_child_count(n); ++i) {
        print_tree(ts_node_child(n, i), level + 1);
    }
}

int main() {
    const char* str = R"(
int f(double a) {
    return a * a;
}
    )";

    TSParser* parser = ts_parser_new();
    ts_parser_set_language(parser, tree_sitter_c());

    TSTree* tree = ts_parser_parse_string(parser, NULL, str, strlen(str));

    TSNode node = ts_tree_root_node(tree);

    print_tree(node, 0);

    ts_parser_delete(parser);
}
