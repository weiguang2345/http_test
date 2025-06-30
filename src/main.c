#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/parser.h"
#include "../include/engine.h"
#include "../include/storage.h"
#include "../include/http_server.h"

int run_cli_mode(){
	Storage* store = storage_create();
	if(!store){
		perror("Failed to create storage");
		return -1;
	}

	char input[512];
	KvCommand cmd;

	printf("MiniKV > ");
	while(fgets(input,sizeof(input),stdin)){
		if(parse_input(input,&cmd) == 0){
			ExecutionResult result = engine_execute(store,&cmd);
			printf("%s\n",result.message);
		}else{
			printf("Invalid command.\n");
		}
		printf("MiniKV > ");
	}

	storage_free(store);
	return 0;
}

int run_http_mode(){
	Storage* store = storage_create();
	if(!store){
		perror("Failed to create storage");
		return -1;
	}

	http_server_start(store,8080);

	storage_free(store);
	return 0;
}

int main(){
	int mode;
	printf("Choose mode: 1 for CLI, 2 for HTTP: ");
	scanf("%d",&mode);
	getchar();

	if(mode == 1){
		return run_cli_mode();
	} else if (mode == 2){
		return run_http_mode();
	} else {
		printf("Invalid mode selected.\n");
		return -1;
	}
}
