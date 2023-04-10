#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"

int main(int argc, char** argv) {
	char filepath[] = "./README.md";

	FILE *fp = fopen(filepath, "r");

	fseek(fp, 0L, SEEK_END);
	int size = ftell(fp);
	// rewind(fp);
	fclose(fp);

	Token tokens[size];

	size_t i = tokenize(tokens, filepath);
	printer(tokens, i);
}