#include <rudp.h>

//class

int receiver_thread(void *common)
{
	while (1)
	{
		//receive packet from socket
		//extract seg from packet
		//valid checksum
		//if contain ACK, potentially remove seg from sender buffer
		//if seg contains data, put data in buffer and send ACK
	}
}
