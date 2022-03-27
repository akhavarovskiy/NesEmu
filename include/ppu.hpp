#ifndef RICOH_2A0X_PPU_H__
#define RICOH_2A0X_PPU_H__

#include <cstdint>
#include <6502.hpp>

namespace nes {

class Ricoh2A0XPPU : public MOS6502::ICPUDataBus {
public:
  virtual uint8_t Read (uint16_t addr)            override;
  virtual void    Write(uint16_t addr, uint8_t v) override;
  virtual void    Tick ()                         override;
}

} // namespace nes

#endif