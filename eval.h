#ifndef EVAL
#define EVAL
#include <stdlib.h>
#include "parser.h"

/****************************************************************
 Function: evaluate()
 -----------------------
 evaluates the given list.
 */
List evaluate(List list, List environment);


/****************************************************************
 Function: initializeEnvironment()
 -----------------------
  sets the environment global variable be equal to an empty node;
 */
void initializeEnvironment();

/****************************************************************
 Function: countParameters()
 -----------------------
 counts the parameters of the list that contains a definition of
 a user defined-function
 */
int countParameters(List list);
#endif
