#include <rudp.h>

void	received_noconn(t_rudp *rudp, t_rudp_peer *peer)
{
	(void)rudp;
	if (peer != NULL)
		peer_switch_state(peer, RUDP_STATE_CLOSED);
}
