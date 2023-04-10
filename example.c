#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"

int main1(int argc, char** argv) {
	char filepath[] = "./lexer.h";

	FILE *fp = fopen(filepath, "r");

	fseek(fp, 0L, SEEK_END);
	int size = ftell(fp);
	// rewind(fp);
	fclose(fp);

	Token tokens[size];

	size_t i = tokenize_file(tokens, filepath);
	printer(tokens, i);
}

int main(int argc, char** argv) {
	Token tokens[1024*10];
	size_t i = tokenize_stdin(tokens);
	printer(tokens, i);
}