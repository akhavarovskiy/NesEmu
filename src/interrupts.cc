#include "../include/interrupts.hpp"

namespace nes {

Ricoh2A0XInterrupts::Ricoh2A0XInterrupts(void) : 
  m_RST(MOS6502::INTERRUPT_STATE::DISABLED),
  m_IRQ(MOS6502::INTERRUPT_STATE::DISABLED),
  m_NMI(MOS6502::INTERRUPT_STATE::DISABLED),
  m_psRST(MOS6502::PIN_STATE::HIGH),
  m_psNMI(MOS6502::PIN_STATE::HIGH),
  m_psIRQ(MOS6502::PIN_STATE::HIGH)
{

}

Ricoh2A0XInterrupts::~Ricoh2A0XInterrupts(void)
{

}

/* Get the interupt state */
MOS6502::INTERRUPT_STATE Ricoh2A0XInterrupts::RST()
{
  return this->m_RST;
}

MOS6502::INTERRUPT_STATE Ricoh2A0XInterrupts::NMI()
{
  return this->m_NMI;
} 

MOS6502::INTERRUPT_STATE Ricoh2A0XInterrupts::IRQ()
{
  return this->m_IRQ;
}

/* Will clear the interrupt flags and set the interrupt as handled */
void Ricoh2A0XInterrupts::clearRST()
{
  this->m_RST = MOS6502::INTERRUPT_STATE::DISABLED;
}

void Ricoh2A0XInterrupts::clearNMI()
{
  this->m_NMI = MOS6502::INTERRUPT_STATE::DISABLED;
}

void Ricoh2A0XInterrupts::clearIRQ()
{
  this->m_IRQ = MOS6502::INTERRUPT_STATE::DISABLED;
}

void Ricoh2A0XInterrupts::setPinRST(MOS6502::PIN_STATE ps)
{
  this->m_psRST = ps;
  /* Active low, once the pin is low we trigger the reset.
    Technically its required that the pin is held low for 2 cycles.*/
  if(this->m_psRST == MOS6502::PIN_STATE::LOW) {
    this->m_RST = MOS6502::INTERRUPT_STATE::ENABLED;
  }
}

void Ricoh2A0XInterrupts::setPinNMI(MOS6502::PIN_STATE ps)
{
  /* Check for an edge change to trigger NMI, then store the new state */
  if(this->m_psNMI != ps) {
    this->m_NMI = MOS6502::INTERRUPT_STATE::ENABLED;
  }
  this->m_psNMI = ps;
}

void Ricoh2A0XInterrupts::setPinIRQ(MOS6502::PIN_STATE ps)
{

  this->m_psIRQ = ps;
  /* Active low, once the pin is low we trigger interrupt */
  if(this->m_psIRQ == MOS6502::PIN_STATE::LOW) {
    this->m_IRQ = MOS6502::INTERRUPT_STATE::ENABLED;
  }
}

} /* namespace nes */