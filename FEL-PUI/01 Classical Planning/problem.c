#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include "problem.h"

static void readSet(FILE *fin, const char *path, int *size, int **values)
{
    if (fscanf(fin, "%d", size) != 1){
        fprintf(stderr, "Error: Invalid STRIPS format in `%s'\n", path);
        exit(-1);
    }
    *values = (int *)malloc(sizeof(int) * *size);
    for (int i = 0; i < *size; ++i){
        if (fscanf(fin, "%d", (*values) + i) != 1){
            fprintf(stderr, "Error: Invalid STRIPS format in `%s'\n", path);
            exit(-1);
        }
    }
}

void stripsRead(strips_t *strips, const char *path)
{
    FILE *fin;
    char name[256];

    bzero(strips, sizeof(*strips));
    fin = fopen(path, "r");
    if (fin == NULL){
        fprintf(stderr, "Error: Could not read `%s'\n", path);
        exit(-1);
    }

    if (fscanf(fin, "%d", &strips->num_facts) != 1){
        fprintf(stderr, "Error: Invalid STRIPS format in `%s'\n", path);
        exit(-1);
    }

    strips->fact_names = (char **)malloc(sizeof(char *) * strips->num_facts);
    for (int i = 0; i < strips->num_facts; ++i){
        if (fscanf(fin, "\n%[^\n]", name) != 1){
            fprintf(stderr, "Error: Invalid STRIPS format in `%s'\n", path);
            exit(-1);
        }
        strips->fact_names[i] = (char *)malloc(strlen(name) + 1);
        memcpy(strips->fact_names[i], name, strlen(name) + 1);
    }

    readSet(fin, path, &strips->init_size, &strips->init);
    readSet(fin, path, &strips->goal_size, &strips->goal);

    if (fscanf(fin, "%d", &strips->num_operators) != 1){
        fprintf(stderr, "Error: Invalid STRIPS format in `%s'\n", path);
        exit(-1);
    }

    strips->operators = (strips_operator_t *)malloc(sizeof(strips_operator_t)
                                                      * strips->num_operators);
    for (int i = 0; i < strips->num_operators; ++i){
        strips_operator_t *op = strips->operators + i;
        if (fscanf(fin, "\n%[^\n]", name + 1) != 1){
            fprintf(stderr, "Error: Invalid STRIPS format in `%s'\n", path);
            exit(-1);
        }
        name[0] = '(';
        int len = strlen(name);
        name[len] = ')';
        name[len + 1] = 0x0;
        op->name = (char *)malloc(strlen(name) + 1);
        memcpy(op->name, name, strlen(name) + 1);
        readSet(fin, path, &op->pre_size, &op->pre);
        readSet(fin, path, &op->add_eff_size, &op->add_eff);
        readSet(fin, path, &op->del_eff_size, &op->del_eff);
        if (fscanf(fin, "%d", &op->cost) != 1){
            fprintf(stderr, "Error: Invalid STRIPS format in `%s'\n", path);
            exit(-1);
        }
    }

    fclose(fin);
}

void stripsFree(strips_t *strips)
{
    for (int i = 0; i < strips->num_facts; ++i){
        if (strips->fact_names[i] != NULL)
            free(strips->fact_names[i]);
    }
    if (strips->fact_names != NULL)
        free(strips->fact_names);

    for (int i = 0; i < strips->num_operators; ++i){
        strips_operator_t *op = strips->operators + i;
        if (op->name != NULL)
            free(op->name);
        if (op->pre != NULL)
            free(op->pre);
        if (op->add_eff != NULL)
            free(op->add_eff);
        if (op->del_eff != NULL)
            free(op->del_eff);
    }
    if (strips->operators != NULL)
        free(strips->operators);
}

static void readPartState(FILE *fin, const char *path, fdr_part_state_t *ps)
{
    if (fscanf(fin, "%d", &ps->size) != 1){
        fprintf(stderr, "Error: Invalid STRIPS format in `%s'\n", path);
        exit(-1);
    }
    ps->facts = (fdr_fact_t *)malloc(sizeof(fdr_fact_t) * ps->size);
    for (int i = 0; i < ps->size; ++i){
        fdr_fact_t *f = ps->facts + i;
        if (fscanf(fin, "%d %d", &f->var, &f->val) != 2){
            fprintf(stderr, "Error: Invalid STRIPS format in `%s'\n", path);
            exit(-1);
        }
    }
}

void fdrRead(fdr_t *fdr, const char *path)
{
    FILE *fin;
    char name[256];

    bzero(fdr, sizeof(*fdr));
    fin = fopen(path, "r");
    if (fin == NULL){
        fprintf(stderr, "Error: Could not read `%s'\n", path);
        exit(-1);
    }

    if (fscanf(fin, "%d", &fdr->num_vars) != 1){
        fprintf(stderr, "Error: Invalid FDR format in `%s'\n", path);
        exit(-1);
    }

    fdr->vars = (fdr_var_t *)malloc(sizeof(fdr_var_t) * fdr->num_vars);
    for (int i = 0; i < fdr->num_vars; ++i){
        fdr_var_t *var = fdr->vars + i;
        if (fscanf(fin, "%d", &var->range) != 1){
            fprintf(stderr, "Error: Invalid FDR format in `%s'\n", path);
            exit(-1);
        }
        var->value_names = (char **)malloc(sizeof(char *) * var->range);
        for (int j = 0; j < var->range; ++j){
            if (fscanf(fin, "\n%[^\n]", name) != 1){
                fprintf(stderr, "Error: Invalid FDR format in `%s'\n", path);
                exit(-1);
            }
            var->value_names[j] = (char *)malloc(strlen(name) + 1);
            memcpy(var->value_names[j], name, strlen(name) + 1);
        }
    }

    fdr->init = (int *)malloc(sizeof(int) * fdr->num_vars);
    for (int i = 0; i < fdr->num_vars; ++i){
        if (fscanf(fin, "%d", fdr->init + i) != 1){
            fprintf(stderr, "Error: Invalid FDR format in `%s'\n", path);
            exit(-1);
        }
    }

    readPartState(fin, path, &fdr->goal);

    if (fscanf(fin, "%d", &fdr->num_operators) != 1){
        fprintf(stderr, "Error: Invalid FDR format in `%s'\n", path);
        exit(-1);
    }

    fdr->operators = (fdr_operator_t *)malloc(sizeof(fdr_operator_t)
                                                * fdr->num_operators);
    for (int i = 0; i < fdr->num_operators; ++i){
        fdr_operator_t *op = fdr->operators + i;

        if (fscanf(fin, "\n%[^\n]", name + 1) != 1){
            fprintf(stderr, "Error: Invalid FDR format in `%s'\n", path);
            exit(-1);
        }
        name[0] = '(';
        int len = strlen(name);
        name[len] = ')';
        name[len + 1] = 0x0;
        op->name = (char *)malloc(strlen(name) + 1);
        op->name = (char *)malloc(strlen(name) + 1);
        memcpy(op->name, name, strlen(name) + 1);

        readPartState(fin, path, &op->pre);
        readPartState(fin, path, &op->eff);

        if (fscanf(fin, "%d", &op->cost) != 1){
            fprintf(stderr, "Error: Invalid FDR format in `%s'\n", path);
            exit(-1);
        }
    }

    fclose(fin);
}

void fdrFree(fdr_t *fdr)
{
    for (int i = 0; i < fdr->num_vars; ++i){
        for (int j = 0; j < fdr->vars[i].range; ++j)
            free(fdr->vars[i].value_names[j]);
        if (fdr->vars[i].value_names != NULL)
            free(fdr->vars[i].value_names);
    }
    if (fdr->vars != NULL)
        free(fdr->vars);
    if (fdr->init != NULL)
        free(fdr->init);
    if (fdr->goal.facts != NULL)
        free(fdr->goal.facts);

    for (int i = 0; i < fdr->num_operators; ++i){
        fdr_operator_t *op = fdr->operators + i;
        if (op->name != NULL)
            free(op->name);
        if (op->pre.facts != NULL)
            free(op->pre.facts);
        if (op->eff.facts != NULL)
            free(op->eff.facts);
    }
    if (fdr->operators != NULL)
        free(fdr->operators);
}
