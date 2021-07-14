/**
 * helpers.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Helper functions for Problem Set 3.
 */
       
#include <cs50.h>
#include <string.h>
#include <stdio.h>

#include "helpers.h"

// function declaration
int findMidPoint(int min, int max);

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    // check that there are values in the array
    if(n < 1)
    {
        return false;
    }
    
    // set the minimum and maximum index of the array 
    int min = 0;
    int max = n - 1;
    
    // implement binary search
    while(max >= min)
    {
        // calculate midpoint
        int midpoint = findMidPoint(min, max);
        
        // check if midpoint value = "needle"
        if(values[midpoint] == value)
        {
            return true;
        }
        // check if midpoint value is less than "needle"
        else if(values[midpoint] < value)
        {
            // set new minimum index
            min = midpoint + 1;
        }
        //check if midpoint value is more than "needle"
        else
        {
            // set new maximum index
            max = midpoint - 1;
        }
    }   
    return false;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // implement selection sort
    for(int i = 0; i < n; i++)
    {
        // set i indexed value as minimum value
        int min = i;
        for(int j = i + 1; j < n + 1; j++)
        {
            // sorting
            if(values[j] < values[min])
            {
                min = j;
            }
            
            if(min != i)    
            {
                // move minimum value in the sorted list
                int k = values[min];
                values[min] = values[i];
                values[i] = k;
            }
             
        }
       
    }
    return;
}

// Function implementation for calculating midpoint
int findMidPoint(int min, int max)
{
    int mid = (min + max)/2;
    return mid;
}