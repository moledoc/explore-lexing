#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "lexer.h"

typedef enum {
	CLEAR_STATE = 0, 
	COLON_STATE,
} STATE;

void formatter(Token* tokens, size_t i) {
	int depth = 0;
	STATE state = 0;
	Token prev;
	int k = 0;
	char null[MAX_STR];
	while (1) {
	// for (int j=0; j<i;++j) {
		switch (tokens->t){
		case CURLYL:
			if ( prev.t == 0 || state == COLON_STATE) {printf("%c\n", tokens->t); ++depth; state = CLEAR_STATE; break;}
			for (int j=0;j<depth;++j) putc(TAB, stdout);
			printf("%c\n", tokens->t);
			++depth;
			break;
		case CURLYR:
			putc(NEWLINE, stdout);
			--depth;
			for (int j=0; j<depth; ++j) putc(TAB, stdout);
			printf("%c", tokens->t);
			break;
		case BRACKETL:
			printf("%c\n", tokens->t);
			++depth;
			if (state == COLON_STATE) state = CLEAR_STATE;
			break;
		case BRACKETR:
			putc(NEWLINE, stdout);
			--depth;
			for (int j=0; j<depth; ++j) putc(TAB, stdout);
			printf("%c", tokens->t);
			break;
		case COMMA:
			printf("%c\n", tokens->t);
			if (state == COLON_STATE) state = CLEAR_STATE;
			break;
		case COLON:
			printf("%c ", tokens->t);
			state = COLON_STATE;
			break;
		case STRING:
			if ( state != COLON_STATE) for (int j=0; j<depth; ++j) putc(TAB, stdout);
			printf("%s", tokens->v);
			state = CLEAR_STATE;
			break;
		case URL:
			if ( state != COLON_STATE) for (int j=0; j<depth; ++j) putc(TAB, stdout);
			printf("\"%s\"", tokens->v);
			state = CLEAR_STATE;
			break;
		case INT:
			if ( state != COLON_STATE) for (int j=0; j<depth; ++j) putc(TAB, stdout);
			printf("%s", tokens->v);
			state = CLEAR_STATE;
			break;
		case FLOAT:
			if ( state != COLON_STATE) for (int j=0; j<depth; ++j) putc(TAB, stdout);
			printf("%s", tokens->v);
			state = CLEAR_STATE;
			break;
		case NUMBER:
			if ( state != COLON_STATE) for (int j=0; j<depth; ++j) putc(TAB, stdout);
			printf("\"%s\"", tokens->v);
			state = CLEAR_STATE;
			break;
		case WORD:
			// IMPROVEME:
			if (tokens->v[0] == 'n' && tokens->v[1] == 'u' && tokens->v[2] == 'l' && tokens->v[3] == 'l' && tokens->v[4] == '\0') printf("%s", tokens->v);
			break;
		case BOOLEAN:
			if ( state != COLON_STATE) for (int j=0; j<depth; ++j) putc(TAB, stdout);
			printf("%s", tokens->v);
			state = CLEAR_STATE;
			break;
		}
		prev = *tokens;
		++tokens;
		++k;
		if (k == i) break;
	}
	putchar('\n');
}


int main(int argc, char *argv[]) {
	if ( argc == 1){
		struct stat stats;
		fstat(fileno(stdin), &stats);	
		if (S_ISFIFO(stats.st_mode)) { // NOTE: read stdin when input is piped in
			Token tokens[MAX_TOKENS];
			size_t size = tokenize(tokens, stdin);
			formatter(tokens, size);
		} else if (S_ISCHR(stats.st_mode)) { // NOTE: this block is basically REPL
			while (1) {
			int bufsize = MAX_STR;
			char tst[bufsize];
			fgets(tst, bufsize, stdin);

			FILE* file = fopen("/tmp/stream", "w");
			if (file == NULL) {printf("file was null\n"); exit(1);}
			fputs(tst, file);
			Token tokens[bufsize];
			fclose(file);

			file = fopen("/tmp/stream", "r");
			size_t size = tokenize(tokens, file);
			fclose(file);
			formatter(tokens, size);
			putc('\n', stdout);
			}
		} else { // NOTE: read stdin when input is as a file (eg ./lexer < LICENCE)
			FILE* strm = stdin;
			fseek(strm, 0L, SEEK_END);
			int max_token_amount = ftell(strm);
			rewind(strm);
			Token tokens[max_token_amount];
		 	size_t size = tokenize(tokens, strm);
			formatter(tokens, size);
		}
	} else { // NOTE: input is read from files that are provided as args
		size_t total;
		size_t sub[argc-1];
		int ids[argc-1];
		int idx = 0;
		for (int i=1;i<argc;++i) {
			FILE* strm = fopen(argv[i], "r");
			if (strm == NULL) {
				printf("SKIP '%s': no such file\n", argv[i]);
				continue;
			}
			fseek(strm, 0L, SEEK_END);
			int max_token_amount = ftell(strm);
			fclose(strm);
			total += max_token_amount;
			sub[idx] = max_token_amount;
			ids[idx] = i;
			++idx;
		}

		if (total == 0) {
			printf("Nothing to tokenize\n");
			return 0;
		}

		size_t total_tokens;
		// Token tokens[total];
		Token tokens[MAX_TOKENS];
		for (int ix=0; ix<idx;++ix) {
			int i = ids[ix];
			FILE* strm = fopen(argv[i], "r");
			// Token tmp_tokens[sub[i-1]];
			Token tmp_tokens[MAX_TOKENS];
		 	size_t tmp_size = tokenize(tmp_tokens, strm);
			fclose(strm);
			total_tokens += tmp_size;
			formatter(tmp_tokens, tmp_size);
		}
	}
}