#include <rudp.h>

//class

int sender(void *common)
{
	t_rudp lrudp = rudp(target_ip, target_port, local_port);
	while (run)
	{
		lrudp.send();
	}
	lrudp.close();
}
