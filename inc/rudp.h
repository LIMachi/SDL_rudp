#ifndef RUDP_H
# define RUDP_H

# include <rudp_header.h>

/*
** RUDP_TIMEOUT: if a client/server was not active for Xms, close the connection
** default: 5000
*/

# define RUDP_TIMEOUT 5000

/*
** RUDP_RESEND_DELAY: interval in ms between two sending of the same packet
** default: 10
*/

# define RUDP_RESEND_DELAY 10

/*
** RUDP_RESEND_TRIES: number of times the packet will be resent before
** considered definitely lost
** default: 100
*/

# define RUDP_RESEND_TRIES 100

//perte de packet:
//methode 1: accepter uniquement les packets dans l'ordre d'ack
//methode 2: detecter la perte et demander un renvoi (reception)
//methode 3: renvoyer les packets jusqu'a validation de l'arrivée (envoi)

//la methode 1 peut produire des pertes conséquentes de temps
//la methode 2 est plus stable mais si la demande de renvois est perdu, peut
//perdre du temps (moins que la methode 1, n'affecte que peux la congestion)
//la methode 3 est la plus rapide mais risque de produire une congestion
//importante

//en methode 2, les packets transitant sont les suivants:
//wildcard: receptionable sans connection (udp) (un seul packet)
//renvoi: cible un client pour renvoyer un packet
//connect: cible un serveur pour tenter une connection
//disconnect: serveur/client, signale la fin d'une connection
//segment: 1-plusieurs packet contenant de la donnée brute
//validation: renvoyé par connect/disconnect
//null: keep-alive

//tous les packets ont un identifiant d'envoi (ack) et un checksum (a la fin)

//quand un packet est reçu, il est dabord testé pour voir si il est valide
//(header/checksum/etc)
//on regarde si il vient d'une connection inconnue, si oui on regarde si c'est
//un packet de connection, si ce n'est pas le cas on renvois un
//LRUDP_SEG_NO_CONNECTION, sinon on tente de connecter le client (si la
//connection ne marche pas on renvois un LRUDP_SEG_NO_CONNECTION), si ça marche
//on renvois LRUDP_SEG_VALIDATION et on utilise le ack de la connection comme
//base pour ce client
//les packets wild sont traité tel quel, sans buffering

typedef enum	e_rudp_segment_type
{
	LRUDP_SEG_NULL = 0,
	LRUDP_SEG_WILD = 1,
	LRUDP_SEG_SEND_BACK = 2,
	LRUDP_SEG_CONNECT = 3,
	LRUDP_SEG_DISCONNECT = 4,
	LRUDP_SEG_DATA = 5,
	LRUDP_SEG_VALIDATION = 6,
	LRUDP_SEG_NO_CONNECTION = 7
};

# pragma pack(push, 1)

typedef Uint16	t_checksum;

/*
** at most data_length is 1000 bytes
*/

/*
** data_length is only used by data and wild segments
** ack is used by CONNECT, DISCONNECT and DATA
** sequence number is only used by DATA
*/

typedef struct	s_rudp_segment_head
{
	Uint16		type : 6;
	Uint16		data_length : 10;
	Uint16		ack;
	Uint16		sequence_number;
}				t_rudp_segment_head;

typedef struct	s_rudp_default_segment
{
	t_rudp_segment_head	head;
	Uint8				body[0];
}				t_rudp_default_segment;

typedef struct s_rudp_default_segment	t_rudp_wild_segment;
typedef struct s_rudp_default_segment	t_rudp_data_segment;

typedef struct	s_rudp_send_bakc_segment
{
	t_rudp_segment_head	head;
	Uint16				ack;
	Uint16				sequence_number;
	t_checksum			checksum;
}				t_rudp_send_bakc_segment;

typedef Uint32	t_uid;

typedef struct	s_rudp_connect_segment
{
	t_rudp_segment_head	head;
	t_checksum			checksum;
}				t_rudp_connect_segment;

typedef struct	s_rudp_disconnect_segment
{
	t_rudp_segment_head	head;
	t_uid				id;
	t_checksum			checksum;
}				t_rudp_disconnect_segment;

typedef struct	s_rudp_validation_segment
{
	t_rudp_segment_head	head;
	t_uid				id;
	t_checksum			checksum;
}				t_rudp_validation_segment;

# pragma pack(pop)

typedef struct	s_rdup_packet_holder
{

}				t_rudp_packet_holder;

typedef struct	s_rudp_client
{
	struct s_rudp_client	*next;
	IPaddress				target;
	Uint16					local_port;
	Uint16					ack;
	t_uid					id;
}				t_rudp_client;

typedef struct	s_rudp
{
	t_rudp_client	*clients;
}				t_rudp;

/*
** prepare threads that will do the IO
** the pointer returned will be used by all functions of this library
*/

t_rudp			*rudp_init(void);

/*
** oppen a connection between your port local_port and the port target_port of
** the server/peer address, this function might block for up to RUDP_TIMEOUT ms
** if NULL is returned, an error occured
*/

t_rudp_client	*rudp_connect(t_rudp *rudp, const char *address, Uint16 target_port, Uint16 local_port);

/*
** queue data, this data is guaranteed to arrive in the same order
** return 0 on success
** return an error code on failure (ex: queue is full)
*/

int				rudp_send(t_rudp *rudp, t_rudp_client *target, Uint8 *data, Uint16 size);

/*
** retrieve data, this data is guaranteed to be in the same order as when sent
** return a positive size if data was read
** return 0 if there was no data to read
** return a negative error code if somthing appened (ex: connection closed)
*/

int				rudp_receive(t_rudp *rudp, t_rudp_client *target, Uint8 *data, Uint16 size);

/*
** will broadcast the data to all ip, the data is not guranteed to arrive
*/

int				rudp_wild(t_rudp *rudp, Uint16 port, Uint8 *data, Uint16 size);

/*
** ping the peer/server to make sure the connection isn't closed due to
** innactivity (return 0 if the connection is still active, an error code
** otherwise)
*/

int				rudp_keep_alive(t_rudp *rudp, t_rudp_client *target);

/*
** close a connection
*/

int				rudp_disconnect(t_rudp *rudp, t_rudp_client *client);

/*
** stop the rudp thread, all data is freed
*/

int				rudp_close(t_rudp *rudp);

#endif
