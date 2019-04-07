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
	out = peer->window.out;
	while (++i < RUDP_MAX_WINDOW)
		if (out[i].not_finished && out[i].mode.need_ack
			&& ack == out[i].mode.ack)
		{
			out[i].not_finished = 0;
			if (out[i].mode.on_ack != NULL)
				out[i].mode.on_ack(rudp, peer, out[i].mode.on_ack_data);
			SDL_free(out[i].packet);
			return (0);
		}
	return (0);
}
