#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MAX_TOKENS 1024*10 // TODO: make more robust and no segfault when Tokens list gets full
#define MAX_STR 256 // TODO: make more robust and no stack smashing when char list gets full
#define URL_HTTP "http"
#define EOT 0x04 // NOTE: 0x04 is hex for EOT (when read from file)

#define PRINT_WHITESPACE 0

typedef enum {
	WORD = -1,
	CHAR = -2,
	STRING = -3,
	INT = -4,
	FLOAT= -5,
	NUMBER = -6,
	URL= -7,
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
		case URL:
			printf("URL(%s)\n", token.v);
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
		++dest;
	}
	*dest = '\0'; 
}

int is_url(char val[]) {
	int i = 0;
	while (val[i] != '\0' && i < 4) {
		if (URL_HTTP[i] != val[i]) return 0;
		++i;
	}
	return 1;
}

// TODO: add check for '://'
int tokenize_url(char buf[], FILE* stream) {
	int c;
	int i = 4; // since we checked, if url, then we know that first 4 chars are 'http' and can continue from there.
	while ((c=fgetc(stream)) != SPACE && c != NEWLINE && c != TAB && c != EOF && c != EOT) {
		buf[i] = c;
		++i;
	}
	ungetc(c, stream);
	return i;
}

size_t tokenize(Token tokens[], FILE* stream) {
	size_t size = 0;
	int c;	
	while ( (c = fgetc(stream)) != EOF && c != EOT) { 
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
			} while (((c=fgetc(stream)) != state || prev == BSLASH) && c != EOF && c != EOT);
			if (c != EOF || c != EOT) { 
				buf[i] = c;
				++i;
			}
			cpy(new.v, buf, (size_t)i);
			new.t = tkn;
		} else if ( c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' ) {
			new.t = WORD;
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
				++i;
				if (i == 4 && is_url(buf)) {
					i = tokenize_url(buf, stream);
					new.t = URL;
				}
				if ( i >= MAX_STR) break; // TODO: improve
			} while ( (c=fgetc(stream))  >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' ||
				c >= '0' && c <= '9' || 
				c == DOT || c == UNDERSCORE || c == DASH );
			ungetc(c, stream);
			cpy(new.v, buf, (size_t)i);
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
				++i;
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
		++size;
	}
	return size;
}

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
			total += max_token_amount * 2;
			sub[idx] = max_token_amount *2;
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
