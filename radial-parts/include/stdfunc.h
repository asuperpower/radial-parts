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

#define OK      0
#define WARN    1
#define ERR     2

void            printl(std::string str, int status);

std::string     handle_input();

#endif
