	int fd[2];
	pipe(fd);
	fcntl(fd[0], F_SETPIPE_SZ, 1000000);
	int size = fcntl(fd[0], F_GETPIPE_SZ);
	printf("PIPE SIZE= %d\n", size);