#include <rudp.h>

/*
** ACK, NOCONN -> switch state to CLOSED
** FIN -> ignored, ACK
** SYN -> switch to INIT state
** DATA -> ACK, NOCONN
** NULL -> ignored
*/

int		listener_closing_state(t_rudp *rudp, UDPpacket *pack, t_rudp_peer *peer)
{
	(void)rudp;
	(void)pack;
	(void)peer;
	return (0);
}
