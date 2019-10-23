/* 3SAT instance generator G2(n,m)	*/
/* created by M.Motoki */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdlib.h>  // rand(), srand()
#include <time.h>    // time()

void	gen_unique_instance(int, int);	/* generate 3CNF with at least 1 solution t*/
void	sat_alloc(int n, int m, int k);
void	write_sat(FILE *fp, int n, int m, int k);
void	shuffle_sat(int m, int k);
void 	genWeights(int mod, int count); /* generate weights line at random with modulus */

int	**v;	/* variable number in each clause */
int	**lit;	/* literal type in each clause */
int	*clause_size;	/* size of clause */
int	*t;	/* solution for formula generator */
int 	*weights;

int	main(int argc, char	*argv[])
{
	int	n;	/* # of variables */
	int	m;	/* # of clauses */
	int 	mod; /* weight modulus */
	int	i, h;	/* loop counter */
	int 	knownSolutionCost;
	time_t	ts;

	if(argc == 4)
	{
		n = atoi(argv[1]);
		m = atoi(argv[2]);
		mod = atoi(argv[3]);
	}
	else
	{
		fprintf(stderr, "%s #variables #clauses mod\n", argv[0]);
		exit(0);
	}
	
	ts = time(NULL);
	srand((int)ts);
	
	sat_alloc(n, m, 3);
	
	for(i = 0; i < n; i++)
	{
		t[i] = rand() % 2;
	}
	
	gen_unique_instance(n, m-n);
	for(i = m-n; i < m; i++)
	{
		clause_size[i] = 3;
		do
		{
			for(h = 0; h < 3; h++)
			{
				v[i][h] = rand() % n;
				lit[i][h] = !t[v[i][h]];
			}
			h = rand() % 3;
			v[i][h] = i - m + n;
			lit[i][h] = t[i - m + n];
		}while((v[i][0] == v[i][1]) || (v[i][1] == v[i][2]) || (v[i][2] == v[i][0]));
	}
	shuffle_sat(m, 3);
	/* alloc array and generate weights */
	weights = (int*) malloc(sizeof(int)*n);
	genWeights(mod,n);

	knownSolutionCost = 0;	
	fprintf(stdout, "c instance by G2\nc solution = ");
	for(i = 0; i < n; i ++){
		fprintf(stdout, "%d", t[i]);
		knownSolutionCost = knownSolutionCost + weights[i];
	}
	fprintf(stdout,"\n");
	fprintf(stdout,"c Known solution cost is %d \n",knownSolutionCost);
	write_sat(stdout, n, m, 3);
	
	/* free alocated mem */	
	free(weights);
	return 0;
}

/* memory allocation to matrixes s.t. n variables m clauses with at most size k */
void	sat_alloc(int n, int m, int k)
{
	int i;
	
	v = malloc(m * k * sizeof(int));
	if(v == NULL)
	{
		fprintf(stderr,"not enough memory\n");
		exit(1);
	}
	for(i = 0; i < m; i++)
	{
		v[i] = malloc(k * sizeof(int));
		if(v[i] == NULL)
		{
			fprintf(stderr,"not enough memory\n");
			exit(1);  
		}
	}

	lit = malloc(m * k * sizeof(int));
	if(lit == NULL)
	{
		fprintf(stderr,"not enough memory\n");
		exit(1);
	}
	for(i = 0; i < m; i++)
	{
		lit[i] = malloc(k * sizeof(int));
		if(lit[i] == NULL)
		{
			fprintf(stderr,"not enough memory\n");
			exit(1);  
		}
	}
	
	clause_size = malloc(m * sizeof(int));
	if(clause_size == NULL)
	{
		fprintf(stderr,"not enough memory\n");
		exit(1);
	}
	
	
	t = malloc(n * sizeof(int));
	if(t == NULL)
	{
		fprintf(stderr,"not enough memory\n");
		exit(1);
	}
	
	return;
}

/* generate specific count of variable weigthts at random */
void genWeights(int mod,int count){
	int i,rnd;
	for(i=0;i<count;i++){
		rnd = abs(rand()) % mod;
		weights[i] = rnd;
	}	
}

/* generate 3CNF with at least 1 solution (1^n) */
void	gen_unique_instance(int n, int m)
{
	int	i, h;	/* loop counter */
	
	for(i = 0; i < m; i++)
	{
		clause_size[i] = 3;
		do
		{
			for(h = 0; h < 3; h++)
			{
				v[i][h] = rand() % n;
				lit[i][h] = rand() % 2;
			}
		}while((v[i][0] == v[i][1]) || (v[i][1] == v[i][2]) || (v[i][2] == v[i][0]) || (lit[i][0] != t[v[i][0]] && lit[i][1] != t[v[i][1]] && lit[i][2] != t[v[i][2]]) );
	}
	return;
}

/* in DIMACS format with Weights for JCOP*/
void	write_sat(FILE *fp, int n, int m, int k)
{
	int	i, j, h; /* loop counter */

	/* config line */	
	fprintf(fp, "p cnf %d %d\n", n, m);

	/* weights */
	fprintf(fp,"w");
	for(i=0;i<n;i++){
		fprintf(fp," %d",weights[i]);
	}
	fprintf(fp,"\n");

	for(j = 0; j < m; j++)
	{
		for(h = 0; h < clause_size[j]; h++)
		{
			if(!lit[j][h])
				fprintf(fp, "-");
			fprintf(fp, "%d ", v[j][h]+1);
		}
		fprintf(fp, "0\n");
	}
	return;
}

void	shuffle_sat(int m, int k)
{
	int	tmp_clause_size;
	int	tmp_v;
	int	tmp_lit;
	int	tmp;
	int	j, h;
	
	for(j = m-1; j > 0; j--)
	{
		tmp = rand() % (j+1);
		tmp_clause_size = clause_size[j];
		clause_size[j] = clause_size[tmp];
		clause_size[tmp] = tmp_clause_size;
		for(h = 0; h < k; h++)
		{
			tmp_v = v[j][h];
			tmp_lit = lit[j][h];
			v[j][h] = v[tmp][h];
			lit[j][h] = lit[tmp][h];
			v[tmp][h] = tmp_v;
			lit[tmp][h] = tmp_lit;
		}
	}
	return;
}

