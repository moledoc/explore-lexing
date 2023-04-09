# include <stdio.h>
# include <stdint.h>
# include <stdlib.h>
# include <string.h>

#define MAX 1024*10 // TODO: make more robust
#define MAX_STR 100 // TODO: make more robust


typedef enum {
	STRING = 0,
	NUMBER = 1,
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

struct MyFloat{
	uint64_t fullpart;
	uint64_t decimal;
};

typedef struct {
	TOKEN token;
	char val[MAX_STR];
} MyToken;

void t2s(MyToken t) {
	switch (t.token) {
		case STRING:
			printf("STRING(%s)\n", t.val);
			break;
		case NUMBER:
			printf("NUMBER(%s)\n",t.val);
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

int main(int argc, char** argv) {
	int c;
	MyToken tokens[MAX];
	char val[MAX_STR];
	int i = 0;
	int k = 0;
	while ((c=getchar()) != EOF) {
	//while ( (c = *test++) != '\0') {
		MyToken token = {};
		if (c >= 'a' && c <= 'z') {
			token.token = STRING;
			val[k] = (char)c;
			k++;
		} else if ( c >= 'A' && c <= 'Z'){
			token.token = STRING;
			val[k] = (char)c;
			k++;
		} else if ( c >= '0' && c <= '9') {
			token.token = NUMBER;
			val[k] = (char)c;
			k++;
		} else {
			// NOTE: when string/number ends, store the token and prep for new symbol
			// TODO: support floats, scientific notation, numbers in strings etc
			if (k > 0) {
				strcpy(token.val,val);
				k=0;
				memset(val, 0, MAX_STR);
				tokens[i] = token;
				i++;
				MyToken token = {};
			}
			token.token = c;
			strcpy(token.val, (char*)&c);
			tokens[i] = token;
			i++;
		}
	}
	
	for (int j=0; j<i;++j) {
		t2s(tokens[j]);
	}
}