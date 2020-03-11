
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/* Michael Sollazzo
	404
*/
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


//function call to handle all the 3R instructions
int encode3R(char *bytes, int opcode) {

	bytes[0] = (opcode << 4) | getRegister(words[1]); //what value does that produce?? is it a binary or a hex?
	bytes[1] = (getRegister(words[2]) << 4) | getRegister(words[3]);
	return 2; //number of bytes returned
}

//function call to handle all the BR1 instructions
int encodeBR1(char *bytes, int opcode, int branchType) {

	bytes[0] = (opcode << 4) | branchType;
	bytes[1] = (getRegister(words[1]) << 4) | getRegister(words[2]);
	int t = (atoi(words[3]) / 2);
	bytes[2] = t >> 8;
	bytes[3] = t; // 16bit is being truncated since we store it in only 8
	return 4;
}

//function call to handle all the BR2 instructions
int encodeBR2(char *bytes, int opcode, int branchType) {

	bytes[0] = (opcode << 4) | branchType;
	int t = (atoi(words[1]) / 2);
	bytes[1] = t >> 8;
	bytes[2] = t >> 8;
	bytes[3] = t; // 16bit is being truncated since we store it in only 8
	return 4;
}

//function call to handle all the Load and store instructions
int encodeLS(char*bytes, int opcode) {
  bytes[0] = (opcode << 4) | getRegister(words[1]);
	bytes[1] = getRegister(words[2]) << 4 | atoi(words[3]);
	return 2;
}


// Function call to handle all the stack Instructions
// Accepts opcode and instruction 00,01,10
int encodeStack(char*bytes, int opcode, int instruction) {

	//if instruction 0 then it is "return"
	if(instruction != 0){
		bytes[0] = opcode << 4 | getRegister(words[1]);
		bytes[1] = instruction << 6 | 0;
			return 2;
	} else
		bytes[0] = opcode << 4 | 0;
		bytes[1] = instruction << 6 | 0; //shift left 6 to handle byte[1]

	return 2;
}

//Function call to handle all the Move Instructions
//Accpets opcode
int encodeMove(char *bytes, int opcode) {
	if(opcode == 11){
		bytes[0] = opcode << 4 | getRegister(words[1]);
		int t = atoi(words[2]);
		bytes[1] = t;
		return 2;
	} else
		bytes[0] = opcode << 4 | 0; //register is ignored
		int t = atoi(words[1]);
		bytes[1] = t;
		return 2;
}

// Figure out from the first word which operation we are doing and do it...
int assembleLine(char *string, char *bytes) {

	getWords(string);
  /****************3R******************/
	if (strcmp(words[0] ,"add") == 0) {
			return encode3R(bytes,1);
	}
	if(strcmp(words[0] , "and") == 0) {
			return encode3R(bytes,2);
	}
	if(strcmp(words[0] , "divide") == 0) {
			return encode3R(bytes,3);
	}
	if(strcmp(words[0] , "halt") == 0) {
			bytes[0] = 0;
			bytes[1] = 0;
			return 2;
	}
	if(strcmp(words[0] , "multiply") == 0) {
			return encode3R(bytes,4);
	}
	if(strcmp(words[0] , "or") == 0) {
			return encode3R(bytes,6);
	}
	if(strcmp(words[0] , "subtract") == 0) {
			return encode3R(bytes,5);
	}
	/****************Branches******************/
	if(strcmp(words[0], "branchIfLess") == 0) {
			return encodeBR1(bytes,7,0);
	}
	if(strcmp(words[0], "branchIfLessOrEqual") == 0) {
			return encodeBR1(bytes,7,1);
	}
	if(strcmp(words[0], "branchIfEqual") == 0) {
			return encodeBR1(bytes,7,2);
	}
	if(strcmp(words[0], "branchIfNotEqual") == 0) {
			return encodeBR1(bytes,7,3);
	}
	if(strcmp(words[0], "branchIfGreater") == 0) {
			return encodeBR1(bytes,7,4);
	}
	if(strcmp(words[0], "branchIfGreaterOrEqual") == 0) {
			return encodeBR1(bytes,7,5);
	}
	if(strcmp(words[0], "call") == 0) {
			return encodeBR2(bytes,7,6);
	}
	if(strcmp(words[0], "jump") == 0) {
			return encodeBR2(bytes,7,7);
	}
		/****************Load/Store******************/
	if(strcmp(words[0], "load") == 0) {
			return encodeLS(bytes,8);
	}
	if(strcmp(words[0], "store") == 0) {
			return encodeLS(bytes,9);
	}
		/****************Stack******************/
	if(strcmp(words[0], "pop") == 0) {
			return encodeStack(bytes,10,1);
	}
	if(strcmp(words[0], "push") == 0) {
			return encodeStack(bytes,10,2);
	}
	if(strcmp(words[0], "return") == 0) {
			return encodeStack(bytes,10,0);
	}
		/****************Move******************/
	if(strcmp(words[0], "move") == 0) {
			return encodeMove(bytes,11);
	}
	if(strcmp(words[0], "interrupt") == 0) {
			return encodeMove(bytes,12);
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
