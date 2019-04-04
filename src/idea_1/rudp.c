#include <rudp.h>

//method

int send(Uint8 *data, size_t size)
{
	//divide data, store them in seg
	//put seg in O buffer
	//rudp_supp.udp_send()
	//prepare timeout
}

int receive(Uint8 *data, size_t size)
{
}

int close(void)
{
	//send a close seg
	//close connection
}

//class

t_rudp	rudp()
{
	//set IO buffer sizes
	//start I thread

	//method:
	int send(Uint8 *data, size_t size);
	int receive(Uint8 *data, size_t size);
	int close(void);
}
