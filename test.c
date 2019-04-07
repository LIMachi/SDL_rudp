#include <rudp.h>

int	server(void *data)
{
	char	*running;
	char	*ip;
	t_rudp	*rudp_obj;
	int		id;

	running = data;
	ip = &((char*)data)[1];
	rudp_obj = rudp(0x4444, 0x4242, 1);
	id = rudp_connect(rudp_obj, ip);
	while (*running)
		rudp_send(rudp_obj, id, "test", 5);
	rudp_disconnect(rudp_obj, id);
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
	rudp_obj = rudp(0x4242, 0x4444, 1);
	id = rudp_connect(rudp_obj, ip);
	while (*running)
		if (rudp_receive(rudp_obj, id, received, 64))
			printf("received '%s'\n", received);
	rudp_disconnect(rudp_obj, id);
	return (0);
}

int	main(int argc, char *argv[])
{
	SDL_Thread	*c;
	SDL_Thread	*s;
	char		ip[16];

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
