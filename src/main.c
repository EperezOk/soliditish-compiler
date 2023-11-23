#include <stdio.h>
#include <unistd.h>

#include "backend/code-generation/generator.h"
#include "backend/support/logger.h"
#include "backend/support/shared.h"
#include "backend/semantic-analysis/symbol-table.h"
#include "frontend/syntactic-analysis/bison-parser.h"

extern char *optarg; // avoid IDE warnings

// Application state
CompilerState state;

// Code generation options
FILE *outputFile = NULL;
int indentationSize = 4;
boolean indentUsingSpaces = true;
boolean indentOutput = true;

void parseCliOptions(int argc, char *argv[]);
void freeResources();

const int main(int argumentCount, char *arguments[]) {
	// Initialize the application state
	state.program = NULL;
	state.succeed = false;
	state.symbolTable = NULL;

	parseCliOptions(argumentCount, arguments);

	LogInfo("Compiling...\n");

	const int result = yyparse(); // reads from stdin, see `script/start.sh`

	switch (result) {
		case 0:
			if (state.succeed) {
				LogInfo("Compilation successful.");
				Generator();
			}
			else {
				LogError("Found %d compilation errors:", state.errorCount);
				for (int i = 0; i < state.errorCount; ++i) {
					LogErrorRaw("%s\n", state.errors[i]);
					free(state.errors[i]);
				}
				freeResources();
				return -1;
			}
			break;
		case 1:
			LogError("[Bison] Syntax error.");
			break;
		case 2:
			LogError("[Bison] Out of memory.");
			break;
		default:
			LogError("[Bison] Unknown error (code %d).", result);
	}

	LogInfo("Freeing up memory...");

	freeResources();
	
	return result;
}

void parseCliOptions(int argc, char *argv[]) {
	LogInfo("Compiler Options:");

    int opt;

    while ((opt = getopt(argc, argv, "o:i:tmh")) != -1) {
        switch (opt) {
            case 'o':
                outputFile = fopen(optarg, "w"); // create or overwrite file
                if (outputFile == NULL) {
                    perror("Error opening output file");
                    exit(EXIT_FAILURE);
                }
				LogRaw("- Output File: %s\n", optarg);
                break;
            case 'i':
                indentationSize = atoi(optarg);
                break;
            case 't':
                indentUsingSpaces = false;
                break;
            case 'm':
                indentOutput = false;
                break;
			case 'h':
				printf("\nUsage: %s [-o output_file] [-i indent_size] [-t] [-m]\n", argv[0]);
				exit(EXIT_SUCCESS);
            case '?':
                fprintf(stderr, "\nUsage: %s [-o output_file] [-i indent_size] [-t] [-m]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

	if (outputFile == NULL) {
		outputFile = fopen("out.sol", "w");
		if (outputFile == NULL) {
			perror("Error opening output file");
			exit(EXIT_FAILURE);
		}
		LogRaw("- Output File: out.sol\n");
	}

	// Print out the options
    LogRaw("- Indent Size: %d\n", indentationSize);
    LogRaw("- Indent with: %s\n", indentUsingSpaces ? "spaces" : "tabs");
    LogRaw("- Indent Output: %s\n", indentOutput ? "true" : "false");
	LogRaw("\n");
}

void freeResources() {
	freeSymbolTable();
	if (outputFile != stdout) fclose(outputFile);
}
