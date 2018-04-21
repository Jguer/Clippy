#ifndef REMOTE_HEADER
#define REMOTE_HEADER

#include "library/clipboard.h"
#include "storage.h"
#include "utils/defs.h"
#include "utils/log.h"
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <ifaddrs.h>
#include <limits.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>
#include <time.h>
#include <unistd.h>

#define SOCK_PATH "/tmp/CLIPBOARD_SOCKET"

char const *portno;
char const *ip;

void *accept_local_client(void *);
void *local_connection(void *);
void *accept_remote_client(void *);
void *remote_connection(void *);

#endif
