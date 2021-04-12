#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKPATH "socket-e"

int
main(int argc, char **argv)
{
	char *buf;
	struct sockaddr_un remote;
	size_t bufsiz;
	ssize_t r;
	int sfd;

	if ((sfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}
	memset(&remote, 0, sizeof(remote));
	remote.sun_family = AF_UNIX;
	strncpy(remote.sun_path, SOCKPATH, sizeof(remote.sun_path) - 1);
	if (connect(sfd, (struct sockaddr *)&remote, sizeof(remote)) == -1) {
		perror("connect");
		exit(1);
	}

	buf = NULL;
	bufsiz = 0;
	for (;;) {
		fputs("> ", stdout);
		r = getline(&buf, &bufsiz, stdin);
		if (r == -1) {
			if (!feof(stdin)) {
				perror("getline");
				exit(1);
			}
			break;
		}
		if (send(sfd, buf, r, 0) == -1) {
			perror("send");
			exit(1);
		}
	}
	close(sfd);

	return 0;
}
