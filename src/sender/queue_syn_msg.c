#include <rudp.h>

int	acknowledged_syn(t_rudp* rudp, t_rudp_peer *peer, void *unused)
{
	(void)unused;
	(void)rudp;
	if (peer->instigator)
	{

	}
	return (0);
}

int	timed_out_syn(t_rudp* rudp, t_rudp_peer *peer, void *unused)
{
	(void)unused;
	(void)peer;
	(void)rudp;
	// clean_peer(peer);
	return (0);
}

int	queue_syn_msg(t_rudp *rudp, t_rudp_peer *peer)
{
	UDPpacket	*pack;
	Uint16		ack;

	if ((pack = SDL_malloc(sizeof(UDPpacket) + 3)) == NULL)
		return (-1);
	ack = ++peer->seq_no;
	*pack = (UDPpacket){.data = &((Uint8*)pack)[sizeof(UDPpacket)], .len = 3,
		.maxlen = 3, .address = {
			.port = rudp->port_out, .host = peer->targeted.host}};
	pack->data[0] = RUDP_TYPE_SYN;
	pack->data[1] = (Uint8)(ack >> 8);
	pack->data[2] = (Uint8)ack;
	return (queue_packet(rudp, peer, pack, (t_queue_mode){.need_ack = 1,
		.can_timeout = 1, .ack = ack, .on_ack = acknowledged_syn,
		.on_ack_data = NULL, .timeout = RUDP_SYN_TIMEOUT,
		.on_timeout = timed_out_syn, .on_timeout_data = NULL, .delay = 0}));
}