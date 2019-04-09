#include <rudp.h>

/*
** detach and remove the packet holder, return the next packet holder
*/

t_packet_out	*remove_packet(t_packet_out *pack_out)
{
	t_packet_out	*r;

	r = pack_out->next;
	if (pack_out->prev != NULL)
		pack_out->prev->next = r;
	if (r != NULL)
		r->prev = pack_out->prev;
	SDL_free(pack_out->packet);
	SDL_free(pack_out);
	return (r);
}
