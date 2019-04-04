#ifndef TRY_3_H
# define TRY_3_H

#include <SDL2/SDL_net.h>

typedef Uint16						t_ack;

typedef enum						e_segment_type
{
	SEGMENT_NULL,
	SEGMENT_DISCONNECT,
	SEGMENT_CONNECT,
	SEGMENT_ACKNOWLEDGEMENT,
	SEGMENT_NOCONN,
	SEGMENT_KEEPALIVE,
	SEGMENT_NACK,
	SEGMENT_SAFE_DATA,
	SEGMENT_LOOSE_DATA,
	SEGMENT_LAN_DISCOVERY
}									t_segment_type;

# pragma pack(push, 1)

typedef struct						s_segment_header
{
	Uint8	type;
	t_ack	ack;
}									t_segment_header;

struct								s_base_segment
{
	struct s_segment_header	header;
};

typedef struct						s_connect_segment
{
	struct s_segment_header	header;
	Uint8					reconnect;
	t_ack					last_recv_ack;
	t_ack					next_recv_ack;
	t_ack					last_sent_ack;
}									t_connect_segment;

typedef struct s_base_segment		t_no_connection_segment;
typedef struct s_base_segment		t_acknowledgement_segment;
typedef struct s_base_segment		t_keep_alive_segment;

typedef struct						s_disconnect_segment
{
	struct s_segment_header	header;
}									t_disconnect_segment;

typedef struct						s_safe_data_segment
{
	struct s_segment_header	header;
	Uint8					data_size;
	Uint8					index;
	Uint16					segment_size;
	Uint8					data[0];
}									t_safe_data_segment;

typedef struct s_safe_data_segment	t_loose_data_segment;

typedef struct						s_lan_discovery_segment
{
	struct s_segment_header	header;
	char					server_name[32];
	Uint8					nb_slots;
	Uint8					used_slots;
}									t_lan_discovery_segment;

# pragma pack(pop)

typedef struct						s_rudp_outbound_packet_holder
{

}									t_rudp_outbound_packet_holder;

typedef struct						s_rudp_inbound_packet_holder
{

}									t_rudp_inbound_packet_holder;

typedef struct s_rudp_object		t_rudp_object;

typedef struct						s_connection_data
{
	struct s_connection_data		*prev;
	struct s_connection_data		*next;
	t_rudp_object					*rudp;
	IPaddress						target;
	Uint32							id;
	t_ack							next_ack;
	t_ack							last_receive_ack;
	Uint32							last_sent;
	Uint32							last_receive;
	Uint8							handshook : 1;
	Uint8							is_client : 1;
	Uint8							is_local_host : 1;
	Uint8							is_open : 1;
	Uint8							is_disconnecting : 1;
}									t_connection_data;

t_connection_data					connection_data(void)
{
	return ((t_connection_data){.id = -1, .target = {.host = 0, .port = 0},
		.next = NULL, .prev = NULL, .handshook = 0, .is_client = 0,
		.is_disconnecting = 0, .is_local_host = 0, .is_open = 0,
		.last_receive = SDL_GetTicks(), .last_receive_ack = -1,
		.last_sent = SDL_GetTicks(), .next_ack = 0, .rudp = NULL});
}

struct								s_rudp_object
{
	UDPsocket						socket_in;
	UDPsocket						socket_out;
	t_rudp_outbound_packet_holder	*out_bound_packets;
	t_rudp_inbound_packet_holder	*inbound_packets;
	t_connection_data				connections[256];
	Uint8							nb_slots;
	Uint8							used_slots;
	int								*running;
};

t_connection_data	*find_connection_data(IPaddress *addr, t_rudp_object *rudp)
{
	Uint8	i;

	i = -1;
	while (++i < rudp->nb_slots)
		if (rudp->connections[i].target.port == addr->port
				&& rudp->connections[i].target.host == addr->host)
			return (&rudp->connections[i]);
	return (NULL);
}

int		in_set(int val, size_t set_size, int set[])
{
	size_t	i;

	i = -1;
	while (++i < set_size)
		if (val == set[i])
			return (1);
	return (0);
}

int		updinack(t_rudp_object *rudp, t_connection_data *cd, UDPpacket *in, int checkprev)
{
	Uint16					last;
	Uint16					next;
	t_segment_header		*head;

	head = (t_segment_header*)in->data;
	if (checkprev && cd != NULL)
	{
		last = head->ack - 1;
		last = parse_recieved(next, last, cd);
	}
	if (last + 1 == head->ack || !checkprev)
	{
		//PacketSwitch(header->type, buffer, bytes, nc, from, sock);
		last = head->ack;
	}
	if (checkprev && cd != NULL && last == head->ack)
		while (1)
		{
			if (!recieved(last + 1, last + 1, cd))
				break ;
			++last;
			parse_recieved(last, last, cd);
		}
	cd = find_connection_data(&in->address, rudp);
	if (cd != NULL && checkprev && head->type != SEGMENT_ACKNOWLEDGEMENT
			&& head->type != SEGMENT_NOCONN)
		if (head->type != SEGMENT_CONNECT && head->type != SEGMENT_DISCONNECT)
			cd->last_receive_ack = last;
}

int is_past_ack(Uint16 test, Uint16 current)
{
	return (((current >= test) && (current - test <= 0x7FFF))
			|| ((test > current) && (test - current > 0x7FFFF)));
}

int		propack(t_rudp_object *rudp, UDPpacket *in, int checkprev)
{
	t_connection_data		*cd;
	t_segment_header		*head;
	t_no_connection_segment	ncs;

	head = (t_segment_header*)in->data;
	cd = find_connection_data(&in->address, rudp);
	if (cd != NULL && head->type != SEGMENT_ACKNOWLEDGEMENT
			&& head->type != SEGMENT_NOCONN)
		acknowledge(rudp, cd, head->ack);
	else if (head->type == SEGMENT_CONNECT || head->type == SEGMENT_DISCONNECT)
		acknowledge(rudp, NULL, head->ack);
	if (cd == NULL && head->type != SEGMENT_CONNECT
			&& head->type != SEGMENT_NOCONN)
	{
		ncs.header.type = SEGMENT_NOCONN;
		send_data(rudp, NULL, &ncs); //SendData((char*)&ncp, sizeof(NoConnectionPacket), from, false, true, NULL, &g_sock, 0, NULL);
		return (0);
	}
	return (updinack(rudp, cd, in, checkprev));
}

void	translate_segment(t_rudp_object *rudp, UDPpacket *in, int bytes,
	int checkprev)
{
	t_segment_header	*head;
	t_connection_data	*cd;
	Uint16				next;
	Uint16				last;

	head = (t_segment_header*)in->data;
	cd = find_connection_data(&in->address, rudp);
	if (cd != NULL)
		cd->last_receive = SDL_GetTicks();
	if (in_set(head->type, 5, (int[5]){SEGMENT_DISCONNECT, SEGMENT_CONNECT,
			SEGMENT_ACKNOWLEDGEMENT, SEGMENT_NOCONN, SEGMENT_NACK}))
		checkprev = 0;
	last = head->ack - (Uint16)1;
	if (checkprev && cd != NULL)
	{
		//must valid the sequence
		next = cd->last_receive_ack + (Uint16)1;
		last = next;
		//old
		if (is_past_ack(head->ack, cd->last_receive_ack) || recieved(head->ack, head->ack, cd))
		{
			acknowledge(rudp, cd, head->ack);
			return ;
		}
		if (head->ack == next)
			last = next; //expected
		else
		{ //future

		}
	}
	return (propack(rudp, in));
}

void	network_input_thread(t_rudp_object *rudp)
{
	IPaddress	ip;
	UDPpacket	*in;
	int			bytes;

	in = SDLNet_AllocPacket(65535);
	while (*rudp->running)
	{
		bytes = SDLNet_UDP_Recv(rudp->socket_in, in);
		if (bytes >= sizeof(UDPpacket) + sizeof(t_segment_header))
			translate_segment(rudp, in, bytes, 1);
	}
	SDLNet_FreePacket(in);
}

#endif
