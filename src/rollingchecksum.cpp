#include <boost/endian/conversion.hpp>

#include "../include/client_p.hpp"

Zsync3::ClientPrivate::RollingChecksum::RollingChecksum()
    : a(0),
      b(0),
      nb(0) { }

//// Expects the given variables to be in big endian
Zsync3::ClientPrivate::RollingChecksum::RollingChecksum(uint16_t gA, uint16_t gB)
    : RollingChecksum() {

    /// Convert to host endian and store.
    /// We need to convert from network endian to host endian,
    /// Network endian is nothing but big endian byte order, So if we have little
    /// endian byte order, We need to convert the data but if we have a big endian
    /// byte order, We can simply avoid this conversion to save computation power.
    /////
    /// But most of the time we will need little endian since intel's microproccessors
    /// always follows the little endian byte order.

    if(boost::endian::order::native == boost::endian::order::little) {
        a = boost::endian::endian_reverse(gA);
        b = boost::endian::endian_reverse(gB);
    }
}

Zsync3::ClientPrivate::RollingChecksum::RollingChecksum(const uint8_t *data, std::size_t len)
    : RollingChecksum() {
    if(!data) {
        return;
    }
    while(len) {
        uint8_t c = *data++;
        a += c;
        b += len * c;
        len--;
    }
}

Zsync3::ClientPrivate::RollingChecksum::RollingChecksum(const RollingChecksum &other) {
    this->a = other.a;
    this->b = other.b;
    this->nb = other.nb;
}


//// Let a = Char1 + Char2 + .... of some data
//// So to update a
//// Let updated a = Char1 + Char2 + ... - Char1 + NewChar1
////               = NewChar1 + Char2 + ...
//// I think you get the idea, I can explain b it takes a lot
//// of space.
void Zsync3::ClientPrivate::RollingChecksum::update(uint8_t oldChar,
        uint8_t newChar,
        int32_t blockShift) {
    a += newChar - oldChar;
    b += a - (oldChar << blockShift);
}

void Zsync3::ClientPrivate::RollingChecksum::operator = (const RollingChecksum &obj) {
    this->a = obj.a;
    this->b = obj.b;
    this->nb = obj.nb;
}

//// Hasher for the RollingChecksum made for std::map variants.
Zsync3::ClientPrivate::RollingChecksumHasher::RollingChecksumHasher() { }

Zsync3::ClientPrivate::RollingChecksumHasher::RollingChecksumHasher(uint16_t wm, uint16_t hm, int matches)
	: wmask(wm),
	  hmask(hm),
	  n_seq(matches) { }

std::size_t Zsync3::ClientPrivate::RollingChecksumHasher::operator () (const RollingChecksum &key) const {
	size_t h = key.b;

	h ^= (n_seq > 1) ? key.nb : key.a & wmask;
	return std::hash<uint32_t>()(h & hmask);
}

bool Zsync3::ClientPrivate::RollingChecksumEqual::operator () (const RollingChecksum &lhs, const RollingChecksum &rhs) const {
	return lhs.a == rhs.a && lhs.b == rhs.b;
}
