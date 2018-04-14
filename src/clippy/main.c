#include "local.h"
#include "remote.h"
#include "storage.h"
/* Standard Libraries */

#define N_ARGS 4

char const *portno;
char const *ip;

void usage(const char *name) { //_Verbose_OPT_* are debug only variables
    fprintf(stdout, "Example Usage: %s -c [ip] [port]\n", name);
    fprintf(stdout, "Arguments:\n"
            "\t-c\t\t[server ip]\n"
            "\t\t\t[server port]\n");
}

int main(int argc, const char *argv[]) {
    if (argc > 3) {
        if (strcmp(argv[1], "-c") == 0) {
            ip = argv[2];
            portno = argv[3];
        } else if (strcmp(argv[1], "-h") == 0) {
            usage(argv[0]);
            return EXIT_FAILURE;
        }
    }

    pthread_t remote_thread;
    pthread_t local_thread;
    char *wa = NULL;
    msg_store = new_storage();

    sigset_t new;
    sigemptyset(&new);
    sigaddset(&new, SIGPIPE);
    if (pthread_sigmask(SIG_BLOCK, &new, NULL) != 0) {
        log_error("Unable to mask SIGPIPE");
        exit(EXIT_FAILURE);
    }

    log_info("IP: %s PORT: %d", ip, portno);

    /* wa = malloc(sizeof(char) * strlen(portno)); */
    /* wa = strncpy(wa, portno, strlen(portno)); */

    /* if (pthread_create(&remote_thread, NULL, remote_connection, wa) < 0) { */
    /*     free(wa); */
    /*     log_error("Unable to create remote thread"); */
    /*     exit(EXIT_FAILURE); */
    /* } */

    if (pthread_create(&local_thread, NULL, local_connection, NULL) < 0) {
        log_error("Unable to create a local thread");
        exit(EXIT_FAILURE);
    }

    /* pthread_join(remote_thread, NULL); */
    pthread_join(local_thread, NULL);

    pthread_exit(NULL);
}