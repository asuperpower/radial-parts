/*+===================================================================
  File:        baseio

  Summary:     Implementation of baseio.h

  Author:      Jesse Buhagiar     [Quaker]    4-July-2015

  Rev History:
  4/7/2015: Intial Revision

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

