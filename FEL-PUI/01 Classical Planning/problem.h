#ifndef _PROBLEM_H_
#define _PROBLEM_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

struct strips_operator {
    char *name;
    int cost;
    int *pre;
    int pre_size;
    int *add_eff;
    int add_eff_size;
    int *del_eff;
    int del_eff_size;
};
typedef struct strips_operator strips_operator_t;

struct strips {
    int num_facts;
    char **fact_names;
    int *init;
    int init_size;
    int *goal;
    int goal_size;
    int num_operators;
    strips_operator_t *operators;
};
typedef struct strips strips_t;

void stripsRead(strips_t *strips, const char *path);
void stripsFree(strips_t *strips);


struct fdr_var {
    int range; /*!< Number of values of the variable */
    char **value_names; /*!< Name of each value */
};
typedef struct fdr_var fdr_var_t;

struct fdr_fact {
    int var;
    int val;
};
typedef struct fdr_fact fdr_fact_t;

struct fdr_part_state {
    int size;
    fdr_fact_t *facts;
};
typedef struct fdr_part_state fdr_part_state_t;

struct fdr_operator {
    char *name;
    int cost;
    fdr_part_state_t pre;
    fdr_part_state_t eff;
};
typedef struct fdr_operator fdr_operator_t;

struct fdr {
    int num_vars;
    fdr_var_t *vars;
    int *init;
    fdr_part_state_t goal;
    int num_operators;
    fdr_operator_t *operators;
};
typedef struct fdr fdr_t;

void fdrRead(fdr_t *fdr, const char *path);
void fdrFree(fdr_t *fdr);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _PROBLEM_H_ */
