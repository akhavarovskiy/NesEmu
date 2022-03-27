#include <iostream>
#include <assert.h>
#include <memory>

#include "../include/main.hpp"
#include "../include/rom.hpp"

namespace nes { 

/* Copy Constructor */
Rom::Rom(const Rom & rom) : m_header(std::make_unique<struct RomHeader>())
{
  std::memcpy(this->m_header.get(), rom.m_header.get(), sizeof(struct RomHeader));
  this->m_trainerROM  = rom.m_trainerROM;
  this->m_programROMs = rom.m_programROMs;
  this->m_characterROMs = rom.m_characterROMs;
}

Rom::Rom(std::vector<uint8_t> & fileContents) : 
  m_header(std::make_unique<struct RomHeader>()), 
  m_trainerROM(NES_HEADER_SIZE), 
  m_programROMs(), 
  m_characterROMs()
{
  std::vector<uint8_t>::iterator it = fileContents.begin();
  assert("File is not big enough to containe iNes Header" && fileContents.size() > NES_HEADER_SIZE);
  /* Code for reading into the header */
  std::memcpy(this->m_header.get(), it.base(), NES_HEADER_SIZE);
  it += NES_HEADER_SIZE;

	if( m_header->m_nes[0] != 'N' || 
      m_header->m_nes[1] != 'E' ||
      m_header->m_nes[2] != 'S' ||  
      m_header->m_nes[3] != 0x1A ) 
  { 
    assert("Invalid NES header format" && false);
  }
  DEBUG(std::cout << "** Printing Format Header **\n");

  /* Print some debug metadata */
  DEBUG(std::cout << "ROM MapperID: " << (m_header->m_lowerMapperNybble | (uint8_t)(m_header->m_upperMapperNybble << 4)) << "\n");
  DEBUG(std::cout << "PROM " << m_header->m_PROMBankCount  << " * 16kb\n");
  DEBUG(std::cout << "CROM " << m_header->m_CROMBankCount  << " * 8kb\n");
  DEBUG(std::cout << "PRAM " << m_header->m_pramSize       << " * 8kb\n");
  DEBUG(std::cout << "BB : " << (m_header->m_batteryBacked ? "true" : "false") << "\n");

  /* Load the trainer data if exists */
  if( m_header->m_trainer )
  {
    assert("File is not big enough to contain iNes Trainer Data" && (int)(fileContents.end() - it) >= NES_TRAINER_SIZE);
    /* Code for reading into the header */
    std::memcpy(m_trainerROM.data(), it.base(), NES_TRAINER_SIZE);
    it += NES_TRAINER_SIZE;
	}
  /* Resize the vector to be the number of banks declared in the header */
  this->m_programROMs.resize(m_header->m_PROMBankCount);
  /* Parse the rom data from the file data */
  for( auto & rom : this->m_programROMs ) 
	{
    rom.resize(NES_PROM_BANK_SIZE);
    assert("File is not big enough to contain the necessary ROM data" && (int)(fileContents.end() - it) >= NES_PROM_BANK_SIZE);
    std::memcpy(rom.data(), it.base(), NES_PROM_BANK_SIZE);
    it += NES_PROM_BANK_SIZE;
	}
  /* Resize the vector to be the number of banks declared in the header */
  this->m_characterROMs.resize(m_header->m_CROMBankCount);
  /* Parse the rom data from the file data */
  for( auto & rom : this->m_characterROMs ) 
	{
    rom.resize(NES_CROM_BANK_SIZE);
    assert("File is not big enough to contain the necessary CROM data" && (int)(fileContents.end() - it) >= NES_CROM_BANK_SIZE);
    std::memcpy(rom.data(), it.base(), NES_CROM_BANK_SIZE);
    it += NES_CROM_BANK_SIZE;
	}
}

Rom::~Rom(void)
{

}

[[nodiscard]]
struct RomHeader Rom::Header(void) noexcept
{
  return *this->m_header.get();
}

[[nodiscard]]
std::vector<uint8_t> & Rom::TrainerROM() noexcept 
{
  return this->m_trainerROM;
}

[[nodiscard]]
std::vector<uint8_t> & Rom::ProgramROM(uint16_t n) 
{
  return this->m_programROMs.at(n);
}

[[nodiscard]]
std::vector<uint8_t> & Rom::CharacterROM(uint16_t n)
{
  return this->m_characterROMs.at(n);
}

} /* namespace nes */