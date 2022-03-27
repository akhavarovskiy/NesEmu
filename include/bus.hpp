#ifndef RICOH_2A0X_BUS_H__
#define RICOH_2A0X_BUS_H__

#include <6502.hpp>
#include <cstdint>
#include <array>
#include <memory>
#include "rom.hpp"
#include "IMapper.hpp"

namespace nes {

class Ricoh2A0XBus : public MOS6502::ICPUDataBus {
  std::shared_ptr<class MOS6502::ICPUInterrupts> m_interrupts;
  std::unique_ptr<class IMapper>                 m_mapper;
  std::unique_ptr<class Rom>                     m_rom;
  std::vector<uint8_t>                           m_stack;
  uint64_t                                       m_ticks;
public:
  Ricoh2A0XBus(std::vector<uint8_t> & romData);
  virtual ~Ricoh2A0XBus();

  std::shared_ptr<class MOS6502::ICPUInterrupts> & Interrupts(void);
  std::unique_ptr<class IMapper>                 & Mapper(void);
  virtual uint8_t                                  Read(uint16_t addr);
  virtual void                                     Write(uint16_t addr, uint8_t v);
  virtual void                                     Tick();
};

} /* namespace nes */

#endif /* RICOH_2A0X_BUS_H__ */