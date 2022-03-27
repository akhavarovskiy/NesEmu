#include "../include/main.hpp"
#include "../include/ricoh2A0X.hpp"
#include "../include/mappers/mapper0.hpp"


namespace nes {

Ricoh2A0X::Ricoh2A0X(std::vector<uint8_t> & romData) : Ricoh2A0XBus::Ricoh2A0XBus(romData), MOS6502::CPU::CPU(this->Interrupts()) 
{
  this->Reset();
  DEBUG(std::cout << "[ Debug ] Resetting CPU, Boot Address: 0x" << std::hex << this->Registers().PC << std::endl);
}

Ricoh2A0X::~Ricoh2A0X() 
{

}

uint8_t Ricoh2A0X::Read(uint16_t address) 
{
  return Ricoh2A0XBus::Read(address);
}

void Ricoh2A0X::Write(uint16_t address, uint8_t v) 
{
  Ricoh2A0XBus::Write(address, v);
}

void Ricoh2A0X::Tick() 
{
  Ricoh2A0XBus::Tick();
}


} /* namespace nes  */