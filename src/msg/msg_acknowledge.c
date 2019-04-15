#include <rudp.h>

int		msg_acknowledge(t_rudp *rudp, Uint32 target, Uint16 ack)
{
	UDPpacket	*pack;

	printf("%s: acknowledge: %d\n", rudp->name, ack);
	if ((pack = SDL_malloc(sizeof(UDPpacket) + 3)) == NULL)
		return (0);
	*pack = (UDPpacket){.data = &((Uint8*)pack)[sizeof(UDPpacket)], .len = 3,
		.maxlen = 3, .address = {.port = rudp->port_out, .host = target}};
	pack->data[0] = RUDP_TYPE_NOCONN;
	write_16(&pack->data[1], ack);
	return (SDLNet_UDP_Send(rudp->sender_socket, -1, pack));
}
