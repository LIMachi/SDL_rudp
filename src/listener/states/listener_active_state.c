#include <rudp.h>

/*
** ACK -> do the ack thing
** SYN -> ACK, ignored
** DATA -> ACK, stored
** NULL -> not ACK, reset timer
** FIN -> switch state to CLOSING, ACK
*/

int		listener_active_state(t_rudp *rudp, UDPpacket *pack, t_rudp_peer *peer)
{
	static Uint64	count = 0;

	if (pack->data[0] == RUDP_TYPE_ACK)
		received_ack(rudp, peer, read_32(&pack->data[1]));
	else if (pack->data[0] == RUDP_TYPE_DATA)
	{
		peer->last_recv = SDL_GetTicks();
		msg_acknowledge(rudp, pack->address.host, read_32(&pack->data[1]));
		// printf("%s: succesfully recieved: %hhu,%hu,%hu,%hu,%u: '%.*s'\n", rudp->name, pack->data[0], read_16(&pack->data[1]), read_16(&pack->data[3]), read_16(&pack->data[5]), pack->len - 7, pack->len - 7, &pack->data[7]);
		count += pack->len - 15;
		// printf("total transfer: o: %llu k: %llu m: %llu g: %llu\n", received_data, received_data >> 10, received_data >> 20, received_data >> 30);
		// NULL; //received_data
		received_data(rudp, peer, pack);
	}
	else if (pack->data[0] == RUDP_TYPE_NULL)
		peer->last_recv = SDL_GetTicks();
	else if (pack->data[0] == RUDP_TYPE_FIN)
	{
		msg_acknowledge(rudp, pack->address.host, read_32(&pack->data[1]));
		peer_switch_state(rudp, peer, RUDP_STATE_CLOSED);
	}
	else if (pack->data[0] == RUDP_TYPE_SYN)
		msg_acknowledge(rudp, pack->address.host, read_32(&pack->data[1]));
	return (0);
}
