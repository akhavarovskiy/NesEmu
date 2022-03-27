#ifndef MAPPER_0_HPP__
#define MAPPER_0_HPP__

#include <6502.hpp>
#include <cstdint>
#include "../rom.hpp"
#include "../IMapper.hpp"

namespace nes { 

namespace mappers {

class Mapper0 : public IMapper {
  // Copy of the rom object for bank switching
  class Rom m_rom;
  // Pointer for caching the address of the bank memory
  std::vector<uint8_t*> m_selectedPROMBanks;
  uint8_t*              m_selectedCROMBank;
public:
  Mapper0(std::unique_ptr<Rom> & rom);
  virtual ~Mapper0(void);
  virtual uint8_t MapperId();
  virtual inline const uint8_t* programROM(PROGRAM_ROM_BANK bank) const;
  virtual inline const uint8_t* characterROM()                    const;

  virtual uint8_t Read(uint16_t address);
  virtual void    Write(uint16_t address, uint8_t v);
  virtual void    Tick();
};

} /* namespace mappers */

} /* namespace nes  */

#endif