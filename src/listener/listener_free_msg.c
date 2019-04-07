#include <rudp.h>

/*
** those packets are stored in a separate queue and only kept for a short period
** of time
*/

void	listener_free_msg(t_rudp *rudp, UDPpacket *pack)
{
	(void)rudp;
	(void)pack;
}
