#ifndef ZSYNC3_CLIENT_PRIVATE_HPP_INCLUDED
#define ZSYNC3_CLIENT_PRIVATE_HPP_INCLUDED
#include <cstdint>
#include <cstddef>
#include <algorithm>
#include <boost/filesystem.hpp>
#include <openssl/sha.h>
#include <openssl/md4.h>
#include <fstream>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Zsync3 {
class ClientPrivate {
    struct RollingChecksum {
        uint16_t a,b,nb;
	RollingChecksum();
        //// Special Constructor which expects the given variables as
        //// A anb B which is in network endian order.
        RollingChecksum(uint16_t, uint16_t);
        RollingChecksum(const uint8_t*, std::size_t);
        RollingChecksum(const RollingChecksum&);
        void update(uint8_t,uint8_t,int32_t);

        void operator = (const RollingChecksum&);
    };

    struct HashEntry {
        int64_t block_id = 0;
        RollingChecksum rsum;
	std::vector<char> md4;
    };

    struct RollingChecksumHasher {
	    uint16_t wmask = 0;
	    uint16_t hmask = 0;
	    int n_seq = 0;

	    RollingChecksumHasher();
	    RollingChecksumHasher(uint16_t, uint16_t, int);
	    std::size_t operator () (const RollingChecksum &key) const;
    };

    struct RollingChecksumEqual {
	    bool operator () (const RollingChecksum &lhs, const RollingChecksum &rhs) const;
    };

    int64_t num_blocks;
    int blocksize = 0,
        blockshift = 0; /// log2(blocksize)
    int num_seq_matches = 0;
    int num_weak_checksum_bytes = 0,
	num_strong_checksum_bytes = 0;

    int64_t num_bytes_written = 0,
            context = 0,    /* precalculated blocksize * seq_matches */
            target_file_length = 0;

    uint16_t hash_mask = 0;
    uint16_t weak_checksum_mask = 0; /* This will be applied to the first 16 bits of the weak checksum. */

    //// Version string.
    std::string zs_version;

    //// Date Time string.
    std::string mtime;

    //// Has all block's hashes.
    std::unique_ptr<std::vector<HashEntry>> block_hashes;

    //// Has the mapping of rsum to specific HashEntry present in the
    //// block_hashes.
    //// The HashEntry is a chain which is a collection of same rsum
    //// mapping.
    std::unique_ptr<std::unordered_multimap<RollingChecksum, HashEntry*, RollingChecksumHasher, RollingChecksumEqual>> rsum_map;

    std::unique_ptr<MD4_CTX> md4_ctx;
    std::unique_ptr<SHA_CTX> sha1_ctx;
    std::unique_ptr<std::fstream> temp_fs;

    std::string target_file_sha1;
    std::string target_file_url;
    std::string target_filename;
    boost::filesystem::path temp_filename;

  public:
    ClientPrivate();
    ~ClientPrivate();

    //// Read a Zsync Meta file and create all neccessary
    //// data structures to be constructed.
    bool SetMetaFile(const std::string&);

    //// Read a file and write useable blocks
    //// to temporary target file.
    bool SubmitSeedFile(const std::string&);
  private:
    //// Builds the mapping of rsum to hash entries
    bool BuildRsumHashTable();

    //// Submit a fragment of data to be zsynced.
    int64_t SubmitSourceData(const char *, size_t, int64_t);
};
}

#endif
