#include "eval.h"
#include <stdlib.h>
#include <stdio.h>


/*********************************************************************
 implementation note: this will be called in main.c to initialize
 the environment once in the beginning.
*/
int countParameters(List list)
{
    int count = 0;
    List current = car(cdr(list)); // get the list that has the formal parameters
    while (cdr(current) != NULL)
    {
        count++;
        current = cdr(current);
    }
    return count;
}


/*********************************************************************
 implementation note: some functions such as isEqual and assoc are coded
 in parcer.c and eval.c just calls them. Other functions such as define or
 cond are just dealt with here in eval.c
*/
List evaluate(List list, List environment)
{
    List current = list;
    if (isSymbol(current))
    {
        if (assoc(current, environment) != getFalse() )
        {
            return car(cdr(assoc(current, environment)));
        }
        else
            return current;
    }

    if (strcmp(getData(car(current)), "car") == 0)
    {
        return car(evaluate(car(cdr(current)), environment));
    }
    else if
    (strcmp(getData(car(current)), "cdr") == 0)
    {
        if (cdr(evaluate(car(cdr(current)), environment)) == NULL)
            return createNode("()", NULL, NULL);
        else
            return cdr(evaluate(car(cdr(current)), environment));
    }
    else if
    (strcmp(getData(car(current)), "cons") == 0)
    {
        List param1 = evaluate(car(cdr(current)), environment );
        List param2 = evaluate(car(cdr(cdr(current))), environment );
        return cons(param1, param2);
    }
    else if
    (strcmp(getData(car(current)), "symbol?") == 0)
    {
        if(isSymbol(evaluate(car(cdr(current)), environment))) return getTrue();
        else return getFalse();
    }
    else if
    (strcmp(getData(car(current)), "quote") == 0)
    {
        return car(cdr(current));
    }
    else if
    (strcmp(getData(car(current)), "append") == 0)
    {
        List list1 = evaluate(car(cdr(current)), environment); // else it's a list
        List list2 = evaluate(car(cdr(cdr(current))), environment);
        return append(list1, list2);
    }
    else if
    (strcmp(getData(car(current)), "null?") == 0)
    {
        List param = evaluate(car(cdr(current)), environment);
        // call isNull? with param
        if (isNull(param))
            return getTrue();
        else
            return getFalse();
    }
    else if
    (strcmp(getData(car(current)), "equal?") == 0 || strcmp(getData(car(current)), "=") == 0)
    {
        List param1 = evaluate(car(cdr(current)), environment);
        List param2 = evaluate(car(cdr(cdr(current))), environment);
        return isEqual(evaluate(car(cdr(current)), environment), evaluate(car(cdr(cdr(current))), environment));
    }
    else if
    (strcmp(getData(car(current)), "assoc") == 0)
    {
        List param1 = evaluate(car(cdr(current)), environment);
        List param2 = evaluate(car(cdr(cdr(current))), environment);
        return assoc(param1, param2);

    }
    else if
    (strcmp(getData(car(current)), "cond") == 0)
    {
        List aList = cdr(current);
        List predicate;
        List response;

        while (aList != NULL)
        {
            List currentPair = car(aList);
            if (isSymbol(car(currentPair)))
            {
                if (strcmp(getData(car(currentPair)), "else") == 0 || strcmp(getData(car(currentPair)), "#t") == 0)
                    predicate = getTrue();
            }
            else
                predicate = evaluate(car(currentPair), environment);

            if (isEqual(predicate, getTrue()) == getTrue())
            {
                response = evaluate(car(cdr(currentPair)), environment);
                return response;
            }
            else
                aList = cdr(aList);
        }
        // what to return when no conditions were true?
        return createNode("", NULL, NULL);
    }
    else if
    (strcmp(getData(car(current)), "if") == 0)
    {
        List predicate = evaluate(car(cdr(current)), environment);
        if (isEqual(predicate, getTrue()) == getTrue())
            return evaluate(car(cdr(cdr(current))), environment);
        else
            return evaluate(car(cdr(cdr(cdr(current)))), environment);
    }
    else if
    (strcmp(getData(car(current)), "define") == 0)
    {
        if (isSymbol(car(cdr(current)))) // defining a symbol
        {
            List name = car(cdr(current));
            List definition = createNode(NULL, evaluate(car(cdr(cdr(current))), environment), NULL);
            List pair =  createNode(NULL, name, definition);
            consToEnvironment(pair);
            return name;
        }
        else  // defining a function
        {
            List nameOfFn = car(car(cdr(current)));
            List definition = createNode(NULL, current, NULL);
            List pair = createNode(NULL, nameOfFn, definition);
            consToEnvironment(pair);
            return nameOfFn;
        }
    }
    else if
    (strcmp(getData(car(current)), "environment") == 0)
    {
        return environment;
    }
    else if
    (strcmp(getData(car(current)), "exit") == 0)
    {
        exit(1);
    }
    else if (assoc(car(current), environment) != getFalse())
    {
        // car(current) is a user-defined function^
        List definition = car(cdr(assoc(car(current), environment)));
        int numParameters = countParameters(definition);
        List formalParams = cdr(car(cdr(definition)));
        List actualParams = cdr(current); // still need to be evaluated
        List localEnvironment = environment;

        // create pairs with formal and actual parameters
        int count = numParameters;
        while (count > 0)
        {
            List currentFormalParam = car(formalParams);
            List currentActualParam = evaluate(car(actualParams), environment);
            // make pair
            List definition1 = createNode(NULL, currentActualParam, NULL);
            List pair = createNode(NULL, currentFormalParam, definition1);

            // update
            count--;
            formalParams = cdr(formalParams);
            actualParams = cdr(actualParams);
            localEnvironment = cons(pair, localEnvironment);
        }

        // evaluate the s_expression in the definition
        List output = evaluate(car(cdr(cdr(definition))), localEnvironment);
        return output;
    }
    else if
    (strcmp(getData(car(current)), "+") == 0)
    {
        int count = 0;
        current = cdr(current);
        while(current != NULL)
        {
            count +=  atoi(getData(evaluate(car(current), environment)));
            current = cdr(current);
        }

        char* temp = (char*) malloc(sizeof(char));
        sprintf(temp, "%d", count);
        return createNode(temp, NULL, NULL);
    }
    else if
    (strcmp(getData(car(current)), "-") == 0)
    {
        int substractFrom = atoi(getData(evaluate(car(cdr(current)), environment)));

        current = cdr(cdr(current));

        while(current != NULL)
        {
            substractFrom -=  atoi(getData(evaluate(car(current), environment)));
            current = cdr(current);
        }

        char* temp = (char*) malloc(sizeof(char));
        sprintf(temp, "%d", substractFrom);
        return createNode(temp, NULL, NULL);
    }
    else if
    (strcmp(getData(car(current)), "*") == 0)
    {
        current = cdr(current);
        int count = 1;
        while(current != NULL)
        {
            count *=  atoi(getData(evaluate(car(current), environment)));
            current = cdr(current);
        }

        char* temp = (char*) malloc(sizeof(char));
        sprintf(temp, "%d", count);
        return createNode(temp, NULL, NULL);
    }
    else if
    (strcmp(getData(car(current)), "list") == 0)
    {
        List list = cdr(current); // actual list with atoms that need to be evaluated.
        List returnList = createNode(NULL, evaluate(car(list), environment), NULL);
        List currentAtom = returnList;
        while (cdr(list) != NULL)
        {
            list = cdr(list);
            setCdr(currentAtom, createNode(NULL, evaluate(car(list), environment), NULL));
            currentAtom = cdr(currentAtom);
        }
        return returnList;
    }
    else if
    (strcmp(getData(car(current)), "<") == 0)
    {
        int param1 = atoi(getData(evaluate(car(cdr(current)), environment)));
        int param2 = atoi(getData(evaluate(car(cdr(cdr(current))), environment)));
        if (param1 < param2)
            return getTrue();
        else
            return getFalse();
    }
    else if
    (strcmp(getData(car(current)), ">") == 0)
    {
        int param1 = atoi(getData(evaluate(car(cdr(current)), environment)));
        int param2 = atoi(getData(evaluate(car(cdr(cdr(current))), environment)));
        if (param1 > param2)
            return getTrue();
        else
            return getFalse();
    }
    else if
    (strcmp(getData(car(current)), "list?") == 0)
    {
        if(isList(evaluate(car(cdr(current)), environment)))
            return getTrue();
        else
            return getFalse();
    }
    else if
    (strcmp(getData(car(current)), "function?") == 0)
    {
        if(assoc(car(cdr(current)), environment) != getFalse())
            return getTrue();
        else
            return getFalse();
    }
    else if
    (strcmp(getData(car(current)), "cadr") == 0)
    {
        return cadr(evaluate(car(cdr(current)), environment));
    }
    else if
    (strcmp(getData(car(current)), "caddr") == 0)
    {
        return caddr(evaluate(car(cdr(current)), environment));
    }
    else if
    (strcmp(getData(car(current)), "cadddr") == 0)
    {
        return cadddr(evaluate(car(cdr(current)), environment));
    }
    else if
    (strcmp(getData(car(current)), "caddddr") == 0)
    {
        return caddddr(evaluate(car(cdr(current)), environment));
    }
    else if
    (strcmp(getData(car(current)), "and") == 0)
    {
        List allPredicates =  cdr(current);
        List currentPredicate;


        while (allPredicates != NULL)
        {
            currentPredicate = evaluate(car(allPredicates), environment);
            if (currentPredicate == getFalse())
                return getFalse();
            allPredicates = cdr(allPredicates);
        }
        return currentPredicate;
    }
    else if
    (strcmp(getData(car(current)), "or") == 0)
    {
        List allPredicates =  cdr(current);
        List currentPredicate;


        while (allPredicates != NULL)
        {
            currentPredicate = evaluate(car(allPredicates), environment);
            if (currentPredicate == getTrue())
                return getTrue();
            allPredicates = cdr(allPredicates);
        }
        return getFalse();
    }else if
    (strcmp(getData(car(current)), "not") == 0)
    {
        List predicateToNegate =  evaluate(car(cdr(current)), environment);
        if (predicateToNegate == getTrue())
            return getFalse();
        else
            return getTrue();
    }else if
    (strcmp(getData(car(current)), "last") == 0)
    {
        List list = evaluate(car(cdr(current)), environment);
        while (cdr(list) != NULL)
        {
            list = cdr(list);
        }
        return list;
    }else if
    (strcmp(getData(car(current)), "length") == 0)
    {
        List list = evaluate(car(cdr(current)), environment);
        int count = 0;
        while (!isNull(list))
        {

            count++;
            list = schemeCdr(list);
        }
        char* temp = (char*) malloc(sizeof(char));
        sprintf(temp, "%d", count);
        return createNode(temp, NULL, NULL);
    }

    return current;
}
