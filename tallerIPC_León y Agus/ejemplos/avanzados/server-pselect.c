#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define SOCKPATH "socket-e"

ssize_t readsocket(int s){
	char buf[1024];
	ssize_t r;

	r = recv(s, buf, sizeof(buf) - 1, 0);
	if (r == -1)
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
	fd_set rdfs;
	struct sockaddr_un local, remote;
	int cfd0, cfd1, remotelen, s, sfd;

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

	/* atiende cliente */
	for (;;) {
		/* setea el fd_set de fd de lectura */
		FD_ZERO(&rdfs);
		FD_SET(cfd0, &rdfs);
		FD_SET(cfd1, &rdfs);

		/* espera lectura, no escritura, no errores, no señalees, sin
		 * timeout */
		s = pselect(MAX(cfd0, cfd1) + 1, &rdfs, NULL, NULL, NULL, NULL);
		if (s == -1) {
			perror("pselect");
			break;
		}

		/* checkea los sockets para leer */
		if (FD_ISSET(cfd0, &rdfs)) {
			if (readsocket(cfd0) == 0)
				break;
		}
		if (FD_ISSET(cfd1, &rdfs)) {
			if (readsocket(cfd1) == 0)
				break;
		}
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
