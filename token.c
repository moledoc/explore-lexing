# include <stdio.h>
# include <stdint.h>
# include <stdlib.h>
# include <string.h>

// test t2s; 1.24, 1,312
#define MAX 1024*10 // TODO: make more robust
#define MAX_STR 100 // TODO: make more robust

typedef enum {
	STRING = 0,
	INT = 1,
	FLOAT= 2,
	TILDE = '~',
	TICK = '`',
	EXCLAIM = '!',
	AT = '@',
	DQUOTE = '"',
	QUOTE = '\'',
	HASH = '#',
	DOLLAR = '$',
	PERCENT = '%',
	CARET = '^',
	AMPER = '&',
	STAR = '*',
	SLASH = '/',
	BSLASH = '\\',
	CURLYL = '{',
	CURLYR = '}',
	PARENL = '(',
	PARENR = ')',
	BRACKETL = '[',
	BRACKETR = ']',
	ANGLEL = '<',
	ANGLER = '>',
	EQUAL = '=',
	DASH = '-',
	UNDERSCORE = '_',
	PLUS = '+',
	QUESTION = '?',
	SCOLON = ';',
	COLON = ':',
	PIPE = '|',
	SPACE = ' ',
	TAB = '\t',
	NEWLINE = '\n',
} TOKEN;

typedef struct {
	TOKEN token;
	char val[MAX_STR];
} MyToken;

void t2s(MyToken t) {
	switch (t.token) {
		case STRING:
			printf("STRING(%s)\n", t.val);
			break;
		case INT:
			printf("INT(%s)\n",t.val);
			break;
		case FLOAT:
			printf("FLOAT(%s)\n",t.val);
			break;
		case TAB:
			printf("SYMOBL(\\t)\n");
			break;
		case NEWLINE:
			printf("SYMBOL(\\n)\n");
			break;
		case SPACE:
			printf("SYMBOL(' ')\n");
			break;
		default:
			printf("SYMBOL(%s)\n", t.val);
	}
}

void append_char(char* val, char c, int *k) {
	for (int i = 0; i<*k; i++) val++;
	*val = (char)c;
	(*k)++;
}

int peek_char(FILE *fp) {
	int c = fgetc(fp);
	ungetc(c, fp);
	return c;
}


size_t tokenize(MyToken* tokens, char filepath[]) {
	// MyToken tokens[MAX];
	char val[MAX_STR];
	int c;
	size_t i = 0;
	int k = 0;
	MyToken token = {};
	FILE *fp = fopen(filepath, "r"); // TODO: take filepath from argv
	while ((c = fgetc(fp)) != EOF) {
		if ((c >= 'a' && c <= 'z') || ( c >= 'A' && c <= 'Z')) {
			token.token = STRING;
			append_char(val, c, &k);
		} else if ( c >= '0' && c <= '9') {
			if (token.token != FLOAT) {
				token.token = INT;
			}
			append_char(val, c, &k);
		} else {
			// NOTE: when string/number ends, store the token and prep for new symbol
			// TODO: support ~~floats~~, scientific notation, ~~numbers in strings~~ etc
			if (k > 0) {
				// check if float
				if ( c == '.') {
					int nc = peek_char(fp);
					if ( nc >= '0' && nc <= '9') {
						token.token = FLOAT;
						append_char(val, c, &k);
						continue;
					}
				}
				strcpy(token.val,val);
				k=0;
				memset(val, 0, MAX_STR);
				*tokens = token;
				tokens++;
				i++;
				MyToken token = {};
			}
			token.token = c;
			strcpy(token.val, (char*)&c);
			*tokens = token;
			tokens++;
			i++;
			MyToken token = {};
		}
	}
	fclose(fp);
	return i;
}

void printer(MyToken* tokens, size_t i) {
	for (int j=0; j<i;++j) {
		t2s(*tokens);
		tokens++;
	}
}

int main(int argc, char** argv) {
	MyToken tokens[MAX];
	size_t i = tokenize(tokens, "./token.c");
	printer(tokens, i);
	
}