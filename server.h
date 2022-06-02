//
// Created by Vladimir Savin on 02/06/2022.
//

#ifndef AWAKENER_SERVER_H
#define AWAKENER_SERVER_H

struct request {
    char method[10];
    char path[100];
};

struct response {
    char *code;
    char *body;
};

void handle_request(struct request req, struct response *res);
int start_sever();
#endif //AWAKENER_SERVER_H
