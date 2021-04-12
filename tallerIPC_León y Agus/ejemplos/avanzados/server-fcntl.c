//Qué problemas presenta este servidor?
//Pista: Ver el uso del CPU del servidor

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKPATH "socket-e"

static int
nonblockingsocket(int s)
{
	int flags;

	if ((flags = fcntl(s, F_GETFL)) == -1)
		return -1;

	return fcntl(s, F_SETFL, flags | O_NONBLOCK);
}

static ssize_t
readsocket(int s)
{
	char buf[1024];
	ssize_t r;

	errno = 0;
	r = recv(s, buf, sizeof(buf) - 1, 0);
	if (r == -1 && errno != EAGAIN && errno != EWOULDBLOCK)
		perror("recv");
	/* cierre de conexión o error */
	if (r == 0 || r == -1)
		goto end;

	/* transforma en string de C */
	buf[r] = '\0';
	printf("[%d] < %s", s, buf);

end:
	return r;
}

int
main(int argc, char **argv)
{
	struct sockaddr_un local, remote;
	int cfd0, cfd1, remotelen, sfd;

	/* crea socket */
	if ((sfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	/* configura dirección */
	memset(&local, 0, sizeof(local));
	local.sun_family = AF_UNIX;
	strncpy(local.sun_path, SOCKPATH, sizeof(local.sun_path) - 1);

	/* asegura que el archivo socket no exista */
	unlink(SOCKPATH);

	/* linkea socket con dirección */
	if (bind(sfd, (struct sockaddr *)&local, sizeof(local)) == -1) {
		perror("bind");
		exit(1);
	}

	/* pone en modo "aceptar conexión" */
	if (listen(sfd, 10) == -1) {
		perror("listen");
		exit(1);
	}

	/* espera conexión */
	remotelen = sizeof(remote);
	if ((cfd0 = accept(sfd, (struct sockaddr *)&remote, (socklen_t *)&remotelen))
	    == -1) {
		perror("accept cfd0");
		exit(1);
	}
	remotelen = sizeof(remote);
	if ((cfd1 = accept(sfd, (struct sockaddr *)&remote, (socklen_t *)&remotelen))
	    == -1) {
		perror("accept cfd1");
		exit(1);
	}

	/* hace no bloqueantes los sockets de clientes */
	if (nonblockingsocket(cfd0) == -1) {
		perror("nonblockingsocket: fcntl cfd0");
		exit(1);
	}
	if (nonblockingsocket(cfd1) == -1) {
		perror("nonblockingsocket: fcntl cfd1");
		exit(1);
	}

	/* atiende cliente */
	for (;;) {
		if (readsocket(cfd0) == 0 || readsocket(cfd1) == 0)
			break;
	}

	/* cierra sockets */
	close(cfd1);
	close(cfd0);
	close(sfd);

	/* remueve el archivo socket */
	if (unlink(SOCKPATH) == -1)
		perror("unlink");

	return 0;
}
