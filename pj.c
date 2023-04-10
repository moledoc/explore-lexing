#include <stdio.h>
#include <string.h>
#include "lexer.h"

typedef enum {
	CLEAR_STATE = 0, 
	STRING_STATE,
	COLON_STATE,
	LIST_STATE,
} STATE;

void format(Token* tokens, size_t i) {
	int depth = 0;
	STATE state = 0;
	for (int j=0; j<i;++j) {
		switch (tokens->token) {
			case CURLYL:
				if (state != COLON_STATE) {
					for (int i=0;i<depth;i++) printf("\t");
				}
				if (state != STRING_STATE) state = CLEAR_STATE;
				printf("%c\n",tokens->token);
				depth++;
				break;
			case CURLYR:
				if (state != STRING_STATE) state = CLEAR_STATE;
				depth--;
				putchar('\n');
				for (int i=0;i<depth;i++) printf("\t");
				printf("%c",tokens->token);
				break;
			case BRACKETL:
				if (state != STRING_STATE) state = LIST_STATE;
				printf("%c\n",tokens->token);
				depth++;
				break;
			case BRACKETR:
				if (state != STRING_STATE) state = CLEAR_STATE;
				depth--;
				putchar('\n');
				for (int i=0;i<depth;i++) printf("\t");
				printf("%c",tokens->token);
				break;
			case COMMA:
				if (state != STRING_STATE) state = CLEAR_STATE;
				printf("%c\n", tokens->token);
				break;
			case COLON:
				printf("%c", tokens->token);
				if (state != STRING_STATE) {
					state = COLON_STATE;
					putchar(' ');
				}
				break;
			case INT:
				if (state != COLON_STATE && state != STRING_STATE) {
					for (int i=0;i<depth;i++) printf("\t");
				} else {
					if (state != STRING_STATE) state = CLEAR_STATE;
				}
				printf("%s", tokens->val);
				break;
			case FLOAT:
				if (state != COLON_STATE && state != STRING_STATE) {
					for (int i=0;i<depth;i++) printf("\t");
				} else {
					if (state != STRING_STATE) state = CLEAR_STATE;
				}
				printf("%s", tokens->val);
				break;
			case DQUOTE:
				if (state == CLEAR_STATE) {
					for (int i=0;i<depth;i++) printf("\t");
					state = STRING_STATE;
				} else if (state == STRING_STATE) {
					state = CLEAR_STATE;
				} else if (state == COLON_STATE) {
					state = STRING_STATE;
				}
				printf("%c", tokens->token);
				break;
			case WORD:
				printf("%s", tokens->val);
				break;
			default:
				if (state == STRING_STATE) printf("%c", tokens->token);
		}
		tokens++;
	}
	putchar('\n');
}


void from_file(char filepath[]) {
	FILE *fp = fopen(filepath, "r");

	fseek(fp, 0L, SEEK_END);
	int size = ftell(fp);
	fclose(fp);

	Token tokens[size];

	size_t i = tokenize_file(tokens, filepath);
	format(tokens, i);
}

void from_stdin() {
	Token tokens[1024*10];
	size_t i = tokenize_stdin(tokens);
	format(tokens, i);
	// printer(tokens, i);
}

int main(int argc, char *argv[]) {
	const char * delim = "=";
	char* filepath;
	for( int i = 1; i < argc; ++i ) {
		char* elems = strtok( argv[i], delim);
		elems = strtok(NULL, delim);
		if (strcmp(elems,"--file") ){
			filepath = elems;
		}
	}
	(sizeof(filepath) > 0) ? from_file(filepath) : from_stdin();
}