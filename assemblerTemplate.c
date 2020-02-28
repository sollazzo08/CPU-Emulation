
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *words[5];

/* Take a string. Split it into different words, putting them in the words array. For example:
 * This is a string
 * Becomes:
 * words[0] = This
 * words[1] = is
 * words[2] = a
 * words[3] = string
 * Only safe when words is big enough...
 */
void getWords(char *string) {

	printf ("input: %s\n",string);
	int curWord = 0;
	char *cur = string;
	words[curWord] = string;

	while (*cur != 0) {
		if (*cur == '\n' || *cur == '\r') *cur = ' ';
		if (*cur == ' ') {
			*cur = 0; // replace space with NUL
			curWord++;
			words[curWord] = cur+1; // set the start of the next word to the character after this one
		}
    cur++;
  }
    for (int i=0;i<curWord;i++)
		printf ("word %d = %s\n",i,words[i]);
}

// takes a string and returns register number or -1 if the string doesn't start with "r" or "R"
int getRegister (char* string) {
	if (string[0] != 'R' && string[0] != 'r')
  return -1;
	return atoi(string+1); //converts string to number R1 = 1
}

int encode3R(char *string, char *bytes, int opcode) {

			bytes[0] = (opcode << 4) | getRegister(words[1]); //what value does that produce?? is it a binary or a hex?
			bytes[1] = (getRegister(words[2]) << 4) | getRegister(words[3]);
			return 2; //number of bytes returned
}

// Figure out from the first word which operation we are doing and do it...
int assembleLine(char *string, char *bytes) {

	getWords(string);

	if (strcmp(words[0] ,"add") == 0) {
		encode3R(string,bytes,1);
	}

	if(strcmp(words[0] , "and") == 0) {
		bytes[0] = (2 << 4) | getRegister(words[1]);
		bytes[1] = (getRegister(words[2]) << 4) | getRegister(words[3]);
		return 2;
	}

	if(strcmp(words[0] , "divide") == 0) {
		bytes[0] = (3 << 4) | getRegister(words[1]);
		bytes[1] = (getRegister(words[2]) << 4) | getRegister(words[3]);
		return 2;
	}

	if(strcmp(words[0] , "multiply") == 0) {
		bytes[0] = (4 << 4) | getRegister(words[1]);
		bytes[1] = (getRegister(words[2]) << 4) | getRegister(words[3]);
		return 2;
	}

	if(strcmp(words[0] , "subtract") == 0) {
		bytes[0] = (5 << 4) | getRegister(words[1]);
		bytes[1] = (getRegister(words[2]) << 4) | getRegister(words[3]);
		return 2;
	}

	if(strcmp(words[0] , "or") == 0) {
		bytes[0] = (6 << 4) | getRegister(words[1]);
		bytes[1] = (getRegister(words[2]) << 4) | getRegister(words[3]);
		return 2;
	}



}

int main (int argc, char **argv)  {

	if (argc != 3)  {printf ("assemble inputFile outputFile\n"); exit(1); }
	FILE *in = fopen(argv[1],"r");
	if (in == NULL) { printf ("unable to open input file\n"); exit(1); }
	FILE *out = fopen(argv[2],"wb");
	if (out == NULL) { printf ("unable to open output file\n"); exit(1); }

	char bytes[4], inputLine[100];

	while (!feof(in)) {
		if (NULL != fgets(inputLine,100,in)) {
			int outSize = assembleLine(inputLine,bytes);
			fwrite(bytes,outSize,1,out); //write out bytes array
		}
	}
	fclose(in);
	fclose(out);
}

//encode 3R AS A POSSIBLE FUCNTION
