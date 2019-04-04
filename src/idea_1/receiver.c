#include <rudp.h>

//class

int receiver(void *common)
{
	t_rudp lrudp = rudp(target_ip, target_port, local_port);
	while (run)
	{
		lrudp.receive();
	}
	lrudp.close();
}
