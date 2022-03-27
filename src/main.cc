#include <iostream>
#include <vector>
#include <assert.h>
#include <fstream>
#include <stdexcept>

#include "../include/main.hpp"
#include "../include/ricoh2A0X.hpp"

std::vector<uint8_t> LoadFileIntoMemory(std::string fileName)
{
  std::ifstream   file(fileName, std::ios::binary | std::ios::ate);
  std::streamsize fileSize = file.tellg();
  file.seekg(0, std::ios::beg);

  /* Return Vector with reserve */
  std::vector<uint8_t> r(fileSize);
  if (!file.read((char*)r.data(), fileSize))
  {
    throw std::exception();
  }
  /* Did we read the whole file ?*/
  assert("Failed to load nes files" && (r.size() == fileSize));
  return r;
}

int main(const int argc, const char* argv[]) 
{
  std::vector<uint8_t> rom = LoadFileIntoMemory("./test/nestest.nes");

  std::unique_ptr<nes::Ricoh2A0X> ricoh = std::make_unique<nes::Ricoh2A0X>(rom);
  while(1) {
    DEBUG(std::cout << "[ Debug ] Address 0x" << std::hex << ricoh->Registers().PC << std::endl);
    ricoh->Step();
    sleep(1);
  }
  return 0;
}