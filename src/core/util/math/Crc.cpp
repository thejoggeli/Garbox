#include "Crc.h"

namespace Garbox {

uint8_t Crc::Crc8(const uint8_t* data, uint32_t len, uint8_t initial){
    uint8_t crc = initial;
    for(uint32_t i = 0; i < len; i++){
        crc ^= data[i];
        for(uint8_t bit = 0; bit < 8; bit++){
            const bool msbSet = ((crc & 0x80) != 0);
            if(msbSet){
                crc = static_cast<uint8_t>((crc << 1) ^ 0x31);
            }
            else {
                crc <<= 1;
            }
        }
    }
    return crc;
}

} // namespace