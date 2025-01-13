#include "LaunchTarget.hpp"

#if CN_CLIENT
    #include "client/Client.hpp"
#elif CN_SERVER
    #include "server/Server.hpp"
#endif

int main()
{
#if CN_CLIENT
    cn::client::Client client;
    client.start();
#elif CN_SERVER
    cn::server::Server server;
    server.start();
#endif

    return 0;
}