#include <rudp.h>

// int	server(void *data)
// {
// 	char	*running;
// 	char	*ip;
// 	t_rudp	*rudp_obj;
// 	int		id;
//
// 	running = data;
// 	ip = &((char*)data)[1];
// 	(void)ip;
// 	// rudp_obj = rudp_init(0x4444, 0x4242, 1);
// 	printf("server 1\n");
// 	// id = rudp_connect(rudp_obj, ip);
// 	printf("server 2\n");
// 	while (*running)
// 		rudp_send(rudp_obj, 0, "test", 5);
// 	printf("server 3\n");
// 	rudp_disconnect(rudp_obj, id);
// 	printf("server 4\n");
// 	return (0);
// }
//
// int	client(void *data)
// {
// 	char	*running;
// 	char	*ip;
// 	t_rudp	*rudp_obj;
// 	int		id;
// 	char	received[64];
//
// 	running = data;
// 	ip = &((char*)data)[1];
// 	rudp_obj = rudp_init(0x4242, 0x4444, 1);
// 	printf("client 1\n");
// 	id = rudp_connect(rudp_obj, ip);
// 	printf("client 2\n");
// 	while (*running)
// 		if (rudp_receive(rudp_obj, id, received, 64))
// 			printf("received '%s'\n", received);
// 	printf("client 3\n");
// 	rudp_disconnect(rudp_obj, id);
// 	printf("client 4\n");
// 	return (0);
// }

int	main(int argc, char *argv[])
{
	char		ip[16];
	t_rudp		*rudp;
	int			id;
	char		buff[256];

	(void)argc;
	(void)argv;
	SDL_Init(SDL_INIT_EVERYTHING);
	stringify_ip(get_my_local_ip(), &ip[1]);
	printf("local ip: %s\n", &ip[1]);
	rudp = rudp_init(0x4242, 0x4242, 1);
	id = rudp_connect(rudp, "127.0.0.1");
	while (SDL_GetTicks() < 5000)
	{
		printf("send\n");
		rudp_send(rudp, id, "test", 5);
		printf("receive\n");
		rudp_receive(rudp, id, buff, 256);
	}
	printf("close\n");
	rudp_close(rudp);
	printf("end\n");
	return (0);
}
