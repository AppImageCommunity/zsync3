#ifndef ZSYNC3_CLIENT_HPP_INCLUDED
#define ZSYNC3_CLIENT_HPP_INCLUDED
#include <memory>
#include <string>

namespace Zsync3 {
class ClientPrivate;
class Client {
    std::unique_ptr<ClientPrivate> d;

  public:
    Client();
    Client(const std::string&);
    Client(const Client&);
    ~Client();

    bool SetMetaFile(const std::string&);

    bool SubmitSeedFile(const std::string&);
};
}

#endif // ZSYNC3_CLIENT_HPP_INCLUDED
