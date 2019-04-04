#include <rudp.h>

t_rudp_client	*rudp_connect(t_rudp *rudp, const char *address,
	Uint16 target_port, Uint16 local_port)
{
	t_rudp_client	*out;

	out = SDL_malloc(sizeof(t_rudp_client));
	if (out == NULL)
		return (NULL);
	//
	(void)rudp;
	(void)address;
	(void)target_port;
	(void)local_port;
}
