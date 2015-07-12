/*+===================================================================
  File:        stdfunc.h

  Summary:     Header file that should be used program wide!
               Contains basic includes such as 'iostream'

  Author:      Jesse Buhagiar     [Quaker]    2-July-2015

  Rev History:
  2/7/2015: Intial Revision

----------------------------------------------------------------------
  Copyright 2015-2016 Radial Technologies, Pty Ltd
===================================================================+*/

#ifndef STDFUNC_H
#define STDFUNC_H

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <string>
#include <vector>

//Check if we are running on windows and include windows.h
#ifdef _WIN32
    #include <windows.h>
    void setcol(int colour);
#endif


#define OK      0
#define WARN    1
#define ERR     2
#define INFO    3

#define OK_COL      0x0A
#define WARN_COL    0x0E
#define ERR_COL     0x0C
#define INFO_COL    0x0B

void                        printl(std::string str, int status);
void                        cls(); //NOTE: THIS IS VERY HACKY!! conio.h MAY NOT EXIST ON ALL SYSTEMS!!

std::string                 handle_input();
std::vector<std::string>    split(const std::string& input, const char& delim);



#endif
