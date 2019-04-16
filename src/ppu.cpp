#include "..//include//ppu.h"
#include "..//include//bus.h"
#include "..//include//map.h"
#include "..//include//rom.h"

#include <string.h>
#include <stdlib.h>

#define NES_RESOLUTION_WIDTH  256
#define NES_RESOLUTION_HEIGHT 240

// Bitfield utilities
template<unsigned bitno, unsigned nbits=1, typename T=u8>
struct RegBit
{
    T data;
    enum { mask = (1u << nbits) - 1u };
    template<typename T2>
    RegBit& operator=(T2 val)
    {
        data = (data & ~(mask << bitno)) | ((nbits > 1 ? val & mask : !!val) << bitno);
        return *this;
    }
    operator unsigned() const { return (data >> bitno) & mask; }
    RegBit& operator++ ()     { return *this = *this + 1; }
    unsigned operator++ (int) { unsigned r = *this; ++*this; return r; }
};

u32 NES_RBGA_PALETTE[64] =
{
	// Color pallet used for rendering NES colors in RGB
	0x7C7C7C00,0x0000FC00,0x0000BC00,0x4428BC00,0x94008400,0xA8002000,0xA8100000,0x88140000,
	0x50300000,0x00780000,0x00680000,0x00580000,0x00405800,0x00000000,0x00000000,0x00000000,
	0xBCBCBC00,0x0078F800,0x0058f800,0x6844FC00,0xD800CC00,0xE4005800,0xF8380000,0xE45C1000,
	0xAC7C0000,0x00B80000,0x00A80000,0x00A84400,0x00888800,0x00000000,0x00000000,0x00000000,
	0xF8F8F800,0x3CBCFC00,0x6888FC00,0x9878F800,0xF878F800,0xF8589800,0xF8785800,0xFCA04400,
	0xF8B80000,0xB8F81800,0x58D85400,0x58F89800,0x00E8D800,0x78787800,0x00000000,0x00000000,
	0xFCFCFC00,0xA4E4FC00,0xB8B8F800,0xD8B8F800,0xF8B8F800,0xF8A4C000,0xF0D0B000,0xFCE0A800,
	0xF8D87800,0xD8F87800,0xB8F8B800,0xB8F8D800,0x00FCFC00,0xF8D8F800,0x00000000,0x00000000
};

union regtype // PPU register file
{
	u32 value;
	// Reg0 (write)             // Reg1 (write)             // Reg2 (read)
	RegBit<0,8,u32> sysctrl;    RegBit< 8,8,u32> dispctrl;  RegBit<16,8,u32> status;
	RegBit<0,2,u32> BaseNTA;    RegBit< 8,1,u32> Grayscale; RegBit<21,1,u32> SPoverflow;
	RegBit<2,1,u32> Inc;        RegBit< 9,1,u32> ShowBG8;   RegBit<22,1,u32> SP0hit;
	RegBit<3,1,u32> SPaddr;     RegBit<10,1,u32> ShowSP8;   RegBit<23,1,u32> InVBlank;
	RegBit<4,1,u32> BGaddr;     RegBit<11,1,u32> ShowBG;    // Reg3 (write)
	RegBit<5,1,u32> SPsize;     RegBit<12,1,u32> ShowSP;    RegBit<24,8,u32> OAMaddr;
	RegBit<6,1,u32> SlaveFlag;  RegBit<11,2,u32> ShowBGSP;  RegBit<24,2,u32> OAMdata;
	RegBit<7,1,u32> NMIenabled; RegBit<13,3,u32> EmpRGB;    RegBit<26,6,u32> OAMindex;
} reg;

	// Raw memory data as read&written by the game
u8 palette[32], OAM[256];
// Decoded sprite information, used & changed during each scanline
struct { u8 sprindex, y, index, attr, x; u16 pattern; } OAM2[8], OAM3[8];

unsigned char NRAM[0x1000];
unsigned char *Nta[4] = { NRAM+0x0000, NRAM+0x0400, NRAM+0x0000, NRAM+0x0400 };

union scrolltype
{
	RegBit<3,16,u32> raw;       // raw VRAM address (16-bit)
	RegBit<0, 8,u32> xscroll;   // low 8 bits of first write to 2005
	RegBit<0, 3,u32> xfine;     // low 3 bits of first write to 2005
	RegBit<3, 5,u32> xcoarse;   // high 5 bits of first write to 2005
	RegBit<8, 5,u32> ycoarse;   // high 5 bits of second write to 2005
	RegBit<13,2,u32> basenta;   // nametable index (copied from 2000)
	RegBit<13,1,u32> basenta_h; // horizontal nametable index
	RegBit<14,1,u32> basenta_v; // vertical   nametable index
	RegBit<15,3,u32> yfine;     // low 3 bits of second write to 2005
	RegBit<11,8,u32> vaddrhi;   // first write to 2006 (with high 2 bits set to zero)
	RegBit<3, 8,u32> vaddrlo;   // second write to 2006
} scroll, vaddr;

unsigned pat_addr, sprinpos, sproutpos, sprrenpos, sprtmp;
u16 tileattr, tilepat, ioaddr;
u32 bg_shift_pat, bg_shift_attr;

int scanline=241, x=0, scanline_end=341, VBlankState=0, cycle_counter=0;
int read_buffer=0, open_bus=0, open_bus_decay_timer=0;
bool even_odd_toggle=false, offset_toggle=false;

void Init( void )
{
	reg.value = 0;
}
/* Memory mapping: Convert PPU memory address into a reference to relevant data */
u8& mmap( MOS6502* em, int i)
{
	NES_BUS * b  = (NES_BUS*)em->m_bus;

	i &= 0x3FFF;
	if(i >= 0x3F00) { if(i%4==0) i &= 0x0F; return palette[i & 0x1F]; }
	if(i < 0x2000) return b->m_rom->m_map->m_selectedCROM[ i ];
	return Nta[(i>>10)&3][i&0x3FF];
}

int RefreshOpenBus( u8 v )
{
	open_bus = v;
	return open_bus_decay_timer = 77777;
}
// auto RefreshOpenBus = [&](u8 v) { return open_bus_decay_timer = 77777, open_bus = v; };

// External I/O: read or write
u8 Access(MOS6502* em, u16 index, u8 v, bool write)
{
	u8 res = open_bus;
	if(write) RefreshOpenBus(v);
	switch(index) // Which port from $200x?
	{
		case 0: if(write) { reg.sysctrl  = v; scroll.basenta = reg.BaseNTA; } break;
		case 1: if(write) { reg.dispctrl = v; } break;
		case 2: if(write) break;
				res = reg.status | (open_bus & 0x1F);
				reg.InVBlank = false;  // Reading $2002 clears the vblank flag.
				offset_toggle = false; // Also resets the toggle for address updates.
				if(VBlankState != -5)
					VBlankState = 0; // This also may cancel the setting of InVBlank.
				break;
		case 3: if(write) reg.OAMaddr        = v; break; // Index into Object Attribute Memory
		case 4: if(write) OAM[reg.OAMaddr++] = v;        // Write or read the OAM (sprites).
				else res = RefreshOpenBus(OAM[reg.OAMaddr] & (reg.OAMdata==2 ? 0xE3 : 0xFF));
				break;
		case 5: if(!write) break; // Set background scrolling offset
			if(offset_toggle) { scroll.yfine   = v & 7; scroll.ycoarse = v >> 3; }
			else              { scroll.xscroll = v; }
			offset_toggle = !offset_toggle;
			break;
		case 6: if(!write) break; // Set video memory position for reads/writes
			if(offset_toggle) { scroll.vaddrlo = v; vaddr.raw = (unsigned) scroll.raw; }
			else              { scroll.vaddrhi = v & 0x3F; }
			offset_toggle = !offset_toggle;
			break;
		case 7:
			res = read_buffer;
			u8& t = mmap(em, vaddr.raw); // Access the video memory.
			if(write) res = t = v;
			else { if((vaddr.raw & 0x3F00) == 0x3F00) // palette?
						res = read_buffer = (open_bus & 0xC0) | (t & 0x3F);
					read_buffer = t; }
			RefreshOpenBus(res);
			vaddr.raw = vaddr.raw + (reg.Inc ? 32 : 1); // The address is automatically updated.
			break;
	}
	return res;
}

void rendering_tick( MOS6502* em )
{
	bool tile_decode_mode = 0x10FFFF & (1u << (x/16)); // When x is 0..255, 320..335

	// Each action happens in two steps: 1) select memory address; 2) receive data and react on it.
	switch(x % 8)
	{
		case 2: // Point to attribute table
			ioaddr = 0x23C0 + 0x400*vaddr.basenta + 8*(vaddr.ycoarse/4) + (vaddr.xcoarse/4);
			if(tile_decode_mode) break; // Or nametable, with sprites.
		case 0: // Point to nametable
			ioaddr = 0x2000 + (vaddr.raw & 0xFFF);
			// Reset sprite data
			if(x == 0) { sprinpos = sproutpos = 0; if(reg.ShowSP) reg.OAMaddr = 0; }
			if(!reg.ShowBG) break;
			// Reset scrolling (vertical once, horizontal each scanline)
			if(x == 304 && scanline == -1) vaddr.raw = (unsigned) scroll.raw;
			if(x == 256) { vaddr.xcoarse   = (unsigned)scroll.xcoarse;
							vaddr.basenta_h = (unsigned)scroll.basenta_h;
							sprrenpos = 0; }
			break;
		case 1:
			if(x == 337 && scanline == -1 && even_odd_toggle && reg.ShowBG) scanline_end = 340;
			// Name table access
			pat_addr = 0x1000*reg.BGaddr + 16*mmap(em ,ioaddr) + vaddr.yfine;
			if(!tile_decode_mode) break;
			// Push the current tile into shift registers.
			// The bitmap pattern is 16 bits, while the attribute is 2 bits, repeated 8 times.
			bg_shift_pat  = (bg_shift_pat  >> 16) + 0x00010000 * tilepat;
			bg_shift_attr = (bg_shift_attr >> 16) + 0x55550000 * tileattr;
			break;
		case 3:
			// Attribute table access
			if(tile_decode_mode)
			{
				tileattr = (mmap(em ,ioaddr) >> ((vaddr.xcoarse&2) + 2*(vaddr.ycoarse&2))) & 3;
				// Go to the next tile horizontally (and switch nametable if it wraps)
				if(!++vaddr.xcoarse) { vaddr.basenta_h = 1-vaddr.basenta_h; }
				// At the edge of the screen, do the same but vertically
				if(x==251 && !++vaddr.yfine && ++vaddr.ycoarse == 30)
					{ vaddr.ycoarse = 0; vaddr.basenta_v = 1-vaddr.basenta_v; }
			}
			else if(sprrenpos < sproutpos)
			{
				// Select sprite pattern instead of background pattern
				auto& o = OAM3[sprrenpos]; // Sprite to render on next scanline
				memcpy(&o, &OAM2[sprrenpos], sizeof(o));
				unsigned y = (scanline) - o.y;
				if(o.attr & 0x80) y ^= (reg.SPsize ? 15 : 7);
				pat_addr = 0x1000 * (reg.SPsize ? (o.index & 0x01) : reg.SPaddr);
				pat_addr +=  0x10 * (reg.SPsize ? (o.index & 0xFE) : (o.index & 0xFF));
				pat_addr += (y&7) + (y&8)*2;
			}
			break;
		// Pattern table bytes
		case 5:
			tilepat = mmap(em, pat_addr|0);
			break;
		case 7: // Interleave the bits of the two pattern bytes
			unsigned p = tilepat | (mmap(em ,pat_addr|8) << 8);
			p = (p&0xF00F) | ((p&0x0F00)>>4) | ((p&0x00F0)<<4);
			p = (p&0xC3C3) | ((p&0x3030)>>2) | ((p&0x0C0C)<<2);
			p = (p&0x9999) | ((p&0x4444)>>1) | ((p&0x2222)<<1);
			tilepat = p;
			// When decoding sprites, save the sprite graphics and move to next sprite
			if(!tile_decode_mode && sprrenpos < sproutpos)
				OAM3[sprrenpos++].pattern = tilepat;
			break;
	}
	// Find which sprites are visible on next scanline (TODO: implement crazy 9-sprite malfunction)
	switch(x>=64 && x<256 && x%2 ? (reg.OAMaddr++ & 3) : 4)
	{
		default:
			// Access OAM (object attribute memory)
			sprtmp = OAM[reg.OAMaddr];
			break;
		case 0:
			if(sprinpos >= 64) { reg.OAMaddr=0; break; }
			++sprinpos; // next sprite
			if(sproutpos<8) OAM2[sproutpos].y        = sprtmp;
			if(sproutpos<8) OAM2[sproutpos].sprindex = reg.OAMindex;
			{int y1 = sprtmp, y2 = sprtmp + (reg.SPsize?16:8);
			if(!( scanline >= y1 && scanline < y2 ))
				reg.OAMaddr = sprinpos != 2 ? reg.OAMaddr+3 : 8;}
			break;
		case 1:
			if(sproutpos<8) OAM2[sproutpos].index = sprtmp;
			break;
		case 2:
			if(sproutpos<8) OAM2[sproutpos].attr  = sprtmp;
			break;
		case 3:
			if(sproutpos<8) OAM2[sproutpos].x = sprtmp;
			if(sproutpos<8) ++sproutpos; else reg.SPoverflow = true;
			if(sprinpos == 2) reg.OAMaddr = 8;
			break;
	}
}

void render_pixel( MOS6502* em )
{
	NES_BUS* b = (NES_BUS*)em->m_bus;
	bool edge   = u8(x+8) < 16; // 0..7, 248..255
	bool showbg = reg.ShowBG && (!edge || reg.ShowBG8);
	bool showsp = reg.ShowSP && (!edge || reg.ShowSP8);

	// Render the background
	unsigned fx = scroll.xfine, xpos = 15 - (( (x&7) + fx + 8*!!(x&7) ) & 15);

	unsigned pixel = 0, attr = 0;
	if(showbg) // Pick a pixel from the shift registers
	{
		pixel = (bg_shift_pat  >> (xpos*2)) & 3;
		attr  = (bg_shift_attr >> (xpos*2)) & (pixel ? 3 : 0);
	}
	else if( (vaddr.raw & 0x3F00) == 0x3F00 && !reg.ShowBGSP )
		pixel = vaddr.raw;

	// Overlay the sprites
	if(showsp)
		for(unsigned sno=0; sno<sprrenpos; ++sno)
		{
			auto& s = OAM3[sno];
			// Check if this sprite is horizontally in range
			unsigned xdiff = x - s.x;
			if(xdiff >= 8) continue; // Also matches negative values
			// Determine which pixel to display; skip transparent pixels
			if(!(s.attr & 0x40)) xdiff = 7-xdiff;
			u8 spritepixel = (s.pattern >> (xdiff*2)) & 3;
			if(!spritepixel) continue;
			// Register sprite-0 hit if applicable
			if(x < 255 && pixel && s.sprindex == 0) reg.SP0hit = true;
			// Render the pixel unless behind-background placement wanted
			if(!(s.attr & 0x20) || !pixel)
			{
				attr = (s.attr & 3) + 4;
				pixel = spritepixel;
			}
			// Only process the first non-transparent sprite pixel.
			break;
		}
	pixel = palette[ (attr*4 + pixel) & 0x1F ] & (reg.Grayscale ? 0x30 : 0x3F);
	b->m_ppu->m_frameBuffer[x + scanline* 256] = NES_RBGA_PALETTE[ pixel | (reg.EmpRGB << 6) ];
//GUI::PutPixel(x, scanline, pixel | (reg.EmpRGB << 6), cycle_counter);
}

// PPU::tick() -- This function is called 3 times per each CPU cycle.
// Each call iterates through one pixel of the screen.
// The screen is divided into 262 scanlines, each having 341 columns, as such:
//
//           x=0                 x=256      x=340
//       ___|____________________|__________|
//  y=-1    | pre-render scanline| prepare  | >
//       ___|____________________| sprites _| > Graphics
//  y=0     | visible area       | for the  | > processing
//          | - this is rendered | next     | > scanlines
//  y=239   |   on the screen.   | scanline | >
//       ___|____________________|______
//  y=240   | idle
//       ___|_______________________________
//  y=241   | vertical blanking (idle)
//          | 20 scanlines long
//  y=260___|____________________|__________|
//
// On actual PPU, the scanline begins actually before x=0, with
// sync/colorburst/black/background color being rendered, and
// ends after x=256 with background/black being rendered first,
// but in this emulator we only care about the visible area.
//
// When background rendering is enabled, scanline -1 is
// 340 or 341 pixels long, alternating each frame.
// In all other situations the scanline is 341 pixels long.
// Thus, it takes 89341 or 89342 PPU::tick() calls to render 1 frame.
void Tick( MOS6502* em )
{
	NES_BUS* b = (NES_BUS*)em->m_bus;

	// Set/clear vblank where needed
	switch(VBlankState)
	{
		case -5: reg.status = 0; break;
		case 2: reg.InVBlank = true; break;
		case 0: em->m_int.NMI_TRIGGER = reg.InVBlank && reg.NMIenabled; break;
			break;
	}
	if(VBlankState != 0) VBlankState += (VBlankState < 0 ? 1 : -1);
	if(open_bus_decay_timer) if(!--open_bus_decay_timer) open_bus = 0;

	// Graphics processing scanline?
	// if(scanline == 0)
	// 	b->m_ppu->m_frameReady = false;
		//GUI::Application::ApplicationInstance()->LockTexture();
		
	if(scanline < 240)
	{
		/* Process graphics for this cycle */
		if(reg.ShowBGSP) rendering_tick( em );
		if(scanline >= 0 && x < 256) render_pixel( em );
	}

	// Done with the cycle. Check for end of scanline.
	//if(++cycle_counter == 3) cycle_counter = 0; // For NTSC pixel shifting
	if(++x >= scanline_end)
	{
		// Begin new scanline
		if( scanline == 240 ) {
			b->m_ppu->m_frameReady = true;
			//GUI::Application::ApplicationInstance()->UnlockTexture();
			//GUI::Application::ApplicationInstance()->SetFrameReady( true );
		}
		
		scanline_end = 341;
		x            = 0;
		// Does something special happen on the new scanline?
		switch(scanline += 1)
		{
			case 261: // Begin of rendering
				scanline = -1; // pre-render line
				even_odd_toggle = !even_odd_toggle;
				// Clear vblank flag
				VBlankState = -5;
				break;
			case 241: // Begin of vertical blanking
				// I cheat here: I did not bother to learn how to use SDL events,
				// so I simply read button presses from a movie file, which happens
				// to be a TAS, rather than from the keyboard or from a joystick.
				//static FILE* fp = fopen(inputfn, "rb");
				// if(fp)
				//  {
					// static unsigned ctrlmask = 0;
					// if(!ftell(fp))
					// {
						//  fseek(fp, 0x05, SEEK_SET);
					//    ctrlmask = fgetc(fp);
					//    fseek(fp, 0x90, SEEK_SET); // Famtasia Movie format.
					//   }
					//if(ctrlmask & 0x80) { IO::joy_next[0] = fgetc(fp); if(feof(fp)) IO::joy_next[0] = 0; }
					//if(ctrlmask & 0x40) { IO::joy_next[1] = fgetc(fp); if(feof(fp)) IO::joy_next[1] = 0; }
				// }
				// Set vblank flag
				VBlankState = 2;
		}
	}
}

uint8_t PPU_Read( MOS6502* em, uint16_t addr )
{
	return Access( em, addr & 7, 0, false );
}

void   PPU_Write( MOS6502* em, uint16_t addr, uint8_t v )
{
	Access( em, addr & 7, v, true );
}

void PPU_Tick( MOS6502* em )
{
	Tick( em );
}

NES_PPU* NesEx_CreatePPU( void )
{
	NES_PPU* r = (NES_PPU*)calloc( 1, sizeof( struct NES_PPU ) );

	r->Read  = PPU_Read;
	r->Write = PPU_Write;
	r->Tick  = PPU_Tick;

	r->m_frameBuffer = (uint32_t*)malloc( NES_RESOLUTION_WIDTH * NES_RESOLUTION_HEIGHT * sizeof( uint32_t ) );
	r->m_frameReady  = false;
	reg.value = 0;

	return r;
}