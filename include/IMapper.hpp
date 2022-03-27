#ifndef IMAPPER_H__
#define IMAPPER_H__

#include <6502.hpp>
#include <cstdint>

namespace nes {

enum PROGRAM_ROM_BANK 
{
  _8000_BFFF = 0,
  _C000_FFFF = 1
};

class IMapper : public MOS6502::ICPUDataBus {
public:
  virtual ~IMapper() = default;
  virtual uint8_t MapperId() = 0;
  virtual const uint8_t* programROM(PROGRAM_ROM_BANK bank) const = 0;
  virtual const uint8_t* characterROM()                    const = 0;
};

} /* namespace nes */

#endif /* IMAPPER_H__ */