/****************************************************************
 File: parser.c
 -------------
 This file implements the interface given in parser.h.
 ****************************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "parser.h"
#include "lexer.h"

/****************************************************************
 Declaration of the char pointer to the current token
*/
char *token;

/****************************************************************
 Declaration of the int variable used to keep track of depth when
 using S_Expression
*/
int depth = 0;

/****************************************************************
 Declaration of the universally used #t node
*/
List isTrue;

/****************************************************************
 Declaration of the universally used #f node
*/
List isFalse;


/****************************************************************
 The cons cell will be implemented with node struct that has space
 for a string, space for a "first" pointer (for the "car" of the list),
 and space for a "rest" pointer (for the "cdr" of a list).
*/
struct node
{
    char *data;
    struct node *first;
    struct node *rest;
};

/***********************************************************************/
char* getData(List list)
{
    return list->data;
}

/***********************************************************************/
List car(List list)
{
    return list->first;
}

/***********************************************************************/
List cdr(List list)
{
        return list->rest;
}

/***********************************************************************/
List schemeCdr(List list)
{
    if (list->rest == NULL)
        return createNode("()", NULL, NULL);
    else
        return list->rest;
}

/***********************************************************************/
List cadr(List list)
{
        return car(cdr(list));
}

/***********************************************************************/
List caddr(List list)
{
        return car(cdr(cdr(list)));
}

/***********************************************************************/
List cadddr(List list)
{
        return car(cdr(cdr(cdr(list))));
}

/***********************************************************************/
List caddddr(List list)
{
        return car(cdr(cdr(cdr(cdr(list)))));
}

/***********************************************************************/
void setCdr(List node, List rest)
{
    node->rest = rest;
}

/***********************************************************************/
List getTrue()
{
    return isTrue;
}

/***********************************************************************/
List getFalse()
{
    return isFalse;
}

/************************************************************************
 implementation note: takes care of the special case where the second
 parameter is the empty list or the #f list
*/
List cons(List param1, List param2)
{
    if (isSymbol(param2))
        if (strcmp(param2->data,"#f") == 0 || strcmp(param2->data,"()") == 0)
            return createNode(NULL, param1, NULL);

    return createNode(NULL, param1, param2);
}

/*************************************************************************/
List append(List  list1, List list2)
{
    if (list1 == NULL)
    {
        return list2;
    }else
    {
        cons(car(list1), append(cdr(list1), list2));
    }
}

/*************************************************************************/
int isNull(List  list)
{
    if (isSymbol(list))
        {
            if (strcmp(list->data, "()") == 0)
                return 1;
            else
                return 0;
        }
        else
            return 0;
}

/*************************************************************************/
List createNode(char *data, List  car, List  cdr)
{
    List node = malloc(sizeof(struct node));

    if (node == NULL)
    {
        printf("Out of memory!\n");
        exit(1);
    }

    node->data = data;
    node->first = car;
    node->rest = cdr;
    return node;
}


/************************************************************************
implementation note: this returns 1 or 0. In eval, the program takes care
of returning #t or #f
*/
int isSymbol(List  node)
{
    if (node->data != NULL && node->first == NULL && node->rest == NULL)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


/************************************************************************
implementation note: this returns 1 or 0. In eval, the program takes care
of returning #t or #f
*/
int isList(List list)
{
    if (list->data == NULL && list->first != NULL)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**************************************************************************/
List isEqual(List param1, List param2)
{
    if ((param1 == NULL) != (param2 == NULL))
        return getFalse();
    else if ((param1 == NULL) && (param2 == NULL)) // either both null or both not null
        return;

    // printf("pt 1");
    if (isSymbol(param1) && isSymbol(param2))
    {
        // printf("both symbols");
        if (strcmp(param1->data, param2->data) != 0)  // both symbols, should equal each other.
        {
            // printf("both symbols pt 2");
            return getFalse();
        }
    }

    // printf("pt 2");

    if (isEqual(param1->first, param2->first) == getFalse()
            || isEqual(param1->rest, param2->rest) == getFalse())
        return getFalse();

    // printf("got through all the recursion");
    return getTrue();

}

/**************************************************************************/
List assoc(List symbol, List alist)
{
    if (alist == NULL)
    {
        return getFalse();
    }
    else if
    (isSymbol(alist))
    {
        if (strcmp(getData(alist), "()") == 0)
            return getFalse();
    }
    else if (isEqual(symbol, car(car(alist))) == getTrue())
    {
        return car(alist);
    }
    else
    {
        return assoc(symbol, cdr(alist));
    }

    printf("this should not be here.. End of assoc");
}


/****************************************************************/
void printList(List list)
{
    if (list == NULL)
        return;

    int firstElement = 1;
    int firstList = 1;
    List current = list;
    if(isSymbol(current))
    {
        printf("%s", current->data);
        return;
    }
    printf("(");
    while(current != NULL)
    {
        if (current->first->data == NULL)
        {
            List nestedList = current->first;
            if (firstList)
                printList(nestedList);
            else
            {
                printf(" ");
                printList(nestedList);
            }
        }
        else
        {
            printf("%s ", current->first->data);

        }
        current = current->rest;
    }

    printf(")");
}

/****************************************************************
 implementation note :initializes token and also the true and false "constant"
 node that will be used universally in the this program.
*/
List  S_Expression()
{
    isTrue = createNode("#t", NULL, NULL);
    isFalse = createNode("#f", NULL, NULL);

    token = (char*) malloc(sizeof(char) * 20);
    strcpy(token, getToken());
    return sHelper();
}

/****************************************************************
 implementation note: uses a global variable depth instead of using
 as a parameter.
*/
List  sHelper ()
{
    if (*token == '(')
    {
        // if token == (), it means we have an empty list
        if (strcmp(token, "()") == 0)
        {
            if (depth > 0)
            {
                strcpy(token, getToken());
            }
            List emptyNode = createNode("()", NULL, NULL);
            return emptyNode;
        }
        depth = depth + 1;

        // beginning of list
        List  firstNode = createNode(NULL, NULL, NULL);

        strcpy(token, getToken());
        firstNode->first = sHelper(); //first element of list
        List  currentNode = firstNode;

        while (*token != ')')
        {
            List  restNode = createNode(NULL, NULL, NULL);
            currentNode->rest = restNode;
            currentNode->rest->first = sHelper();
            currentNode = currentNode->rest;
        }

        depth = depth - 1;
        if (depth > 0)
        {
            strcpy(token, getToken());
        }
        return firstNode;
    }
    else if (*token == '#')
    {
        char* data = (char*) malloc(sizeof(char) * 20);
        strcpy(data,token);
        List nodeSymbol = createNode(data, NULL, NULL);
        if (depth > 0)
        {
            strcpy(token, getToken());
        }
        return nodeSymbol;
    }
    else if (*token == '\'')
    {
        char* data = (char*) malloc(sizeof(char) * 20);
        strcpy(data,token);
        strcpy(token, getToken());
        List nodeSymbol = createNode("quote", NULL, NULL);
        List  restNode = createNode(NULL, sHelper(), NULL);
        List  firstNode = createNode(NULL, nodeSymbol, restNode);
        return firstNode;
    }
    else
    {
        // returns a single cell with data in it
        char* data = (char*) malloc(sizeof(char) * 20);;
        strcpy(data,token);
        List consNode = createNode(data, NULL, NULL);

        if (depth > 0)
        {
            strcpy(token, getToken());
        }
        return consNode;
    }
}



