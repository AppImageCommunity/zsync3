#include <boost/endian/conversion.hpp>

#include "../include/client_p.hpp"

Zsync3::ClientPrivate::RollingChecksum::RollingChecksum()
    : a(0),
      b(0) { }

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

Zsync3::ClientPrivate::RollingChecksum::RollingChecksum(const Zsync3::ClientPrivate::RollingChecksum &other) {
    this->a = other.a;
    this->b = other.b;
}

uint16_t Zsync3::ClientPrivate::RollingChecksum::GetA() {
    return a;
}

uint16_t Zsync3::ClientPrivate::RollingChecksum::GetB() {
    return b;
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

bool Zsync3::ClientPrivate::RollingChecksum::operator == (const Zsync3::ClientPrivate::RollingChecksum &obj) {
    return (this->a == obj.a) && (this->b == obj.b);
}

bool Zsync3::ClientPrivate::RollingChecksum::operator != (const Zsync3::ClientPrivate::RollingChecksum &obj) {
    return (this->a != obj.a) || (this->b != obj.b);
}

void Zsync3::ClientPrivate::RollingChecksum::operator = (const Zsync3::ClientPrivate::RollingChecksum &obj) {
    this->a = obj.a;
    this->b = obj.b;
}
