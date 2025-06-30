#include <stdio.h>
#include <string.h>
#include "../include/engine.h"
#include "../include/storage.h"

ExecutionResult engine_execute(Storage* storage,const KvCommand* cmd){
	ExecutionResult result = {0};

	switch(cmd->type){
		case CMD_SET:
			if(storage_set(storage,cmd->key,cmd->value) == 0){
				snprintf(result.message,sizeof(result.message),"{\"status\":\"ok\"}");
				result.code = 0;
			} else {
				snprintf(result.message,sizeof(result.message),"{\"error\":\"set failde\"}");
				result.code = -1;
			}
			break;

		case CMD_GET:
			const char* val = storage_get(storage,cmd->key);
			if(val){
				snprintf(result.message,sizeof(result.message),"{\"value\":\"%s\"}",val);
				result.code = 0;
			} else {
				snprintf(result.message,sizeof(result.message),"{\"error\":\"not found\"}");
				result.code = -1;
			}
			break;

		default:
			snprintf(result.message,sizeof(result.message),"{\"error\":\"unknown command\"}");
			result.code = -1;
			break;
	}

	return result;
}
