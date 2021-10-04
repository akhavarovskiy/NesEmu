#include "../include/6502.hpp"
#include <cstring>

/* Vector Address's */
#define NMI_ADDRESS 0xFFFA
#define RST_ADDRESS 0xFFFC
#define IRQ_ADDRESS 0xFFFE

/* Macros defining the instuction and its address mode */
#define MOS6502_ADC_IMMEDIATE   0x69
#define MOS6502_AND_IMMEDIATE   0x29  
#define MOS6502_CMP_IMMEDIATE   0xC9 
#define MOS6502_CPY_IMMEDIATE   0xC0 
#define MOS6502_CPX_IMMEDIATE   0xE0 
#define MOS6502_EOR_IMMEDIATE   0x49 
#define MOS6502_LDA_IMMEDIATE   0xA9 
#define MOS6502_LDX_IMMEDIATE   0xA2 
#define MOS6502_LDY_IMMEDIATE   0xA0 
#define MOS6502_ORA_IMMEDIATE   0x09 
#define MOS6502_SBC_IMMEDIATE   0xE9 

#define MOS6502_ASL_ACCUMULATOR 0x0A
#define MOS6502_LSR_ACCUMULATOR 0x4A
#define MOS6502_ROL_ACCUMULATOR 0x2A
#define MOS6502_ROR_ACCUMULATOR 0x6A

#define MOS6502_ADC_ZEROPAGE    0x65
#define MOS6502_AND_ZEROPAGE    0x25
#define MOS6502_ASL_ZEROPAGE    0x06
#define MOS6502_BIT_ZEROPAGE    0x24
#define MOS6502_CMP_ZEROPAGE    0xC5
#define MOS6502_CPX_ZEROPAGE    0xE4
#define MOS6502_CPY_ZEROPAGE    0xC4
#define MOS6502_EOR_ZEROPAGE    0x45
#define MOS6502_LDA_ZEROPAGE    0xA5
#define MOS6502_LDX_ZEROPAGE    0xA6
#define MOS6502_LDY_ZEROPAGE    0xA4
#define MOS6502_ORA_ZEROPAGE    0x05
#define MOS6502_SBC_ZEROPAGE    0xE5
#define MOS6502_STA_ZEROPAGE    0x85
#define MOS6502_STX_ZEROPAGE    0x86
#define MOS6502_STY_ZEROPAGE    0x84
#define MOS6502_DEC_ZEROPAGE    0xC6
#define MOS6502_INC_ZEROPAGE    0xE6
#define MOS6502_LSR_ZEROPAGE    0x46
#define MOS6502_ROL_ZEROPAGE    0x26
#define MOS6502_ROR_ZEROPAGE    0x66

#define MOS6502_ADC_ZEROPAGEX   0x75
#define MOS6502_AND_ZEROPAGEX   0x35
#define MOS6502_CMP_ZEROPAGEX   0xD5
#define MOS6502_EOR_ZEROPAGEX   0x55
#define MOS6502_LDA_ZEROPAGEX   0xB5
#define MOS6502_LDY_ZEROPAGEX   0xB4
#define MOS6502_ORA_ZEROPAGEX   0x15
#define MOS6502_SBC_ZEROPAGEX   0xF5
#define MOS6502_STA_ZEROPAGEX   0x95
#define MOS6502_STY_ZEROPAGEX   0x94
#define MOS6502_ASL_ZEROPAGEX   0x16
#define MOS6502_DEC_ZEROPAGEX   0xD6
#define MOS6502_INC_ZEROPAGEX   0xF6
#define MOS6502_LSR_ZEROPAGEX   0x56
#define MOS6502_ROL_ZEROPAGEX   0x36
#define MOS6502_ROR_ZEROPAGEX   0x76

#define MOS6502_LDX_ZEROPAGEY   0xB6 
#define MOS6502_STX_ZEROPAGEY   0x96 

#define MOS6502_ADC_ABSOLUTE    0x6D
#define MOS6502_AND_ABSOLUTE    0x2D
#define MOS6502_BIT_ABSOLUTE    0x2C
#define MOS6502_CMP_ABSOLUTE    0xCD
#define MOS6502_CPX_ABSOLUTE    0xEC
#define MOS6502_CPY_ABSOLUTE    0xCC
#define MOS6502_EOR_ABSOLUTE    0x4D
#define MOS6502_JMP_ABSOLUTE    0x4C
#define MOS6502_LDA_ABSOLUTE    0xAD
#define MOS6502_LDX_ABSOLUTE    0xAE
#define MOS6502_LDY_ABSOLUTE    0xAC
#define MOS6502_ORA_ABSOLUTE    0x0D
#define MOS6502_SBC_ABSOLUTE    0xED
#define MOS6502_STA_ABSOLUTE    0x8D
#define MOS6502_STX_ABSOLUTE    0x8E
#define MOS6502_STY_ABSOLUTE    0x8C    
#define MOS6502_ASL_ABSOLUTE    0x0E
#define MOS6502_DEC_ABSOLUTE    0xCE
#define MOS6502_INC_ABSOLUTE    0xEE
#define MOS6502_LSR_ABSOLUTE    0x4E
#define MOS6502_ROL_ABSOLUTE    0x2E
#define MOS6502_ROR_ABSOLUTE    0x6E
#define MOS6502_JSR_ABSOLUTE    0x20

#define MOS6502_ASC_ABSOLUTEX   0x7D
#define MOS6502_AND_ABSOLUTEX   0x3D
#define MOS6502_CMP_ABSOLUTEX   0xDD
#define MOS6502_EOR_ABSOLUTEX   0x5D
#define MOS6502_LDA_ABSOLUTEX   0xBD
#define MOS6502_LDY_ABSOLUTEX   0xBC
#define MOS6502_ORA_ABSOLUTEX   0x1D
#define MOS6502_SBC_ABSOLUTEX   0xFD 
#define MOS6502_STA_ABSOLUTEX   0x9D
#define MOS6502_ASL_ABSOLUTEX   0x1E
#define MOS6502_DEC_ABSOLUTEX   0xDE
#define MOS6502_INC_ABSOLUTEX   0xFE
#define MOS6502_LSR_ABSOLUTEX   0x5E
#define MOS6502_ROL_ABSOLUTEX   0x3E
#define MOS6502_ROR_ABSOLUTEX   0x7E

#define MOS6502_ADC_ABSOLUTEY   0x79
#define MOS6502_AND_ABSOLUTEY   0x39
#define MOS6502_CMP_ABSOLUTEY   0xD9
#define MOS6502_EOR_ABSOLUTEY   0x59
#define MOS6502_LDA_ABSOLUTEY   0xB9
#define MOS6502_LDX_ABSOLUTEY   0xBE
#define MOS6502_ORA_ABSOLUTEY   0x19
#define MOS6502_SBC_ABSOLUTEY   0xF9
#define MOS6502_STA_ABSOLUTEY   0x99

#define MOS6502_BCC_RELATIVE    0x90
#define MOS6502_BCS_RELATIVE    0xB0
#define MOS6502_BEQ_RELATIVE    0xF0
#define MOS6502_BMI_RELATIVE    0x30
#define MOS6502_BNE_RELATIVE    0xD0
#define MOS6502_BPL_RELATIVE    0x10
#define MOS6502_BVC_RELATIVE    0x50
#define MOS6502_BVS_RELATIVE    0x70
#define MOS6502_JMP_INDIRECT    0x6C

#define MOS6502_ADC_INDIRECTX   0x61
#define MOS6502_AND_INDIRECTX   0x21
#define MOS6502_CMP_INDIRECTX   0xC1
#define MOS6502_EOR_INDIRECTX   0x41
#define MOS6502_LDA_INDIRECTX   0xA1
#define MOS6502_ORA_INDIRECTX   0x01
#define MOS6502_SBC_INDIRECTX   0xE1
#define MOS6502_STA_INDIRECTX   0x81

#define MOS6502_AND_INDIRECTY   0x31
#define MOS6502_ADX_INDIRECTY   0x71
#define MOS6502_CMP_INDIRECTY   0xD1
#define MOS6502_EOR_INDIRECTY   0x51
#define MOS6502_LDA_INDIRECTY   0xB1
#define MOS6502_ORA_INDIRECTY   0x11
#define MOS6502_SBC_INDIRECTY   0xF1
#define MOS6502_STA_INDIRECTY   0x91

#define MOS6502_BRK_IMPLIED     0x00
#define MOS6502_CLC_IMPLIED     0x18 
#define MOS6502_CLD_IMPLIED     0xD8  
#define MOS6502_CLI_IMPLIED     0x58 
#define MOS6502_CLV_IMPLIED     0xB8 
#define MOS6502_DEX_IMPLIED     0xCA 
#define MOS6502_DEY_IMPLIED     0x88 
#define MOS6502_INX_IMPLIED     0xE8 
#define MOS6502_INY_IMPLIED     0xC8 
#define MOS6502_NOP_IMPLIED     0xEA 
#define MOS6502_SEC_IMPLIED     0x38 
#define MOS6502_SED_IMPLIED     0xF8 
#define MOS6502_SEI_IMPLIED     0x78 
#define MOS6502_TAX_IMPLIED     0xAA 
#define MOS6502_TAY_IMPLIED     0xA8 
#define MOS6502_TSX_IMPLIED     0xBA 
#define MOS6502_TXA_IMPLIED     0x8A 
#define MOS6502_TXS_IMPLIED     0x9A 
#define MOS6502_TYA_IMPLIED     0x98 
#define MOS6502_PHA_IMPLIED     0x48
#define MOS6502_PHP_IMPLIED     0x08     
#define MOS6502_PLA_IMPLIED     0x68 
#define MOS6502_PLP_IMPLIED     0x28 
#define MOS6502_RTI_IMPLIED     0x40
#define MOS6502_RTS_IMPLIED     0x60

namespace MOS6502 {


CPU::CPU() 
{
  this->RST();
}

inline void CPU::TriggerIRQ() {
  this->m_interrupt.IRQ_TRIGGER = !this->m_interrupt.IRQ_TRIGGER;
}

inline void CPU::TriggerNMI() {
  this->m_interrupt.NMI_TRIGGER = !this->m_interrupt.NMI_TRIGGER;
}

inline void CPU::TriggerRST() {
  this->m_interrupt.RST_TRIGGER = !this->m_interrupt.RST_TRIGGER;
}

inline void CPU::Step(void) {
  /* Grab the instruction and increment the Program Counter */
  uint8_t c = this->Read( this->m_register.PC++ );
  switch (c)
  {
  /* Accumulator */
  case MOS6502_ASL_ACCUMULATOR: this->ASL_A(); break;
  case MOS6502_LSR_ACCUMULATOR: this->LSR_A(); break;
  case MOS6502_ROL_ACCUMULATOR: this->ROL_A(); break;
  case MOS6502_ROR_ACCUMULATOR: this->ROR_A(); break;
  /* Implied instructions */
  case MOS6502_BRK_IMPLIED: this->BRK(); break;
  case MOS6502_CLC_IMPLIED: this->CLC(); break;
  case MOS6502_CLD_IMPLIED: this->CLD(); break;
  case MOS6502_CLI_IMPLIED: this->CLI(); break;
  case MOS6502_CLV_IMPLIED: this->CLV(); break;
  case MOS6502_DEX_IMPLIED: this->DEX(); break;
  case MOS6502_DEY_IMPLIED: this->DEY(); break;
  case MOS6502_INX_IMPLIED: this->INX(); break;
  case MOS6502_INY_IMPLIED: this->INY(); break;
  case MOS6502_NOP_IMPLIED: this->NOP(); break;
  case MOS6502_SEC_IMPLIED: this->SEC(); break;
  case MOS6502_SED_IMPLIED: this->SED(); break;
  case MOS6502_SEI_IMPLIED: this->SEI(); break;
  case MOS6502_TAX_IMPLIED: this->TAX(); break;
  case MOS6502_TAY_IMPLIED: this->TAY(); break;
  case MOS6502_TSX_IMPLIED: this->TSX(); break;
  case MOS6502_TXA_IMPLIED: this->TXA(); break;
  case MOS6502_TXS_IMPLIED: this->TXS(); break;
  case MOS6502_TYA_IMPLIED: this->TYA(); break;
  case MOS6502_PHA_IMPLIED: this->PHA(); break;
  case MOS6502_PHP_IMPLIED: this->PHP(); break;
  case MOS6502_PLA_IMPLIED: this->PLA(); break;
  case MOS6502_PLP_IMPLIED: this->PLP(); break;
  case MOS6502_RTI_IMPLIED: this->RTI(); break;
  case MOS6502_RTS_IMPLIED: this->RTS(); break;

  default:
    break;
  }
}

inline void CPU::RST() 
{
  /* Rest the register and set address */
  this->m_register.A         = 0;
  this->m_register.X         = 0;
  this->m_register.Y         = 0;
  this->m_register.PC        = this->Read(RST_ADDRESS) | (this->Read(RST_ADDRESS + 1) << 8);
  this->m_register.SP        = 0xFD;
  this->m_register.PS.m_data = 0; 
    
  /* RST Takes 7 clock cycles */
  for( uint8_t iter = 0; iter < 9; iter++ )
    this->Tick();
}

inline void CPU::NMI() 
{
  this->m_register.PS.B = 0;

  this->Write( this->m_register.SP-- | 0x100, this->m_register.PC >> 8 );
  this->Write( this->m_register.SP-- | 0x100, this->m_register.PC );
  this->Write( this->m_register.SP-- | 0x100, this->m_register.PS.m_data );

  this->m_register.PS.I      = 1;
  this->m_register.PC        = this->Read(NMI_ADDRESS) | (this->Read(NMI_ADDRESS + 1) << 8);

  /* NMI Takes 7 clock cycles */
  for( uint8_t iter = 0; iter < 7; iter++ )
    this->Tick();
}

inline void CPU::IRQ() 
{
  if( this->m_register.PS.I != 1 )
  {
    this->m_register.PS.B = 0;
    this->Write( this->m_register.SP-- | 0x100, this->m_register.PC >> 8 );
    this->Write( this->m_register.SP-- | 0x100, this->m_register.PC );
    this->Write( this->m_register.SP-- | 0x100, this->m_register.PS.m_data );
    this->m_register.PS.I = 0;
    this->m_register.PC   = this->Read(IRQ_ADDRESS) | (this->Read(IRQ_ADDRESS + 1) << 8);

    /* IRQ Takes 7 clock cycles */
    for( uint8_t iter = 0; iter < 7; iter++ )
      this->Tick();
  }
}

inline uint16_t CPU::IMMEDIATE()
{
  return this->m_register.PC++;
}

inline uint16_t CPU::RELATIVE() 
{
	return (this->m_register.PC + 1) + (int8_t)this->Read( this->m_register.PC++ );
}


inline void CPU::ADC(uint16_t address) {
  uint8_t  m = this->Read( address );
  uint16_t t = this->m_register.A + m + this->m_register.PS.C;
    
  this->m_register.PS.N =  (t >> 0x07);
  this->m_register.PS.Z = !(t &  0xFF);
  this->m_register.PS.C =  (t >> 0x08);
  this->m_register.PS.V =  ((~(this->m_register.A ^ m) & (this->m_register.A ^ t)) >> 0x07);
  this->m_register.A    =  (t &  0xFF);
}

inline void CPU::AND(uint16_t address) {
  uint8_t  m = this->Read( address );

  this->m_register.A    =  this->m_register.A  & m;
  this->m_register.PS.N =  this->m_register.A >> 0x07;
  this->m_register.PS.Z = !this->m_register.A;
}

inline void CPU::ASL(uint16_t address) {
  uint8_t  m =  this->Read( address );

  this->m_register.PS.C =  m >> 0x07;
  m                     =  m << 0x01;
  this->m_register.PS.Z = !m;
  this->m_register.PS.N =  m >> 0x07;

  this->Write( address, m );
}

inline void CPU::ASL_A() {
  this->m_register.PS.C =  this->m_register.A >> 0x07;
  this->m_register.A    =  this->m_register.A << 0x01;
  this->m_register.PS.Z = !this->m_register.A;
  this->m_register.PS.N =  this->m_register.A >> 0x07;
}

inline void CPU::BCC()
{

}






} /* namespace MOS6502 */