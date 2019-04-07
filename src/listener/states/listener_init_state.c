#include <rudp.h>

/*
** ACK -> switch state to ACTIVE
** SYN -> ACK
** DATA, FIN, NULL -> NOCONN
** NOCONN -> switch state to CLOSED
*/

int		listener_init_state(t_rudp *rudp, UDPpacket *pack, t_rudp_peer *peer)
{
	(void)rudp;
	(void)pack;
	(void)peer;
	return (0);
}
