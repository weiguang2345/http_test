#ifndef ENGINE_H
#define ENGINE_H

#include "parser.h"
#include "storage.h"

typedef struct {
	int code;
	char message[256];
} ExecutionResult;

ExecutionResult engine_execute(Storage* storage,const KvCommand* cmd);

#endif
