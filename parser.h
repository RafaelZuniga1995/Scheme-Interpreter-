#ifndef PARSER
#define PARSER
#include <stdlib.h>

/****************************************************************
    declaration of a List object.
*/
typedef struct node *List;


/****************************************************************
 Function: createNode()
 -----------------------
 Creates a new queue and returns it.

 Example usages:
   List node;
   node = createNode(param1, param2, param3);
 */
List createNode(char *data, List  car, List  cdr);

/****************************************************************
 Function: S_Expression()
 -----------------------
 reads in the first token and calls sHelper

 */
List S_Expression ();

/****************************************************************
 Function: sHelper()
 -----------------------
 parses the input list and returns the conscell structure of that list

 */
List sHelper();


/****************************************************************
 Function: printList(List list)
 -----------------------
  prints every single atom of the given list
 */
void printList(List list);

/****************************************************************
 Function: getData(List node)
 -----------------------
  returns the "data" pointer of the given node
 */
char* getData(List list);

/****************************************************************
 Function: car(List list)
 -----------------------
  returns the "first" pointer of the given list
 */
List car(List list);

/****************************************************************
 Function: schemeCdr(List list)
 -----------------------
  returns the "rest" pointer of the given list, takes care of the
  () corner case.
 */
List schemeCdr(List list);

/****************************************************************
 Function: cdr(List list)
 -----------------------
  returns the "rest" pointer of the given list
 */
List cdr(List list);

/****************************************************************
 Function: cadr(List list)
 -----------------------
  returns the second element of the list.
 */
List cadr(List list);

/****************************************************************
 Function: caddr(List list)
 -----------------------
  returns the third element of the list.
 */
List caddr(List list);

/****************************************************************
 Function: cadddr(List list)
 -----------------------
  returns the fourth element of the list.
 */
List cadddr(List list);

/****************************************************************
 Function: caddddr(List list)
 -----------------------
  returns the fifth element of the list.
 */
List caddddr(List list);

/****************************************************************
 Function: setCdr(List node, List rest)
 -----------------------
  sets the "rest" pointer of the first parameter point to the second parameter
 */
void setCdr(List node, List rest);

/****************************************************************
 Function: getTrue()
 -----------------------
  returns a node that represents #t
  more specifically returns the following node created with the following parameters
    List isTrue = createNode("#t", NULL, NULL);
 */
List getTrue();

/****************************************************************
 Function: getFalse()
 -----------------------
  returns a node that represents #f
  more specifically returns the following node created with the following parameters
    List isFalse = createNode("#f", NULL, NULL);
 */
List getFalse();

/****************************************************************
 Function: cons()
 -----------------------
  makes the first parameter the first element of the second parameter
 */
List cons(List param1, List param2);

/****************************************************************
 Function: append()
 -----------------------
 appends the second list to the end of the first list.
 */
List append(List  list1, List list2);

/****************************************************************
 Function: isEqual()
 -----------------------
 Recursive implementation of equal. This function checks if the two given
 parameters have the same structure and data.
 */
List isEqual(List param1, List param2);

/****************************************************************
 Function: assoc()
 -----------------------
 Recursive implementation of assoc. This has two parameters, the first of
 which is a symbol, the second a so-called "association list. It returns the
 pair associated with the symbol, and #f if the symbol is not the first
 element of any pair.
 */
List assoc(List symbol, List alist);

/****************************************************************
 Function: isSymbol()
 -----------------------
 Checks if the given node/list is a "symbol" node. A node is
 "symbol" node if its car and its cdr are null.
 */
int isSymbol(List  node);

/****************************************************************
 Function: isList()
 -----------------------
 Checks if the given node/list is a scheme list.
 */
int isList(List list);

#endif
