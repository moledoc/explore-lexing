# include <stdio.h>
# include <stdint.h>

#define MAX 1024*10

typedef enum {
	CHAR = 0,
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
	char value;
} MyToken;


int main(int argc, char** argv) {
	// printf(MyToken(At,"@"));
	char* test = "see lause < on test> ?";
	printf("Hello world %d %d\n", NUMBER, PIPE);
	printf("%s %d\n", test, (TOKEN)'|');
	int c;

	MyToken tokens[MAX];
	char* val;
	int i = 0;
	while ((c=getchar()) != EOF) {
		MyToken token;
		token.value = c;
		if (c >= 'a' && c <= 'z') {
			token.token = CHAR;
		} else if ( c >= 'A' && c <= 'Z'){
			token.token = CHAR;
		} else {
			token.token = c;
		}
		tokens[i] = token;
		i++;
	}
	
	for (int j=0; j<i;++j) {
		// printf("TOKEN: %d %d\n", tokens[j].token, tokens[j].value);
		printf("%c", (char)tokens[j].value);
	}

}