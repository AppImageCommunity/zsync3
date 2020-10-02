#include <iostream>
#include <string>
#include <Zsync3>


int main(int argc, char **argv) {
    Zsync3::Client zs_client;
    if(argc == 1) {
        std::cout << "Usage: " << argv[0] << " [PATH TO ZSYNC META FILE]"
                  << std::endl;
        return 0;
    }

    std::string path(argv[1]);
    if(!zs_client.SetMetaFile(path)) {
        std::cout << "ERROR: cannot parse zsync meta file" << std::endl;
    }
    return 0;
}
