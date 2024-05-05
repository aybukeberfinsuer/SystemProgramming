#ifndef MODULE_H
#define MODULE_H

#include "../lib/libfdr/fields.h"

void processCommands(const char* inputFileName, const char* outputFileName);
void processWriteCommand(IS is, FILE *os);
void processDeleteCommand(IS is, FILE *os);
void processGoToEndCommand(IS is, FILE *os);
void processStopCommand(IS is, FILE *os);

#endif
