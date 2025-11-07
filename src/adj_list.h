#ifndef ADJ_LIST_H
#define ADJ_LIST_H

#include <stdio.h>

typedef struct s_cell {
    int dest;      /* sommet d'arrivée */
    float proba;   /* poids / probabilité */
    struct s_cell *next;
} t_cell;

typedef struct s_list {
    t_cell *head;
} t_list;

typedef struct s_adj_list {
    int size;     /* nombre de sommets */
    t_list *lists;/* tableau de listes (index 0 -> sommet 1) */
} t_adj_list;

/* création / destruction */
t_cell *create_cell(int dest, float proba);
t_list create_empty_list(void);
t_adj_list create_adj_list(int n);
void free_adj_list(t_adj_list *g);

/* modifications / affichage */
void add_edge_to_list(t_list *l, int dest, float proba);
void display_list(const t_list *l);
void display_adj_list(const t_adj_list *g);

/* lecture / validation / export mermaid */
t_adj_list readGraph(const char *filename);
int is_markov_graph(const t_adj_list *g);
int write_mermaid(const t_adj_list *g, const char *out_filename);

/* utilitaire identifiant A,B,...,Z,AA,... */
char *getId(int num);

#endif
