#include <rudp.h>

/*
** ACK -> switch state to ACTIVE (the switch is donne by the callback)
** SYN -> ACK
** DATA, FIN, NULL -> NOCONN
** NOCONN -> switch state to CLOSED
*/

int		listener_init_state(t_rudp *rudp, UDPpacket *pack, t_rudp_peer *peer)
{
	if (in_set(pack->data[0], 3, (int[3]){RUDP_TYPE_DATA, RUDP_TYPE_NULL,
			RUDP_TYPE_FIN}))
		msg_no_connection(rudp, pack->address.host);
	else if (pack->data[0] == RUDP_TYPE_NOCONN)
		peer_switch_state(peer, RUDP_STATE_CLOSED);
	else if (pack->data[0] == RUDP_TYPE_ACK)
		received_ack(rudp, peer, read_16(&pack->data[1]));
	else if (pack->data[0] == RUDP_TYPE_SYN)
		msg_acknowledge(rudp, pack->address.host, read_16(&pack->data[1]));
	return (0);
}