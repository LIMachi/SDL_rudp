#include <rudp.h>

/*
** SYN -> ACK, ignored
** DATA -> ACK, stored
** NULL -> not ACK, reset timer
** FIN -> switch state to CLOSING, ACK
** NOCONN -> switch state to CLOSED
*/

int		listener_active_state(t_rudp *rudp, UDPpacket *pack, t_rudp_peer *peer)
{
	(void)rudp;
	(void)pack;
	(void)peer;
	return (0);
}
