#include <rudp.h>

t_rudp	*rudp_init(char *name, Uint16 port_in, Uint16 port_out,
					Uint32 maximum_number_of_connections)
{
	t_rudp	*out;

	out = rudp(port_in, port_out, maximum_number_of_connections);
	if (out == NULL)
		return (NULL);
	out->name = name;
	out->running = 1;
	out->listener_thread = SDL_CreateThread((SDL_ThreadFunction)listener_thread,
		"rudp_listener_thread", out);
	out->sender_thread = SDL_CreateThread((SDL_ThreadFunction)sender_thread,
											"rudp_sender_thread", out);
	return (out);
}
