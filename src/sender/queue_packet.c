#include <rudp.h>

static inline void	i_queue(t_rudp_window *win, t_packet_out *out)
{
	t_packet_out	*tmp;

	printf("queue error?\n");
	if (win->queue != NULL)
	{
		tmp = win->queue;
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = out;
		out->prev = tmp;
		out->next = NULL;
	}
	else
	{
		win->queue = out;
		out->prev = NULL;
		out->next = NULL;
	}
	printf("no\n");
}

/*
** packet that do not need ack and aren't delayed are sent right away
** if there is place in the window, store the packet imediately
** otherwise queue the packet to be sent when the window is less full
*/

int					queue_packet(t_rudp *rudp, t_rudp_peer *peer,
								UDPpacket *packet, t_queue_mode mode)
{
	t_packet_out	*out;
	int				i;

	printf("in queue_packet\n");
	if (mode.need_ack || mode.delay > 0)
	{
		i = -1;
		if ((out = SDL_malloc(sizeof(t_packet_out))) == NULL)
			return (RUDP_ERROR_NO_MEMORY);
		*out = (t_packet_out){.mode = mode, .packet = packet,
			.next = NULL, .tick_queued = SDL_GetTicks(),
			.last_sent = SDL_GetTicks() - RUDP_RESEND_TIMEOUT};
		printf("lock mutex\n");
		// SDL_LockMutex(peer->mutex);
		printf("ok\n");
		i_queue(&peer->window, out);
		// SDL_UnlockMutex(peer->mutex);
		return (RUDP_ERROR_OK);
	}
	i = SDLNet_UDP_Send(rudp->sender_socket, -1, packet);
	SDL_free(packet);
	return (i == 1 ? RUDP_ERROR_OK : RUDP_ERROR_SDLNET);
}
