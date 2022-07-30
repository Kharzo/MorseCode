/*
Author: Joseph Kharzo
Date: 5/31/2022
Description: Morse code is translated from English and to English using input from a text file and outputs the resultingtext to a file.
Usage:<exe> <input.txt> <-mc|-e> <output.txt>
*/

// Libraries
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// Methods
const char* englishToMorse(char englishText, int commandLineInput);
char morseToEnglish(const char* morseText, int commandLineInput);
int commandLineChecker(char* sourceString, int flag);

int main(int argc, char** argv){
	// The file pointers that will be used to read and write input
	FILE* fileRead;
	FILE* fileWrite;
	

	
	// If the commands entered by the user are greater than 5 or less than 4, then there are too many or too little commands entered and the program will not run.
	// The optional verbose command is accounted for
	int verbose = 0;
	if(argc == 5){
		if(strcmp("-v",argv[4]) == 0){
			verbose= 1;
		}
	}

	if((argc < 4 || argc > 5) && verbose == 0){
		printf("Error: Please enter the input text file, the translation type, and the output file name.\n");
		printf("<exe><input.txt><-mc|-e|-mcx|-ex><output.txt><-v>\n");
	}
	else{ // The commands entered meet the minimum requirements

		
		// The variables that store the result of the check of the commands entered by the user
		// If they are valid, then the program will continue to run
		int firstTextFileToken = commandLineChecker(argv[1], 0);
		int secondTextFileToken = commandLineChecker(argv[3], 0);
		int morseOrEnglishToken = commandLineChecker(argv[2],1);	

		// If the text files are not valid, then the program will stop running
		if(firstTextFileToken != 0 || secondTextFileToken != 0){

			printf("Error: Please provide a .txt file.\n");
		
		}// If the command used to tell the program to translate from morse code to English or vise versa is not valid, then the program will stop running
		else if(morseOrEnglishToken == -1){
		
			printf("Error: Please enter -mc or -e to translate a file.\n");

		}// The command line inputs were valid and the program will be translated to morse code
		else if(firstTextFileToken == 0 && (morseOrEnglishToken == 1 || morseOrEnglishToken == 3) && secondTextFileToken == 0){
			
			
			// These are the file addresses that will be used for input and output
			// argv[#] does not include the .txt, so it must be concatenated
			char* englishTextFile = strcat(argv[1],".txt");
			char* outputTextFile = strcat(argv[3],".txt");
			
			if(verbose == 0){	
				printf("Translating %s to morse code\n", englishTextFile);
			}else{
				printf("\n");
				printf("Processing %s...", englishTextFile);
			}
			// The input file is being opened
			// This also checks for errors associated with opening the file
			fileRead = fopen(englishTextFile,"r");
			if(fileRead == NULL){
				printf("Error!\n");
				exit(1);
			}
			
			// The getline parameters are initialized
			char* line = NULL;
			size_t len = 255;
			ssize_t read;
			
			// The output file is being opened
			// Checks for errors associated with opening the file
			fileWrite = fopen(outputTextFile,"w");
			if(fileWrite == NULL){
				printf("Error!\n");
				exit(1);
			}
			
			// A line is read using getline
			// The while loop will continue running until there are no new lines to read, the end of the text file is reached	
			while((read = getline(&line,&len,fileRead))!= -1){
				
				int i;
				if(verbose == 1){
					printf("\n");
					printf("READ:: %s", line);
					printf("WROTE:: ");
				}
				// This nested loop runs for each character in the line
				for(i = 0; i < read; i++){
					// Nothing will be done if a new line token is read
					if(strcmp("new line",englishToMorse(line[i],morseOrEnglishToken)) == 0){
						//Do nothing
					}
					else{
						// The final letter does not have | at the end, so the if statement checks whether the last letter is being written
						if(i+2 == read){
							if(verbose == 1){
								printf("%s", englishToMorse(line[i],morseOrEnglishToken));		
							}
							fprintf(fileWrite,"%s",englishToMorse(line[i],morseOrEnglishToken));
						}
						// For all the other letter, a | is added at the end
						else{
							if(verbose == 1){
								printf("%s|",englishToMorse(line[i],morseOrEnglishToken));
							}
							fprintf(fileWrite,"%s|",englishToMorse(line[i],morseOrEnglishToken));
						} 
					}
				}
				// A new line for each sentence
				fprintf(fileWrite,"\n");
			}
	
			// The files are closed
			fclose(fileRead);
			fclose(fileWrite);	
			if(verbose == 0){
				printf("%s file completed.\n", outputTextFile);
			}else{
				printf("\n");
				printf("\n");
				printf("%s completed.\n", outputTextFile);
			}
		}// This is for translating the morse code to English
		else if(firstTextFileToken == 0 && (morseOrEnglishToken == 2 || morseOrEnglishToken == 4) && secondTextFileToken == 0){
	
			// The text files are being prepared to be opened
			char* morseTextFile = strcat(argv[1],".txt");
			char* outputTextFile = strcat(argv[3],".txt");
			
			if(verbose == 0){	
				printf("Translating %s to English\n", morseTextFile);
			}else{
				printf("\n");
				printf("Processing %s...", morseTextFile);
			}
			// This will store the lines of morse code
			char* morseLine;
			
			// Input file is opened and errors are accounted for
			fileRead = fopen(morseTextFile,"r");
			if(fileRead == NULL){
				printf("Error!\n");
				exit(1);
			}
			
			// getline parameters are prepared
			char* line = NULL;
			size_t len = 1785;
			ssize_t read;
			
			// The output is opened and errors are accounted for
			fileWrite = fopen(outputTextFile,"w");
			if(fileWrite == NULL){
				printf("Error!\n");
				exit(1);
			}
		
			// A line of morse code is read
			// The loop will continue to run until the end of the text file is reached
			while((read = getline(&line,&len,fileRead)) != -1){
				
				// This part was somewhat confusing; the end of string character was making it so that the last morse code letter would not be translated,
				// so I got rid of the final character by using strtok on the read line from the text file but with new line as the delimeter.
				// Then, I realized that a simple while loop with morseLine = strtok(NULL, "|") was returning (null), so I made another while loop and
				// acted as though I never did the previous steps and was just using strtok(<sourceString>, "|") from the beginning.
				// TLDR: The new line or end of string character was giving me trouble, so I got around it by pre-using strtok with morseLine and moving on from there..
				
				// This contains the entire line but without a non-morse code character at the end
				morseLine = strtok(line,"\n");
				if(verbose == 1){
					printf("\n");
					printf("READ:: %s\n", line);
					printf("WROTE:: ");
				}
				// The loop runs for each token in the line, indicated by the delimeter |	
				while(morseLine != NULL){
					
					// This morseLine begins to store the morse code letters
					morseLine = strtok(morseLine,"|");

					// This runs for each letter and translates them using a function	
					while(morseLine!= NULL){
						if(verbose == 1){
							printf("%c",morseToEnglish(morseLine,morseOrEnglishToken));
						}
						fprintf(fileWrite,"%c",morseToEnglish(morseLine,morseOrEnglishToken));
						morseLine = strtok(NULL, "|");
						
					
					
					}//End of translator while loop

				}// End of pre-pump while loop

				// A new line is made for each sentence	
				fprintf(fileWrite,"\n");			
					
			}// End of sentence reading while loop
			
			// The text files are closed
			fclose(fileRead);
			fclose(fileWrite);
			if(verbose == 0){	
				printf("%s file completed.\n", outputTextFile);
			}else{
				printf("\n");
				printf("\n");
				printf("%s completed.\n", outputTextFile);
			}
		}// end of nested if-else statement

		
	}// end of original if-else statement			
	
			
	
	return 0;
}// end of main

// This function checks the validity of the user's inputs
// Has two parameters; the string that the user entered and whether the text file or the translation code is being checked
int commandLineChecker(char* sourceString, int flag){
	
	// Where the input is stored
	char* commandLineInput;
	
	// The text files are checked
	if(flag == 0){
		
		// The name of the file is irrelevent, so it is passed over
		// The file extension is important and is stored in the appropriate variable
		commandLineInput = strtok(sourceString, ".");
		commandLineInput = strtok(NULL, "");
		
		// If the user inputed a file that ends with .txt, then the function will return a 0
		// If the user had a wrong input, then -1 is returned
		if(strcmp("txt",commandLineInput) == 0){
			return 0;
		}
		else{
			return -1;
		}
	
	}// The translation token is checked
	else{
		// The entire input is stored in commandLineInput
		commandLineInput = strtok(sourceString, "");
		
		// Checks if the user typed in -mc, -e, -mcx or -ex
		// If they did, then the function will return 1. 2, 3, 4 respectively
		// If something else was entered in the command line, then the function will return -1	
		if(strcmp("-mc",commandLineInput) == 0){
			return 1;
		}else if(strcmp("-e",commandLineInput) == 0){
			return 2;
		}else if(strcmp("-mcx",commandLineInput) == 0){
			return 3;
		}else if(strcmp("-ex",commandLineInput) == 0){
			return 4;
		}
		else{
			return -1;
		}		

	}// End of if-else statement
}// End of function


// Converts english letters to morse code
// Returns a string and has a character parameter
const char* englishToMorse(char letter, int commandLineInput){
	
	// Any English letter is lower cased and stored in fixedLetter
	char fixedLetter = tolower(letter);
	
	// These if-else statements check for an English character and translate it to morse code
	// This string is then returned
	if(fixedLetter == 'a'){
		return "*-";
	}else if(fixedLetter == ' '){ // If a space exists in the input file
		return "/";
	}else if(fixedLetter == 'b'){
		return "-***";
	}else if(fixedLetter == 'c'){
		return "-*-*";
	}else if(fixedLetter == 'd'){
		return "-**";
	}else if(fixedLetter == 'e'){
		return "*";
	}else if(fixedLetter == 'f'){
		return "**-*";
	}else if(fixedLetter == 'g'){
		return "--*";
	}else if(fixedLetter == 'h'){
		return "****";
	}else if(fixedLetter == 'i'){
		return "**";
	}else if(fixedLetter == 'j'){
		return "*---";
	}else if(fixedLetter == 'k'){
		return "-*-";
	}else if(fixedLetter == 'l'){
		return "*-**";
	}else if(fixedLetter == 'm'){
		return "--";
	}else if(fixedLetter == 'n'){
		return "-*";
	}else if(fixedLetter == 'o'){
		return "---";
	}else if(fixedLetter == 'p'){
		return "*--*";
	}else if(fixedLetter == 'q'){
		return "--*-";
	}else if(fixedLetter == 'r'){
		return "*-*";
	}else if(fixedLetter == 's'){
		return "***";
	}else if(fixedLetter == 't'){
		return "-";
	}else if(fixedLetter == 'u'){
		return "**-";
	}else if(fixedLetter == 'v'){
		return "***-";
	}else if(fixedLetter == 'w'){
		return "*--";
	}else if(fixedLetter == 'x'){
		return "-**-";
	}else if(fixedLetter == 'y'){
		return "-*--";
	}else if(fixedLetter == 'z'){
		return "--**";
	}else if(fixedLetter == '\n'){// A new line character is found and is not included when writing to the output file
		return "new line";
	}
	else{ // In case the user had an input file without valid input
		// Enables the user to use punctuation marks
		if(commandLineInput == 3){
			if(fixedLetter == '0'){
				return "-----";
			}else if(fixedLetter == '1'){
				return "*----";
			}else if(fixedLetter == '2'){
				return "**---";
			}else if(fixedLetter == '3'){
				return "***--";
			}else if(fixedLetter == '4'){
				return "****-";
			}else if(fixedLetter == '5'){
				return "*****";
			}else if(fixedLetter == '6'){
				return "-****";
			}else if(fixedLetter == '7'){
				return "--***";
			}else if(fixedLetter == '8'){
				return "---**";
			}else if(fixedLetter == '9'){
				return "----*";
			}else if(fixedLetter == '*'){
				return "*-*-*-";
			}else if(fixedLetter == ','){
				return "--**--";
			}else if(fixedLetter == '?'){
				return "**--**";
			}else if(fixedLetter == '\''){
				return "*----*";
			}else if(fixedLetter == '!'){
				return "-*-*--";
			}else if(fixedLetter == '/'){
				return "-**-*";
			}else if(fixedLetter == '('){
				return "-*--*";
			}else if(fixedLetter == ')'){
				return "-*--*-";
			}else if(fixedLetter == '&'){
				return "*-***";
			}else if(fixedLetter == ':'){
				return "---***";
			}else if(fixedLetter == ';'){
				return "-*-*-*";
			}else if(fixedLetter == '='){
				return "-***-";
			}else if(fixedLetter == '+'){
				return "*-*-*";
			}else if(fixedLetter == '-'){
				return "-****-";
			}else if(fixedLetter == '_'){
				return "**--*-";
			}else if(fixedLetter == '\"'){
				return "*-**-*";
			}else if(fixedLetter == '$'){
				return "***-**-";
			}else if(fixedLetter == '@'){
				return "*--*-*";
			}else{
				return "XXXX";
			}
		}else{
			return "XXXX";
		}
	}// End of if-else statements


}// End of function


// This converts morse code to English letters
// Returns individual characters
// Has a string for the parameter
char morseToEnglish(const char* line, int commandLineInput){
	
	// The if-else statements convert the line of morse code to English characters
	if(strcmp("*-",line) ==0){
		return 'A';
	}else if(strcmp("-***",line)==0){
		return 'B';
	}else if(strcmp("-*-*",line)==0){
		return 'C';
	}else if(strcmp("-**",line)==0){
		return 'D';
	}else if(strcmp("*",line)==0){
		return 'E';
	}else if(strcmp("**-*",line)==0){
		return 'F';
	}else if(strcmp("--*",line)==0){
		return 'G';
	}else if(strcmp("****",line)==0){
		return 'H';
	}else if(strcmp("**",line)==0){
		return 'I';
	}else if(strcmp("*---",line)==0){
		return 'J';
	}else if(strcmp("-*-",line)==0){
		return 'K';
	}else if(strcmp("*-**",line)==0){
		return 'L';
	}else if(strcmp("--",line)==0){
		return 'M';
	}else if(strcmp("-*",line)==0){
		return 'N';
	}else if(strcmp("---",line)==0){
		return 'O';
	}else if(strcmp("*--*",line)==0){
		return 'P';
	}else if(strcmp("--*-",line)==0){
		return 'Q';
	}else if(strcmp("*-*",line)==0){
		return 'R';
	}else if(strcmp("***",line)==0){
		return 'S';
	}else if(strcmp("-",line)==0){
		return 'T';
	}else if(strcmp("**-",line)==0){
		return 'U';
	}else if(strcmp("***-",line)==0){
		return 'V';
	}else if(strcmp("*--",line)==0){
		return 'W';
	}else if(strcmp("-**-",line)==0){
		return 'X';
	}else if(strcmp("-*--",line)==0){
		return 'Y';
	}else if(strcmp("--**",line)==0){
		return 'Z';
	}else if(strcmp("XXXX",line)==0){
		return '#';
	}else if(strcmp("/",line)==0){
		return ' ';
	}else{// If the morse code is unrecognizable
		// Accounts for puncuation
		if(commandLineInput == 4){
			if(strcmp("-----",line)==0){
				return '0';
			}else if(strcmp("*----",line)==0){
				return '1';
			}else if(strcmp("**---",line)==0){
				return '2';
			}else if(strcmp("***--",line)==0){
				return '3';
			}else if(strcmp("****-",line)==0){
				return '4';
			}else if(strcmp("*****",line)==0){
				return '5';
			}else if(strcmp("-****",line)==0){
				return '6';
			}else if(strcmp("--***",line)==0){
				return '7';
			}else if(strcmp("---**",line)==0){
				return '8';
			}else if(strcmp("----*",line)==0){
				return '9';
			}else if(strcmp("*-*-*-",line)==0){
				return '*';
			}else if(strcmp("--**--",line)==0){
				return ',';
			}else if(strcmp("**--**",line)==0){
				return '?';
			}else if(strcmp("*----*",line)==0){
				return '\'';
			}else if(strcmp("-*-*--",line)==0){
				return '!';
			}else if(strcmp("-**-*",line)==0){
				return '/';
			}else if(strcmp("-*--*",line)==0){
				return '(';
			}else if(strcmp("-*--*-",line)==0){
				return ')';
			}else if(strcmp("*-***",line)==0){
				return '&';
			}else if(strcmp("---***",line)==0){
				return ':';
			}else if(strcmp("-*-*-*",line)==0){
				return ';';
			}else if(strcmp("-***-",line)==0){
				return '=';
			}else if(strcmp("*-*-*",line)==0){
				return '+';
			}else if(strcmp("-****-",line)==0){
				return '-';
			}else if(strcmp("**--*-",line)==0){
				return '_';
			}else if(strcmp("*-**-*",line)==0){
				return '\"';
			}else if(strcmp("***-**-",line)==0){
				return '$';
			}else if(strcmp("*--*-*",line)==0){
				return '@';
			}else{
				return '%';
			}
		}else{
			return '%';
		}
	}// End of if-else statements

}// End of function

