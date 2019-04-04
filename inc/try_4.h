#ifndef TRY_4_H
# define TRY_4_H

#include <SDL2/SDL_net.h>

/*
** https://daoyuan.li/category/projects/12-rudp/
*/

/*
** e_state can be used as a mask of acceptable (on listener) e_type packets
*/

enum	e_state
{
	RUDP_STATE_INIT = 0b00000111,
	RUDP_STATE_PASSIVE = 0b01001111,
	RUDP_STATE_ACTIVE = 0b01111111,
	RUDP_STATE_CLOSING = 0b00111011,
	RUDP_STATE_CLOSED = 0b00000000
};

enum	e_type
{
	RUDP_TYPE_NULL = 0,
	RUDP_TYPE_FREE = 1,
	RUDP_TYPE_SYN = 2,
	RUDP_TYPE_ACK = 3,
	RUDP_TYPE_DATA = 4,
	RUDP_TYPE_FIN = 5,
	RUDP_TYPE_NOCONN = 6
};

enum	e_type_bit
{
	RUDP_TYPE_BIT_NULL = 1 << RUDP_TYPE_NULL,
	RUDP_TYPE_BIT_FREE = 1 << RUDP_TYPE_FREE,
	RUDP_TYPE_BIT_SYN = 1 << RUDP_TYPE_SYN,
	RUDP_TYPE_BIT_ACK = 1 << RUDP_TYPE_ACK,
	RUDP_TYPE_BIT_DATA = 1 << RUDP_TYPE_DATA,
	RUDP_TYPE_BIT_FIN = 1 << RUDP_TYPE_FIN,
	RUDP_TYPE_BIT_NOCONN = 1 << RUDP_TYPE_NOCONN
};

#define RUDP_NEED_ACK   0b00110100
#define RUDP_RESET_TIME 0b00111101

#define RUDP_PORT 0x4242

typedef struct	s_rudp_peer
{
	Uint8		instigator;
	Uint32		last_recv;
	Uint16		seq_no;
	Uint32		state;
	IPaddress	targeted;
}				t_rudp_peer;

typedef struct	s_rudp
{
	Uint16		port;
	UDPsocket	listener_socket;
	UDPsocket	sender_socket;
	Uint32		nb_connections;
	Uint32		used_connections;
	int			*running;
	t_rudp_peer	*peers;
}				t_rudp;

t_rudp_peer	*find_peer(t_rudp *rudp, IPaddress target)
{
	Uint32	i;

	i = (Uint32)-1;
	while (++i < rudp->used_connections)
		if (rudp->peers[i].targeted.host == target.host
				&& rudp->peers[i].targeted.port == target.port)
			return (&rudp->peers[i]);
	return (NULL);
}

t_rudp_peer	*new_peer(t_rudp *rudp, IPaddress target)
{
	Uint32	i;

	i = (Uint32)-1;
	while (++i < rudp->nb_connections)
		if (rudp->peers[i].state == RUDP_STATE_CLOSED)
		{
			rudp->peers[i] = (t_rudp_peer){.targeted = target,
					.instigator = 0, .last_recv = SDL_GetTicks(), .seq_no = 0,
					.state = RUDP_STATE_INIT};
			return (&rudp->peers[i]);
		}
	return (NULL);
}

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

t_rudp	*rudp(Uint16 port, Uint32 maximum_number_of_connections)
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
	out->port = port;
	if ((out->listener_socket = SDLNet_UDP_Open(port)) == NULL)
		return (rudp_fail(out));
	if ((out->sender_socket = SDLNet_UDP_Open(0)) == NULL)
		return (rudp_fail(out));
	out->nb_connections = maximum_number_of_connections;
	out->used_connections = 0;
	return (out);
}

Uint16	read_16(const Uint8 *data)
{
	return ((((Uint16)data[0]) << 8) | (Uint16)data[1]);
}

void	write_16(Uint8 *data, Uint16 v)
{
	data[0] = (Uint8)(v >> 8);
	data[1] = (Uint8)v;
}

Uint32	read_32(const Uint8 *data)
{
	return (((Uint32)data[0]) << 24 | (((Uint32)data[1]) << 16)
		| (((Uint32)data[2]) << 8) | (Uint32)data[3]);
}

void	write_32(Uint8 *data, Uint32 v)
{
	data[0] = (Uint8)(v >> 24);
	data[1] = (Uint8)(v >> 16);
	data[2] = (Uint8)(v >> 8);
	data[3] = (Uint8)v;
}

Uint64	read_64(const Uint8 *data)
{
	return ((((Uint64)data[0]) << 56) | (((Uint64)data[1]) << 48)
		| (((Uint64)data[2]) << 40) | (((Uint64)data[3]) << 32)
		| (((Uint64)data[4]) << 24) | (((Uint64)data[5]) << 16)
		| (((Uint64)data[6]) << 8) | ((Uint64)data[7]));
}

void	write_64(Uint8 *data, Uint64 v)
{
	data[0] = (Uint8)(v >> 56);
	data[1] = (Uint8)(v >> 48);
	data[2] = (Uint8)(v >> 40);
	data[3] = (Uint8)(v >> 32);
	data[4] = (Uint8)(v >> 24);
	data[5] = (Uint8)(v >> 16);
	data[6] = (Uint8)(v >> 8);
	data[7] = (Uint8)v;
}

int		in_set(int v, size_t l, int s[])
{
	size_t	i;

	i = (size_t)-1;
	while (++i)
		if (v == s[i])
			return (1);
	return (0);
}

void	acknowledge(t_rudp *rudp, Uint32 target, Uint16 ack)
{
	UDPpacket	pack;

	pack = (UDPpacket){.data = (Uint8[3]){RUDP_TYPE_ACK, (Uint8)(ack >> 8),
		(Uint8)ack}, .len = 3, .maxlen = 3, .address = {
		.port = rudp->port, .host = target}};
	SDLNet_UDP_Send(rudp->sender_socket, -1, &pack);
}

void	no_connection_msg(t_rudp *rudp, Uint32 target, Uint16 ack)
{
	UDPpacket	pack;

	pack = (UDPpacket){.data = (Uint8[3]){RUDP_TYPE_NOCONN, (Uint8)(ack >> 8),
		(Uint8)ack}, .len = 1, .maxlen = 1, .address = {
		.port = rudp->port, .host = target}};
	SDLNet_UDP_Send(rudp->sender_socket, -1, &pack);
}

/*
** Listener:
** RUDP_TYPE_NULL are always ignored and not ACK but reset the timeout timer
** RUDP_TYPE_FREE are never ACK and are always accepted (do not reset timer)
** RUDP_TYPE_SYN are always ACK but ignored if not in INIT or PASSIVE (reset timer)
** RUDP_TYPE_ACK are never ACK (obsiously) (reset timer)
** RUDP_TYPE_DATA are always ACK and are stored in ACK order for reassembly if in PASSIVE or ACTIVE state (reset timer)
** RUDP_TYPE_FIN are always ACK and only accepted in ACTIVE or CLOSING state, NOCONN response otherwise
** RUDP_TYPE_NOCONN special response, not ACK, close the connection if possible (do not reset timer)
*/

/*
** C1 want to connect to C2 (double confirmation)
** C1S: SYN out (until ACK/NOCONN/timeout), INSTIGATOR
** C2L: SYN in, NEW peer, PASSIVE (only accept FIN/SYN/ACK) state, ACK out (once)
** C1L: ACK in, PASSIVE state
** C2S: SYN out (until ACK/timeout)
** C1L: SYN in (from targeted peer), ACTIVE state, ACK out (once)
** C2L: ACK in, ACTIVE state
*/

/*
** C1 want to disconnect gracefully from C2 (double confirmation)
** C1S: FIN out (until ACK/timeout), INSTIGATOR
** C2L: FIN in, CLOSING state, ACK out (once)
** C1L: ACK in, CLOSING state
** C2S: FIN out (until ACK/timeout)
** C1L: FIN in (from targeted peer), CLOSED state, ACK out (once)
** C2L: ACK in, CLOSED state
*/


int listener_thread(t_rudp *rudp)
{
	UDPpacket		*pack;
	int				bytes;
	int				ts;
	Uint8			*data;
	t_rudp_peer		*peer;

	pack = SDLNet_AllocPacket((Uint16)-1);
	while (*rudp->running)
		if ((bytes = SDLNet_UDP_Recv(rudp->listener_socket, pack)) > 0) //only accept expected packets
		{
			ts = 1 << pack->data[0];
			peer = find_peer(rudp, pack->address);
			if (pack->data[0] == RUDP_TYPE_FREE)
			{
				//broadcasted/non-targeted message
				continue ;
			}
			if (peer == NULL || (in_set(peer->state, 2, (int[2]){
				RUDP_STATE_CLOSED, RUDP_STATE_INIT}) && in_set(pack->data[0], 3,
					(int[3]){RUDP_TYPE_DATA, RUDP_TYPE_FIN, RUDP_TYPE_NULL})))
			{
				if (pack->data[0] != RUDP_TYPE_NULL)
					no_connection_msg(rudp, pack->address.host,
						read_16(&pack->data[1]));
				continue ;
			}
			if (peer->state == RUDP_STATE_CLOSED)
			{
				if (pack->data[0] == RUDP_TYPE_SYN)
				{
					//reconnect
				}
			}
			else if (peer->state == RUDP_STATE_INIT)
			{
				if (pack->data[0] == RUDP_TYPE_SYN)
				{
					//connect
				}
			}
			if (ts & RUDP_RESET_TIME)
				peer->last_recv = SDL_GetTicks();
		}
}

#endif
