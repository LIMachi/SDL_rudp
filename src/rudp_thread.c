#include <rudp.h>

int	listen(t_rudp *rudp, UDPpacket *pack)
{
	t_rudp_peer	*peer;

	if (pack->data[0] == RUDP_TYPE_FREE)
		listener_free_msg(rudp, pack);
	else
	{
		if ((peer = find_peer(rudp, pack->address)) != NULL)
			SDL_LockMutex(peer->mutex);
		if (pack->data[0] == RUDP_TYPE_NOCONN)
			received_noconn(rudp, peer);
		else if (peer != NULL)
		{
			peer->state_function(rudp, pack, peer);
			SDL_UnlockMutex(peer->mutex);
		}
		else
			listener_closed_state(rudp, pack, NULL);
	}
}

int	rudp_thread(t_rudp *rudp)
{
	UDPpacket	*listened_packet;
	t_rudp_peer	*peer;
	Uint32		i;

	listened_packet = SDLNet_AllocPacket((Uint16)-1);
	while (rudp->running)
	{
		if (SDLNet_UDP_Recv(rudp->listener_socket, listened_packet))
			listen(rudp, listened_packet);
		i = -1;
		while (++i < rudp->nb_connections)
		{
			peer = &rudp->peers[i];
			SDL_LockMutex(peer->mutex);
			// do the send and timeout
			//
			SDL_UnlockMutex(peer->mutex);
		}
	}
	SDLNet_FreePacket(listened_packet);
	return (0);
}
