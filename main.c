#include <stdlib.h>
#include <stdio.h>
#include "lexer.h"
#include "parser.h"
#include "eval.h"
#include "main.h"


/*******************************************************
global environment variable
*/
static List environment;

/*********************************************************************
 implementation note: this will be called in main.c to initialize
 the environment in the beginning of the program.
*/
void initializeEnvironment()
{
    environment = createNode("()", NULL, NULL);
}


int main(void)
{
    List input;
    initializeEnvironment();
    startTokens(20);
    printf("scheme> ");
    while(1)
    {
        input = S_Expression();
        List output = evaluate(input, environment);
        printList(output);
        printf("\nscheme> ");
    }
    return 0;
}

void consToEnvironment(List list)
{
    environment = cons(list, environment);
}

List getEnvironment()
{
    return environment;
}
