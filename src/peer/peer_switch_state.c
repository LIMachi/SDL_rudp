#include <rudp.h>

void	peer_destroy_window(t_rudp_window *win)
{
	struct s_received_data	*it1;
	t_packet_out			*it2;
	void					*tmp;

	it1 = win->received_data;
	while (it1 != NULL)
	{
		tmp = it1;
		it1 = it1->next;
		SDL_free(tmp);
	}
	it1 = win->reassembled_data;
	while (it1 != NULL)
	{
		tmp = it1;
		it1 = it1->next;
		SDL_free(tmp);
	}
	it2 = win->queue;
	while (it2 != NULL)
	{
		tmp = it2;
		it2 = it2->next;
		SDL_free(tmp);
	}
}

int		peer_switch_state(t_rudp *rudp, t_rudp_peer *peer, Uint32 state)
{
	static int		(*state_functions[4])(t_rudp*, UDPpacket*, t_rudp_peer*) = {
		[RUDP_STATE_INIT] = listener_init_state,
		[RUDP_STATE_ACTIVE] = listener_active_state,
		[RUDP_STATE_CLOSING] = listener_closing_state,
		[RUDP_STATE_CLOSED] = listener_closed_state};

	printf("%s: new state: %s\n", rudp->name, stringify_rudp_state(state));
	if (state == RUDP_STATE_CLOSED)
	{
		peer->last_recv = 0;
		peer->instigator = 0;
		peer_destroy_window(&peer->window);
		peer->window = (t_rudp_window){.received_data = NULL,
									.reassembled_data = NULL, .queue = NULL};
	}
	peer->state_function = state_functions[state];
	peer->state = state;
	return (0);
}
