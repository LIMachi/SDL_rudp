#include <rudp.h>

t_rudp	*rudp_fail(t_rudp *out)
{
	if (out->listener_socket != NULL)
		SDLNet_UDP_Close(out->listener_socket);
	if (out->sender_socket != NULL)
		SDLNet_UDP_Close(out->sender_socket);
	SDL_free(out->peers);
	SDL_free(out);
	return (NULL);
}

/*
** port_in: listener_port of this rudp object
** port_out: listener_port of peer/client/server rudp object
** the port of the sender socket is always random and used as part of the
** identification of the peer/client/server in pair with the address
** if port_in == port_out -> peer connection
** if port_in != port_out -> server/client connection, opposite connection must
** have reversed port_in/port_out pair to be able to communicate
** if port_in == 0 || port_out == 0 -> invalid connection
*/

t_rudp	*rudp(Uint16 port_in, Uint16 port_out,
			Uint32 maximum_number_of_connections)
{
	t_rudp	*out;

	if (maximum_number_of_connections < 1 || maximum_number_of_connections > 64)
		return (NULL);
	if ((out = (t_rudp*)SDL_malloc(sizeof(t_rudp))) == NULL)
		return (NULL);
	if ((out->peers = (t_rudp_peer*)SDL_malloc(sizeof(t_rudp_peer)
											   * maximum_number_of_connections)) == NULL)
	{
		SDL_free(out);
		return (NULL);
	}
	out->port_in = port_in;
	out->port_out = port_out;
	if ((out->listener_socket = SDLNet_UDP_Open(port_in)) == NULL)
		return (rudp_fail(out));
	if ((out->sender_socket = SDLNet_UDP_Open(0)) == NULL)
		return (rudp_fail(out));
	out->nb_connections = maximum_number_of_connections;
	out->used_connections = 0;
	return (out);
}

/*
** start rudp threads
*/

int		start_rudp(t_rudp *rudp)
{
	(void)rudp;
	return (0);
}

/*
** close and free rudp
*/

void	stop_rudp(t_rudp *rudp)
{
	(void)rudp;
}
