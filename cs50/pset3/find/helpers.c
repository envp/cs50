/**
 * helpers.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Helper functions for Problem Set 3.
 */
       
#include <cs50.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    return binary_search(value, values, n);
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    int i, j, temp;
    if(n == 1)
    {
        return ;
    }
    for(i = 0; i < n; ++i)
    {
        for(j = 1; j < n - i; ++j)
        {
            if(values[j - 1] > values[j])
            {
                temp = values[j - 1];
                values[j - 1] = values[j];
                values[j] = temp;
            }
        }
    }
    return;
}

/**
 * Searches linearly through an array of numbers
 * Returns true if key exists in array, false otherwise
 */
bool linear_search(int value, int values[], int n)
{
    // dummy counters
    int i;
    for(i = 0; i < n; ++i)
    {
        // Key found at index `i`
        if(value == values[i])
        {
            return true;
        }
    }
    return false;
}

/**
 * Performs binary search on an array of numbers
 * Pre-conditions: Array must be sorted (ascending)
 * Returns true if key exists in array, false otherwise
 */
bool binary_search(int value, int values[], int n)
{
    if(n == 0)
    {
        return false;
    }
    else if(n == 1)
    {
        if(values[0] == value)
        {
            return true;
        }
        return false;
    }
    else if(n > 1)
    {
        return bin_search_core(value, values, n);
    }
    // Only negative n remain
    return false;
}

/**
 * Implemented under the assumption of getting good clean inputs only
 * Pre-conditions: n > 1, values is sorted in ascending order
 * Returns true if key exists in array, false otherwise
 */
bool bin_search_core(int value, int values[], int n)
{
    int mid, low = 0, high = n - 1;
    while(low < high)
    {
        mid = (low + high) / 2;
        if(value == values[mid])
        {
            return true;
        }
        else if(value > values[mid])
        {
            // Discard lower half
            low = mid;
        }
        else if(value < values[mid])
        {
            // Discard upper half
            high = mid;
        }
    }
    
    // If we haven't returned anything yet there ain't nothing to check.
    return false;
}
