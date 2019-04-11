#include <rudp.h>

int		valid_param(t_rudp *rudp, int id, void *data, int size)
{
	if (size <= 0)
		return (RUDP_ERROR_INVALID_SIZE);
	if (rudp == NULL || data == NULL)
		return (RUDP_ERROR_NULL_POINTER);
	if (id < 0 || id > (int)rudp->nb_connections)
		return (RUDP_ERROR_INVALID_ID);
	if (rudp->peers[id].state != RUDP_STATE_ACTIVE)
		return (RUDP_ERROR_CLOSED_CONNECTION);
	return (0);
}

int		rudp_send(t_rudp *rudp, int id, void *data, int size)
{
	int				cut;
	UDPpacket		*pack;
	t_queue_mode	mode;
	Uint16			len;
	Uint16			original_ack;

	if ((cut = valid_param(rudp, id, data, size)))
		return (cut);
	mode = (t_queue_mode){.delay = 0, .need_ack = 1, .can_timeout = 0,
		.on_ack = NULL};
	len = (Uint16)(size / RUDP_MAXIMUM_DATA_SIZE);
	len += size > len * RUDP_MAXIMUM_DATA_SIZE;
	original_ack = rudp->peers[id].seq_no + (Uint16)1;
	while (size > 0)
	{
		cut = size > RUDP_MAXIMUM_DATA_SIZE ? RUDP_MAXIMUM_DATA_SIZE : size;
		pack = SDL_malloc(sizeof(UDPpacket) + cut + 7);
		if (pack == NULL)
			return (-1);
		*pack = (UDPpacket){.data = &((Uint8*)pack)[sizeof(UDPpacket)],
			.maxlen = cut, .len = cut, .address = {
				.host = rudp->peers[id].targeted.host, .port = rudp->port_out}};
		mode.ack = ++rudp->peers[id].seq_no;
		pack->data[0] = RUDP_TYPE_DATA;
		write_16(&pack->data[1], mode.ack);
		write_16(&pack->data[3], original_ack);
		write_16(&pack->data[5], len);
		SDL_memcpy(&pack->data[7], data, (size_t)cut);
		if (queue_packet(rudp, &rudp->peers[id], pack, mode))
			return (-1);
		size -= cut;
		data += cut;
	}
	return (0);
}
