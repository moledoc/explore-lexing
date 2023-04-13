#include <stdio.h>

#define MAX_TOKENS 1024*10
#define MAX_STR 256

typedef enum {
	WORD = -1,
	CHAR = -2,
	STRING = -3,
	INT = -4,
	FLOAT= -5,
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
			printf("SYMBOL(%s)\n", token.v);
	}
}

void printer(Token tokens[], size_t n){
	for (int i=0;i<n;i++) {
		// --tokens;
		to_string(tokens[i]);
		//tokens++;
	}
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
		if ( c == DQUOTE) {
			do {
				buf[i] = c;
				i++;
			} while ((c=fgetc(stream)) != DQUOTE && c != EOF );
			if (c != EOF) buf[i] = c;
			cpy(new.v, buf, (size_t)i);
			new.t = STRING;
		} else if ( c == QUOTE) {
			do {
				buf[i] = c;
				i++;
			} while ((c=fgetc(stream)) != QUOTE && c != EOF );
			if (c != EOF) buf[i] = c;
			cpy(new.v, buf, (size_t)i);
			new.t = CHAR;
		} else if ( c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' ) {
			do {
				buf[i] = c;
				i++;
			} while ( ((c=fgetc(stream))  >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' ) && c != EOF);
			ungetc(c, stream);
			cpy(new.v, buf, (size_t)i);
			new.t = WORD;
		} else if ( c >= '0' && c <= '9' ) {
			do {
				buf[i] = c;
				i++;
			} while ( ((c=fgetc(stream))  >= '0' && c <= '9' ) && c != EOF);
			ungetc(c, stream);
			cpy(new.v, buf, (size_t)i);
			new.t = WORD;
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