#ifndef API_HANDLER_H
#define API_HANDLER_H

#include "storage.h"

typedef void (*ApiHandlerFn)(Storage* store,const char * body,char* response,int max_len);

void handle_api_request(Storage* store,const char* path,const char* body,char* response,int max_len);

#endif
