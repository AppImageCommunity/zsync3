#ifndef ZSYNC3_CLIENT_PRIVATE_HPP_INCLUDED
#define ZSYNC3_CLIENT_PRIVATE_HPP_INCLUDED
#include <cstdint>
#include <cstddef>
#include <boost/filesystem.hpp>
#include <openssl/sha.h>
#include <openssl/md4.h>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

namespace Zsync3 {
class ClientPrivate {
    /// Constants
    const int kBitHashBits = 3;
    /// ---

    class RollingChecksum {
        uint16_t a,b;
      public:
        RollingChecksum();
        //// Special Constructor which expects the given variables as
        //// A anb B which is in network endian order.
        RollingChecksum(uint16_t, uint16_t);
        RollingChecksum(const uint8_t*, std::size_t);
        RollingChecksum(const RollingChecksum&);
        uint16_t GetA();
        uint16_t GetB();
        void update(uint8_t,uint8_t,int32_t);

        void operator = (const RollingChecksum&);
        bool operator == (const RollingChecksum&);
        bool operator != (const RollingChecksum&);
    };

    struct HashEntry {
        int64_t block_id = 0;
        RollingChecksum rsum;
        std::string md4;

        //// Next hash entry with the same rsum
        HashEntry *next = nullptr;
    };

    int64_t num_blocks;
    int blocksize = 0,
        blockshift = 0; /// log2(blocksize)
    int64_t num_bytes_written = 0,
            context = 0,    /* precalculated blocksize * seq_matches */
            num_weak_checksum_bytes = 0,
            num_strong_checksum_bytes = 0, /* no. of bytes available for the strong checksum. */
            num_seq_matches = 0,
            target_file_length = 0;

    int64_t bit_hash_mask = 0;
    int64_t hash_mask = 0;
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
    std::unique_ptr<std::vector<HashEntry*>> rsum_mapping;

    /// This bit hash as 1 bit per rsum value. If a rsum value exists in the
    /// target file then we will have the relevant bit set to 1 if not
    /// 0. So this provides us a fast negative lookup.
    std::unique_ptr<std::vector<uint8_t>> bit_hashes;

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

    //// The mapping function for the rsum hashes.
    //// This function returns the index in where the HashEntry
    //// resides in the rsum_hashes.
    uint32_t RsumHash(HashEntry&);

    //// Builds the mapping of rsum to hash entries
    bool BuildRsumHashTable();

    //// Submit a fragment of data to be zsynced.
    int64_t SubmitSourceData(const char *, size_t, int64_t);
};
}

#endif
