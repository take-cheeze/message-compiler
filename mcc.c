#include <tree_sitter/api.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern TSLanguage *tree_sitter_c();

const char* str = R"(
int f(double a) {
    return a * a + 0;
}

// test comment
)";

void print_tree(TSNode n, int level) {
    uint32_t start = ts_node_start_byte(n), end = ts_node_end_byte(n);
    printf("%*s%s (%d)\n%.*s\n", level * 2, "", ts_node_type(n), end - start, end - start, str + start);

    for (uint32_t i = 0; i < ts_node_child_count(n); ++i) {
        const char* field_name = ts_node_field_name_for_child(n, i);
        if (field_name) {
            printf("%*s%s:\n", level * 2 + 1, "", field_name);
        }
        print_tree(ts_node_child(n, i), level + 1);
    }
}

int main() {
    TSParser* parser = ts_parser_new();
    ts_parser_set_language(parser, tree_sitter_c());

    TSTree* tree = ts_parser_parse_string(parser, NULL, str, strlen(str));

    TSNode node = ts_tree_root_node(tree);

    print_tree(node, 0);

    char* s_expr = ts_node_string(node);
    printf("%s\n", s_expr);
    free(s_expr);

    ts_parser_delete(parser);
}
