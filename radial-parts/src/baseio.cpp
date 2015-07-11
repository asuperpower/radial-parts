/*+===================================================================
  File:        baseio

  Summary:     Implementation of baseio.h

  Author:      Jesse Buhagiar     [Quaker]    4-July-2015

  Rev History:
  4/7/2015: Intial Revision
  7/7/2015: Added file_exists function

----------------------------------------------------------------------
  Copyright 2015-2016 Radial Technologies, Pty Ltd
===================================================================+*/

#include <stdfunc.h>
#include <baseio.h>

/*
 * Function:  write_line
 * --------------------
 *
 *  File Test and ability to dump console
 */
void write_line(const char* fname, std::string text)
{
    std::fstream file;
    file.open(fname, std::ios::out | std::ios_base::app);
    file << text << std::endl;
    file.close();
}

/*
 * Function:  file_exists
 * --------------------
 *
 *  Test whether the file exists
 */
bool file_exists(const char* fname)
{
    std::fstream*   temp = new std::fstream(); //Temporary file pointer
    bool            exists = false;

    temp->open(fname); //Open the file

    if(temp->good())
        exists = true;

    //Delete the temporary file (there are no memory leaks in this dojo!)
    delete temp;

    return exists;

}

