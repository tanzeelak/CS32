//
//  linear.cpp
//  cs32hw3
//
//  Created by Tanzeela Khan on 2/4/16.
//  Copyright © 2016 me. All rights reserved.
//
//NUMBER 2
#include <iostream>
#include <string>
using namespace std;


bool anyTrue(const double a[], int n)
{
    if (n == 0) //n == 1?
        return false;
    if (somePredicate(a[0]) == true)
        return true;
    int x = anyTrue(a+1, n-1);
    
    
    if (x == 1)
        return true;
    else
        return false;
}

// Return the number of elements in the array for which the
// somePredicate function returns true.
int countTrue(const double a[], int n)
{
    
    if (n <= 0)
        return 0; //nothing is true;
    if (somePredicate(a[0]))
        return 1 + countTrue(a+1, n-1);
    else
        return countTrue(a+1, n-1);
    
}

// Return the subscript of the first element in the array for which
// the somePredicate function returns true.  If there is no such
// element, return -1.
int firstTrue(const double a[], int n)
{
    if (n == 0)
        return -1;
    
    if (somePredicate(*a))
    {
        return 0;
    }
    else //in the futur!
    {
        int result = firstTrue(a+1, n-1); //you searched whole array!
        if (result == -1)
        {
            return -1;
        }
        else //return the result
        {
            return result +1;
        }
    }
    
}

// Return the subscript of the smallest element in the array (i.e.,
// the one whose value is <= the value of all elements).  If more
// than one element has the same smallest value, return the smallest
// subscript of such an element.  If the array has no elements to
// examine, return -1.
int indexOfMin(const double a[], int n)
{
    
    if (n == 0)
    {
        return -1;
    }
    
    if (n == 1)
        return 0;
    
    if (n == 2 && a[0] <= a[1])
    {
        return 0;
    }
    else if (n == 2 && a[0] > a[1])
    {
        return 1;
    }
    
    int result1 = indexOfMin(a, n/2);
    
    //take care of odds!
    int result2;
    if (n%2 == 0)
        result2 = (n/2 +indexOfMin(a+n/2, n/2));
    else
        result2 = (n/2 +indexOfMin(a+n/2, n/2+1));
    
    
    if (a[result1] <= a[result2])
    {
        return result1;
    }
    return result2;
    
}

// If all n2 elements of a2 appear in the n1 element array a1, in
// the same order (though not necessarily consecutively), then
// return true; otherwise (i.e., if the array a1 does not include
// a2 as a not-necessarily-contiguous subsequence), return false.
// (Of course, if a2 is empty (i.e., n2 is 0), return true.)
// For example, if a1 is the 7 element array
//    10 50 40 20 50 40 30
// then the function should return true if a2 is
//    50 20 30
// or
//    50 40 40
// and it should return false if a2 is
//    50 30 20
// or
//    10 20 20
bool includes(const double a1[], int n1, const double a2[], int n2)
{
    if (n2 == 0)
        return true;
    if (n1 < n2)
        return false;
    if (n1 < 0 || n2 < 0)
        return false;
    
    if (a1[0] == a2[0])
        return includes(a1+1, n1-1, a2+1, n2-1);
    else //(a1[0] != a2[0])
        return includes(a1+1, n1-1, a2, n2);
    /*
     if a1's first element matches a2's first element
     call includes while shifiting a1 and a2
     else if a1's first element doesn't match a2's first elemnt
     call includes shift a1
     
     */
    return false;  // This is not always correct.
}