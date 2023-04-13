#include <stdio.h>

#define MAX_TOKENS 1024*10 // TODO: make more robust and no segfault when Tokens list gets full
#define MAX_STR 256 // TODO: make more robust and no stack smashing when char list gets full

#define PRINT_WHITESPACE 0

typedef enum {
	WORD = -1,
	CHAR = -2,
	STRING = -3,
	INT = -4,
	FLOAT= -5,
	NUMBER = -6,
	//
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
	DOT = '.',
	COMMA = ',',
} TOKEN;

typedef struct {
	TOKEN t;
	char v[MAX_STR];
} Token;

void to_string(Token token){
	switch (token.t) {
		case WORD:
			printf("WORD(%s)\n", token.v);
			break;
		case CHAR:
			printf("CHAR(%s)\n", token.v);
			break;
		case STRING:
			printf("STRING(%s)\n", token.v);
			break;
		case INT:
			printf("INT(%s)\n",token.v);
			break;
		case FLOAT:
			printf("FLOAT(%s)\n",token.v);
			break;
		case NUMBER:
			printf("NUMBER(%s)\n", token.v);
			break;
		case TAB:
			if (PRINT_WHITESPACE) printf("SYMBOL(\\t)\n");
			break;
		case NEWLINE:
			if (PRINT_WHITESPACE) printf("SYMBOL(\\n)\n");
			break;
		case SPACE:
			if (PRINT_WHITESPACE) printf("SYMBOL(' ')\n");
			break;
		default:
			printf("SYMBOL(%s)\n", token.v);
	}
}

void printer(Token tokens[], size_t n){
	for (int i=0;i<n;i++) to_string(tokens[i]);
}

void cpy(char* dest, char* src, size_t n) {
	for( int i=0; i<n; i++) {
		*dest= *src++;
		dest++;
	}
	*dest = '\0'; 
}

size_t tokenize(Token tokens[], FILE* stream) {
	size_t size = 0;
	int c;
	while ( (c = fgetc(stream)) != EOF) {
		char buf[MAX_STR];
		Token new={};
		int i = 0;
		if ( c == DQUOTE || c == QUOTE) {
			int state = c; // strore whether we are in DQOUTE or QUOTE
			int tkn = CHAR; // store the token type
			if (state == DQUOTE); tkn = STRING;
			int prev = 0;
			do {
				buf[i] = c;
				++i;
				// if there are 2 consecutive BSLASHs, then it's BSLASH literal and we set prev to 0; else set prev normally
				prev = (c == BSLASH && prev == BSLASH) ? 0 : c;
				if (i >= MAX_STR) break; // TODO: improve
			} while (((c=fgetc(stream)) != state || prev == BSLASH) && c != EOF );
			if (c != EOF) { 
				buf[i] = c;
				++i;
			}
			cpy(new.v, buf, (size_t)i);
			new.t = tkn;
		} else if ( c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' ) {
			do {
				if ( c == DOT || c == COMMA || c == COLON || c == SCOLON) {
					// check if char after dot is part of word or not.
					// if not, break out of the loop, if yes then put the peeked char back to sream.
					int c0 = fgetc(stream);
					if ( c0 >= 'a' && c0 <= 'z' || c0 >= 'A' && c0 <= 'Z' ||
						c0 >= '0' && c0 <= '9' || 
						c0 == DOT || c0 == UNDERSCORE || c0 == DASH ) {
						ungetc(c0, stream);
					} else {
						break;
					}
				}
				buf[i] = c;
				i++;
				if ( i >= MAX_STR) break; // TODO: improve
			} while ( (c=fgetc(stream))  >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' ||
				c >= '0' && c <= '9' || 
				c == DOT || c == UNDERSCORE || c == DASH );
			ungetc(c, stream);
			cpy(new.v, buf, (size_t)i);
			new.t = WORD;
		} else if ( c >= '0' && c <= '9' ) {
			int dot_count = 0;
			do {
				if ( c == DOT) {
					// check if char after dot is number or not.
					// if not, break out of the loop, if yes then put the peeked char back to sream.
					int c0 = fgetc(stream);
					if (c0 < '0' || c0 > '9') {
						break;
					} else {
						ungetc(c0, stream);
					}
					++dot_count;
				}
				if (dot_count == 1) new.t = FLOAT;
				if (dot_count == 2) new.t = NUMBER;
				buf[i] = c;
				i++;
			} while ( (c=fgetc(stream))  >= '0' && c <= '9'  || c == DOT);
			ungetc(c, stream);
			cpy(new.v, buf, (size_t)i);
			if (!dot_count) new.t = INT;
		} else {
			char val[1];
			new.t = c;
			buf[0] = c;
			buf[1] = '\0';
			cpy(new.v, buf, 2);
		}
		tokens[size]=new;
		size++;
		//to_string(new);
	}
	return size;
}

int main(int argc, char* argv) {
	Token tokens[MAX_TOKENS];

	size_t size = tokenize(tokens, stdin);
	printer(tokens, size);
}