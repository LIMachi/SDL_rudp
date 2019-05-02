#include <rudp.h>

#define MSG_LEN 8192 + 13
static char test_msg[MSG_LEN] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Suspendisse egestas malesuada nibh, mattis rutrum eros eleifend in. Vivamus tincidunt, turpis vel laoreet tempor, mauris tortor blandit eros, ac lacinia massa augue vel nisl. Aenean condimentum non ipsum vel fringilla. Nam rutrum orci sit amet nisl mollis, nec mattis justo congue. Donec erat ex, blandit a fermentum et, vestibulum porta sem. Donec rutrum nisi sed ante iaculis molestie iaculis nec neque. Fusce vitae mollis ligula, et molestie mauris. Donec ut lectus vestibulum lacus ornare commodo. Aliquam sagittis ex vehicula accumsan dictum. Proin non lorem molestie, consequat nisi ac, tincidunt felis. Cras a dolor nisl. Proin euismod sem at turpis molestie, in tempor sapien gravida. Vivamus sagittis dignissim nisl eu hendrerit. Phasellus hendrerit lorem quis aliquam aliquet. Proin lobortis, est at cursus efficitur, tellus odio consectetur risus, rhoncus tristique sapien metus eu purus. Pellentesque rhoncus lectus suscipit sapien porttitor, quis sodales orci dictum. Nunc vitae dolor vulputate, ullamcorper augue sit amet, porttitor lectus. Ut tempor diam tellus, at interdum sem lobortis eget. Vestibulum faucibus, nisi at dapibus sodales, mauris ipsum hendrerit ipsum, vitae vulputate elit erat non nisi. Suspendisse sollicitudin congue nisl nec laoreet. Sed ut lobortis justo. Etiam felis urna, cursus nec tempor et, sollicitudin et ex. Phasellus euismod diam at diam blandit, a lacinia turpis egestas. Donec pellentesque rhoncus blandit. Sed eget orci et massa accumsan tristique. Morbi ac semper elit, sit amet convallis purus. Pellentesque dignissim metus eu euismod tristique. Cras a quam in risus aliquam congue. Vivamus quis dui sem. Vestibulum vulputate, neque quis semper luctus, justo diam fermentum ipsum, ac placerat tellus sapien a est. Maecenas nec sollicitudin odio, sed ultrices nunc. Sed vitae augue rhoncus, egestas lacus sed, condimentum odio. Aenean sit amet posuere sem. Etiam eu efficitur erat. Proin quis sem nunc. Curabitur facilisis, tortor ac efficitur cursus, erat tortor blandit ligula, et eleifend purus dui quis tortor. Mauris a lacus ut tortor rutrum accumsan eget a quam. Sed rutrum justo nunc, ut varius turpis rhoncus et. Integer luctus vel sapien nec rhoncus. Etiam viverra ultricies augue eget mattis. Morbi efficitur varius dolor. Mauris imperdiet ipsum tristique diam volutpat bibendum. Praesent pulvinar convallis sem, at sollicitudin dolor vestibulum vitae. Phasellus tincidunt urna nibh, eget rhoncus orci sollicitudin a. Integer lobortis justo libero, a mattis erat venenatis nec. Nunc sollicitudin consequat nulla non ultrices. Aliquam nec interdum magna. Donec auctor arcu vitae dignissim lobortis. Donec eget purus et risus luctus consequat. Nullam sit amet erat tempor, pulvinar elit eget, egestas nibh. Class aptent taciti sociosqu ad litora torquent per conubia nostra, per inceptos himenaeos. Praesent sagittis mi tellus, non vulputate leo fermentum a. Suspendisse congue, velit non placerat aliquam, mi enim scelerisque magna, ac auctor risus nisl sit amet ipsum. Nam est risus, ultricies in urna lobortis, convallis convallis nisi. Proin quis purus arcu. Curabitur euismod sagittis augue, sit amet feugiat est pretium in. Maecenas placerat interdum nisl, eu volutpat est auctor pharetra. Nullam vel nisl quam. Maecenas feugiat enim est, eget maximus leo fermentum vitae. Sed maximus luctus nisi, vel gravida urna molestie vitae. Donec neque neque, finibus vitae volutpat eget, tristique in libero. Vivamus pretium nisi felis, tempus volutpat massa lobortis sit amet. Sed commodo dapibus dapibus. Duis accumsan turpis ac consequat volutpat. Donec ullamcorper lorem orci, id vulputate ipsum pharetra et. Nulla vel blandit orci. Vivamus suscipit iaculis sem. Vivamus faucibus dolor sed est ultricies varius. Praesent ex diam, semper auctor condimentum ut, dictum vel nunc. Suspendisse sapien ante, pellentesque et dui at, cursus vestibulum nisl. Cras et neque urna. Curabitur eu lacus nunc. Phasellus metus lacus, molestie in enim vitae, maximus vehicula diam. Nam quis fermentum quam. Vivamus nunc lacus, tristique non nisl a, dapibus tempus magna. Proin congue molestie nisi. Vivamus iaculis justo ex, ac lacinia leo facilisis a. Aliquam erat volutpat. Phasellus tempor diam sit amet felis rutrum feugiat. Nunc quis dignissim ante. Vestibulum fringilla commodo vehicula. Fusce consequat tortor eget justo vestibulum gravida. Suspendisse aliquam ligula nec justo ornare, sit amet eleifend mi fermentum. Aliquam vulputate, enim ornare placerat pharetra, nibh odio molestie augue, sed convallis nunc dolor at enim. Duis congue tempus vulputate. Etiam bibendum consequat purus, sed mollis dolor laoreet quis. Fusce tempor leo lacinia elementum porta. Nulla sed nisl ante. Ut aliquam risus sed mi tempus, non eleifend dui accumsan. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Class aptent taciti sociosqu ad litora torquent per conubia nostra, per inceptos himenaeos. Suspendisse non aliquam massa. Mauris vulputate efficitur quam, vitae pulvinar nisl dapibus a. Orci varius natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Sed ultricies faucibus dui sit amet suscipit. Duis et imperdiet libero, sed consectetur erat. In ut risus porttitor, aliquet odio a, porttitor libero. Curabitur et elit pulvinar, venenatis lorem et, tincidunt lectus. Nunc condimentum, lectus quis congue dapibus, ligula ipsum pretium lacus, eget lobortis orci sem non metus. Aliquam malesuada sit amet elit eu pharetra. Aliquam eget mi nibh. Proin non laoreet erat, vulputate ultrices dui. Curabitur sodales quis diam eu imperdiet. Duis nec ipsum in diam varius varius id nec est. In hac habitasse platea dictumst. Ut massa ante, tincidunt at mi et, vehicula pretium arcu. Aliquam iaculis vel ligula at dapibus. Quisque interdum lacus eu arcu elementum elementum. Aenean fringilla a massa mattis porta. Proin cursus ultricies lacus, a lacinia diam feugiat at. Proin at erat molestie, feugiat tortor sit amet, lobortis tortor. Duis finibus bibendum nisl, ac ornare nibh pellentesque sit amet. Etiam non gravida ante, eu hendrerit odio. Morbi libero eros, mattis ut velit sed, consectetur fermentum ex. Ut tempor ac sapien vel volutpat. Duis consectetur dolor mauris, id volutpat lectus sollicitudin sit amet. Aenean odio augue, interdum nec sapien nec, facilisis semper lacus. Morbi magna ligula, blandit sed nibh at, hendrerit finibus nisi. Nunc mi dolor, sollicitudin non mi in, porta pellentesque massa. Etiam malesuada condimentum urna, eget sollicitudin felis ultrices ac. Nunc et nunc neque. Nam nec bibendum nibh. In sed sem id ex dapibus maximus sit amet vel libero. Nullam porta dui ut consectetur aliquam. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. Nulla lacinia urna in nibh tincidunt euismod. Nam eget laoreet arcu, ac feugiat nulla. Quisque id elementum ante. Etiam lobortis egestas magna sed consectetur. Quisque non tincidunt sapien, in efficitur nunc. Morbi non tortor ut sapien lacinia elementum. Praesent ut mollis libero. Curabitur tempor suscipit convallis. Nullam mollis massa mauris, ut elementum nisi egestas vitae. Aliquam et justo nulla. Maecenas at tortor tincidunt, placerat dolor ut, porttitor purus. Vestibulum eu elit ac erat sagittis cursus vitae vel lectus. Pellentesque vel elit augue. Fusce sit amet tincidunt odio. Ut egestas dignissim nisl sit amet tincidunt. Vivamus quis sollicitudin ante. Donec ullamcorper nibh sagittis nisi tristique, molestie vehicula magna porttitor. Nunc dictum molestie odio, et tristique nulla placerat sit amet. Vivamus ut tellus est. Donec tincidunt ullamcorper nulla, iaculis sagittis lorem sagittis at. Quisque sed lacus eu sapien ullamcorper interdum nec quis nisl. Proin eu nibh quam. Fusce faucibus turpis risus, ac interdum libero dapibus eget. Etiam cursus fermentum sapien. Nam non quam et justo vehicula ultrices ut ut ante. Donec quis turpis et lacus efficitur bibendum. Nunc neque lorem, semper vel ullamcorper vestibulum, placerat pellentesque mi. Donec porta sem quam, ac faucibus nisi porta eu. Nam sit amet turpis ac neque laoreet orci aliquam.";

int	server(void *data)
{
	char	*running;
	t_rudp	*rudp_obj;
	Uint32	tick;

	running = data;
	rudp_obj = rudp_init("server", 0x4444, 0x4242, 1);
	rudp_obj->initial_seq_no = 123456;
	tick = SDL_GetTicks();
	while (*running || rudp_obj->used_connections)
	{
		if (SDL_GetTicks() > tick + 16)
		{
			rudp_send(rudp_obj, 0, test_msg, 8192 + 13);
			tick = SDL_GetTicks();
		}
	}
	rudp_close(rudp_obj);
	return (0);
}

int	client(void *data)
{
	char	*running;
	char	*ip;
	t_rudp	*rudp_obj;
	int		id;
	char	received[MSG_LEN + 10];
	int		l;
	int		total;

	running = data;
	ip = &((char*)data)[1];
	rudp_obj = rudp_init("client", 0x4242, 0x4444, 1);
	rudp_obj->initial_seq_no = 654321;
	id = rudp_connect(rudp_obj, ip);
	if (id < 0)
		printf("fail to connect\n");
	total = 0;
	while (id >= 0 && *running)
		if ((l = rudp_receive(rudp_obj, id, received, MSG_LEN + 10)) > 0)
		{
			if (l != MSG_LEN || SDL_memcmp(test_msg, received, MSG_LEN))
				exit(0 * printf("DA FAILURE %d '%.*s'\n", l, l, received));
			total += l;
			printf("(%d, %dM %dK %d)\n", l, total >> 20, (total >> 10) & 0x3FF, total & 0x3FF);
		}
	rudp_disconnect(rudp_obj, id);
	rudp_close(rudp_obj);
	return (0);
}

//localhost: 127.0.0.1

int	main(int argc, char *argv[])
{
	char		ip[17];
	char		local_ip[16];
	SDL_Thread	*t;

	if (argc < 2 || (SDL_strcmp(argv[1], "client") && SDL_strcmp(argv[1], "server")))
	{
		printf("%s [client <ip>] [server]\n", argv[0]);
		return (0);
	}
	SDL_Init(SDL_INIT_EVERYTHING);
	stringify_ip(get_my_local_ip(), local_ip);
	printf("local ip: %s\n", local_ip);
	ip[0] = 1;
	if (!SDL_strcmp(argv[1], "client"))
	{
		if (argc > 2)
		{
			SDL_memcpy(&ip[1], argv[2], SDL_strlen(argv[2]) < 15 ? SDL_strlen(argv[2]) + 1 : 15);
			ip[16] = '\0';
		}
		else
			SDL_memcpy(&ip[1], "127.0.0.1", 10);
		t = SDL_CreateThread(client, "client", ip);
	}
	else
		t = SDL_CreateThread(server, "server", ip);
	SDL_Delay(15000);
	ip[0] = 0;
	SDL_WaitThread(t, NULL);
	return (0);
}
