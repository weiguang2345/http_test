#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "../include/http_server.h"
#include "../include/parser.h"
#include "../include/engine.h"
#include "../include/api_handler.h"

#define BUFFER_SIZE 1024

void handle_client(int client_sock,Storage* store){
	char buffer[BUFFER_SIZE];
	ssize_t bytes_read;

	bytes_read = read(client_sock,buffer,sizeof(buffer) - 1);
	if(bytes_read <= 0){
		perror("read");
		close(client_sock);
		return;
	}
	buffer[bytes_read] = '\0';

	char method[8],path[64];
	if(sscanf(buffer,"%7s %63s",method,path) != 2){
		const char* msg = "{\"error\": \"Malformed request line\"}";
		char response[BUFFER_SIZE];
		snprintf(response,sizeof(response),
			"HTTP/1.0 400 Bad Request\r\n"
			"Content_Type: application/json\r\n"
			"content-Length: %zu\r\n"
			"\r\n"
			"%s",strlen(msg),msg);
		write(client_sock,response,strlen(response));
		close(client_sock);
		return;
	}

	char* body = strstr(buffer,"\r\n\r\n");
	if(!body || strlen(body) < 4){
		const char* msg = "{\"error\": \"Missing request body\"}";
		char response[BUFFER_SIZE];
		snprintf(response,sizeof(response),
			"HTTP/1.0 400 Bad Request\r\n"
			"Content_Type: application/json\r\n"
			"content-Length: %zu\r\n"
			"\r\n"
			"%s",strlen(msg),msg);
		write(client_sock,response,strlen(response));
		close(client_sock);
		return;
	}

	body += 4;


	char msg[BUFFER_SIZE];
	handle_api_request(store,path,body,msg,sizeof(msg));

	char response[BUFFER_SIZE * 2];
	snprintf(response,sizeof(response),
			"HTTP/1.0 200 OK\r\n"
			"Content_Type: application/json\r\n"
			"content-Length: %zu\r\n"
			"\r\n"
			"%s",strlen(msg),msg);

	write(client_sock,response,strlen(response));
	close(client_sock);
}

void http_server_start(Storage* store,int port){
	int server_sock,client_sock;
	struct sockaddr_in server_addr,client_addr;
	socklen_t client_len = sizeof(client_addr);

	server_sock = socket(AF_INET,SOCK_STREAM,0);
	if (server_sock < 0){
		perror("socket");
		exit(1);
	}

	memset(&server_addr,0,sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(port);
	
	if(bind(server_sock,(struct sockaddr*)&server_addr,sizeof(server_addr)) < 0){
		perror("bind");
		close(server_sock);
		exit(1);
	}

	if(listen(server_sock,5) < 0){
		perror("listen");
		close(server_sock);
		exit(1);
	}

	printf("HTTP server running on port %d...\n",port);

	while(1){
		client_sock = accept(server_sock,(struct sockaddr*)&client_addr,&client_len);
		if(client_sock < 0){
			perror("accept");
			continue;
		}
		handle_client(client_sock,store);
	}
	close(server_sock);
}
