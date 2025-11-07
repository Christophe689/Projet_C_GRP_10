#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "adj_list.h"

t_cell *create_cell(int dest, float proba) {
    t_cell *c = (t_cell*) malloc(sizeof(t_cell));
    if (!c) { perror("malloc"); exit(EXIT_FAILURE); }
    c->dest = dest;
    c->proba = proba;
    c->next = NULL;
    return c;
}

t_list create_empty_list(void) {
    t_list l; l.head = NULL; return l;
}

t_adj_list create_adj_list(int n) {
    t_adj_list g;
    g.size = n;
    g.lists = (t_list*) malloc(sizeof(t_list) * n);
    if (!g.lists) { perror("malloc"); exit(EXIT_FAILURE); }
    for (int i = 0; i < n; ++i) g.lists[i] = create_empty_list();
    return g;
}

void free_adj_list(t_adj_list *g) {
    if (!g || !g->lists) return;
    for (int i = 0; i < g->size; ++i) {
        t_cell *cur = g->lists[i].head;
        while (cur) {
            t_cell *tmp = cur; cur = cur->next; free(tmp);
        }
    }
    free(g->lists); g->lists = NULL; g->size = 0;
}

void add_edge_to_list(t_list *l, int dest, float proba) {
    t_cell *c = create_cell(dest, proba);
    c->next = l->head; l->head = c;
}

void display_list(const t_list *l) {
    const t_cell *cur = l->head;
    while (cur) {
        printf("(%d, %.2f) -> ", cur->dest, cur->proba);
        cur = cur->next;
    }
    printf("NULL\n");
}

void display_adj_list(const t_adj_list *g) {
    for (int i = 0; i < g->size; ++i) {
        printf("Liste sommet %d: ", i+1);
        display_list(&g->lists[i]);
    }
}

t_adj_list readGraph(const char *filename) {
    FILE *file = fopen(filename, "rt");
    if (!file) { perror("open input"); exit(EXIT_FAILURE); }
    int nbvert;
    if (fscanf(file, "%d", &nbvert) != 1) {
        fprintf(stderr, "format: expected N\n"); fclose(file); exit(EXIT_FAILURE);
    }
    t_adj_list g = create_adj_list(nbvert);
    int depart, arrivee; float proba;
    while (fscanf(file, "%d %d %f", &depart, &arrivee, &proba) == 3) {
        if (depart < 1 || depart > nbvert || arrivee < 1 || arrivee > nbvert) continue;
        add_edge_to_list(&g.lists[depart-1], arrivee, proba);
    }
    fclose(file);
    return g;
}

int is_markov_graph(const t_adj_list *g) {
    int ok = 1;
    for (int i = 0; i < g->size; ++i) {
        double sum = 0.0;
        for (t_cell *cur = g->lists[i].head; cur; cur = cur->next) sum += cur->proba;
        if (!(sum >= 0.99 && sum <= 1.0 + 1e-6)) {
            printf("Somme sortante sommet %d = %.6f (attendu ~1)\n", i+1, sum);
            ok = 0;
        }
    }
    if (ok) printf("OK: graphe de Markov\n");
    return ok;
}

char *getId(int num) {
    char tmp[32]; int pos = 0; int n = num;
    while (n > 0) { int r = (n-1)%26; tmp[pos++] = 'A'+r; n = (n-1)/26; }
    char *res = (char*) malloc(pos+1);
    for (int i = 0; i < pos; ++i) res[i] = tmp[pos-1-i];
    res[pos] = '\0'; return res;
}

int write_mermaid(const t_adj_list *g, const char *out_filename) {
    FILE *f = fopen(out_filename, "wt");
    if (!f) { perror("open mermaid"); return 0; }
    fprintf(f, "---\nconfig:\n   layout: elk\n   theme: neo\n   look: neo\n---\n\n");
    fprintf(f, "flowchart LR\n");
    for (int i = 0; i < g->size; ++i) {
        char *id = getId(i+1);
        fprintf(f, "%s((%d))\n", id, i+1);
        free(id);
    }
    fprintf(f, "\n");
    for (int i = 0; i < g->size; ++i) {
        for (t_cell *cur = g->lists[i].head; cur; cur = cur->next) {
            char *src = getId(i+1), *dst = getId(cur->dest);
            fprintf(f, "%s -->|%.2f|%s\n", src, cur->proba, dst);
            free(src); free(dst);
        }
    }
    fclose(f); return 1;
}
