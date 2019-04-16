#ifndef MOS6502_HPP__
#define MOS6502_HPP__

/*=======================================================================
	6502.hpp :
	* Aleksandr Khavarovskiy
	* Washington State University - Vancouver
	======================================================================
	* Designed to be reusable for emulation of systems using the 6502.
*/
#include <stdint.h>

/*=======================================================================
    Proccess status register, done via union as opposed to bitmasks
  =======================================================================*/
typedef union _M6502_PSR
{
    struct
    {
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
/*=======================================================================
   6502 System Registers
  =======================================================================*/
typedef struct _M6502_REG
{
    PSR      PS; // Proccess status
	uint16_t PC; // Program Counter
	uint8_t   A; // Accumulator
	uint8_t   X; // X indexing register
	uint8_t   Y; // Y indexing register
	uint8_t  SP; // Stack Pointer
}
M6502_REG;
/*=======================================================================
   6502 Bus interface
  =======================================================================
    * Abstract class for polymorphic implementation of systems
  */
class M6502_BUS
{
public:
	virtual uint8_t Read ( uint16_t addr )            = 0;
	virtual void    Write( uint16_t addr, uint8_t v ) = 0;
};
/*=======================================================================
   6502 CPU Class
  =======================================================================
  */
class M6502
{
public:
	M6502( M6502_BUS * bus );
	virtual ~M6502( void );
	/*===============================================================
	   Run()
	  ===============================================================
	   - Will execute a single instruction.
	*/
	void Run( void );
private:
	M6502_BUS * m_bus;
	M6502_REG * m_reg;
};
#endif