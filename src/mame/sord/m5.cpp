// license:BSD-3-Clause
// copyright-holders:Curt Coder, Ales Dlabac
/***************************************************************************

    Sord m.5

    http://www.retropc.net/mm/m5/
    http://www.museo8bits.es/wiki/index.php/Sord_M5 not working
    http://k5.web.klfree.net/content/view/10/11/ not working
    http://k5.web.klfree.net/images/stories/sord/m5heap.htm  not working
    http://k5.klfree.net/index.php?option=com_content&task=view&id=5&Itemid=3
    http://k5.klfree.net/index.php?option=com_content&task=view&id=10&Itemid=11
    http://k5.klfree.net/index.php?option=com_content&task=view&id=14&Itemid=3
    http://www.dlabi.cz/?s=sord
    https://www.facebook.com/groups/59667560188/
    http://www.oldcomp.cz/viewtopic.php?f=103&t=1164

****************************************************************************/

/***************************************************************************

TODO:

    - fd5 floppy
    - SI-5 serial interface (8251, ROM)
    - ramdisk for KRX Memory expansion
    - rewrite fd5 floppy as unpluggable device
    - move dipswitch declaration to softwarelist file?
    - 64krx: get windows ROM version with cpm & ramdisk support (Stuchlik S.E.I. version)

    - brno mod: make the dsk image writeable
    - brno mod: in console version lost data on RAMDISK after soft reset
    - brno mod: add support for lzr floppy disc format
    - brno mod: include basic-i



CHANGELOG:

10.02.2016
    - fixed bug: crash if rom card was only cart
    - fixed bug: when em-5 selected monitor rom wasn't paged in
    - brno mod: spin motor on upon restart
    - brno mod: windowed boot as default rom
    - brno mod: fixed bug: tape command in menu now works

05.02.2016
    - added BRNO modification - 1024kB Ramdisk + CP/M support
    - 32/64KB RAM expansions EM-5, 64KBI, 64KBF, 64KRX
    - since now own version of rom and slot handlers
    - 2 slots for carts


******************************************************************************


Controlling (paging) of homebrew 64KB RAM carts
================================================

Used ports:
EM-64, 64KBI:   OUT 6CH,00H - enables ROM
                OUT 6CH,01H - enables RAM
64KBF:          OUT 30H,00000xxxB   - enables RAM or ROM, see bellow
64KRD, 64KRX:   OUT 7FH,00000000B   - enables RAM
                OUT 7FH,11111111B   - enables ROM
                OUT 7FH,xxxxxxxxB   - enables RAM and ROM, see bellow

===========================================================================================================================

RAM/ROM modes of EM-64/64KBI cart
------------------------------------------
mode 0: 0x0000-0x6fff ROM 0x7000-0xffff RAM (it is possible to limit actual ROM size by DIP switch only to 32kb)
mode 1: 0x0000-0xffff RAM

===========================================================================================================================

RAM/ROM modes of 64KBF version 2C cart
------------------------------------------
Memory paging is done by using "OUT &30,mod".

MODE    READ                            WRITE
----------------------------------------------------------------------
 00 8 KB MON + 20 KB BF + 36 KB RAM     28 KB DIS + 36 KB RAM
 01 64 KB RAM                           64 KB RAM
 02 8 KB MON + 56 KB RAM                64 KB RAM
 03 64 KB RAM                           28 KB DIS + 36 KB RAM
 04 64 KB RAM                           16 KB DIS + 48 KB RAM
 05 8 KB MON + 20 KB BF + 36 KB RAM     64 KB RAM
 06 8 KB MON + 20 KB DIS + 36 KB RAM    64 KB RAM
 07 64 KB DIS                           64 KB DIS

Version LZR ( 2C )
================

+------------+
|////////////|  READ ONLY AREA
+------------+
|\\\\\\\\\\\\|  WRITE ONLY AREA
+------------+
|XXXXXXXXXXXX|  R&W AREA
+------------+
|            |  DISABLED R&W
+------------+

      0   0   0   1   1   2   2   2   3   3   4   4   4   5   5   6   6
kB    0   4   8   2   6   0   4   8   2   6   0   4   8   2   6   0   4
      +-------+-------------------+
ROM   |MONITOR|      BASIC-F      |
      +-------+-------+-------+---+---+-------+-------+-------+-------+
RAM   |       |       |       |       |       |       |       |       |
      +-------+-------+-------+-------+-------+-------+-------+-------+
CART  |       |       |       |       |       |       |       |       |
      +-------+-------+-------+-------+-------+-------+-------+-------+


Mode
    +-------+-------------------+
    |///////|///////////////////|
    +-------+-------+-------+---+---+-------+-------+-------+-------+
M0  |       |       |       |   |XXX|XXXXXXX|XXXXXXX|XXXXXXX|XXXXXXX|
    +-------+-------+-------+-------+-------+-------+-------+-------+

    +-------+-------------------+
    |       |                   |
    +-------+-------+-------+---+---+-------+-------+-------+-------+
M1  |XXXXXXX|XXXXXXX|XXXXXXX|XXXXXXX|XXXXXXX|XXXXXXX|XXXXXXX|XXXXXXX|
    +-------+-------+-------+-------+-------+-------+-------+-------+

    +-------+-------------------+
    |///////|                   |
    +-------+-------+-------+---+---+-------+-------+-------+-------+
M2  |\\\\\\\|XXXXXXX|XXXXXXX|XXXXXXX|XXXXXXX|XXXXXXX|XXXXXXX|XXXXXXX|
    +-------+-------+-------+-------+-------+-------+-------+-------+

    +-------+-------------------+
    |       |                   |
    +-------+-------+-------+---+---+-------+-------+-------+-------+
M3  |///////|///////|///////|///|XXX|XXXXXXX|XXXXXXX|XXXXXXX|XXXXXXX|
    +-------+-------+-------+-------+-------+-------+-------+-------+

    +-------+-------------------+
    |       |                   |
    +-------+-------+-------+---+---+-------+-------+-------+-------+
M4  |///////|///////|XXXXXXX|XXXXXXX|XXXXXXX|XXXXXXX|XXXXXXX|XXXXXXX|
    +-------+-------+-------+-------+-------+-------+-------+-------+

    +-------+-------------------+
    |///////|///////////////////|
    +-------+-------+-------+---+---+-------+-------+-------+-------+
M5  |\\\\\\\|\\\\\\\|\\\\\\\|\\\|XXX|XXXXXXX|XXXXXXX|XXXXXXX|XXXXXXX|
    +-------+-------+-------+-------+-------+-------+-------+-------+

    +-------+-------------------+
    |///////|                   |
    +-------+-------+-------+---+---+-------+-------+-------+-------+
M6  |\\\\\\\|\\\\\\\|\\\\\\\|\\\|XXX|XXXXXXX|XXXXXXX|XXXXXXX|XXXXXXX|
    +-------+-------+-------+---+---+-------+-------+-------+-------+
            |///////|///////|///|
            +-------+-------+---+

    +-------+-------------------+
    |       |                   |
    +-------+-------+-------+---+---+-------+-------+-------+-------+
M7  |       |       |       |       |       |       |       |       |
    +-------+-------+-------+-------+-------+-------+-------+-------+
    |XXXXXXX|XXXXXXX|XXXXXXX|XXXXXXX|XXXXXXX|XXXXXXX|XXXXXXX|XXXXXXX|
    +---------------------------------------------------------------+

===========================================================================================================

Memory map of ROM and RAM in configuration SORD M5 + 64 KRX memory cart
-----------------------------------------------------------------------

         cart     inside Sord   inside Sord     cart        cart
FFFF +----------+ +----------+ +----------+ +----------+ +----------+
     |          |                           |          | |          |
     |          |                           | EPROM 16K| | EPROM 16K|
     |          |                           |        5 | |        7 |
C000 |   DRAM   |                           +----------+ +----------+
     |          |                           |          | |          |
     |          |                           | EPROM 16K| | EPROM 16K|
     |          |                           |        4 | |        6 |
7FFF +----------+ +----------+              +----------+ +----------+
                  |   SRAM   |
7000 +----------+ +----------+
     |          |
6000 |          |                           +----------+
     |          |                           |          |
5000 |          |                           | EPROM 8K |
     |          |                           |        3 |
4000 |          |              +----------+ +----------+
     |          |              |          |
3000 |   DRAM   |              | EPROM 8K |
     |          |              |        2 |
2000 |          |              +----------+
     |          |              |          |
1000 |          |              | EPROM 8K |
     |          |              |        1 |
0000 +----------+ +----------+ +----------+ +----------+ +----------+

1 - MONITOR ROM
2 - WINDOWS + BASIC-F 3rd part
3 - BASIC-I
4 - 2nd part of BASIC-F + 1st part of BASIC-F
5 - 1st part of BASIC-G + 2nd part of BASIC-G
6 - 1st part of MSX 1.C
7 - 2nd part of MSX 1.C

Note: position 3 could be replaced with SRAM 8KB with battery power backup!

Upon powering up either SRAM + 1,2,3,4,5 or SRAM + 1,2,3,6,7 are selected.
Switching between 4,5 and 6,7 is provided by hw switch, selecting ROM/RAM mode happens
using OUT (7FH),A, where each bit of A means 8KB memory chunk ( state: 0=RAM,
1=ROM, bit: 0=1, 1=2, 2=3, 3=always SRAM, 4=4, 5=5, 6=6, 7=7 ).


*/


#include "emu.h"

#include "cpu/z80/z80.h"
#include "machine/i8251.h"
#include "machine/i8255.h"
#include "machine/ram.h"
#include "machine/upd765.h"
#include "machine/wd_fdc.h" //brno mod
#include "machine/z80daisy.h"
#include "machine/z80ctc.h"
#include "sound/sn76496.h"
#include "video/tms9928a.h"

#include "bus/rs232/rs232.h"
#include "bus/centronics/ctronics.h"
#include "bus/m5/rom.h"
#include "bus/m5/slot.h"

#include "imagedev/cassette.h"
#include "imagedev/floppy.h"
#include "imagedev/snapquik.h"

#include "softlist_dev.h"
#include "speaker.h"

#include "formats/m5_dsk.h"
#include "formats/sord_cas.h"


class m5_state : public driver_device
{
public:
	m5_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag)
		, m_maincpu(*this, "maincpu")
		, m_ctc(*this, "ctc")
		, m_fd5cpu(*this, "z80fd5")
		, m_ppi(*this, "ppi")
		, m_fdc(*this, "upd765")
		, m_floppy0(*this, "upd765:0:525dd")
		, m_cassette(*this, "cassette")
		, m_cart1(*this, "cartslot1")
		, m_cart2(*this, "cartslot2")
		, m_centronics(*this, "centronics")
		, m_ram(*this, RAM_TAG)
		, m_reset(*this, "RESET")
		, m_DIPS(*this, "DIPS")
		, m_centronics_busy(0)
	{ }

	void m5(machine_config &config);
	void pal(machine_config &config);
	void ntsc(machine_config &config);

protected:
	required_device<z80_device> m_maincpu;
	required_device<z80ctc_device> m_ctc;
	optional_device<cpu_device> m_fd5cpu;
	optional_device<i8255_device> m_ppi;
	optional_device<upd765a_device> m_fdc;
	optional_device<floppy_image_device> m_floppy0;
	required_device<cassette_image_device> m_cassette;
	optional_device<m5_cart_slot_device> m_cart1;
	optional_device<m5_cart_slot_device> m_cart2;
	required_device<centronics_device> m_centronics;
	optional_device<ram_device> m_ram;
	required_ioport m_reset;
	optional_ioport m_DIPS;
	m5_cart_slot_device *m_cart_ram, *m_cart;

	u8 sts_r();
	void com_w(u8 data);

	virtual void machine_start() override;
	virtual void machine_reset() override;
private:
	u8 ppi_pa_r();
	void ppi_pa_w(u8 data);
	void ppi_pb_w(u8 data);
	u8 ppi_pc_r();
	void ppi_pc_w(u8 data);

	u8 fd5_data_r();
	void fd5_data_w(u8 data);
	u8 fd5_com_r();
	void fd5_com_w(u8 data);
	void fd5_ctrl_w(u8 data);
	void fd5_tc_w(u8 data);

	static void floppy_formats(format_registration &fr);

	DECLARE_WRITE_LINE_MEMBER(write_centronics_busy);

	void fd5_io(address_map &map);
	void fd5_mem(address_map &map);
	void m5_io(address_map &map);
	void m5_mem(address_map &map);

	// video state
//  const TMS9928a_interface *m_vdp_intf;

protected:
	bool m_centronics_busy;

private:
	u8 m_ram_mode;
	//u8 m_ram_type;
	//memory_region *m_cart_rom;

	// floppy state for fd5
	u8 m_fd5_data;
	u8 m_fd5_com;
	int m_intra;
	int m_ibfa;
	int m_obfa;
};




//**************************************************************************
//  MEMORY BANKING
//**************************************************************************

WRITE_LINE_MEMBER( m5_state::write_centronics_busy )
{
	m_centronics_busy = state;
}

//-------------------------------------------------
//  sts_r -
//-------------------------------------------------

u8 m5_state::sts_r()
{
	/*

	    bit     description

	    0       cassette input
	    1       busy
	    2
	    3
	    4
	    5
	    6
	    7       RESET key

	*/

	u8 data = 0;

	// cassette input
	data |= m_cassette->input() >= 0 ? 1 : 0;

	// centronics busy
	if (m_centronics_busy)
		data |= 2;

	// RESET key
	data |= m_reset->read();

	return data;
}


//-------------------------------------------------
//  com_w -
//-------------------------------------------------

void m5_state::com_w(u8 data)
{
	/*

	    bit     description

	    0       cassette output, centronics strobe
	    1       cassette remote
	    2
	    3
	    4
	    5
	    6
	    7

	*/

	// cassette output
	m_cassette->output( BIT(data, 0) ? -1.0 : 1.0);

	// centronics strobe
	m_centronics->write_strobe(BIT(data, 0));

	// cassette remote
	m_cassette->change_state(BIT(data,1) ? CASSETTE_MOTOR_ENABLED : CASSETTE_MOTOR_DISABLED, CASSETTE_MASK_MOTOR);
}



//**************************************************************************
//  FD-5
//**************************************************************************

//-------------------------------------------------
//  fd5_data_r -
//-------------------------------------------------

u8 m5_state::fd5_data_r()
{
	m_ppi->pc6_w(0);

	return m_fd5_data;
}


//-------------------------------------------------
//  fd5_data_w -
//-------------------------------------------------

void m5_state::fd5_data_w(u8 data)
{
	m_fd5_data = data;

	m_ppi->pc4_w(0);
}


//-------------------------------------------------
//  fd5_com_r -
//-------------------------------------------------

u8 m5_state::fd5_com_r()
{
	/*

	    bit     description

	    0       ?
	    1       1?
	    2       IBFA?
	    3       OBFA?
	    4
	    5
	    6
	    7

	*/

	return m_obfa << 3 | m_ibfa << 2 | 0x02;
}


//-------------------------------------------------
//  fd5_com_w -
//-------------------------------------------------

void m5_state::fd5_com_w(u8 data)
{
	/*

	    bit     description

	    0       PPI PC2
	    1       PPI PC0
	    2       PPI PC1
	    3
	    4
	    5
	    6
	    7

	*/

	m_fd5_com = data;
}


//-------------------------------------------------
//  fd5_com_w -
//-------------------------------------------------

void m5_state::fd5_ctrl_w(u8 data)
{
	/*

	    bit     description

	    0
	    1
	    2
	    3
	    4
	    5
	    6
	    7

	*/

	m_floppy0->mon_w(!BIT(data, 0));
}


//-------------------------------------------------
//  fd5_com_w -
//-------------------------------------------------

void m5_state::fd5_tc_w(u8 data)
{
	m_fdc->tc_w(true);
	m_fdc->tc_w(false);
}

//**************************************************************************
//  ADDRESS MAPS
//**************************************************************************

//-------------------------------------------------
//  ADDRESS_MAP( m5_mem )
//-------------------------------------------------

void m5_state::m5_mem(address_map &map)
{
	map.unmap_value_high();
	map(0x0000, 0x1fff).rom().region("maincpu", 0); //monitor rom(bios)
	map(0x7000, 0xffff).ram();
}


//-------------------------------------------------
//  ADDRESS_MAP( m5_io )
//-------------------------------------------------

void m5_state::m5_io(address_map &map)
{
	map.unmap_value_high();
	map.global_mask(0xff);
	map(0x00, 0x03).mirror(0x0c).rw(m_ctc, FUNC(z80ctc_device::read), FUNC(z80ctc_device::write));
	map(0x10, 0x11).mirror(0x0e).rw("tms9928a", FUNC(tms9928a_device::read), FUNC(tms9928a_device::write));
	map(0x20, 0x20).mirror(0x0f).w("sgc", FUNC(sn76489a_device::write));
	map(0x30, 0x30).mirror(0x08).portr("Y0"); // 64KBF paging
	map(0x31, 0x31).mirror(0x08).portr("Y1");
	map(0x32, 0x32).mirror(0x08).portr("Y2");
	map(0x33, 0x33).mirror(0x08).portr("Y3");
	map(0x34, 0x34).mirror(0x08).portr("Y4");
	map(0x35, 0x35).mirror(0x08).portr("Y5");
	map(0x36, 0x36).mirror(0x08).portr("Y6");
	map(0x37, 0x37).mirror(0x08).portr("JOY");
	map(0x40, 0x40).mirror(0x0f).w("cent_data_out", FUNC(output_latch_device::write));
	map(0x50, 0x50).mirror(0x0f).rw(FUNC(m5_state::sts_r), FUNC(m5_state::com_w));
//  map(0x60, 0x63) SIO
//	map(0x70, 0x73).rw(m_ppi, FUNC(i8255_device::read), FUNC(i8255_device::write));
}


//-------------------------------------------------
//  ADDRESS_MAP( fd5_mem )
//-------------------------------------------------

void m5_state::fd5_mem(address_map &map)
{
	map(0x0000, 0x3fff).rom();
	map(0x4000, 0xffff).ram();
}


//-------------------------------------------------
//  ADDRESS_MAP( fd5_io )
//-------------------------------------------------

void m5_state::fd5_io(address_map &map)
{
	map.global_mask(0xff);
	map(0x00, 0x01).m(m_fdc, FUNC(upd765a_device::map));
	map(0x10, 0x10).rw(FUNC(m5_state::fd5_data_r), FUNC(m5_state::fd5_data_w));
	map(0x20, 0x20).w(FUNC(m5_state::fd5_com_w));
	map(0x30, 0x30).r(FUNC(m5_state::fd5_com_r));
	map(0x40, 0x40).w(FUNC(m5_state::fd5_ctrl_w));
	map(0x50, 0x50).w(FUNC(m5_state::fd5_tc_w));
}



//**************************************************************************
//  INPUT PORTS
//**************************************************************************

//-------------------------------------------------
//  INPUT_PORTS( m5 )
//-------------------------------------------------

static INPUT_PORTS_START( m5 )
	PORT_START("Y0")
	PORT_BIT(0x01, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_NAME("Ctrl") PORT_CODE(KEYCODE_LCONTROL) PORT_CHAR(UCHAR_SHIFT_2)
	PORT_BIT(0x02, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_NAME("Func") PORT_CODE(KEYCODE_TAB) PORT_CHAR(UCHAR_MAMEKEY(F1))
	PORT_BIT(0x04, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_LSHIFT)      PORT_CHAR(UCHAR_SHIFT_1)
	PORT_BIT(0x08, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_RSHIFT)      PORT_CHAR(UCHAR_MAMEKEY(RSHIFT))
	PORT_BIT(0x10, IP_ACTIVE_HIGH, IPT_UNUSED)
	PORT_BIT(0x20, IP_ACTIVE_HIGH, IPT_UNUSED)
	PORT_BIT(0x40, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_SPACE)       PORT_CHAR(' ')
	PORT_BIT(0x80, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_ENTER)       PORT_CHAR(13)

	PORT_START("Y1")
	PORT_BIT(0x01, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_1)           PORT_CHAR('1') PORT_CHAR('!')
	PORT_BIT(0x02, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_2)           PORT_CHAR('2') PORT_CHAR('"')
	PORT_BIT(0x04, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_3)           PORT_CHAR('3') PORT_CHAR('#')
	PORT_BIT(0x08, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_4)           PORT_CHAR('4') PORT_CHAR('$')
	PORT_BIT(0x10, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_5)           PORT_CHAR('5') PORT_CHAR('%')
	PORT_BIT(0x20, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_6)           PORT_CHAR('6') PORT_CHAR('&')
	PORT_BIT(0x40, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_7)           PORT_CHAR('7') PORT_CHAR('\'')
	PORT_BIT(0x80, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_8)           PORT_CHAR('8') PORT_CHAR('(')

	PORT_START("Y2")
	PORT_BIT(0x01, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_Q)           PORT_CHAR('q') PORT_CHAR('Q')
	PORT_BIT(0x02, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_W)           PORT_CHAR('w') PORT_CHAR('W')
	PORT_BIT(0x04, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_E)           PORT_CHAR('e') PORT_CHAR('E')
	PORT_BIT(0x08, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_R)           PORT_CHAR('r') PORT_CHAR('R')
	PORT_BIT(0x10, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_T)           PORT_CHAR('t') PORT_CHAR('T')
	PORT_BIT(0x20, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_Y)           PORT_CHAR('y') PORT_CHAR('Y')
	PORT_BIT(0x40, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_U)           PORT_CHAR('u') PORT_CHAR('U')
	PORT_BIT(0x80, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_I)           PORT_CHAR('i') PORT_CHAR('I')

	PORT_START("Y3")
	PORT_BIT(0x01, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_A)           PORT_CHAR('a') PORT_CHAR('A')
	PORT_BIT(0x02, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_S)           PORT_CHAR('s') PORT_CHAR('S')
	PORT_BIT(0x04, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_D)           PORT_CHAR('d') PORT_CHAR('D')
	PORT_BIT(0x08, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_F)           PORT_CHAR('f') PORT_CHAR('F')
	PORT_BIT(0x10, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_G)           PORT_CHAR('g') PORT_CHAR('G')
	PORT_BIT(0x20, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_H)           PORT_CHAR('h') PORT_CHAR('H')
	PORT_BIT(0x40, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_J)           PORT_CHAR('j') PORT_CHAR('J')
	PORT_BIT(0x80, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_K)           PORT_CHAR('k') PORT_CHAR('K')

	PORT_START("Y4")
	PORT_BIT(0x01, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_Z)           PORT_CHAR('z') PORT_CHAR('Z')
	PORT_BIT(0x02, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_X)           PORT_CHAR('x') PORT_CHAR('X')
	PORT_BIT(0x04, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_C)           PORT_CHAR('c') PORT_CHAR('C')
	PORT_BIT(0x08, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_V)           PORT_CHAR('v') PORT_CHAR('V')
	PORT_BIT(0x10, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_B)           PORT_CHAR('b') PORT_CHAR('B')
	PORT_BIT(0x20, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_N)           PORT_CHAR('n') PORT_CHAR('N')
	PORT_BIT(0x40, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_M)           PORT_CHAR('m') PORT_CHAR('M')
	PORT_BIT(0x80, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_COMMA)       PORT_CHAR(',') PORT_CHAR('<')

	PORT_START("Y5")
	PORT_BIT(0x01, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_9)           PORT_CHAR('9') PORT_CHAR(')')
	PORT_BIT(0x02, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_0)           PORT_CHAR('0')
	PORT_BIT(0x04, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_MINUS)       PORT_CHAR('-') PORT_CHAR('=')
	PORT_BIT(0x08, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_EQUALS)  PORT_CHAR('^') PORT_CHAR('~')
	PORT_BIT(0x10, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_STOP)        PORT_CHAR('.') PORT_CHAR('>')
	PORT_BIT(0x20, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_SLASH)       PORT_CHAR('/') PORT_CHAR('?') PORT_CHAR(UCHAR_MAMEKEY(DOWN))
	PORT_BIT(0x40, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_NAME("_  Triangle") PORT_CODE(KEYCODE_TILDE) PORT_CHAR('_')
	PORT_BIT(0x80, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_BACKSPACE)   PORT_CHAR('\\') PORT_CHAR('|') // backslash ok for m5p, shows as ¥ on m5.

	PORT_START("Y6")
	PORT_BIT(0x01, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_O)           PORT_CHAR('o') PORT_CHAR('O')
	PORT_BIT(0x02, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_P)           PORT_CHAR('p') PORT_CHAR('P')
	PORT_BIT(0x04, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_OPENBRACE)   PORT_CHAR('@') PORT_CHAR('`') PORT_CHAR(UCHAR_MAMEKEY(UP))
	PORT_BIT(0x08, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_CLOSEBRACE)  PORT_CHAR('[') PORT_CHAR('{')
	PORT_BIT(0x10, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_L)           PORT_CHAR('l') PORT_CHAR('L')
	PORT_BIT(0x20, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_COLON)       PORT_CHAR(';') PORT_CHAR('+') PORT_CHAR(UCHAR_MAMEKEY(LEFT))
	PORT_BIT(0x40, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_QUOTE)       PORT_CHAR(':') PORT_CHAR('*') PORT_CHAR(UCHAR_MAMEKEY(RIGHT))
	PORT_BIT(0x80, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_BACKSLASH)   PORT_CHAR(']') PORT_CHAR('}')

	PORT_START("JOY")
	PORT_BIT(0x01, IP_ACTIVE_HIGH, IPT_JOYSTICK_RIGHT) PORT_PLAYER(1)
	PORT_BIT(0x02, IP_ACTIVE_HIGH, IPT_JOYSTICK_UP)    PORT_PLAYER(1)
	PORT_BIT(0x04, IP_ACTIVE_HIGH, IPT_JOYSTICK_LEFT)  PORT_PLAYER(1)
	PORT_BIT(0x08, IP_ACTIVE_HIGH, IPT_JOYSTICK_DOWN)  PORT_PLAYER(1)
	PORT_BIT(0x10, IP_ACTIVE_HIGH, IPT_JOYSTICK_RIGHT) PORT_PLAYER(2)
	PORT_BIT(0x20, IP_ACTIVE_HIGH, IPT_JOYSTICK_UP)    PORT_PLAYER(2)
	PORT_BIT(0x40, IP_ACTIVE_HIGH, IPT_JOYSTICK_LEFT)  PORT_PLAYER(2)
	PORT_BIT(0x80, IP_ACTIVE_HIGH, IPT_JOYSTICK_DOWN)  PORT_PLAYER(2)

	PORT_START("RESET")
	PORT_BIT(0x80, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_NAME("Reset") PORT_CODE(KEYCODE_ESC) PORT_CHAR(UCHAR_MAMEKEY(ESC)) /* 1st line, 1st key from right! */

	PORT_START("DIPS")
	PORT_DIPNAME(0x01, 0x01, "KRX: BASIC[on]/MSX[off]") //switching between BASIC and MSX ROMs which share same address area
	PORT_DIPSETTING(   0x00, DEF_STR( Off ))
	PORT_DIPSETTING(   0x01, DEF_STR( On ))
	PORT_DIPNAME(0x02, 0x00, "KBI: AUTOSTART")  //pages out cart and starts loading from tape
	PORT_DIPSETTING(   0x00, DEF_STR( Off ))
	PORT_DIPSETTING(   0x02, DEF_STR( On ))
	PORT_DIPNAME(0x04, 0x00, "KBI: 32kb only") //compatible with em-5
	PORT_DIPSETTING(   0x00, DEF_STR( Off ))
	PORT_DIPSETTING(   0x04, DEF_STR( On ))
INPUT_PORTS_END

//-------------------------------------------------
//  I8255 Interface
//-------------------------------------------------

u8 m5_state::ppi_pa_r()
{
	return m_fd5_data;
}

u8 m5_state::ppi_pc_r()
{
	/*

	    bit     description

	    0       ?
	    1       ?
	    2       ?
	    3
	    4       STBA
	    5
	    6       ACKA
	    7

	*/

	return (
			/* FD5 bit 0-> M5 bit 2 */
			((m_fd5_com & 0x01)<<2) |
			/* FD5 bit 2-> M5 bit 1 */
			((m_fd5_com & 0x04)>>1) |
			/* FD5 bit 1-> M5 bit 0 */
			((m_fd5_com & 0x02)>>1)
	);
}

void m5_state::ppi_pa_w(u8 data)
{
	m_fd5_data = data;
}

void m5_state::ppi_pb_w(u8 data)
{
	/*

	    bit     description

	    0
	    1
	    2
	    3
	    4
	    5
	    6
	    7

	*/

	if (data == 0xf0)
	{
		m_fd5cpu->set_input_line(INPUT_LINE_RESET, ASSERT_LINE);
		m_fd5cpu->set_input_line(INPUT_LINE_RESET, CLEAR_LINE);
	}
}

void m5_state::ppi_pc_w(u8 data)
{
	/*

	    bit     description

	    0
	    1
	    2
	    3       INTRA
	    4
	    5       IBFA
	    6
	    7       OBFA

	*/

	m_intra = BIT(data, 3);
	m_ibfa = BIT(data, 5);
	m_obfa = BIT(data, 7);
}

//-------------------------------------------------
//  upd765_interface fdc_intf
//-------------------------------------------------

void m5_state::floppy_formats(format_registration &fr)
{
	fr.add_mfm_containers();
	fr.add(FLOPPY_M5_FORMAT);
}

static void m5_floppies(device_slot_interface &device)
{
	device.option_add("525dd", FLOPPY_525_DD);
}

static void m5_cart(device_slot_interface &device)
{
	device.option_add_internal("std",  M5_ROM_STD);
	device.option_add_internal("ram",  M5_ROM_RAM);
}

//-------------------------------------------------
//  z80_daisy_config m5_daisy_chain
//-------------------------------------------------

static const z80_daisy_config m5_daisy_chain[] =
{
	{ "ctc" },
	{ nullptr }
};

//**************************************************************************
//  MACHINE INITIALIZATION
//**************************************************************************

//-------------------------------------------------
//  MACHINE_START( m5 )
//-------------------------------------------------
void m5_state::machine_start()
{
	m_cart_ram = nullptr;
	m_cart = nullptr;

	// register for state saving
	save_item(NAME(m_fd5_data));
	save_item(NAME(m_fd5_com));
	save_item(NAME(m_intra));
	save_item(NAME(m_ibfa));
	save_item(NAME(m_obfa));
	save_item(NAME(m_centronics_busy));
}

void m5_state::machine_reset()
{
	address_space &program = m_maincpu->space(AS_PROGRAM);
	std::string region_tag;

	//is ram/rom cart plugged in?
	if (m_cart1->exists())
	{
		if (m_cart1->get_type() > 0)
			m_cart_ram = m_cart1;
		else
			m_cart = m_cart1;
	}

	if (m_cart2->exists())
	{
		if (m_cart2->get_type() > 0)
			m_cart_ram = m_cart2;
		else
			m_cart = m_cart2;
	}

	// no cart inserted - there is nothing to do - not allowed in original Sord m5
	if (m_cart_ram == nullptr && m_cart == nullptr)
	{
		program.unmap_write(0x0000, 0x1fff);
		return;
	}

	//ram cart wasn't found so if rom cart present install it
	if (m_cart)
	{
		program.install_rom(0x0000, 0x1fff, memregion("maincpu")->base());
		program.unmap_write(0x0000, 0x1fff);
		program.install_read_handler(0x2000, 0x6fff, read8sm_delegate(*m_cart, FUNC(m5_cart_slot_device::read_rom)));
		program.unmap_write(0x2000, 0x6fff);
	}

	m_ram_mode=0;
}

//**************************************************************************
//  MACHINE CONFIGURATION
//**************************************************************************

//-------------------------------------------------
//  machine_config( m5 )
//-------------------------------------------------

void m5_state::m5(machine_config &config)
{
	// basic machine hardware
	Z80(config, m_maincpu, 10.738635_MHz_XTAL / 3);
	m_maincpu->set_addrmap(AS_PROGRAM, &m5_state::m5_mem);
	m_maincpu->set_addrmap(AS_IO, &m5_state::m5_io);
	m_maincpu->set_daisy_config(m5_daisy_chain);

	Z80(config, m_fd5cpu, 16_MHz_XTAL / 4);
	m_fd5cpu->set_addrmap(AS_PROGRAM, &m5_state::fd5_mem);
	m_fd5cpu->set_addrmap(AS_IO, &m5_state::fd5_io);

	// sound hardware
	SPEAKER(config, "mono").front_center();
	SN76489A(config, "sgc", 10.738635_MHz_XTAL / 3).add_route(ALL_OUTPUTS, "mono", 1.00);

	// devices
	Z80CTC(config, m_ctc, 10.738635_MHz_XTAL / 3);
	m_ctc->intr_callback().set_inputline(m_maincpu, INPUT_LINE_IRQ0);
	m_ctc->set_clk<2>(10.738635_MHz_XTAL / 24);
	// CK0 = EXINT, CK1 = GND, CK2 = TCK, CK3 = VDP INT
	// ZC2 = EXCLK

	CENTRONICS(config, m_centronics, centronics_devices, "printer");
	m_centronics->busy_handler().set(FUNC(m5_state::write_centronics_busy));

	output_latch_device &cent_data_out(OUTPUT_LATCH(config, "cent_data_out"));
	m_centronics->set_output_latch(cent_data_out);

	CASSETTE(config, m_cassette);
	m_cassette->set_formats(sordm5_cassette_formats);
	m_cassette->set_default_state(CASSETTE_PLAY);
	m_cassette->add_route(ALL_OUTPUTS, "mono", 0.05);
	m_cassette->set_interface("m5_cass");

	I8255(config, m_ppi);
	m_ppi->in_pa_callback().set(FUNC(m5_state::ppi_pa_r));
	m_ppi->out_pa_callback().set(FUNC(m5_state::ppi_pa_w));
	m_ppi->out_pb_callback().set(FUNC(m5_state::ppi_pb_w));
	m_ppi->in_pc_callback().set(FUNC(m5_state::ppi_pc_r));
	m_ppi->out_pc_callback().set(FUNC(m5_state::ppi_pc_w));

	UPD765A(config, m_fdc, 16_MHz_XTAL / 4, true, true); // clocked by SED9420C
	m_fdc->intrq_wr_callback().set_inputline(m_fd5cpu, INPUT_LINE_IRQ0);
	FLOPPY_CONNECTOR(config, "upd765:0", m5_floppies, "525dd", m5_state::floppy_formats);

	// cartridge
	M5_CART_SLOT(config, m_cart1, m5_cart, nullptr);
	M5_CART_SLOT(config, m_cart2, m5_cart, nullptr);

	// software lists
	SOFTWARE_LIST(config, "cart_list").set_original("m5_cart");
	SOFTWARE_LIST(config, "cass_list").set_original("m5_cass");
	//SOFTWARE_LIST(config, "flop_list").set_original("m5_flop");

	// internal ram
	//68K is not possible, 'cos internal ram always overlays any expansion memory in that area
	//RAM(config, m_ram).set_default_size("4K").set_extra_options("36K,64K");
}


//-------------------------------------------------
//  machine_config( ntsc )
//-------------------------------------------------

void m5_state::ntsc(machine_config &config)
{
	m5(config);
	// video hardware
	tms9928a_device &vdp(TMS9928A(config, "tms9928a", 10.738635_MHz_XTAL));
	vdp.set_screen("screen");
	vdp.set_vram_size(0x4000);
	vdp.int_callback().set(m_ctc, FUNC(z80ctc_device::trg3)).invert();
	SCREEN(config, "screen", SCREEN_TYPE_RASTER);
}


//-------------------------------------------------
//  machine_config( pal )
//-------------------------------------------------

void m5_state::pal(machine_config &config)
{
	m5(config);
	// video hardware
	tms9929a_device &vdp(TMS9929A(config, "tms9928a", 10.738635_MHz_XTAL));
	vdp.set_screen("screen");
	vdp.set_vram_size(0x4000);
	vdp.int_callback().set(m_ctc, FUNC(z80ctc_device::trg3)).invert();
	SCREEN(config, "screen", SCREEN_TYPE_RASTER);
}

//**************************************************************************
//  ROMS
//**************************************************************************

//-------------------------------------------------
//  ROM( m5 )
//-------------------------------------------------

ROM_START( m5 )
	ROM_REGION( 0x7000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD( "sordjap.ic21", 0x0000, 0x2000, CRC(92cf9353) SHA1(b0a4b3658fde68cb1f344dfb095bac16a78e9b3e) )

	ROM_REGION( 0x4000, "z80fd5", 0 )
	ROM_LOAD( "sordfd5.rom", 0x0000, 0x4000, CRC(7263bbc5) SHA1(b729500d3d2b2e807d384d44b76ea5ad23996f4a))
ROM_END


//-------------------------------------------------
//  ROM( m5p )
//-------------------------------------------------

ROM_START( m5p )
	ROM_REGION( 0x7000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD( "sordint.ic21", 0x0000, 0x2000, CRC(78848d39) SHA1(ac042c4ae8272ad6abe09ae83492ef9a0026d0b2) )

	ROM_REGION( 0x4000, "z80fd5", 0 )
	ROM_LOAD( "sordfd5.rom", 0x0000, 0x4000, CRC(7263bbc5) SHA1(b729500d3d2b2e807d384d44b76ea5ad23996f4a))
ROM_END

//**************************************************************************
//  SYSTEM DRIVERS
//**************************************************************************

//    YEAR  NAME      PARENT  COMPAT  MACHINE  INPUT  CLASS       INIT        COMPANY  FULLNAME                 FLAGS
COMP( 1983, m5,       0,      0,      ntsc,    m5,    m5_state,   empty_init, "Sord",  "m.5 (Japan)",             0 )
COMP( 1983, m5p,      m5,     0,      pal,     m5,    m5_state,   empty_init, "Sord",  "m.5 (Europe)",            0 )
