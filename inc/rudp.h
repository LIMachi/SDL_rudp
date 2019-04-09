#ifndef RUDP_H
# define RUDP_H

# include <SDL2/SDL_net.h>

# define RUDP_CONNECTION_TIMEOUT 5000

typedef struct s_queue_mode		t_queue_mode;
typedef struct s_packet_out		t_packet_out;
typedef struct s_rudp_window	t_rudp_window;
typedef struct s_rudp_peer		t_rudp_peer;
typedef struct s_rudp			t_rudp;

enum							e_state
{
	RUDP_STATE_INIT,
	RUDP_STATE_ACTIVE,
	RUDP_STATE_CLOSING,
	RUDP_STATE_CLOSED
};

enum							e_state_flags
{
	RUDP_STATE_FLAGS_CLOSED = 0b00000111,
	RUDP_STATE_FLAGS_INIT = 0b01001111,
	RUDP_STATE_FLAGS_ACTIVE = 0b01111111,
	RUDP_STATE_FLAGS_CLOSING = 0b00111011
};

enum							e_type
{
	RUDP_TYPE_NULL = 0,
	RUDP_TYPE_FREE = 1,
	RUDP_TYPE_SYN = 2,
	RUDP_TYPE_ACK = 3,
	RUDP_TYPE_DATA = 4,
	RUDP_TYPE_FIN = 5,
	RUDP_TYPE_NOCONN = 6
};

enum							e_type_bit
{
	RUDP_TYPE_BIT_NULL = 1 << RUDP_TYPE_NULL,
	RUDP_TYPE_BIT_FREE = 1 << RUDP_TYPE_FREE,
	RUDP_TYPE_BIT_SYN = 1 << RUDP_TYPE_SYN,
	RUDP_TYPE_BIT_ACK = 1 << RUDP_TYPE_ACK,
	RUDP_TYPE_BIT_DATA = 1 << RUDP_TYPE_DATA,
	RUDP_TYPE_BIT_FIN = 1 << RUDP_TYPE_FIN,
	RUDP_TYPE_BIT_NOCONN = 1 << RUDP_TYPE_NOCONN
};

/*
** RUDP_NEED_ACK:
**   RUDP_TYPE_BIT_SYN
**   RUDP_TYPE_BIT_DATA
**   RUDP_TYPE_BIT_FIN
*/

#define RUDP_NEED_ACK   0b00110100

/*
** RUDP_RESET_TIME:
**   RUDP_TYPE_NULL
**   RUDP_TYPE_SYN
**   RUDP_TYPE_ACK
**   RUDP_TYPE_DATA
*/

#define RUDP_RESET_TIME 0b00011101

/*
** suggested ports for peer/client/server
*/

#define RUDP_PEER_PORT 0x4242
#define RUDP_SERVER_PORT 0x4444

/*
** maximum number of packet transiting between 2 peer
*/

#define RUDP_MAX_WINDOW 64

/*
** delay after wich a syn packet is considered lost (default aggresive: 500ms,
**   recommended: same as CONNECTION_TIMEOUT: around 5000ms)
*/

#define RUDP_SYN_TIMEOUT 500

/*
** delay between resending unacknowledged packets (default aggresive: 5ms,
**   recommended 30ms)
*/

#define RUDP_RESEND_TIMEOUT 5

struct							s_queue_mode
{
	Uint8						need_ack : 1;
	Uint8						can_timeout : 1;
	Uint16						ack;
	int							(*on_ack)(t_rudp*, t_rudp_peer*, void *);
	void						*on_ack_data;
	Uint32						timeout;
	int							(*on_timeout)(t_rudp*, t_rudp_peer*, void *);
	void						*on_timeout_data;
	Uint32						delay;
};

struct							s_packet_out
{
	struct s_packet_out			*prev;
	struct s_packet_out			*next;
	UDPpacket					*packet;
	t_queue_mode				mode;
	Uint32						tick_queued;
	Uint32						last_sent;
};

/*
** the only type of packet to be bufferized is data
** all other packet are treated as is, ack is kept for
** the seq fields are ignored in reassembled_data
*/

struct							s_received_data
{
	struct s_received_data		*next;
	Uint16						seq_no;
	Uint16						seq_start;
	Uint16						seq_len;
	size_t						size;
	void						*data;
};

struct							s_rudp_window
{
	struct s_received_data		*received_data;
	struct s_received_data		*reassembled_data;
	t_packet_out				*queue;
};

struct							s_rudp_peer
{
	Uint8						instigator;
	Uint32						last_recv;
	Uint16						seq_no;
	Uint32						state;
	SDL_mutex					*mutex;
	int							(*state_function)(t_rudp*,
													UDPpacket*,
													t_rudp_peer*);
	IPaddress					targeted;
	t_rudp_window				window;
};

struct							s_rudp
{
	Uint16						port_in;
	Uint16						port_out;
	UDPsocket					listener_socket;
	UDPsocket					sender_socket;
	Uint32						nb_connections;
	Uint32						used_connections;
	int							*running;
	t_rudp_peer					*peers;
};

/*
** interface:
*/

t_rudp							*rudp_init(Uint16 port_in, Uint16 port_out,
										Uint32 maximum_number_of_connections);
int								rudp_close(t_rudp *rudp);
int								rudp_connect(t_rudp *rudp, const char ip[]);
int								rudp_disconnect(t_rudp *rudp, int id);
int								rudp_send(t_rudp *rudp, int id, void *data,
											size_t size);
int								rudp_receive(t_rudp *rudp, int id, void *data,
											size_t max_size);

/*
** common:
*/

t_rudp							*rudp(Uint16 port_in, Uint16 port_out,
										Uint32 maximum_number_of_connections);
int								start_rudp(t_rudp *rudp);
void							stop_rudp(t_rudp *rudp);

/*
** peer:
*/

t_rudp_peer						*find_peer(t_rudp *rudp, IPaddress target);
t_rudp_peer						*new_peer(t_rudp *rudp, IPaddress target);
int								peer_switch_state(t_rudp_peer *peer,
													Uint32 state);

/*
** listener:
*/

int								listener_thread(t_rudp *rudp);
void							listener_free_msg(t_rudp *rudp,
													UDPpacket *pack);
int								received_ack(t_rudp *rudp, t_rudp_peer *peer,
												Uint16 ack);
void							received_noconn(t_rudp *rudp,
												t_rudp_peer *peer);

/*
** -> states:
*/

int								listener_closed_state(t_rudp *rudp,
														UDPpacket *pack,
														t_rudp_peer *peer);
int								listener_init_state(t_rudp *rudp,
														UDPpacket *pack,
														t_rudp_peer *peer);
int								listener_active_state(t_rudp *rudp,
														UDPpacket *pack,
														t_rudp_peer *peer);
int								listener_closing_state(t_rudp *rudp,
														UDPpacket *pack,
														t_rudp_peer *peer);

/*
** sender:
*/

int								queue_packet(t_rudp *rudp, t_rudp_peer *peer,
										UDPpacket *packet, t_queue_mode mode);
int								queue_syn_msg(t_rudp *rudp, t_rudp_peer *peer);

/*
** msg:
*/

int								msg_acknowledge(t_rudp *rudp, Uint32 target,
												Uint16 ack);
int								msg_no_connection(t_rudp *rudp, Uint32 target);

/*
** utility:
*/

Uint32							get_my_local_ip(void);
void							stringify_ip(Uint32 addr, char buff[15]);
int								in_set(int v, size_t l, int s[]);
Uint16							read_16(const Uint8 *data);
Uint32							read_32(const Uint8 *data);
Uint64							read_64(const Uint8 *data);
void							write_16(Uint8 *data, Uint16 v);
void							write_32(Uint8 *data, Uint32 v);
void							write_64(Uint8 *data, Uint64 v);

#endif
