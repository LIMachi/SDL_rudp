#include <rudp.h>

/*
** search the message bound to the ack to flag it as accepted
** if the ack is not catch by any msg, it is ignored
*/

int		received_ack(t_rudp *rudp, t_rudp_peer *peer, Uint16 ack)
{
	int				i;
	t_packet_out	*out;

	i = -1;
	SDL_LockMutex(peer->mutex);
	out = peer->window.queue;
	while (++i < RUDP_MAX_WINDOW && out != NULL)
		if (out->mode.need_ack && ack == out->mode.ack)
		{
			if (out->mode.on_ack != NULL)
				out->mode.on_ack(rudp, peer, out[i].mode.on_ack_data);
			remove_packet(&peer->window, out);
			break ;
		}
	SDL_UnlockMutex(peer->mutex);
	return (0);
}
