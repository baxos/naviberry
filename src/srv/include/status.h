#ifndef STATUS_H
#define STATUS_H

extern const char SERVER_STATUS_WAITING[];
extern const char SERVER_STATUS_BUSY[]   ;
extern const char SERVER_STATUS_READY[]  ;
extern const char SERVER_STATUS_FAILURE[];


char* server_GetStatus();
void server_SetStatus(const char status[]);

#endif
