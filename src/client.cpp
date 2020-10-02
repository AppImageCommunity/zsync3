#include "../include/client.hpp"
#include "../include/client_p.hpp"

using namespace Zsync3;

Client::Client() {
    d.reset(new ClientPrivate);
}

Client::Client(const Client &other) {
    this->d.reset(other.d.get());
}

Client::Client(const std::string &meta_file_path)
    : Client() {
    SetMetaFile(meta_file_path);
}

Client::~Client() { }

bool Client::SetMetaFile(const std::string &path) {
    return d->SetMetaFile(path);
}

bool Client::SubmitSeedFile(const std::string &path) {
    return d->SubmitSeedFile(path);
}
