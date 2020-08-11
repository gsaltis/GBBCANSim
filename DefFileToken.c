/*******************************************************************************
 * FILE         : DefFileToken.c
 * DATE         : September 26 2018
 * PROJECT      : GBB CAN Simulator
 * COPYRIGHT    : Copyright (c) 2018 Vertiv Company
 *******************************************************************************/
 
 /*******************************************************************************!
 * Global Headers
 *******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>

/*******************************************************************************!
 * Local Headers
 *******************************************************************************/
#include "DefFileToken.h"

/*******************************************************************************!
 * Function : ReadDefTokenFromFile
 *******************************************************************************/
char*
ReadDefTokenFromFile
(
 char*                                  InReturnToken,
 FILE*                                  InFile,
 uint16_t                               InMaxTokenLength
)
{
    char                                readString[DEF_FILE_TOKEN_MAX_LINE_LENGTH+1];
    
    if ( NULL == InReturnToken || NULL == InFile ) {
        return NULL;
    }
    
    // Check that we are not at end of file
    if ( feof(InFile) ) {
        return NULL;
    }
    
    // Try reading a line and check that we read something
    if ( NULL == fgets(readString, DEF_FILE_TOKEN_MAX_LINE_LENGTH+1, InFile) ) {
        return NULL;
    }
    
    // Send it to the string version of read token
    return ReadDefToken(InReturnToken, readString, InMaxTokenLength);
}

/*******************************************************************************!
 * Function : ReadDefToken
 *******************************************************************************/
char*
ReadDefToken
(
 char*                                  InReturnToken,
 char*                                  InString,
 uint16_t                               InMaxTokenLength
)
{
    char*                               commentStart;
    char*                               tokenStart;
    char*                               tokenEnd;
    char*                               stringEnd;
    size_t                              i, n, m, j;
    bool                                commentFound;
    bool                                tokenFound;
    bool                                noToken;
    
    if ( NULL == InReturnToken || NULL == InString || 0 == InMaxTokenLength ) {
        return NULL;
    }
    
    n = strlen(InString);
    
    // Point to last character in input string
    stringEnd = InString + (n-1);
    commentFound = false;
    
    // Find the start of any comment, a '//' string
    for ( commentStart = InString ; 
          commentStart < stringEnd && *commentStart && !commentFound ;
          commentStart++ ) {
        // Comments string can be embedded in a token by the \// sequence
        if ( *commentStart == '\\') {
            commentStart++;
            continue;
        }
        if ( *commentStart == '/' ) {
            if ( commentStart[1] == '\\' ) {
                continue;
            }
            if ( commentStart[1] == '/' ) {
                commentFound = true;
                break;
            }
        }
    }
    
    // If we didn't find it, set it to the true end of the string
    // This gives the starting place to search for the last character of 
    // the token
    if ( !commentFound ) {
        commentStart = stringEnd + 1;
    }
    
    // Find the start of a token (the first non-space character)
    tokenFound = false;
    noToken = false;
    for ( tokenStart = InString ; 
          *tokenStart && !tokenFound && !noToken;
          tokenStart++) {
        // CHeck to see that have not bumped into the comment start
        if ( tokenStart == commentStart ) {
            // We did - we don't have a token
            noToken = true;
        } else if ( !isspace(*tokenStart) ) {
            tokenFound = true;
            break;
        }
    }
    
    // We have reached the end of the string - return an empty string
    if ( *tokenStart == 0x00 ) {
        *InReturnToken = 0x00;
        return InReturnToken;
    }
    // We did not find a token, return an empty string
    if ( noToken ) {
        *InReturnToken = 0x00;
        return InReturnToken;
    }
    
    // Now find the last non-space character in the token
    tokenFound = false;
    for (tokenEnd = commentStart-1 ;
         !tokenFound ;
         tokenEnd -- ) {
        if ( !isspace(*tokenEnd) ) {
            tokenFound = true;
            break;
        }
    }
    // Bump to one character past the last character
    tokenEnd++;
    
    // Copy the characters found without / and return them as the token
    // Copy only up InMaxTokenLength characters
    m = tokenEnd - tokenStart;
    for (j = i = 0; i < m; i++) {
        if ( tokenStart[i] != '\\') {
            if ( j < InMaxTokenLength ) {
                InReturnToken[j] = tokenStart[i];
                j++;
            }
        }
    }
    InReturnToken[j] = 0x00;
    return InReturnToken;
}