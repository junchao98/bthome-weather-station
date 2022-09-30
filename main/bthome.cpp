
#ifdef __cplusplus
extern "C" {
#endif

#include "bthome.h"

#include "endian.h"
#include "esp_log.h"

#include <cstring>

#define NAME "bthome"

namespace bthome
{

    namespace encode
    {
        int32_t _write_object_info(enum constants::OBJECT_FORMAT const objectType,
                                   enum constants::DATA_TYPE const dataType, uint32_t const dataLen, uint8_t dest[],
                                   uint32_t const destLen)
        {
            static constexpr uint16_t OBJECT_FORMAT_SHIFT = 5;
            int32_t bytesWritten                          = -1;

            // Plus 1 to account for the data type byte
            dest[0]      = (static_cast<uint8_t>(objectType) << OBJECT_FORMAT_SHIFT) | (dataLen + 1);
            bytesWritten = 1;

            dest[1] = static_cast<uint8_t>(dataType);
            bytesWritten++;

            return bytesWritten;
        }

        int32_t _write_data_bytes(uint16_t const data, uint8_t dest[], uint8_t destLen)
        {
            dest[0] = data & 0xff;

            dest[1] = (data >> 8) & 0xff;

            return 2;
        }

        int32_t temperature(float const temperature, uint8_t dest[], uint32_t const destLen)
        {
            int32_t bytesWritten   = -1;
            int16_t const temp_val = static_cast<int16_t>(100.0 * temperature);

            bytesWritten = _write_object_info(constants::OBJECT_FORMAT::SIGNED_INT, constants::DATA_TYPE::TEMPERATURE,
                                              2, dest, destLen);

            bytesWritten += _write_data_bytes(temp_val, &dest[bytesWritten], destLen - bytesWritten);

            return bytesWritten;
        }

        int32_t humidity(float const humidity, uint8_t dest[], uint32_t destLen)
        {
            int32_t bytesWritten = -1;

            uint16_t const scaled_humidity = static_cast<uint16_t>((100.0 * humidity));

            bytesWritten = _write_object_info(constants::OBJECT_FORMAT::UNSIGNED_INT, constants::DATA_TYPE::HUMIDITY, 2,
                                              dest, destLen);

            bytesWritten += _write_data_bytes(scaled_humidity, &dest[bytesWritten], destLen - bytesWritten);

            return bytesWritten;
        }

        int32_t battery(float const batteryPercent, uint8_t dest[], uint32_t destLen)
        {
            int32_t bytesWritten = -1;

            uint16_t const scaled_battery = static_cast<uint16_t>(batteryPercent);

            bytesWritten = _write_object_info(constants::OBJECT_FORMAT::UNSIGNED_INT, constants::DATA_TYPE::BATTERY, 2,
                                              dest, destLen);

            bytesWritten += _write_data_bytes(scaled_battery, &dest[bytesWritten], destLen - bytesWritten);

            return bytesWritten;
        }

        int32_t pressure(float const pressure, uint8_t dest[], uint32_t destLen)
        {
            int32_t bytesWritten = -1;

            uint32_t const scaled_pressure = static_cast<uint32_t>(100.0 * pressure);

            bytesWritten = _write_object_info(constants::OBJECT_FORMAT::UNSIGNED_INT, constants::DATA_TYPE::PRESSURE, 3,
                                              dest, destLen);

            dest[bytesWritten] = scaled_pressure & 0xff;
            bytesWritten++;

            dest[bytesWritten] = (scaled_pressure >> 8) & 0xff;
            bytesWritten++;

            dest[bytesWritten] = (scaled_pressure >> 16) & 0xff;
            bytesWritten++;

            return bytesWritten;
        }

        int32_t packet_id(uint8_t const packetId, uint8_t dest[], uint32_t destLen)
        {
            int32_t bytesWritten = -1;
            bytesWritten = _write_object_info(constants::OBJECT_FORMAT::UNSIGNED_INT, constants::DATA_TYPE::PACKET_ID,
                                              1, dest, destLen);

            bytesWritten += _write_data_bytes(packetId, &dest[bytesWritten], destLen - bytesWritten);

            return bytesWritten;
        }

    }; // namespace encode

}; // namespace bthome

#ifdef __cplusplus
}
#endif