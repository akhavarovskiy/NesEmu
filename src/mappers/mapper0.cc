#include "../../include/mappers/mapper0.hpp"

namespace nes { 

namespace mappers {

Mapper0::Mapper0(std::unique_ptr<Rom> & rom) : m_rom(*rom.get())
{
	switch (this->m_rom.Header().m_PROMBankCount)
	{
	case 1:
		this->m_selectedPROMBanks = { 
			this->m_rom.ProgramROM(0).data(),
			this->m_rom.ProgramROM(0).data() 
		};
		this->m_selectedCROMBank = this->m_rom.CharacterROM(0).data();
		break;
	case 2:
		this->m_selectedPROMBanks = { 
			this->m_rom.ProgramROM(0).data(),
			this->m_rom.ProgramROM(1).data()
		};
		this->m_selectedCROMBank = this->m_rom.CharacterROM(0).data();
		break;
	default:
		throw std::exception();
	}
}

Mapper0::~Mapper0(void) 
{

}

uint8_t Mapper0::MapperId() 
{ 
  return 0;
}

inline const uint8_t* Mapper0::programROM(PROGRAM_ROM_BANK bank) const
{
	return this->m_selectedPROMBanks[bank];
}

inline const uint8_t* Mapper0::characterROM() const
{
	return this->m_selectedCROMBank;
}

uint8_t Mapper0::Read(uint16_t address) 
{
	switch( address )
	{
	case 0x4012 ... 0x7FFF:
		break;
	case 0x8000 ... 0xBFFF:
		return this->m_selectedPROMBanks[PROGRAM_ROM_BANK::_8000_BFFF][ address & 0x3FFF ];
	case 0xC000 ... 0xFFFF:
		return this->m_selectedPROMBanks[PROGRAM_ROM_BANK::_C000_FFFF][ address & 0x3FFF ];
	default:
		throw std::exception();
	}
	return 0;
}

void Mapper0::Write(uint16_t address, uint8_t v) 
{
  switch( address )
	{
	case 0x4012 ... 0x7FFF:
		break;
	case 0x8000 ... 0xBFFF:
		this->m_selectedPROMBanks[0][ address & 0x3FFF ] = v;
		break;
	case 0xC000 ... 0xFFFF:
		this->m_selectedPROMBanks[1][ address & 0x3FFF ] = v;
		break;
	default:
		std::cout << "Address : " << std::hex << address << std::endl;
		throw std::exception();
	}
}

void Mapper0::Tick() {}

}/* namespace mappers */

}/* namespace nes */