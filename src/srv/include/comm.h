

/*
  CLIENT MESSAGES
*/
const char comm_CLIENT_START[] = "CLIENT_START\n";
const char comm_CLIENT_PING[]  = "CLIENT_PING\n";
const char comm_CLIENT_DISCONNECT[] = "CLIENT_DISCONNECT\n";
const char comm_CLIENT_STATUS[]     = "CLIENT_STATUS\n";

/*
 REPLY MESSAGES
*/

const char comm_REPLY_SUCCESS[]     = "SERVER_SUCCESS\n";
const char comm_REPLY_FAILURE[]     = "SERVER_FAILURE\n";
const char comm_REPLY_WAITING[]     = "SERVER_WAITING\n";
const char comm_REPLY_PONG[]        = "SERVER_PONGING\n";
const char comm_REPLY_DISCONNECT[]  = "SERVER_DISCONNECT\n";
const char comm_REPLY_BUSY[]        = "SERVER_BUSY\n";
const char comm_REPLY_UNKNOWN_CMD[]   = "SERVER_UNKOWN_COMMAND\n";

/*
  PUBLIC FUNCTIONS
*/

void comm_sendtxt(int socket, const char* txt);
