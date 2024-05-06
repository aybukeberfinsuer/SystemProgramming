#ifndef MODULE_H
#define MODULE_H

#include "../lib/libfdr/fields.h"
#include "../lib/libfdr/dllist.h"

void processCommands(const char* inputFileName, const char* outputFileName);
void processWriteCommand(Dllist buffer, IS is);
void processDeleteCommand(Dllist buffer, IS is);
void processGoToEndCommand(Dllist buffer, FILE *os);
void processStopCommand(Dllist buffer, FILE *os);

#endif