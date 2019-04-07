#include <rudp.h>

int		msg_no_connection(t_rudp *rudp, Uint32 target)
{
	UDPpacket	pack;

	pack = (UDPpacket){.data = (Uint8[1]){RUDP_TYPE_NOCONN}, .len = 1,
		.maxlen = 1, .address = {.port = rudp->port_out, .host = target}};
	return (SDLNet_UDP_Send(rudp->sender_socket, -1, &pack));
}
