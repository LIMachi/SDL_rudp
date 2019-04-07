#include <rudp.h>

int		msg_acknowledge(t_rudp *rudp, Uint32 target, Uint16 ack)
{
	UDPpacket	pack;

	pack = (UDPpacket){.data = (Uint8[3]){RUDP_TYPE_ACK, (Uint8)(ack >> 8),
		(Uint8)ack}, .len = 3, .maxlen = 3, .address = {
		.port = rudp->port_out, .host = target}};
	return (SDLNet_UDP_Send(rudp->sender_socket, -1, &pack));
}
