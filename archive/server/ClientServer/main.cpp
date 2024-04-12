#include "ClientServer.h"

int main(int argc, char *argv[])
{
	CClientServer server(argc, argv);

	server.Run();

	return 0;
}
