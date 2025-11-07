#include <stdio.h>
#include <stdlib.h>
#include "adj_list.h"

int main(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s input.txt output_mermaid.txt\n", argv[0]);
        return EXIT_FAILURE;
    }
    t_adj_list g = readGraph(argv[1]);
    display_adj_list(&g);
    is_markov_graph(&g);
    if (!write_mermaid(&g, argv[2])) {
        fprintf(stderr, "Erreur écriture mermaid\n");
        free_adj_list(&g);
        return EXIT_FAILURE;
    }
    free_adj_list(&g);
    return EXIT_SUCCESS;
}
