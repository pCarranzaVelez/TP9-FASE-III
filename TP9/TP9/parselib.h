#include "stdafx.h"
#ifndef PARSELIB_H
#define PARSELIB_H

int parseCmdLine(int argc, char *argv[], int parseCallback(char *key, char *value, void *UserData), void *UserData);

#endif /* PARSELIB_H */

