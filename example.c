#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "lexer.h"

 int main(int argc, char** argv) {
	if ( argc == 1){
		struct stat stats;
		fstat(fileno(stdin), &stats);	
		if (S_ISFIFO(stats.st_mode)) { // NOTE: read stdin when input is piped in
			Token tokens[MAX_TOKENS];
			size_t size = tokenize(tokens, stdin);
			printer(tokens, size);
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
			printer(tokens, size);
			putc('\n', stdout);
			}
		} else { // NOTE: read stdin when input is as a file (eg ./lexer < LICENCE)
			FILE* strm = stdin;
			fseek(strm, 0L, SEEK_END);
			int max_token_amount = ftell(strm);
			rewind(strm);
			Token tokens[max_token_amount];
		 	size_t size = tokenize(tokens, strm);
			printer(tokens, size);
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
		Token tokens[total];
		for (int ix=0; ix<idx;++ix) {
			int i = ids[ix];
			FILE* strm = fopen(argv[i], "r");
			Token tmp_tokens[sub[i-1]];
		 	size_t tmp_size = tokenize(tmp_tokens, strm);
			fclose(strm);
			total_tokens += tmp_size;
			printer(tmp_tokens, tmp_size);
		}
	}
}
