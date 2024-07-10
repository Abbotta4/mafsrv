#include <iostream>
#include <pgdb.h>
#include <users.h>
#include <restserver.h>

int main(int argc, char* argv[]) {
    Pistache::Port port(9080);
    int thr = 2;
    if (argc >= 2)
    {
        port = static_cast<uint16_t>(std::stol(argv[1]));

        if (argc == 3)
            thr = std::stoi(argv[2]);
    }

    Pistache::Address addr(Pistache::Ipv4::any(), port);

    std::cout << "Cores = " << Pistache::hardware_concurrency() << std::endl;
    std::cout << "Using " << thr << " threads" << std::endl;

    RESTEndpoint rest(addr);

    rest.init(thr);
    rest.start();

    return 0;
}
