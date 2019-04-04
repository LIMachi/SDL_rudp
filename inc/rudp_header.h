#ifndef RUDP_HEADER_H
# define RUDP_HEADER_H

# include <SDL2/SDL_net.h>

/*
** Mutually exclusive: SYN, EAK, RST, TCS.
** NUL implies ACK.
** LRUDP_HF_ZERO bit MUST be 0.
** EACK, NULL and RST implie that length must be of the size of the header.
** Special case: the proposed standard tell that ACK must be set for user data.
** I think the user can chose in special cases to not secure the arrival of a
** packet.
*/

enum			e_rudp_header_flags
{
	LRUDP_HF_SYN = 0b10000000,
	LRUDP_HF_ACK = 0b01000000,
	LRUDP_HF_EAK = 0b00100000,
	LRUDP_HF_RST = 0b00010000,
	LRUDP_HF_NUL = 0b00001000,
	LRUDP_HF_CHK = 0b00000100,
	LRUDP_HF_TCS = 0b00000010,
	LRUDP_HF_ZERO = 0b00000001,
	LRUDP_HF_EXCLUSIVE = 0b10110010,
	LRUDP_HF_NULL_LEN = 0b00111000
};

# pragma pack(push, 1)

typedef Uint16	t_checksum;

/*
** a checksum is always appended to the body
** so a packet will at least be 6 bytes of data (4 header, 2 checksum)
*/

struct			s_rudp_header
{
	Uint8	flags;
	Uint8	length;
	Uint8	sequence_number;
	Uint8	acknowledgment_number;
	Uint8	body[0];
};

/*
** flags: 0b1x000000
** length: 28
** usage: establish a connection and synchronize the ack numbers
** if LRUDP_HF_ACK is set, head.acknowledgment_number contains the ack of the
** opposite rudp
*/

struct			s_rudp_syn_segment
{
	struct s_rudp_header	head;
	Uint8					vers : 4;
	Uint8					spare_0 : 4;
	Uint8					max_number_of_outstanding_segs;
	Uint8					options_flags;
	Uint8					spare_1;
	Uint16					maximum_segment_size;
	Uint16					retransmission_timeout_val;
	Uint16					cumulative_ack_timeout_val;
	Uint16					null_segment_timeout_val;
	Uint16					transfer_state_timeout_val;
	Uint8					max_retransmissions;
	Uint8					max_cumulative_ack;
	Uint8					max_out_of_sequence;
	Uint8					max_auto_reset;
	Uint32					connection_identifier;
	t_checksum				checksum;
};

/*
** flags: 0b01000000
** length: 6
*/

struct			s_rudp_acknowledgment_segment
{
	struct s_rudp_header	head;
	t_checksum				checksum;
};

/*
** flags: 0b01100000
** length: N + 6
** note: t_checksum checksum; is present at the end of the segment
*/

struct			s_rudp_extra_acknowledgment_segment
{
	struct s_rudp_header	head;
	Uint8					out_of_sequence_acknowledgment_number[0];
};

/*
** flags: 0b0x010000
** length: 6
** close/reset a connection: flush all non acknowledgement packet from the out
** buffer
*/

struct			s_rudp_reset_segment
{
	struct s_rudp_header	head;
	t_checksum				checksum;
};

/*
** flags: 0b01001000
** length: 6
** keep-alive, noop
*/

struct			s_rudp_null_segment
{
	struct s_rudp_header	head;
	t_checksum				checksum;
};

/*
** flags: 0b0x000010
** length: 12
*/

struct			s_transfer_state_of_connection_segment
{
	struct s_rudp_header	head;
	Uint8					sequence_adjacent_factor;
	Uint8					spare;
	Uint32					connection_identifier;
	t_checksum				checksum;
};



# pragma pack(pop)

int				valid_flag(Uint8 flag)
{
	if ((flag & LRUDP_HF_ZERO)
		|| (flag & LRUDP_HF_NUL && !(flag & LRUDP_HF_ACK))
		|| (flag & LRUDP_HF_SYN && (flag & (LRUDP_HF_EXCLUSIVE ^ LRUDP_HF_SYN)))
		|| (flag & LRUDP_HF_EAK && (flag & (LRUDP_HF_EXCLUSIVE ^ LRUDP_HF_EAK)))
		|| (flag & LRUDP_HF_RST && (flag & (LRUDP_HF_EXCLUSIVE ^ LRUDP_HF_RST)))
		|| (flag & LRUDP_HF_TCS
			&& (flag & (LRUDP_HF_EXCLUSIVE ^ LRUDP_HF_TCS))))
		return (0);
	return (1);
}

int				valid_length(struct s_rudp_header *header)
{
	if (header->length < sizeof(header) + sizeof(t_checksum)
		|| (header->length > sizeof(header) + sizeof(t_checksum)
			&& header->flags & LRUDP_HF_NULL_LEN))
		return (0);
	return (1);
}

/*
** multiplication and addition of all bytes in the packet
*/

Uint16			checksum_1(struct s_rudp_header *header)
{
	Uint32	len;
	Uint32	i;
	Uint8	mul;
	Uint8	sum;
	Uint8	*ptr;

	if (header->flags & LRUDP_HF_CHK)
		len = header->length - sizeof(t_checksum);
	else
		len = sizeof(*header);
	i = (Uint32)-1;
	sum = 0;
	mul = 1;
	ptr = (Uint8*)header;
	while (++i < len)
	{
		sum += ptr[i];
		if (ptr[i])
			mul *= ptr[i];
	}
	return ((((Uint16)mul) << 8) | (Uint16)sum);
}

/*
** Uint16 addition of all bytes in the packet
*/

Uint16			checksum_2(struct s_rudp_header *header)
{
	Uint32	len;
	Uint32	i;
	Uint16	sum;
	Uint8	*ptr;

	if (header->flags & LRUDP_HF_CHK)
		len = header->length - sizeof(t_checksum);
	else
		len = sizeof(*header);
	i = (Uint32)-1;
	sum = 0;
	ptr = (Uint8*)header;
	while (++i < len)
		sum += (Uint16)ptr[i];
	return (sum);
}

int				valid_checksum(struct s_rudp_header *header)
{
	return (header->body[header->length - sizeof(t_checksum)]
		== checksum_1(header));
}

int				valid_packet(struct s_rudp_header *header)
{
	return (valid_flag(header->flags) && valid_length(header)
		&& valid_checksum(header) && valid_type(header));
}

#endif
