#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define EPSILON 'z'



struct p_rules {
	int n_rules;
	char **rules;
};

int indexOf(char ip, char symbol[]) {
	if (ip == '$') return strlen(symbol);
	for(int i = 0; i < strlen(symbol); i++)
		if(symbol[i]==ip)
			return i;
	return -1;
}

char *substr(char *s, int l, int r) {
	int len = (r - l), i;
	char *sub = malloc((len + 1) * sizeof(char));
	for (i = 0; i < len; i++) sub[i] = s[i + l];
	sub[len] = '\0';
	return sub;
}

int main(void) {
	int n_terminals, n_non_terminals, n_production_rules;
	int i, j, k, l, l1, bita;
	char *terminals, *non_terminals, start_symbol;
	struct p_rules *production_rules;
	char s[50], nt;

	// Terminals
	printf("Enter number of terminals including epsilon (denote epsilon by 'e'): ");
	scanf("%d", &n_terminals);
	terminals = malloc((n_terminals + 1) * sizeof(char));
	for (i = 0; i < n_terminals; ++i) {
		printf("Enter terminal symbol %d: ", (i + 1));
		scanf(" %c", terminals + i);
	}

	// Non terminals
	printf("Enter number of non terminals: ");
	scanf("%d", &n_non_terminals);
	non_terminals = malloc((n_non_terminals + 1) * sizeof(char));
	for (i = 0; i < n_non_terminals; ++i) {
		printf("Enter non terminal symbol %d: ", (i + 1));
		scanf(" %c", non_terminals + i);
	}

	// Production rules
	production_rules = malloc(n_non_terminals * sizeof(struct p_rules));
	for (i = 0; i < n_non_terminals; i++) {
		printf("Number of production rules for %c: ", non_terminals[i]);
		scanf("%d", &k);
		production_rules[i].n_rules = k;
		production_rules[i].rules = malloc(k * sizeof(char*));
		for (j = 0; j < k; j++) {
			production_rules[i].rules[j] = malloc(10 * sizeof(char));
			printf("%c -> ", non_terminals[i]);
			scanf("%s", production_rules[i].rules[j]);
		}
	}

	// Start symbol
	printf("Enter the start symbol: ");
	scanf(" %c", &start_symbol);

	// Print the grammar in the form of (V, E, P, S)
	printf("\nThe grammar is ({");
	for (i = 0; i < n_non_terminals - 1; i++) printf("%c, ", non_terminals[i]);
	printf("%c}, {", non_terminals[i]);
	for (i = 0; i < n_terminals - 1; i++) printf("%c, ", terminals[i]);
	printf("%c}, {", terminals[i]);
	for(i = 0; i < n_non_terminals - 1; i++) {
		printf("%c -> ", non_terminals[i]);
		for(j = 0; j < production_rules[i].n_rules - 1; j++) printf("%s|", production_rules[i].rules[j]);
		printf("%s, ", production_rules[i].rules[j]);
	}
	printf("%c -> ", non_terminals[i]);
	for (j = 0; j < production_rules[i].n_rules - 1; j++) printf("%s|", production_rules[i].rules[j]);
	printf("%s}, %c)\n", production_rules[i].rules[j], start_symbol);

	//first table
	int first[n_non_terminals][n_terminals];
	memset(first, 0, n_non_terminals*n_terminals*sizeof(int));
	int flag, flag2, z, e_index = indexOf(EPSILON,terminals);;

	//calculations for first table
	do {
		flag = 0;
		// For each non terminal
		for (i = 0; i < n_non_terminals; i++) {
			struct p_rules rules = production_rules[i], temp_rules;
			// For each production rule
			for (j = 0; j < rules.n_rules; j++) {
				z = 0;
				next_pos: k = indexOf(rules.rules[j][z], terminals);
				if (k >= 0) {
					// Terminal
					if (first[i][k] == 0) {
						first[i][k] = flag = 1;
					}
				} else {
					// Non terminal
					k = indexOf(rules.rules[j][z], non_terminals);
					temp_rules = production_rules[k];
					for (l = 0, flag2 = 0; l < temp_rules.n_rules; l++) {
						if (temp_rules.rules[l][0] == EPSILON) {
							flag2 = 1;
							break;
						}
					}
					if (flag2) {
						// Epsilon in 'k'
						for (l = 0; l < n_terminals; l++) {
							if ((first[i][l] == 0) && (first[k][l] == 1)) {
								first[i][l] = flag = 1;
							}
						}

						if (z < strlen(rules.rules[j])) {
							z++;
							goto next_pos;
						}
					} else {
						// Epsilon is not in 'k'
						for (l = 0; l < n_terminals; l++) {
							if ((first[i][l] == 0) && (first[k][l] == 1)) {
								first[i][l] = flag = 1;
							}
						}
					}
				}
			}
		}
	} while(flag);

	printf("\nFirst Table:\n");

	/*for (i = 0; i < n_terminals; i++) {
		printf("\t%c", terminals[i]);
	}
	printf("\n");
	for (i = 0; i < n_non_terminals; i++) {
		printf("%c:\t", non_terminals[i]);
		for (j = 0; j < n_terminals; j++) {
			printf("%d\t", first[i][j]);
		}
		printf("\n");
	}
	printf("\n");*/
	for (i = 0; i < n_non_terminals; i++) {
		printf("%c: ", non_terminals[i]);
		for (j = 0; j < n_terminals; j++) {
			if(first[i][j])
				printf("%c ", terminals[j]);
		}
		printf("\n");
	}

	//follow table
	int follow[n_non_terminals][n_terminals + 1];
	memset(follow, 0, n_non_terminals * (n_terminals + 1) * sizeof(int));
	follow[indexOf(start_symbol, non_terminals)][n_terminals] = 1;
	char *sub1, *sub2;

	//calculations for follow table
	do {
		flag = 0;
		for (i = 0; i < n_non_terminals; i++) {
			struct p_rules rules = production_rules[i], temp_rules;
			// For each production rule
			for (j = 0; j < rules.n_rules; j++) {
				// For each character in the rule
				for (k = 0; k < strlen(rules.rules[j]); k++) {
					z = indexOf(rules.rules[j][k], non_terminals);
					if (z < 0) continue;

					// kth element is non terminal
					sub1 = substr(rules.rules[j], k + 1, strlen(rules.rules[j]));
					if (strlen(sub1) <= 0) {
						// follow[i] -> follow[z];
						for (l = 0; l < n_terminals + 1; l++) {
							if (follow[i][l] == 1 && follow[z][l] == 0) follow[z][l] = flag = 1;
						}
					}
					else{
						flag2 = 0;
						for (l = 0; l < strlen(sub1); l++) {
							// first(sub1[l]) -> follow(z)
							//terminal
							if((bita = indexOf(sub1[l],terminals)) >= 0 && l != e_index && follow[z][bita] == 0){
								follow[z][bita] = flag = 1;
								break;
							}
							//non_terminal
							else if((bita = indexOf(sub1[l],non_terminals)) >= 0){
								for (l1 = 0; l1 < n_terminals; l1++) {
									if(l1 == e_index){
										if( first[bita][l1] == 1) flag2 = 1; // epsilon exist
										continue;
									}
									if (first[bita][l1] == 1 && follow[z][l1] == 0) follow[z][l1] = flag = 1;
								}
								if(flag2 == 1){
									for (l1 = 0; l1 < n_terminals + 1; l1++)
										if (follow[i][l1] == 1 && follow[z][l1] == 0) follow[z][l1] = flag = 1;
								}
								else break;
							}
						}
					}
				}
			}
		}
	} while (flag);


	printf("\nFollow Table:\n");
	/*for (i = 0; i < n_terminals; i++) {
		if(i == e_index) continue;
		printf("\t%c", terminals[i]);
	}
	printf("\t$\n");
	for (i = 0; i < n_non_terminals; i++) {
		printf("%c:\t", non_terminals[i]);
		for (j = 0; j < n_terminals + 1; j++) {
			if(j == e_index) continue;
			printf("%d\t", follow[i][j]);
		}
		printf("\n");
	}*/

	
	for (i = 0; i < n_non_terminals; i++) {
		printf("%c: ", non_terminals[i]);
		for (j = 0; j < n_terminals; j++) {
			if(follow[i][j])
				printf("%c ", terminals[j]);
		}
		if(follow[i][n_terminals]) printf("$");
		printf("\n");
	}


	printf("\nProduction Rules:\n");
	for(i = 0; i < n_non_terminals - 1; i++) {
		printf("%c -> ", non_terminals[i]);
		for(j = 0; j < production_rules[i].n_rules - 1; j++) printf("%s|", production_rules[i].rules[j]);
		printf("%s, ", production_rules[i].rules[j]);
	}
	printf("%c -> ", non_terminals[i]);
	for (j = 0; j < production_rules[i].n_rules - 1; j++) printf("%s|", production_rules[i].rules[j]);
	printf("%s\n", production_rules[i].rules[j]);


	char *t = "\0";
	//parse table
	char* parse_table[n_non_terminals][n_terminals + 1];
	for(i = 0; i < n_non_terminals; i++)
		for(j = 0; j < n_terminals + 1; j++)
			parse_table[i][j] = t;

	//calculations for parse table
	for(i = 0; i < n_non_terminals; i++){
		for(j = 0; j < production_rules[i].n_rules; j++){
			char* grammar = production_rules[i].rules[j];
			// For each character in the rule
			for (k = 0; k < strlen(grammar); k++) {
				z = indexOf(grammar[k], terminals);
				if (z >= 0) {
					if(z == e_index){
						for(l = 0; l < n_terminals + 1; l++){
							if(follow[i][l]){
								if(strstr(parse_table[i][l] , t)!=NULL){
									parse_table[i][l] = grammar;
								}
								else{
									strcat(parse_table[i][l] ," ");
									strcat(parse_table[i][l] , grammar);
								}
							}
						}
						continue;
					}
					parse_table[i][z] = grammar;
					break;
				} else { //non_terminal
					flag2 = 0;
					z = indexOf(grammar[k], non_terminals);
					for(l = 0; l < n_terminals; l++){
						if(l == e_index){
							if(first[z][l]) flag2 = 1; // epsilon exist
							continue;
						}
						if(first[z][l])
							parse_table[i][l] = grammar;
					}
					if(flag2){
						for(l = 0; l < n_terminals + 1; l++){
							if(follow[i][l]){
								parse_table[i][l] = grammar;
							}
						}
					}
					else break;
				}
			}
		}
	}

	printf("\nParsing Table:\n");
	for (i = 0; i < n_terminals; i++) {
		if(i == e_index) continue;
		printf("\t%c\t|", terminals[i]);
	}
	printf("\t$\t|\n");
	for (i = 0; i < n_non_terminals; i++) {
		printf("%c:\t", non_terminals[i]);
		for (j = 0; j < n_terminals + 1; j++) {
			if(j == e_index) continue;
			if(strlen(parse_table[i][j]))
				printf("%c->%s\t|\t",non_terminals[i],parse_table[i][j]);
			else printf("%s\t|\t", parse_table[i][j]);
		}
		printf("\n");
	}

	// Free the memory
	free(terminals);
	free(non_terminals);
	for (i = 0; i < n_non_terminals; i++) {
		for (j = 0; j < production_rules[i].n_rules; j++) free(production_rules[i].rules[j]);
		free(production_rules[i].rules);
	}
	free(production_rules);
	return 0;
}
