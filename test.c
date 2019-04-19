#include <rudp.h>

int	server(void *data)
{
	char	*running;
	char	*ip;
	t_rudp	*rudp_obj;
	// int		id;

	running = data;
	ip = &((char*)data)[1];
	(void)ip;
	rudp_obj = rudp_init("server", 0x4444, 0x4242, 1);
	rudp_obj->initial_seq_no = 1234;
	(void)rudp;
	printf("server 1\n");
	// id = rudp_connect(rudp_obj, ip);
	printf("server 2\n");
	while (*running)
		rudp_send(rudp_obj, 0, "test", 5);
	printf("server 3\n");
	rudp_close(rudp_obj);
	// rudp_disconnect(rudp_obj, id);
	printf("server 4\n");
	return (0);
}

int	client(void *data)
{
	char	*running;
	char	*ip;
	t_rudp	*rudp_obj;
	int		id;
	char	received[64];

	running = data;
	ip = &((char*)data)[1];
	rudp_obj = rudp_init("client", 0x4242, 0x4444, 1);
	rudp_obj->initial_seq_no = 4321;
	printf("client 1\n");
	id = rudp_connect(rudp_obj, "127.0.0.1");
	printf("client 2\n");
	while (*running)
		if (rudp_receive(rudp_obj, id, received, 64))
			printf("received '%s'\n", received);
	printf("client 3\n");
	rudp_disconnect(rudp_obj, id);
	rudp_close(rudp_obj);
	printf("client 4\n");
	return (0);
}

int	main(int argc, char *argv[])
{
	char		ip[16];
	SDL_Thread	*s;
	SDL_Thread	*c;

	(void)argc;
	(void)argv;
	SDL_Init(SDL_INIT_EVERYTHING);
	stringify_ip(get_my_local_ip(), &ip[1]);
	printf("local ip: %s\n", &ip[1]);
	ip[0] = 1;
	s = SDL_CreateThread(server, "server", ip);
	c = SDL_CreateThread(client, "client", ip);
	SDL_Delay(5000);
	ip[0] = 0;
	SDL_WaitThread(c, NULL);
	SDL_WaitThread(s, NULL);
	return (0);
}
