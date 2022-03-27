#ifndef MOS6502_HPP__
#define MOS6502_HPP__

#include <cstdint>
#include <memory>
#include <gtest/gtest.h>
#include <gtest/gtest_prod.h>

#define MOS6502_DEFINE_INSTRUCTION_IMPLIED( instruction_name ) \
  inline void #instruction_name (void);

#define MOS6502_DEFINE_INSTRUCTION_ADDRESSED( instruction_name ) \
  inline void #instruction_name (uint16_t address);

#define MOS6502_DEFINE_INSTRUCTION_ACCUMULATOR( instruction_name ) \
  inline void instruction_name ## _A();

namespace MOS6502 {
/************************************************************************
 * Enums
 ************************************************************************/
enum INTERRUPT_STATE {
  DISABLED = 0,
  ENABLED  = 1
};

enum PIN_STATE {
  LOW  = 0,
  HIGH = 1
};
/************************************************************************
 * Constants
 ************************************************************************/
static const unsigned long k6502AddressSpace = (1 << 16);
static const unsigned long k6502StackSpace   = (1 << 12);

/************************************************************************
 * Proccess status register, done via union as opposed to bitmasks
 ************************************************************************/
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

/************************************************************************
 * 6502 System Registers
 ************************************************************************/
typedef struct {
  PSR      PS;  // Proccess status
  uint16_t PC;  // Program Counter
  uint8_t  A;   // Accumulator
  uint8_t  X;   // X indexing register
  uint8_t  Y;   // Y indexing register
  uint8_t  SP;  // Stack Pointer
}
REG;

/************************************************************************
 * 6502 Interrupt Flags
 ************************************************************************/
typedef struct
{
  bool IRQ_TRIGGER;
  bool NMI_TRIGGER;
  bool NMI_TRIGGER_EDGE;
  bool RST_TRIGGER;
}
INT;

/* Interface for handling external interrupts */
class ICPUInterrupts {
public:
  virtual ~ICPUInterrupts() = default;
  /* Get the interupt state */
  virtual INTERRUPT_STATE RST() = 0; // External Reset Pin
  virtual INTERRUPT_STATE NMI() = 0; // Used by PPU in NES (edge triggered)
  virtual INTERRUPT_STATE IRQ() = 0; // Used by APU in NES

  /* Will clear the interrupt flags and set the interrupt as handled */
  virtual void clearRST() = 0;
  virtual void clearNMI() = 0;
  virtual void clearIRQ() = 0;

  /* Set the external PIN for interrupt, IRQ is triggered on low, NMI is edge triggered. */
  virtual void setPinRST(PIN_STATE ps) = 0;
  virtual void setPinNMI(PIN_STATE ps) = 0;
  virtual void setPinIRQ(PIN_STATE ps) = 0;
};

/************************************************************************
 * 6502 Bus interface
 ************************************************************************
 * Purposely undefined to enforce this the function implementation */
class ICPUDataBus {
public:
  virtual uint8_t Read (uint16_t addr)            = 0;
  virtual void    Write(uint16_t addr, uint8_t v) = 0;
  virtual void    Tick ()                         = 0;
};

/************************************************************************
 * 6502 CPU State Class
 ************************************************************************/
class CPU : public ICPUDataBus
{
  std::shared_ptr<class ICPUInterrupts> m_interrupts;
  REG m_register;
public:
  CPU(std::shared_ptr<class ICPUInterrupts> & interrupts);
  virtual ~CPU();
  /* Get a copy of the current registers */
  REG Registers();
  /* Execute single instruction */
  void Step(void);
  /* Reset the CPU */
  void Reset(void);
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
  inline void BCC(uint16_t address);
  inline void BCS(uint16_t address);
  inline void BEQ(uint16_t address);
  inline void BIT(uint16_t address);
  inline void BMI(uint16_t address);
  inline void BNE(uint16_t address);
  inline void BPL(uint16_t address);
  inline void BRK();
  inline void BVC(uint16_t address);
  inline void BVS(uint16_t address);
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
  inline void EOR(uint16_t address);
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

#ifdef MOS6502_UNIT_TEST
  friend class TestCPU;
#endif
};

/* CPU Used for testing */
#ifdef MOS6502_UNIT_TEST
class TestCPU : public CPU {

  uint8_t  m_memory[k6502AddressSpace];
  uint64_t m_ticks;
public:
  TestCPU();
  virtual ~TestCPU();

  /* Get the CPU register by reference */
  REG & Register();

  /* Addressing mode wrapper functions */
  uint16_t IMMEDIATE();
  uint16_t ZEROPAGE();
  uint16_t ZEROPAGE_X();
  uint16_t ZEROPAGE_Y();
  uint16_t ABSOLUTE();
  uint16_t ABSOLUTE_X();
  uint16_t ABSOLUTE_Y();
  uint16_t RELATIVE();
  uint16_t INDIRECT();
  uint16_t INDIRECT_X();
  uint16_t INDIRECT_Y();
  
  /* Virtual overrides */
  uint8_t  Read (uint16_t address);
  void     Write(uint16_t address, uint8_t v);
  void     Tick ();

  /* Helper test functions */
  void     Reset();
  void     SetBootAddress(uint16_t address);
  void     GetBootAddress(uint16_t address);
  /* Non Invasive Read, regular read triggers a tick */
  uint8_t  GetMemory(uint16_t address);
  void     SetMemory(uint16_t address, uint8_t v); 
  /* Get the elapsed amount of ticks */
  uint64_t GetTicks();
  void     SetTicks(uint64_t ticks);
};
#endif
} /* namespace MOS6502 */

#endif /* MOS6502_HPP__ */