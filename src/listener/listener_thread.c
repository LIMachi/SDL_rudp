#include <rudp.h>

/*
** Listener:
** RUDP_TYPE_NULL are always ignored and not ACK but reset the timeout timer
** RUDP_TYPE_FREE are never ACK and are always accepted (do not reset timer)
** RUDP_TYPE_SYN are always ACK but ignored if not in INIT or PASSIVE (reset timer)
** RUDP_TYPE_ACK are never ACK (obsiously) (reset timer)
** RUDP_TYPE_DATA are always ACK and are stored in ACK order for reassembly if in PASSIVE or ACTIVE state (reset timer)
** RUDP_TYPE_FIN are always ACK and only accepted in ACTIVE or CLOSING state, NOCONN response otherwise
** RUDP_TYPE_NOCONN special response, not ACK, close the connection if possible (do not reset timer)
*/

/*
** C1 want to connect to C2 (double confirmation)
** C1S: SYN out (until ACK/NOCONN/timeout), INIT state, INSTIGATOR
** C2L: SYN in, NEW peer, INIT state, ACK out (once)
** C2S: SYN out (until ACK/timeout)
** C1L: ACK in, wait for SYN
** C1L: SYN in (from targeted peer), ACTIVE state, ACK out (once)
** C2L: ACK in, ACTIVE state
*/

/*
** C1 want to disconnect gracefully from C2 (double confirmation)
** C1S: FIN out (until ACK/timeout), INSTIGATOR
** C2L: FIN in, CLOSING state, ACK out (once)
** C1L: ACK in, CLOSING state
** C2S: FIN out (until ACK/timeout)
** C1L: FIN in (from targeted peer), CLOSED state, ACK out (once)
** C2L: ACK in, CLOSED state
*/

/*
enum	e_type
{
	RUDP_TYPE_NULL = 0,
	RUDP_TYPE_FREE = 1,
	RUDP_TYPE_SYN = 2,
	RUDP_TYPE_ACK = 3,
	RUDP_TYPE_DATA = 4,
	RUDP_TYPE_FIN = 5,
	RUDP_TYPE_NOCONN = 6
};
*/

int listener_thread(t_rudp *rudp)
{
	UDPpacket		*pack;
	int				(*state_functions[5])(t_rudp*, UDPpacket*, t_rudp_peer*);
	t_rudp_peer		*peer;

	pack = SDLNet_AllocPacket((Uint16)-1);
	state_functions[RUDP_STATE_INIT] = listener_init_state;
	state_functions[RUDP_STATE_ACTIVE] = listener_active_state;
	state_functions[RUDP_STATE_CLOSING] = listener_closing_state;
	state_functions[RUDP_STATE_CLOSED] = listener_closed_state;
	while (*rudp->running)
		if (SDLNet_UDP_Recv(rudp->listener_socket, pack))
		{
			if (pack->data[0] == RUDP_TYPE_FREE)
			{
				listener_free_msg(rudp, pack);
				continue ;
			}
			peer = find_peer(rudp, pack->address);
			if (peer != NULL)
				state_functions[peer->state](rudp, pack, peer);
			else
				state_functions[RUDP_STATE_CLOSED](rudp, pack, NULL);
		}
	SDLNet_FreePacket(pack);
	return (0);
}
