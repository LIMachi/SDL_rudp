#include <rudp.h>

/*
** insert in order in received_data if not already treated (target_seq_no >= ack or ack already present in list)
*/

int		received_data(t_rudp *rudp, t_rudp_peer *peer, UDPpacket *pack)
{
	t_received_data	*tmp;
	t_received_data	*out;
	Uint32			ack;
	Uint16			psize;

	// printf("%s: received:\n", rudp->name);
	// print_packet_info(2, pack);
	(void)rudp;
	ack = read_32(&pack->data[RUDP_OFFSET_ACK]);
	// printf("testing oldness: %d < %d\n", ack, peer->target_seq_no);
	if (ack < peer->target_seq_no) //already treated
	{
		// printf("YA OLD MATE\n");
		return (0);
	}
	// printf(" *** *** *** *** treating new data\n");
	if ((tmp = peer->window.received_data) != NULL)
	{
		// printf("*** ** ** *** finding insert: %d\n", ack);
		if (tmp->seq_no > ack) // need to be inserted first
		{
			// printf("** ** ** ** ** insert try: <%d> %d\n", ack, tmp->seq_no);
			tmp = NULL;
		}
		else if (tmp->seq_no <= ack)
		{
			while (tmp->next != NULL && tmp->next->seq_no <= ack)
				tmp = tmp->next;
			if (tmp->seq_no == ack)
			{
				// printf("** ** ** ** ** duplicate\n");
				return (0); //duplicate, do not reincert it
			}
			// printf("** ** ** ** ** insert try: %d <%d> %d\n", tmp->seq_no, ack, tmp->next!= NULL ? tmp->next->seq_no : -1);
		}
	}
	psize = read_16(&pack->data[RUDP_OFFSET_SIZE]);
	if ((out = SDL_malloc(sizeof(t_received_data) + psize)) == NULL)
		return (-1);
	*out = (t_received_data){.seq_no = ack, .prev = tmp, .size = psize,
		.seq_len = read_32(&pack->data[RUDP_OFFSET_LEN]),
		.seq_start = ack - read_32(&pack->data[RUDP_OFFSET_CUT])};
	SDL_memcpy(out->data, &pack->data[RUDP_OFFSET_DATA], out->size);
	if (tmp != NULL)
	{
		// printf("********* new node\n");
		out->next = tmp->next;
		tmp->next = out;
		if (out->next != NULL)
			out->next->prev = out;
	}
	else
	{
		// printf("********* new head\n");
		out->next = peer->window.received_data;
		peer->window.received_data = out;
	}
	return (0);
}
