#ifndef MOS6502_HPP__
#define MOS6502_HPP__

#include <stdint.h>

#define MOS6502_DEFINE_IMPLIED( instruction_name ) \
  inline void #instruction_name ();

#define MOS6502_DEFINE_ADDRESS( instruction_name ) \
  inline void #instruction_name (uint16_t address);

#define MOS6502_ADDRESS_SPACE (1 << 16) // 2^16 65536

namespace MOS6502 {
/*
======================================================================
  Proccess status register, done via union as opposed to bitmasks
======================================================================
*/
typedef union {
  struct {
    unsigned C : 1; // Carry
    unsigned Z : 1; // Zero
    unsigned I : 1; // Interrupt
    unsigned D : 1; // Decimal
    unsigned B : 1; // Break
    unsigned P : 1; // Not Used
    unsigned V : 1; // Overflow
    unsigned N : 1; // Negative
  };
  uint8_t m_data;
}
PSR;

/*
=======================================================================
  6502 System Registers
=======================================================================
*/
typedef struct {
  PSR      PS;  // Proccess status
  uint16_t PC;  // Program Counter
  uint8_t  A;   // Accumulator
  uint8_t  X;   // X indexing register
  uint8_t  Y;   // Y indexing register
  uint8_t  SP;  // Stack Pointer
}
REG;

/*
=======================================================================
  6502 Interrupt Flags
=======================================================================
*/
typedef struct 
{
  bool IRQ_TRIGGER;
  bool NMI_TRIGGER;
  bool NMI_TRIGGER_EDGE;
  bool RST_TRIGGER; 
}
INT;

/*
=======================================================================
  6502 Bus interface
=======================================================================
  * Purposely undefined to enforce this the function implementation
*/
class ICPUBus {
public:
  virtual uint8_t Read (uint16_t addr) = 0;
  virtual void    Write(uint16_t addr, uint8_t v);
  virtual void    Tick ();
};

/*
=======================================================================
  6502 CPU State Class
=======================================================================
*/
class CPU : public ICPUBus 
{
  REG m_register;
  INT m_interrupt;
public:
  CPU();
  virtual ~CPU();
  /* Execute single instruction */
  inline void Step(void);
  /* External interrupt triggers */
  inline void TriggerIRQ();
  inline void TriggerNMI();
  inline void TriggerRST();
private:
  /* Interrupt handlers */
  inline void RST();
  inline void NMI();
  inline void IRQ();
  /* Addressing Modes */
  inline uint16_t IMMEDIATE();
  inline uint16_t ZEROPAGE();
  inline uint16_t ZEROPAGE_X();
  inline uint16_t ZEROPAGE_Y();
  inline uint16_t ABSOLUTE();
  inline uint16_t ABSOLUTE_X();
  inline uint16_t ABSOLUTE_Y();
  inline uint16_t RELATIVE();
  inline uint16_t INDIRECT();
  inline uint16_t INDIRECT_X();
  inline uint16_t INDIRECT_Y();  
  /* Official Instruction Set */ 
  inline void ADC(uint16_t address);
  inline void AND(uint16_t address);
  inline void ASL(uint16_t address);
  inline void ASL_A();
  inline void BCC();
  inline void BCS();
  inline void BEQ();
  inline void BIT(uint16_t address);
  inline void BMI();
  inline void BNE();
  inline void BPL();
  inline void BRK();
  inline void BVC();
  inline void BVS();
  inline void CLC();
  inline void CLD();
  inline void CLI();
  inline void CLV();
  inline void CMP(uint16_t address);
  inline void CPX(uint16_t address);
  inline void CPY(uint16_t address);
  inline void DEC(uint16_t address);
  inline void DEX();
  inline void DEY();
  inline void EOR();
  inline void INC(uint16_t address);
  inline void INX();
  inline void INY();
  inline void JMP(uint16_t address);
  inline void JSR(uint16_t address);
  inline void LDA(uint16_t address);
  inline void LDX(uint16_t address);
  inline void LDY(uint16_t address);
  inline void LSR(uint16_t address);
  inline void LSR_A();
  inline void NOP();
  inline void ORA(uint16_t address);
  inline void PHA();
  inline void PHP();
  inline void PLA();
  inline void PLP();
  inline void ROL(uint16_t address);
  inline void ROL_A();
  inline void ROR(uint16_t address);
  inline void ROR_A();
  inline void RTI();
  inline void RTS();
  inline void SBC(uint16_t address);
  inline void SEC();
  inline void SED();
  inline void SEI();
  inline void STA(uint16_t address);
  inline void STX(uint16_t address);
  inline void STY(uint16_t address);
  inline void TAX();
  inline void TAY();
  inline void TSX();
  inline void TXA();
  inline void TXS();
  inline void TYA();
};

} /* namespace MOS6502 */  
#endif