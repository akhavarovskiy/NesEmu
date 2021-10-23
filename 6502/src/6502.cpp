#include "../include/6502.hpp"
#include "../include/6502def.hpp"

#include <cstring>
#include <exception>

namespace MOS6502 {

CPU::CPU()
{
  // RST();
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
  case MOS6502_ADC_ZEROPAGE_X: a = ZEROPAGE_X(); ADC(a); break;
  case MOS6502_AND_ZEROPAGE_X: a = ZEROPAGE_X(); AND(a); break;
  case MOS6502_CMP_ZEROPAGE_X: a = ZEROPAGE_X(); CMP(a); break;
  case MOS6502_EOR_ZEROPAGE_X: a = ZEROPAGE_X(); EOR(a); break;
  case MOS6502_LDA_ZEROPAGE_X: a = ZEROPAGE_X(); LDA(a); break;
  case MOS6502_LDY_ZEROPAGE_X: a = ZEROPAGE_X(); LDY(a); break;
  case MOS6502_ORA_ZEROPAGE_X: a = ZEROPAGE_X(); ORA(a); break;
  case MOS6502_SBC_ZEROPAGE_X: a = ZEROPAGE_X(); SBC(a); break;
  case MOS6502_STA_ZEROPAGE_X: a = ZEROPAGE_X(); STA(a); break;
  case MOS6502_STY_ZEROPAGE_X: a = ZEROPAGE_X(); STY(a); break;
  case MOS6502_ASL_ZEROPAGE_X: a = ZEROPAGE_X(); ASL(a); break;
  case MOS6502_DEC_ZEROPAGE_X: a = ZEROPAGE_X(); DEC(a); break;
  case MOS6502_INC_ZEROPAGE_X: a = ZEROPAGE_X(); INC(a); break;
  case MOS6502_LSR_ZEROPAGE_X: a = ZEROPAGE_X(); LSR(a); break;
  case MOS6502_ROL_ZEROPAGE_X: a = ZEROPAGE_X(); ROL(a); break;
  case MOS6502_ROR_ZEROPAGE_X: a = ZEROPAGE_X(); ROR(a); break;
  /* Zeropage Y */
  case MOS6502_LDX_ZEROPAGE_Y: a = ZEROPAGE_Y(); LDX(a); break;
  case MOS6502_STX_ZEROPAGE_Y: a = ZEROPAGE_Y(); STX(a); break;
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
  case MOS6502_ADC_ABSOLUTE_X: a = ABSOLUTE_X(); ADC(a); break;
  case MOS6502_AND_ABSOLUTE_X: a = ABSOLUTE_X(); AND(a); break;
  case MOS6502_CMP_ABSOLUTE_X: a = ABSOLUTE_X(); CMP(a); break;
  case MOS6502_EOR_ABSOLUTE_X: a = ABSOLUTE_X(); EOR(a); break;
  case MOS6502_LDA_ABSOLUTE_X: a = ABSOLUTE_X(); LDA(a); break;
  case MOS6502_LDY_ABSOLUTE_X: a = ABSOLUTE_X(); LDY(a); break;
  case MOS6502_ORA_ABSOLUTE_X: a = ABSOLUTE_X(); ORA(a); break;
  case MOS6502_SBC_ABSOLUTE_X: a = ABSOLUTE_X(); SBC(a); break;
  case MOS6502_STA_ABSOLUTE_X: a = ABSOLUTE_X(); STA(a); break;
  case MOS6502_ASL_ABSOLUTE_X: a = ABSOLUTE_X(); ASL(a); break;
  case MOS6502_DEC_ABSOLUTE_X: a = ABSOLUTE_X(); DEC(a); break;
  case MOS6502_INC_ABSOLUTE_X: a = ABSOLUTE_X(); INC(a); break;
  case MOS6502_LSR_ABSOLUTE_X: a = ABSOLUTE_X(); LSR(a); break;
  case MOS6502_ROL_ABSOLUTE_X: a = ABSOLUTE_X(); ROL(a); break;
  case MOS6502_ROR_ABSOLUTE_X: a = ABSOLUTE_X(); ROR(a); break;
  /* Absolute Y */
  case MOS6502_ADC_ABSOLUTE_Y: a = ABSOLUTE_Y(); ADC(a); break;
  case MOS6502_AND_ABSOLUTE_Y: a = ABSOLUTE_Y(); AND(a); break;
  case MOS6502_CMP_ABSOLUTE_Y: a = ABSOLUTE_Y(); CMP(a); break;
  case MOS6502_EOR_ABSOLUTE_Y: a = ABSOLUTE_Y(); EOR(a); break;
  case MOS6502_LDA_ABSOLUTE_Y: a = ABSOLUTE_Y(); LDA(a); break;
  case MOS6502_LDX_ABSOLUTE_Y: a = ABSOLUTE_Y(); LDX(a); break;
  case MOS6502_ORA_ABSOLUTE_Y: a = ABSOLUTE_Y(); ORA(a); break;
  case MOS6502_SBC_ABSOLUTE_Y: a = ABSOLUTE_Y(); SBC(a); break;
  case MOS6502_STA_ABSOLUTE_Y: a = ABSOLUTE_Y(); STA(a); break;
  /* Relative (Branching Instrutions) */
  case MOS6502_BCC_RELATIVE: a = RELATIVE(); BCC(a); break;
  case MOS6502_BCS_RELATIVE: a = RELATIVE(); BCS(a); break;
  case MOS6502_BEQ_RELATIVE: a = RELATIVE(); BEQ(a); break;
  case MOS6502_BMI_RELATIVE: a = RELATIVE(); BMI(a); break;
  case MOS6502_BNE_RELATIVE: a = RELATIVE(); BNE(a); break;
  case MOS6502_BPL_RELATIVE: a = RELATIVE(); BPL(a); break;
  case MOS6502_BVC_RELATIVE: a = RELATIVE(); BVC(a); break;
  case MOS6502_BVS_RELATIVE: a = RELATIVE(); BVS(a); break;
  /* Indirect */
  case MOS6502_JMP_INDIRECT: a = INDIRECT(); JMP(a); break;
  /* Indirect X */
  case MOS6502_ADC_INDIRECT_X: a = INDIRECT_X(); ADC(a); break;
  case MOS6502_AND_INDIRECT_X: a = INDIRECT_X(); AND(a); break;
  case MOS6502_CMP_INDIRECT_X: a = INDIRECT_X(); CMP(a); break;
  case MOS6502_EOR_INDIRECT_X: a = INDIRECT_X(); EOR(a); break;
  case MOS6502_LDA_INDIRECT_X: a = INDIRECT_X(); LDA(a); break;
  case MOS6502_ORA_INDIRECT_X: a = INDIRECT_X(); ORA(a); break;
  case MOS6502_SBC_INDIRECT_X: a = INDIRECT_X(); SBC(a); break;
  case MOS6502_STA_INDIRECT_X: a = INDIRECT_X(); STA(a); break;
  /* Inirect Y */
  case MOS6502_AND_INDIRECT_Y: a = INDIRECT_Y(); AND(a); break;
  case MOS6502_ADC_INDIRECT_Y: a = INDIRECT_Y(); ADC(a); break;
  case MOS6502_CMP_INDIRECT_Y: a = INDIRECT_Y(); CMP(a); break;
  case MOS6502_EOR_INDIRECT_Y: a = INDIRECT_Y(); EOR(a); break;
  case MOS6502_LDA_INDIRECT_Y: a = INDIRECT_Y(); LDA(a); break;
  case MOS6502_ORA_INDIRECT_Y: a = INDIRECT_Y(); ORA(a); break;
  case MOS6502_SBC_INDIRECT_Y: a = INDIRECT_Y(); SBC(a); break;
  case MOS6502_STA_INDIRECT_Y: a = INDIRECT_Y(); STA(a); break;
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
  Tick();
}

inline void CPU::BCC(uint16_t address)
{
  if( !m_register.PS.C )
  {
    /* Additional Cycle on taken branch */
    Tick();
    /* Additional Cycle on page boundary cross*/
    if((address ^ m_register.PC) & 0xFF00)
      Tick();

    m_register.PC = address;
  }
}

inline void CPU::BCS(uint16_t address)
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

inline void CPU::BEQ(uint16_t address)
{
  uint16_t r = RELATIVE();
  if( m_register.PS.Z )
  {
    /* Additional Cycle on taken branch */
    Tick();
    /* Additional Cycle on page boundary cross*/
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

inline void CPU::BMI(uint16_t address)
{
  if( m_register.PS.N )
  {
    /* Additional Cycle on taken branch */
    Tick();
    /* Additional Cycle on page boundary cross*/
    if((address ^ m_register.PC) & 0xFF00)
      Tick();

    m_register.PC = address;
  }
}

inline void CPU::BNE(uint16_t address)
{
  if( !m_register.PS.Z )
  {
    /* Additional Cycle on taken branch */
    Tick();
    /* Additional Cycle on page boundary cross*/
    if((address ^ m_register.PC) & 0xFF00)
      Tick();

    m_register.PC = address;
  }
}

inline void CPU::BPL(uint16_t address)
{
  if( !m_register.PS.N )
  {
    /* Additional Cycle on taken branch */
    Tick();
    /* Additional Cycle on page boundary cross*/
    if((address ^ m_register.PC) & 0xFF00)
      Tick();

    m_register.PC = address;
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

inline void CPU::BVC(uint16_t address)
{
  if( !m_register.PS.V )
  {
    /* Additional Cycle on taken branch */
    Tick();
    /* Additional Cycle on page boundary cross*/
    if((address ^ m_register.PC) & 0xFF00)
      Tick();

    m_register.PC = address;
  }
}

inline void CPU::BVS(uint16_t address)
{
  if( m_register.PS.V )
  {
    /* Additional Cycle on taken branch */
    Tick();
    /* Additional Cycle on page boundary cross*/
    if((address ^ m_register.PC) & 0xFF00)
      Tick();

    m_register.PC = address;
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
  Tick();
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

#ifdef MOS6502_UNIT_TEST
TestCPU::TestCPU() {
  m_ticks  = 0;
  std::memset(m_memory, 0, 1 << 16);
}
TestCPU::~TestCPU() { }

REG & TestCPU::Register() {
  return m_register;
}

void     TestCPU::Reset() {
  RST();
}

void     TestCPU::SetBootAddress(uint16_t address) {
  m_memory[RST_ADDRESS+1] = (address >> 8) & 0xFF;
  m_memory[RST_ADDRESS  ] = (address)      & 0xFF;
}

uint8_t  TestCPU::Read (uint16_t address) {
  Tick();
  return m_memory[address];
}
void     TestCPU::Write(uint16_t address, uint8_t v) {
  Tick();
  m_memory[address] = v;
}
void     TestCPU::Tick () {
  m_ticks++;
}

/* Non Invasive Read, regular read triggers a tick */
uint8_t  TestCPU::NI_Read (uint16_t address) {
  return m_memory[address];
}
/* Non Invasive Write, regular write triggers a tick */
void     TestCPU::NI_Write(uint16_t address, uint8_t v) {
  m_memory[address] = v;
}
uint64_t TestCPU::GetTicks() {
  return m_ticks;
}
void     TestCPU::SetTicks(uint64_t ticks){
  m_ticks = ticks;
}
#endif

} /* namespace MOS6502 */