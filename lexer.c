#include <stdio.h>

#define MAX_TOKENS 1000
#define MAX_STR 100

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

void to_string(Token* token){
	switch (token->t) {
		case WORD:
			printf("WORD(%s)\n", token->v);
			break;
		case CHAR:
			printf("CHAR(%s)\n", token->v);
		case STRING:
			printf("STRING(%s)\n", token->v);
		case INT:
			printf("INT(%s)\n",token->v);
			break;
		case FLOAT:
			printf("FLOAT(%s)\n",token->v);
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
			printf("SYMBOL(%s)\n", token->v);
	}
}

void printer(Token* tokens, size_t n){
	for (int i=0;i<n;i++) {
		// --tokens;
		to_string(tokens);
		tokens++;
	}
}

void cpy(char* dest, char* src, size_t n) {
	for( int i=0; i<n; i++) {
		*dest= *src++;
		dest++;
	}
	*dest = '\0'; 
}

size_t tokenize(Token* tokens, FILE* stream) {
	size_t size = 0;
	int c;
	while ( (c = fgetc(stream)) != EOF) {
		char buf[MAX_STR];
		Token new={};
		if ( c == DQUOTE) {
			int i = 0;
			while ((c=fgetc(stream)) != DQUOTE && c != EOF ) {
				buf[i] = c;
				i+=sizeof(c);
			}
			char val[i+1];
			cpy(new.v, buf, (size_t)i);
			new.t = STRING;
		} else if ( c == QUOTE) {
			int i = 0;
			char buf[MAX_STR];
			while ((c=fgetc(stream)) != QUOTE && c != EOF ) {
				buf[i] = c;
				i+=sizeof(c);
			}
			char val[i+1];
			cpy(new.v, buf, (size_t)i);
			new.t = CHAR;
		} else if ( c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' ) {
			int i = 0;
			char buf[MAX_STR];
			buf[i] = 0;
			do {
				buf[i] = c;
				i+= sizeof(c);
			} while ((c=fgetc(stream)) != EOF && ( c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' ) );
			//ungetc(c, stream);
			char val[i+1];
			cpy(new.v, buf, (size_t)i);
			new.t = WORD;
		} else {
			char val[2];
			new.t = c;
			val[0] = c;
			cpy(new.v, val, 2);
		}
		*tokens=new;
		tokens++;
		size++;
		//to_string(new);
	}
	for (int j=0;j<size;++j) --tokens;
	return size;
}

int main(int argc, char* argv) {
	char* word = "tere t\"ul\\ema\"st";
	Token tokens[MAX_TOKENS];

	size_t  size = tokenize(tokens, stdin);
	printer(tokens, size);
}