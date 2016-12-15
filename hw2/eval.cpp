//
//  eval.cpp
//  hw2
//
//  Created by Tanzeela Khan on 1/31/16.
//  Copyright © 2016 me. All rights reserved.
//
//NUMBER 5
#include "Map.h"
#include <iostream>
#include <ctype.h>
#include <stack>
#include <cassert>
using namespace std;

bool isValidChar(string ch)
{
    //run forwards to find first char
    bool foundFirst = false;
    int i = 0;
    while (!foundFirst && i < ch.size())
    {
        if (ch[i] == ' ') //ignore spaces
            i++;
        else
        {
            if (isalpha(ch[i]))
            {
                foundFirst = true;
                break;
            }
            if (ch[i] == '(') //ignore brackets
            {
                i++;
                continue;
            }
            return false;
        }
    }
    
    if (foundFirst == false) ////if ya didn't find a begin/end letter, false
        return false;
    
    
    //run backwards to find last char
    bool foundLast = false;
    i = ch.size()-1;
    while (!foundLast && i >= 0)
    {
        if (ch[i] == ' ') //ignore spaces
            i--;
        else
        {
            if (isalpha(ch[i]))
            {
                foundLast = true;
                break;
            }
            if (ch[i] == ')') //ignore brackets
            {
                i--;
                continue;
            }
            return false;
        }
    }
    
    if (foundLast == false) //if ya didn't find a begin/end letter, false
        return false;
    
    
    for (int i = 0; i < ch.size(); i++)
    {
        if (isalpha(ch[i]))
        {
            if (!islower(ch[i]))
                return false;
        }
        if (!(ch[i] == '/' || ch[i] == '*' || ch[i] == '+' || ch[i] == '-' || isalpha(ch[i]) || (ch[i] == '(') || ch[i] == ' ' || ch[i] == ')'))
            return  false;
    }
    
    return  true;
}

int prescedence(char ch)
{
    if (ch == '/' || ch == '*')
        return 2;
    if (ch == '+' || ch == '-')
        return 1;
    return 0;
}


int evaluate(string infix, const Map& values, string& postfix, int& result)
{
    //if valid
    if (infix.size() == 0)
        return 1;
    if (isValidChar(infix) == false)
        return 1;
    
    // now check if () balanced
    
    stack<char> parenth;
    for (int i = 0; i < infix.size(); i++)
    {
        if (infix[i] == '(')
        {
            parenth.push(infix[i]);
        }
        else if (infix[i] == ')')
        {
            if (parenth.empty())
            {
                return 1;
            }
            else
                parenth.pop();
        }
    }
    if (!parenth.empty())
        return 1;
    
    
    //syntactically valid!
    //CONVERT INFIX TO POSTFIX
    string holdpostfix; //manipulate postfix before final
    
    stack<char> aStack;
    for (int i = 0; i < infix.size(); i++)
    {
        if (isalpha(infix[i]))
        {
            holdpostfix += infix[i];
        }
        else if (infix[i] == ' ')
        {}
        else
        {
            switch(infix[i])
            {
                case '(':
                    aStack.push(infix[i]);
                    break;
                case '+':
                    while (!aStack.empty() && (aStack.top() != '(') && prescedence(infix[i]) <= prescedence(aStack.top()))
                    {
                        holdpostfix += aStack.top();
                        aStack.pop();
                    }
                    aStack.push(infix[i]);
                    break;
                case '-':
                    while (!aStack.empty() && (aStack.top() != '(') && prescedence(infix[i]) <= prescedence(aStack.top()))
                    {
                        holdpostfix += aStack.top();
                        aStack.pop();
                    }
                    aStack.push(infix[i]);
                    break;
                case '*':
                    while (!aStack.empty() && (aStack.top() != '(') && prescedence(infix[i]) <= prescedence(aStack.top()))
                    {
                        holdpostfix += aStack.top();
                        aStack.pop();
                    }
                    aStack.push(infix[i]);
                    break;
                case '/':
                    while (!aStack.empty() && (aStack.top() != '(') && prescedence(infix[i]) <= prescedence(aStack.top()))
                    {
                        holdpostfix += aStack.top();
                        aStack.pop();
                    }
                    aStack.push(infix[i]);
                    break;
                case ')':
                    while (!aStack.empty() && aStack.top() != '(')
                    {
                        holdpostfix += aStack.top();
                        aStack.pop();
                    }
                    if (!aStack.empty())
                        aStack.pop();
                    break;
            }
        }
    }
    while (!(aStack.empty()))
    {
        holdpostfix += aStack.top();
        aStack.pop();
    }
    
    //check for whether values map to something
    for (int i = 0; i < infix.size(); i++)
    {
        if (isalpha(infix[i]))
        {
            if (!values.contains(infix[i]))
            {
                postfix = holdpostfix;
                return 2;
            }
        }
    }
    
    //now evaluate!
    Map valuesM = values; //manipulating copy of map
    int retVal;
    
    for (int i = 0; i < holdpostfix.size(); i++)
    {
        if (isalpha(holdpostfix[i]))
            aStack.push(holdpostfix[i]);
        else
        { //operator! must have two values
            int temp = 0, v; //ready to pop two values off stack
            valuesM.get(aStack.top(), temp);
            
            aStack.pop();
            valuesM.get(aStack.top(), v);
            switch (holdpostfix[i])
            {
                case '+':
                    retVal = v + temp;
                    break;
                case '-':
                    retVal = v - temp;
                    break;
                case '*':
                    retVal = v * temp;
                    break;
                case '/':
                    if (temp == 0) //attempts to divide by zero!
                    {
                        postfix = holdpostfix;
                        return 3;
                    }
                    retVal = v / temp; //division operation?
                    break;
            }
            aStack.pop();
            valuesM.insertOrUpdate('&', retVal);
            aStack.push('&');
        }
        
    }
    //should be only one letter in stack;
    if (aStack.size() > 1)
        return 1;
    postfix = holdpostfix;
    valuesM.get(aStack.top(), retVal);
    result = retVal;
    aStack.pop();
    
    return 0;
}
// Evaluates an integer arithmetic expression
// Precondition: infix is an infix integer arithmetic
//   expression consisting of single lower case letter operands,
//   parentheses, and the operators +, -, *, /, with embedded blanks
//   allowed for readability.
// Postcondition: If infix is a syntactically valid infix integer
//   expression whose only operands are single lower case letters
//   (whether or not they appear in the values map), then postfix is
//   set to the postfix form of the expression; otherwise postfix may
//   or may not be changed, result is unchanged, and the function
//   returns 1.  If infix is syntactically valid but contains at
//   least one lower case letter operand that does not appear in the
//   values map, then result is unchanged and the function returns 2.
//   If infix is syntactically valid and all its lower case operand
//   letters appear in the values map, then if evaluating the
//   expression (using for each letter in the expression the value in
//   the map that corresponds to it) attempts to divide by zero, then
//   result is unchanged and the function returns 3; otherwise,
//   result is set to the value of the expression and the function
//   returns 0.
