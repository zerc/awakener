//
// Created by Vladimir Savin on 02/06/2022.
//

#ifndef AWAKENER_SERVER_H
#define AWAKENER_SERVER_H

#define RESPONSE_BODY_SIZE 2048

struct request
{
    char method[10];
    char path[100];
};

struct response
{
    char *code;
    char body[RESPONSE_BODY_SIZE];
    int body_size;
};

struct response handle_request(struct request req);
int start_server(void);
int parse_request(char *buf, struct request *r);
#endif // AWAKENER_SERVER_H
