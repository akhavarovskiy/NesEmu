#include "../include/6502.hpp"
#include <cstring>
#include <exception>

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

#define MOS6502_ADC_ABSOLUTEX   0x7D
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
#define MOS6502_ADC_INDIRECTY   0x71
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
  RST();
}

CPU::~CPU()
{

}

inline void CPU::TriggerIRQ()
{
  m_interrupt.IRQ_TRIGGER = !m_interrupt.IRQ_TRIGGER;
}

inline void CPU::TriggerNMI()
{
  m_interrupt.NMI_TRIGGER = !m_interrupt.NMI_TRIGGER;
}

inline void CPU::TriggerRST()
{
  m_interrupt.RST_TRIGGER = !m_interrupt.RST_TRIGGER;
}

void CPU::Step(void)
{
  /* Grab the instruction and increment the Program Counter */
  uint16_t a;
  uint8_t c = Read( m_register.PC++ );
  switch (c)
  {
  /* Immediate Instructions */
  case MOS6502_ADC_IMMEDIATE: a = IMMEDIATE();  ADC(a); break;
  case MOS6502_AND_IMMEDIATE: a = IMMEDIATE();  AND(a); break;
  case MOS6502_CMP_IMMEDIATE: a = IMMEDIATE();  CMP(a); break;
  case MOS6502_CPY_IMMEDIATE: a = IMMEDIATE();  CPY(a); break;
  case MOS6502_CPX_IMMEDIATE: a = IMMEDIATE();  CPX(a); break;
  case MOS6502_EOR_IMMEDIATE: a = IMMEDIATE();  EOR(a); break;
  case MOS6502_LDA_IMMEDIATE: a = IMMEDIATE();  LDA(a); break;
  case MOS6502_LDX_IMMEDIATE: a = IMMEDIATE();  LDX(a); break;
  case MOS6502_LDY_IMMEDIATE: a = IMMEDIATE();  LDY(a); break;
  case MOS6502_ORA_IMMEDIATE: a = IMMEDIATE();  ORA(a); break;
  case MOS6502_SBC_IMMEDIATE: a = IMMEDIATE();  SBC(a); break;
  /* Zeropage */
  case MOS6502_ADC_ZEROPAGE:  a = ZEROPAGE();   ADC(a); break;
  case MOS6502_AND_ZEROPAGE:  a = ZEROPAGE();   AND(a); break;
  case MOS6502_ASL_ZEROPAGE:  a = ZEROPAGE();   ASL(a); break;
  case MOS6502_BIT_ZEROPAGE:  a = ZEROPAGE();   BIT(a); break;
  case MOS6502_CMP_ZEROPAGE:  a = ZEROPAGE();   CMP(a); break;
  case MOS6502_CPX_ZEROPAGE:  a = ZEROPAGE();   CPX(a); break;
  case MOS6502_CPY_ZEROPAGE:  a = ZEROPAGE();   CPY(a); break;
  case MOS6502_EOR_ZEROPAGE:  a = ZEROPAGE();   EOR(a); break;
  case MOS6502_LDA_ZEROPAGE:  a = ZEROPAGE();   LDA(a); break;
  case MOS6502_LDX_ZEROPAGE:  a = ZEROPAGE();   LDX(a); break;
  case MOS6502_LDY_ZEROPAGE:  a = ZEROPAGE();   LDY(a); break;
  case MOS6502_ORA_ZEROPAGE:  a = ZEROPAGE();   ORA(a); break;
  case MOS6502_SBC_ZEROPAGE:  a = ZEROPAGE();   SBC(a); break;
  case MOS6502_STA_ZEROPAGE:  a = ZEROPAGE();   STA(a); break;
  case MOS6502_STX_ZEROPAGE:  a = ZEROPAGE();   STX(a); break;
  case MOS6502_STY_ZEROPAGE:  a = ZEROPAGE();   STY(a); break;
  case MOS6502_DEC_ZEROPAGE:  a = ZEROPAGE();   DEC(a); break;
  case MOS6502_INC_ZEROPAGE:  a = ZEROPAGE();   INC(a); break;
  case MOS6502_LSR_ZEROPAGE:  a = ZEROPAGE();   LSR(a); break;
  case MOS6502_ROL_ZEROPAGE:  a = ZEROPAGE();   ROL(a); break;
  case MOS6502_ROR_ZEROPAGE:  a = ZEROPAGE();   ROR(a); break;
  /* Zeropage X */
  case MOS6502_ADC_ZEROPAGEX: a = ZEROPAGE_X(); ADC(a); break;
  case MOS6502_AND_ZEROPAGEX: a = ZEROPAGE_X(); AND(a); break;
  case MOS6502_CMP_ZEROPAGEX: a = ZEROPAGE_X(); CMP(a); break;
  case MOS6502_EOR_ZEROPAGEX: a = ZEROPAGE_X(); EOR(a); break;
  case MOS6502_LDA_ZEROPAGEX: a = ZEROPAGE_X(); LDA(a); break;
  case MOS6502_LDY_ZEROPAGEX: a = ZEROPAGE_X(); LDY(a); break;
  case MOS6502_ORA_ZEROPAGEX: a = ZEROPAGE_X(); ORA(a); break;
  case MOS6502_SBC_ZEROPAGEX: a = ZEROPAGE_X(); SBC(a); break;
  case MOS6502_STA_ZEROPAGEX: a = ZEROPAGE_X(); STA(a); break;
  case MOS6502_STY_ZEROPAGEX: a = ZEROPAGE_X(); STY(a); break;
  case MOS6502_ASL_ZEROPAGEX: a = ZEROPAGE_X(); ASL(a); break;
  case MOS6502_DEC_ZEROPAGEX: a = ZEROPAGE_X(); DEC(a); break;
  case MOS6502_INC_ZEROPAGEX: a = ZEROPAGE_X(); INC(a); break;
  case MOS6502_LSR_ZEROPAGEX: a = ZEROPAGE_X(); LSR(a); break;
  case MOS6502_ROL_ZEROPAGEX: a = ZEROPAGE_X(); ROL(a); break;
  case MOS6502_ROR_ZEROPAGEX: a = ZEROPAGE_X(); ROR(a); break;
  /* Zeropage Y */
  case MOS6502_LDX_ZEROPAGEY: a = ZEROPAGE_Y(); LDX(a); break;
  case MOS6502_STX_ZEROPAGEY: a = ZEROPAGE_Y(); STX(a); break;
  /* Absolute */
  case MOS6502_ADC_ABSOLUTE:  a = ABSOLUTE();   ADC(a); break;
  case MOS6502_AND_ABSOLUTE:  a = ABSOLUTE();   AND(a); break;
  case MOS6502_BIT_ABSOLUTE:  a = ABSOLUTE();   BIT(a); break;
  case MOS6502_CMP_ABSOLUTE:  a = ABSOLUTE();   CMP(a); break;
  case MOS6502_CPX_ABSOLUTE:  a = ABSOLUTE();   CPX(a); break;
  case MOS6502_CPY_ABSOLUTE:  a = ABSOLUTE();   CPY(a); break;
  case MOS6502_EOR_ABSOLUTE:  a = ABSOLUTE();   EOR(a); break;
  case MOS6502_JMP_ABSOLUTE:  a = ABSOLUTE();   JMP(a); break;
  case MOS6502_LDA_ABSOLUTE:  a = ABSOLUTE();   LDA(a); break;
  case MOS6502_LDX_ABSOLUTE:  a = ABSOLUTE();   LDX(a); break;
  case MOS6502_LDY_ABSOLUTE:  a = ABSOLUTE();   LDY(a); break;
  case MOS6502_ORA_ABSOLUTE:  a = ABSOLUTE();   ORA(a); break;
  case MOS6502_SBC_ABSOLUTE:  a = ABSOLUTE();   SBC(a); break;
  case MOS6502_STA_ABSOLUTE:  a = ABSOLUTE();   STA(a); break;
  case MOS6502_STX_ABSOLUTE:  a = ABSOLUTE();   STX(a); break;
  case MOS6502_STY_ABSOLUTE:  a = ABSOLUTE();   STY(a); break;
  case MOS6502_ASL_ABSOLUTE:  a = ABSOLUTE();   ASL(a); break;
  case MOS6502_DEC_ABSOLUTE:  a = ABSOLUTE();   DEC(a); break;
  case MOS6502_INC_ABSOLUTE:  a = ABSOLUTE();   INC(a); break;
  case MOS6502_LSR_ABSOLUTE:  a = ABSOLUTE();   LSR(a); break;
  case MOS6502_ROL_ABSOLUTE:  a = ABSOLUTE();   ROL(a); break;
  case MOS6502_ROR_ABSOLUTE:  a = ABSOLUTE();   ROR(a); break;
  case MOS6502_JSR_ABSOLUTE:  a = ABSOLUTE();   JSR(a); break;
  /* Absolute X */
  case MOS6502_ADC_ABSOLUTEX: a = ABSOLUTE_X(); ADC(a); break;
  case MOS6502_AND_ABSOLUTEX: a = ABSOLUTE_X(); AND(a); break;
  case MOS6502_CMP_ABSOLUTEX: a = ABSOLUTE_X(); CMP(a); break;
  case MOS6502_EOR_ABSOLUTEX: a = ABSOLUTE_X(); EOR(a); break;
  case MOS6502_LDA_ABSOLUTEX: a = ABSOLUTE_X(); LDA(a); break;
  case MOS6502_LDY_ABSOLUTEX: a = ABSOLUTE_X(); LDY(a); break;
  case MOS6502_ORA_ABSOLUTEX: a = ABSOLUTE_X(); ORA(a); break;
  case MOS6502_SBC_ABSOLUTEX: a = ABSOLUTE_X(); SBC(a); break;
  case MOS6502_STA_ABSOLUTEX: a = ABSOLUTE_X(); STA(a); break;
  case MOS6502_ASL_ABSOLUTEX: a = ABSOLUTE_X(); ASL(a); break;
  case MOS6502_DEC_ABSOLUTEX: a = ABSOLUTE_X(); DEC(a); break;
  case MOS6502_INC_ABSOLUTEX: a = ABSOLUTE_X(); INC(a); break;
  case MOS6502_LSR_ABSOLUTEX: a = ABSOLUTE_X(); LSR(a); break;
  case MOS6502_ROL_ABSOLUTEX: a = ABSOLUTE_X(); ROL(a); break;
  case MOS6502_ROR_ABSOLUTEX: a = ABSOLUTE_X(); ROR(a); break;
  /* Absolute Y */
  case MOS6502_ADC_ABSOLUTEY: a = ABSOLUTE_Y(); ADC(a); break;
  case MOS6502_AND_ABSOLUTEY: a = ABSOLUTE_Y(); AND(a); break;
  case MOS6502_CMP_ABSOLUTEY: a = ABSOLUTE_Y(); CMP(a); break;
  case MOS6502_EOR_ABSOLUTEY: a = ABSOLUTE_Y(); EOR(a); break;
  case MOS6502_LDA_ABSOLUTEY: a = ABSOLUTE_Y(); LDA(a); break;
  case MOS6502_LDX_ABSOLUTEY: a = ABSOLUTE_Y(); LDX(a); break;
  case MOS6502_ORA_ABSOLUTEY: a = ABSOLUTE_Y(); ORA(a); break;
  case MOS6502_SBC_ABSOLUTEY: a = ABSOLUTE_Y(); SBC(a); break;
  case MOS6502_STA_ABSOLUTEY: a = ABSOLUTE_Y(); STA(a); break;
  /* Relative (Branching Instrutions) */
  case MOS6502_BCC_RELATIVE: BCC(); break;
  case MOS6502_BCS_RELATIVE: BCS(); break;
  case MOS6502_BEQ_RELATIVE: BEQ(); break;
  case MOS6502_BMI_RELATIVE: BMI(); break;
  case MOS6502_BNE_RELATIVE: BNE(); break;
  case MOS6502_BPL_RELATIVE: BPL(); break;
  case MOS6502_BVC_RELATIVE: BVC(); break;
  case MOS6502_BVS_RELATIVE: BVS(); break;
  /* Indirect */
  case MOS6502_JMP_INDIRECT: a = INDIRECT(); JMP(a); break;
  /* Indirect X */
  case MOS6502_ADC_INDIRECTX: a = INDIRECT_X(); ADC(a); break;
  case MOS6502_AND_INDIRECTX: a = INDIRECT_X(); AND(a); break;
  case MOS6502_CMP_INDIRECTX: a = INDIRECT_X(); CMP(a); break;
  case MOS6502_EOR_INDIRECTX: a = INDIRECT_X(); EOR(a); break;
  case MOS6502_LDA_INDIRECTX: a = INDIRECT_X(); LDA(a); break;
  case MOS6502_ORA_INDIRECTX: a = INDIRECT_X(); ORA(a); break;
  case MOS6502_SBC_INDIRECTX: a = INDIRECT_X(); SBC(a); break;
  case MOS6502_STA_INDIRECTX: a = INDIRECT_X(); STA(a); break;
  /* Inirect Y */
  case MOS6502_AND_INDIRECTY: a = INDIRECT_Y(); AND(a); break;
  case MOS6502_ADC_INDIRECTY: a = INDIRECT_Y(); ADC(a); break;
  case MOS6502_CMP_INDIRECTY: a = INDIRECT_Y(); CMP(a); break;
  case MOS6502_EOR_INDIRECTY: a = INDIRECT_Y(); EOR(a); break;
  case MOS6502_LDA_INDIRECTY: a = INDIRECT_Y(); LDA(a); break;
  case MOS6502_ORA_INDIRECTY: a = INDIRECT_Y(); ORA(a); break;
  case MOS6502_SBC_INDIRECTY: a = INDIRECT_Y(); SBC(a); break;
  case MOS6502_STA_INDIRECTY: a = INDIRECT_Y(); STA(a); break;
  /* Accumulator */
  case MOS6502_ASL_ACCUMULATOR: ASL_A(); break;
  case MOS6502_LSR_ACCUMULATOR: LSR_A(); break;
  case MOS6502_ROL_ACCUMULATOR: ROL_A(); break;
  case MOS6502_ROR_ACCUMULATOR: ROR_A(); break;
  /* Implied instructions */
  case MOS6502_BRK_IMPLIED: BRK(); break;
  case MOS6502_CLC_IMPLIED: CLC(); break;
  case MOS6502_CLD_IMPLIED: CLD(); break;
  case MOS6502_CLI_IMPLIED: CLI(); break;
  case MOS6502_CLV_IMPLIED: CLV(); break;
  case MOS6502_DEX_IMPLIED: DEX(); break;
  case MOS6502_DEY_IMPLIED: DEY(); break;
  case MOS6502_INX_IMPLIED: INX(); break;
  case MOS6502_INY_IMPLIED: INY(); break;
  case MOS6502_NOP_IMPLIED: NOP(); break;
  case MOS6502_SEC_IMPLIED: SEC(); break;
  case MOS6502_SED_IMPLIED: SED(); break;
  case MOS6502_SEI_IMPLIED: SEI(); break;
  case MOS6502_TAX_IMPLIED: TAX(); break;
  case MOS6502_TAY_IMPLIED: TAY(); break;
  case MOS6502_TSX_IMPLIED: TSX(); break;
  case MOS6502_TXA_IMPLIED: TXA(); break;
  case MOS6502_TXS_IMPLIED: TXS(); break;
  case MOS6502_TYA_IMPLIED: TYA(); break;
  case MOS6502_PHA_IMPLIED: PHA(); break;
  case MOS6502_PHP_IMPLIED: PHP(); break;
  case MOS6502_PLA_IMPLIED: PLA(); break;
  case MOS6502_PLP_IMPLIED: PLP(); break;
  case MOS6502_RTI_IMPLIED: RTI(); break;
  case MOS6502_RTS_IMPLIED: RTS(); break;
  default:
    throw "Unsupported OP Code";
  }
}

inline void CPU::RST()
{
  /* Rest the register and set address */
  m_register.A         = 0;
  m_register.X         = 0;
  m_register.Y         = 0;
  m_register.PC        = Read(RST_ADDRESS) | (Read(RST_ADDRESS + 1) << 8);
  m_register.SP        = 0xFD;
  m_register.PS.m_data = 0;

  /* RST Takes 7 clock cycles */
  for( uint8_t iter = 0; iter < 9; iter++ )
    Tick();
}

inline void CPU::NMI()
{
  m_register.PS.B = 0;

  Write( m_register.SP-- | 0x100, m_register.PC >> 8 );
  Write( m_register.SP-- | 0x100, m_register.PC );
  Write( m_register.SP-- | 0x100, m_register.PS.m_data );

  m_register.PS.I      = 1;
  m_register.PC        = Read(NMI_ADDRESS) | (Read(NMI_ADDRESS + 1) << 8);

  /* NMI Takes 7 clock cycles */
  for( uint8_t iter = 0; iter < 7; iter++ )
    Tick();
}

inline void CPU::IRQ()
{
  if( m_register.PS.I != 1 )
  {
    m_register.PS.B = 0;
    Write( m_register.SP-- | 0x100, m_register.PC >> 8 );
    Write( m_register.SP-- | 0x100, m_register.PC );
    Write( m_register.SP-- | 0x100, m_register.PS.m_data );
    m_register.PS.I = 0;
    m_register.PC   = Read(IRQ_ADDRESS) | (Read(IRQ_ADDRESS + 1) << 8);

    /* IRQ Takes 7 clock cycles */
    for( uint8_t iter = 0; iter < 7; iter++ )
      Tick();
  }
}

inline uint16_t CPU::IMMEDIATE()
{
  return m_register.PC++;
}

inline uint16_t CPU::ZEROPAGE()
{
  return Read( m_register.PC++ );
}

inline uint16_t CPU::ZEROPAGE_X()
{
  return (Read(m_register.PC++) + m_register.X) & 0xFF;
}

inline uint16_t CPU::ZEROPAGE_Y()
{
  return (Read(m_register.PC++) + m_register.Y) & 0xFF;
}

inline uint16_t CPU::ABSOLUTE()
{
  return Read(m_register.PC++) | (Read(m_register.PC++) << 8);
}

inline uint16_t CPU::ABSOLUTE_X()
{
  uint16_t m = Read(m_register.PC++) | (Read(m_register.PC++) << 8);
  uint16_t r = m + m_register.X;

  if((( r ^ m ) & 0xFF00))
    Tick();

  return r;
}

inline uint16_t CPU::ABSOLUTE_Y()
{
  uint16_t m = Read(m_register.PC++) | (Read(m_register.PC++) << 8);
  uint16_t r = m + m_register.Y;

  if((( r ^ m ) & 0xFF00) )
    Tick();

  return r;
}

inline uint16_t CPU::RELATIVE()
{
  return (m_register.PC + 1) + (int8_t)Read(m_register.PC++);
}

inline uint16_t CPU::INDIRECT()
{
  uint16_t a = Read(m_register.PC++) | (Read(m_register.PC++) << 8);
  uint16_t p = a & 0xFF00;
  uint8_t  i = a & 0x00FF;
  return Read(a) | (Read(p | ++i) << 8);
}

inline uint16_t CPU::INDIRECT_X()
{
  uint16_t i = Read( m_register.PC++ ) + m_register.X;
  return Read(i) | (Read( ++i ) << 8);
}

inline uint16_t CPU::INDIRECT_Y()
{
  uint8_t  i = Read(m_register.PC++);
  uint16_t m = Read(i) | (Read(++i) << 8);
  uint16_t r = m + m_register.Y;

  if(( r ^ m ) & 0xFF00)
    Tick();

  return r;
}

inline void CPU::ADC(uint16_t address) {
  uint8_t  m = Read(address);
  uint16_t t = m_register.A + m + m_register.PS.C;
  m_register.PS.N =  (t >> 0x07);
  m_register.PS.Z = !(t &  0xFF);
  m_register.PS.C =  (t >> 0x08);
  m_register.PS.V =  ((~(m_register.A ^ m) & (m_register.A ^ t)) >> 0x07);
  m_register.A    =  (t &  0xFF);
}

inline void CPU::AND(uint16_t address) {
  uint8_t  m = Read( address );
  m_register.A    =  m_register.A  & m;
  m_register.PS.N =  m_register.A >> 0x07;
  m_register.PS.Z = !m_register.A;
}

inline void CPU::ASL(uint16_t address) {
  uint8_t  m =  Read( address );
  m_register.PS.C =  m >> 0x07;
  m               =  m << 0x01;
  m_register.PS.Z = !m;
  m_register.PS.N =  m >> 0x07;
  Write( address, m );
}

inline void CPU::ASL_A() {
  m_register.PS.C =  m_register.A >> 0x07;
  m_register.A    =  m_register.A << 0x01;
  m_register.PS.Z = !m_register.A;
  m_register.PS.N =  m_register.A >> 0x07;
}

inline void CPU::BCC()
{
  uint16_t r = RELATIVE();
  if( !m_register.PS.C )
  {
    /* Additional Cycle on taken branch */
    Tick();
    /* Additional Cycle on page boundry cros*/
    if((r ^ m_register.PC ) & 0xFF00)
      Tick();

    m_register.PC = r;
  }
}

inline void CPU::BCS()
{
  uint16_t r = RELATIVE();
  if( m_register.PS.C )
  {
    /* Additional Cycle on taken branch */
    Tick();
    /* Additional Cycle on page boundary cross*/
    if((r ^ m_register.PC ) & 0xFF00)
      Tick();

    m_register.PC = r;
  }
}

inline void CPU::BEQ()
{
  uint16_t r = RELATIVE();
  if( m_register.PS.Z )
  {
    /* Additional Cycle on taken branch */
    Tick();
    /* Additional Cycle on page boundry cros*/
    if((r ^ m_register.PC ) & 0xFF00)
      Tick();

    m_register.PC = r;
  }
}

inline void CPU::BIT(uint16_t address)
{
  uint8_t m       = Read( address );
  m_register.PS.N =  (m >> 0x07);
  m_register.PS.V =  (m >> 0x06);
  m_register.PS.Z = !(m  & m_register.A);
}

inline void CPU::BMI()
{
  uint16_t r = RELATIVE();
  if( m_register.PS.N )
  {
    /* Additional Cycle on taken branch */
    Tick();
    /* Additional Cycle on page boundry cros*/
    if((r ^ m_register.PC ) & 0xFF00)
      Tick();

    m_register.PC = r;
  }
}

inline void CPU::BNE()
{
  uint16_t r = RELATIVE();
  if( !m_register.PS.Z )
  {
    /* Additional Cycle on taken branch */
    Tick();
    /* Additional Cycle on page boundry cros*/
    if((r ^ m_register.PC ) & 0xFF00)
      Tick();

    m_register.PC = r;
  }
}

inline void CPU::BPL()
{
  uint16_t r = RELATIVE();
  if( !m_register.PS.N )
  {
    /* Additional Cycle on taken branch */
    Tick();
    /* Additional Cycle on page boundry cros*/
    if((r ^ m_register.PC ) & 0xFF00)
      Tick();

    m_register.PC = r;
  }
}

inline void CPU::BRK()
{
  m_register.PC++;
  Write(m_register.SP-- | 0x100, (uint8_t)(m_register.PC >> 8));
  Write(m_register.SP-- | 0x100, (uint8_t)(m_register.PC));
  m_register.PS.B = 1;
  Write(m_register.SP-- | 0x100, m_register.PS.m_data );
  m_register.PS.I = 1;
  m_register.PC   = Read( IRQ_ADDRESS );
}

inline void CPU::BVC()
{
  uint16_t r = RELATIVE();
  if( !m_register.PS.V )
  {
    /* Additional Cycle on taken branch */
    Tick();
    /* Additional Cycle on page boundry cros*/
    if((r ^ m_register.PC ) & 0xFF00)
      Tick();

    m_register.PC = r;
  }
}

inline void CPU::BVS()
{
  uint16_t r = RELATIVE();
  if( m_register.PS.V )
  {
    /* Additional Cycle on taken branch */
    Tick();
    /* Additional Cycle on page boundry cros*/
    if((r ^ m_register.PC ) & 0xFF00)
      Tick();

    m_register.PC = r;
  }
}

inline void CPU::CLC()
{
  m_register.PS.C = 0;
}

inline void CPU::CLI()
{
  m_register.PS.I = 0;
}

inline void CPU::CLD()
{
  m_register.PS.D = 0;
}

inline void CPU::CLV()
{
  m_register.PS.V = 0;
}

inline void CPU::CMP(uint16_t address)
{
  uint16_t t = m_register.A - Read(address);
  m_register.PS.N =  t >> 0x07;
  m_register.PS.Z = !t;
  m_register.PS.C =~(t >> 0x08);
}

inline void CPU::CPX(uint16_t address)
{
  uint16_t t = m_register.X - Read(address);
  m_register.PS.N =  t >> 0x07;
  m_register.PS.Z = !t;
  m_register.PS.C =~(t >> 0x08);
}

inline void CPU::CPY(uint16_t address)
{
  uint16_t t = m_register.Y - Read(address);
  m_register.PS.N =  t >> 0x07;
  m_register.PS.Z = !t;
  m_register.PS.C =~(t >> 0x08);
}

inline void CPU::DEC(uint16_t address)
{
  uint8_t  m = Read(address);
  m--;
  m_register.PS.N =  m >> 0x07;
  m_register.PS.Z = !m;
  Write(address, m);
}

inline void CPU::DEX()
{
  m_register.X--;
  m_register.PS.N =  m_register.X >> 0x07;
  m_register.PS.Z = !m_register.X;
}

inline void CPU::DEY()
{
  m_register.Y--;
  m_register.PS.N =  m_register.Y >> 0x07;
  m_register.PS.Z = !m_register.Y;
}

inline void CPU::EOR(uint16_t address)
{
  uint8_t  m      =  Read(address);
  m_register.A    =  m_register.A  ^ m;
  m_register.PS.N =  m_register.A >> 0x07;
  m_register.PS.Z = !m_register.A;
}

inline void CPU::INC(uint16_t address)
{
  uint8_t m = Read(address);
  m++;
  m_register.PS.N =  m >> 0x07;
  m_register.PS.Z = !m;
  Write(address, m);
}

inline void CPU::INX()
{
  m_register.X++;
  m_register.PS.N =  m_register.X >> 0x07;
  m_register.PS.Z = !m_register.X;
}

inline void CPU::INY()
{
  m_register.Y++;
  m_register.PS.N =  m_register.Y >> 0x07;
  m_register.PS.Z = !m_register.Y;
}

inline void CPU::JMP(uint16_t address)
{
  m_register.PC = address;
}

inline void CPU::JSR(uint16_t address)
{
  uint16_t r = m_register.PC + 1;
  Write(m_register.SP-- | 0x100, r >> 8);
  Write(m_register.SP-- | 0x100, r);
  m_register.PC = address;
}

inline void CPU::LDA(uint16_t address)
{
  uint8_t  m = Read(address);
  m_register.A    =  m;
  m_register.PS.N =  m >> 0x07;
  m_register.PS.Z = !m;
}

inline void CPU::LDX(uint16_t address)
{
  uint8_t t = Read(address);
  m_register.X    =  t;
  m_register.PS.N =  t >> 0x07;
  m_register.PS.Z = !t;
}

inline void CPU::LDY(uint16_t address)
{
  uint8_t t = Read(address);
  m_register.Y    =  t;
  m_register.PS.N =  t >> 0x07;
  m_register.PS.Z = !t;
}

inline void CPU::LSR(uint16_t address)
{
  uint8_t m = Read(address);
  m_register.PS.C =  m  & 0x01;
  m_register.PS.N =  0x00;
  m               =  m >> 0x01;
  m_register.PS.Z = !m;
  Write(address, m);
}

inline void CPU::LSR_A()
{
  m_register.PS.N =  0x00;
  m_register.PS.C =  m_register.A  & 0x01;
  m_register.A    =  m_register.A >> 0x01;
  m_register.PS.Z = !m_register.A;
}

inline void CPU::NOP()
{
  Tick();
}

inline void CPU::ORA(uint16_t address)
{
  uint8_t  m = Read(address);
  m_register.A    =  m_register.A | m;
  m_register.PS.N =  m_register.A >> 0x07;
  m_register.PS.Z = !m_register.A;
}

inline void CPU::PHA()
{
  Write(m_register.SP-- | 0x100, m_register.A);
}

inline void CPU::PHP()
{
  Write(m_register.SP-- | 0x100, m_register.PS.m_data);
}

inline void CPU::PLA()
{
  m_register.A    =  Read(++m_register.SP | 0x100);
  m_register.PS.N =  m_register.A >> 0x07;
  m_register.PS.Z = !m_register.A;
}

inline void CPU::PLP()
{
  m_register.PS.m_data = Read(++m_register.SP | 0x100);
  m_register.PS.B      = 0;
  m_register.PS.P      = 1;
}

inline void CPU::ROL(uint16_t address)
{
  uint16_t m = Read(address);
  uint16_t t = ((uint16_t)m << 0x01) | m_register.PS.C;
  m_register.PS.C =  t >> 0x08;
  m_register.PS.N =  t >> 0x07;
  m_register.PS.Z = !t;
  Write(address, t);
}

inline void CPU::ROL_A()
{
  uint16_t t  = ((uint16_t)m_register.A << 0x01) | m_register.PS.C;
  m_register.PS.C =  t >> 0x08;
  m_register.PS.N =  t >> 0x07;
  m_register.PS.Z = !t;
  m_register.A    =  t;
}

inline void CPU::ROR(uint16_t address)
{
  uint8_t  m = Read(address);
  uint16_t t = m | ((uint16_t)m_register.PS.C << 8);
  m_register.PS.C =  t  & 0x01;
  t               =  t >> 0x01;
  m_register.PS.N =  t >> 0x07;
  m_register.PS.Z = !t;
  Write(address, t);
}

inline void CPU::ROR_A()
{
  uint16_t t  = m_register.A | ((uint16_t)m_register.PS.C << 8);
  m_register.PS.C =  t  & 0x01;
  t               =  t >> 0x01;
  m_register.PS.N =  t >> 0x07;
  m_register.PS.Z = !t;
  m_register.A    =  t;
}

inline void CPU::RTI()
{
  m_register.PS.m_data = Read(++m_register.SP | 0x100);
  m_register.PS.P      = 1;
  m_register.PC = Read(++m_register.SP | 0x100) | \
    (Read(++m_register.SP | 0x100) << 8 );
}

inline void CPU::RTS()
{
  m_register.PC = Read(++m_register.SP | 0x100) | \
    (Read(++m_register.SP | 0x100 ) << 8);
  m_register.PC++;
}

inline void CPU::SBC(uint16_t address)
{
  uint8_t  m = Read(address) ^ 0xFF;
  uint16_t t = m_register.A + m + m_register.PS.C;
  m_register.PS.N =  (t >> 0x07);
  m_register.PS.Z = !(t &  0xFF);
  m_register.PS.C =  (t >> 0x08);
  m_register.PS.V =  (~(m_register.A ^ m) & (m_register.A ^ t) & 0x80) >> 0x07;
  m_register.A    =  (t &  0xFF);
}

inline void CPU::SEC()
{
  m_register.PS.C = 1;
}

inline void CPU::SED()
{
  m_register.PS.D = 1;
}

inline void CPU::SEI()
{
  m_register.PS.I = 1;
}

inline void CPU::STA(uint16_t address)
{
  Write(address, m_register.A);
}

inline void CPU::STX(uint16_t address)
{
  Write(address, m_register.X);
}

inline void CPU::STY(uint16_t address)
{
  Write(address, m_register.Y);
}

inline void CPU::TAX()
{
  m_register.X    =  m_register.A;
  m_register.PS.N =  m_register.X >> 0x07;
  m_register.PS.Z = !m_register.X;
}

inline void CPU::TAY()
{
  m_register.Y    =  m_register.A;
  m_register.PS.N =  m_register.Y >> 0x07;
  m_register.PS.Z = !m_register.Y;
}

inline void CPU::TSX()
{
  m_register.X    =  m_register.SP;
  m_register.PS.N =  m_register.X >> 0x07;
  m_register.PS.Z = !m_register.X;
}

inline void CPU::TXA()
{
  m_register.A    =  m_register.X;
  m_register.PS.N =  m_register.A >> 0x07;
  m_register.PS.Z = !m_register.A;
}

inline void CPU::TXS()
{
  m_register.SP = m_register.X;
}

inline void CPU::TYA()
{
  m_register.A    =  m_register.Y;
  m_register.PS.N =  m_register.A >> 0x07;
  m_register.PS.Z = !m_register.A;
}

} /* namespace MOS6502 */