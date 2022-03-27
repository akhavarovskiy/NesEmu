#include "../include/bus.hpp"
#include "../include/rom.hpp"
#include "../include/interrupts.hpp"
#include "../include/mappers/mapper0.hpp"

namespace nes { 

Ricoh2A0XBus::Ricoh2A0XBus(std::vector<uint8_t> & romData) : 
  m_stack     (MOS6502::k6502StackSpace),
  m_rom       (std::make_unique<Rom> (romData)),
  m_interrupts(std::make_shared<Ricoh2A0XInterrupts>())
{
  const uint64_t mapperId = ((this->m_rom->Header().m_upperMapperNybble) << 4) & 
                            ((this->m_rom->Header().m_lowerMapperNybble));
  switch(mapperId) {
    case 0:
      this->m_mapper = std::make_unique<mappers::Mapper0>(this->m_rom);
      break;
    default:
      throw std::exception();
  }
}

Ricoh2A0XBus::~Ricoh2A0XBus() 
{

}

std::unique_ptr<class IMapper> & Ricoh2A0XBus::Mapper(void)
{
  return this->m_mapper;
}

std::shared_ptr<class MOS6502::ICPUInterrupts> & Ricoh2A0XBus::Interrupts(void)
{
  return this->m_interrupts;
}

uint8_t Ricoh2A0XBus::Read(uint16_t address) 
{
  uint8_t r = 0;
  switch (address)
  {
  /* Stack */
  case 0x0000 ... 0x1FFF:
    r = this->m_stack[ address & 0x7FF ];
    break;
  
  /* ROM */
  case 0x4018 ... 0xFFFF:
    r = this->m_mapper->Read(address);
    break;
  }
  return r;
}

void Ricoh2A0XBus::Write(uint16_t address, uint8_t v)
{
  switch (address)
  {
  /* Stack */
  case 0x0000 ... 0x1FFF:
    this->m_stack[address & 0x7FF] = v;
    break;
  
  /* ROM */
  case 0x4018 ... 0xFFFF:
    this->m_mapper->Write(address, v);
    break;
  }
}

void Ricoh2A0XBus::Tick() 
{
  this->m_ticks++;
}

} /* namespace nes */