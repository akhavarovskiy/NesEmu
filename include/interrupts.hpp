#ifndef RICOH_2A0X_PPU_H__
#define RICOH_2A0X_PPU_H__

#include <6502.hpp>

namespace nes {

class Ricoh2A0XInterrupts : public MOS6502::ICPUInterrupts {
  /*Flags for interrupt handling*/
  MOS6502::INTERRUPT_STATE m_RST;
  MOS6502::INTERRUPT_STATE m_NMI;
  MOS6502::INTERRUPT_STATE m_IRQ;
  /* Flags for storing the stage of the PIN */
  MOS6502::PIN_STATE m_psRST;
  MOS6502::PIN_STATE m_psNMI;
  MOS6502::PIN_STATE m_psIRQ;
public:
  Ricoh2A0XInterrupts(void);
  virtual ~Ricoh2A0XInterrupts(void);
  
  /* Get the interupt state */
  virtual MOS6502::INTERRUPT_STATE RST(); // External Reset Pin
  virtual MOS6502::INTERRUPT_STATE NMI(); // Used by PPU in NES (edge triggered)
  virtual MOS6502::INTERRUPT_STATE IRQ(); // Used by APU in NES
  /* Will clear the interrupt flags and set the interrupt as handled */
  virtual void clearRST();
  virtual void clearNMI();
  virtual void clearIRQ();
  /* Set the external PIN for interrupt, IRQ is triggered on low, NMI is edge triggered. */
  virtual void setPinRST(MOS6502::PIN_STATE ps);
  virtual void setPinNMI(MOS6502::PIN_STATE ps);
  virtual void setPinIRQ(MOS6502::PIN_STATE ps);
};

} /* namespace nes */

#endif