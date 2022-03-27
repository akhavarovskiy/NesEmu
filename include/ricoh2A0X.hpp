#ifndef RICOH_2A0X_H__
#define RICOH_2A0X_H__

#include <6502.hpp>
#include <iostream>
#include "bus.hpp"
#include "rom.hpp"
#include "IMapper.hpp"

#define RICOH_2A0X_RESOLUTION_X (256)
#define RICOH_2A0X_RESOLUTION_Y (240)

namespace nes {

class Ricoh2A0X : public nes::Ricoh2A0XBus, public MOS6502::CPU {
public:
  Ricoh2A0X(std::vector<uint8_t> & rom);
  virtual ~Ricoh2A0X();
private:
  uint8_t  Read (uint16_t address);
  void     Write(uint16_t address, uint8_t v);
  void     Tick ();
};

} /* namespace nes */

#endif