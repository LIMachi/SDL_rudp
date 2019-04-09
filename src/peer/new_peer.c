#include <rudp.h>

t_rudp_peer	*new_peer(t_rudp *rudp, IPaddress target)
{
	Uint32	i;

	i = (Uint32)-1;
	while (++i < rudp->nb_connections)
		if (rudp->peers[i].state == RUDP_STATE_CLOSED)
		{
			rudp->peers[i] = (t_rudp_peer){.targeted = target,
				.instigator = 0, .last_recv = SDL_GetTicks(), .seq_no = 0,
				.state = RUDP_STATE_INIT,
				.state_function = listener_init_state,
				.window = {.queue = NULL, .reassembled_data = NULL,
					.received_data = NULL}};
			return (&rudp->peers[i]);
		}
	return (NULL);
}
