#ifdef WIN32
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include <iostream>
#include <boost/program_options.hpp>

#include "server_session.h"

#include "logger.h"

using namespace std;

int main(int argc, char* argv[])
{
#ifdef WIN32
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF|_CRTDBG_LEAK_CHECK_DF);
    // DO NOT uncomment and check in
    //_crtBreakAlloc = 392;
#endif

    LogLevel level = LL_ERROR;

    boost::program_options::options_description desc;
    desc.add_options()
        ("help", "command line help")
        ("debug", "running server in debug mode")
#ifndef WIN32
        ("daemon", "daemonize process")
#endif
    ;

    boost::program_options::variables_map varmap;
    boost::program_options::store(boost::program_options::parse_command_line(
        argc, argv, desc), varmap);
    boost::program_options::notify(varmap);

#ifndef WIN32
    if (varmap.count("daemon"))
    {
        std::cout << "Daemonizing..." << std::endl;
        int i = fork();
        if (i < 0) exit(1);
        if (i > 0) exit(0);
    }
#endif

    if (varmap.count("help"))
    {
        std::cout << desc << "\n";
        return 1;
    }
    
    if (varmap.count("debug"))
    {
        level = LL_INFO;
    }

    ServerSession session(level);

    if (session.start())
    {
        session.run();
        session.stop();
    } else
    {
        std::cout << "Cannot start ant server." << std::endl;
    }

    return 0;
}
