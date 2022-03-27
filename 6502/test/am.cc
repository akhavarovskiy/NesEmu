#include <iostream>
#include <cstring>
#include <stdint.h>

#include <6502.hpp>
#include <6502def.hpp>

#include <gtest/gtest.h>
#include <gtest/gtest_prod.h>

using namespace MOS6502;

TEST(ADDRESSING_MODE, IMMEDIATE) {
    TestCPU cpu;
    /************************************************************************
     * Setup
     ************************************************************************/
    cpu.SetBootAddress(0x100);
    cpu.Reset();
    cpu.SetTicks(0);
    cpu.SetMemory(0x100, 0x42);
    /************************************************************************
     * Verify that Immediate mode will read the current address at the PC 
     ************************************************************************/
    EXPECT_EQ(cpu.IMMEDIATE(), 0x100);
    EXPECT_EQ(cpu.GetTicks(), 0);
}

TEST(ADDRESSING_MODE, ZEROPAGE) {
    TestCPU cpu;
    /************************************************************************
     * Setup
     ************************************************************************/
    cpu.SetBootAddress(0x100);
    cpu.Reset();
    cpu.SetTicks(0);
    cpu.SetMemory(0x100, 0x42);
    /************************************************************************
     * Verify that we read from the zeropage
     ************************************************************************/
    EXPECT_EQ(cpu.ZEROPAGE(), 0x42);
    EXPECT_EQ(cpu.GetTicks(), 1);
}

TEST(ADDRESSING_MODE, ZEROPAGE_X) {
    TestCPU cpu;
    REG & r = cpu.Register();
    /************************************************************************
     * Setup
     ************************************************************************/
    cpu.SetBootAddress(0x100);
    cpu.Reset();
    cpu.SetTicks(0);
    cpu.SetMemory(0x100, 0x42);
    r.X = 0x42;
    /************************************************************************
     * Verify that we read from the zeropage
     ************************************************************************/
    EXPECT_EQ(cpu.ZEROPAGE_X(), 0x84);
    EXPECT_EQ(cpu.GetTicks(), 2);

    /************************************************************************
     * Setup
     ************************************************************************/
    cpu.Reset();
    cpu.SetTicks(0);
    cpu.SetMemory(0x100, 0xFF);
    r.X = 0x02;
    /************************************************************************
     * Verify that we read from the zeropage, with overflow
     ************************************************************************
        * If the zero page address is 0x02 and the X register is 0xFF
        * then their sum is 0x101. In this case the expected behaviour is
        * to wrap around to 0x01.
     */
    EXPECT_EQ(cpu.ZEROPAGE_X(), 0x01);
    EXPECT_EQ(cpu.GetTicks(), 2);
}

TEST(ADDRESSING_MODE, ZEROPAGE_Y) {
    TestCPU cpu;
    REG & r = cpu.Register();
    /************************************************************************
     * Setup
     ************************************************************************/
    cpu.SetBootAddress(0x100);
    cpu.Reset();
    cpu.SetTicks(0);
    cpu.SetMemory(0x100, 0x42);
    r.Y = 0x42;
    /************************************************************************
     * Verify that we read from the zeropage
     ************************************************************************/
    EXPECT_EQ(cpu.ZEROPAGE_Y(), 0x84);
    EXPECT_EQ(cpu.GetTicks(), 2);

    /************************************************************************
     * Verify that we read from the zeropage, with overflow
     ************************************************************************
        * If the zero page address is 0x02 and the Y register is 0xFF
        * then their sum is 0x101. In this case the expected behaviour is
        * to wrap around to 0x01.
     */
    cpu.Reset();
    cpu.SetTicks(0);
    cpu.SetMemory(0x100, 0xFF);
    r.Y = 0x02;
    EXPECT_EQ(cpu.ZEROPAGE_Y(), 0x01);
    EXPECT_EQ(cpu.GetTicks(), 2);
}

TEST(ADDRESSING_MODE, ABSOLUTE)
{
    TestCPU cpu;
    /************************************************************************
     * Setup
     ************************************************************************/
    cpu.SetBootAddress(0x100);
    cpu.Reset();
    cpu.SetTicks(0);
    cpu.SetMemory(0x100, 0x00);
    cpu.SetMemory(0x101, 0x10);
    /************************************************************************
     * Verify that we read from the absolute
     ************************************************************************/
    EXPECT_EQ(cpu.ABSOLUTE(), 0x1000);
    EXPECT_EQ(cpu.GetTicks(), 2);
}


TEST(ADDRESSING_MODE, ABSOLUTE_X)
{
    TestCPU cpu;
    REG & r = cpu.Register();
    /************************************************************************
     * Setup
     ************************************************************************/
    cpu.SetBootAddress(0x100);
    cpu.Reset();
    cpu.SetTicks(0);
    cpu.SetMemory(0x100, 0xFF);
    cpu.SetMemory(0x101, 0x10);
    r.X = 0x01;
    /************************************************************************
     * Verify that we read from the absolute
     ************************************************************************/
    EXPECT_EQ(cpu.ABSOLUTE_X(), 0x1100);
    EXPECT_EQ(cpu.GetTicks(), 3); // 3 Ticks on page cross

    /************************************************************************
     * Setup
     ************************************************************************/
    cpu.Reset();
    cpu.SetTicks(0);
    cpu.SetMemory(0x100, 0x00);
    cpu.SetMemory(0x101, 0x10);
    r.X = 0x01;
    /************************************************************************
     * Verify that we read from the absolute
     ************************************************************************/
    EXPECT_EQ(cpu.ABSOLUTE_X(), 0x1001);
    EXPECT_EQ(cpu.GetTicks(), 2); // 3 Ticks on page cross
}


TEST(ADDRESSING_MODE, ABSOLUTE_Y)
{
    TestCPU cpu;
    REG & r = cpu.Register();
    /************************************************************************
     * Setup
     ************************************************************************/
    cpu.SetBootAddress(0x100);
    cpu.Reset();
    cpu.SetTicks(0);
    cpu.SetMemory(0x100, 0xFF);
    cpu.SetMemory(0x101, 0x10);
    r.Y = 0x01;
    /************************************************************************
     * Verify that we read from the absolute
     ************************************************************************/
    EXPECT_EQ(cpu.ABSOLUTE_Y(), 0x1100);
    EXPECT_EQ(cpu.GetTicks(), 3); // 3 Ticks on page cross

    /************************************************************************
     * Setup
     ************************************************************************/
    cpu.Reset();
    cpu.SetTicks(0);
    cpu.SetMemory(0x100, 0x00);
    cpu.SetMemory(0x101, 0x10);
    r.Y = 0x01;
    /************************************************************************
     * Verify that we read from the absolute
     ************************************************************************/
    EXPECT_EQ(cpu.ABSOLUTE_Y(), 0x1001);
    EXPECT_EQ(cpu.GetTicks(), 2);
}

TEST(ADDRESSING_MODE, INDIRECT_X)
{
    TestCPU cpu;
    REG & r = cpu.Register();
    /************************************************************************
     * Setup
     ************************************************************************/
    cpu.SetBootAddress(0x100);
    cpu.Reset();
    cpu.SetTicks(0);
    cpu.SetMemory(0x0100, 0x00);
    cpu.SetMemory(0x0000, 0x01);
    cpu.SetMemory(0x0001, 0x01);
    r.X = 0x00;
    /************************************************************************
     * Verify that we read from the absolute
     ************************************************************************/
    EXPECT_EQ(cpu.INDIRECT_X(), 0x0101);
    EXPECT_EQ(cpu.GetTicks(), 3);

    /************************************************************************
     * Setup
     ************************************************************************/
    cpu.Reset();
    cpu.SetTicks(0);
    cpu.SetMemory(0x0100, 0x00);
    cpu.SetMemory(0x00A0, 0x20);
    cpu.SetMemory(0x00A1, 0x01);
    r.X = 0xA0;
    /************************************************************************
     * Verify that we read from the absolute
     ************************************************************************/
    EXPECT_EQ(cpu.INDIRECT_X(), 0x0120);
    EXPECT_EQ(cpu.GetTicks(), 3);
}

TEST(ADDRESSING_MODE, INDIRECT_Y)
{
    TestCPU cpu;
    REG & r = cpu.Register();
    /************************************************************************
     * Setup
     ************************************************************************/
    cpu.SetBootAddress(0x100);
    cpu.Reset();
    cpu.SetTicks(0);
    cpu.SetMemory(0x0100, 0x00);
    cpu.SetMemory(0x0000, 0x01);
    cpu.SetMemory(0x0001, 0x01);
    r.Y = 0xA0;
    /************************************************************************
     * Verify that we read from the absolute
     ************************************************************************/
    EXPECT_EQ(cpu.INDIRECT_Y(), 0x01A1);
    EXPECT_EQ(cpu.GetTicks(), 3);
    
    /************************************************************************
     * Setup
     ************************************************************************/
    cpu.Reset();
    cpu.SetTicks(0);
    cpu.SetMemory(0x0100, 0x00);
    cpu.SetMemory(0x0000, 0xFF);
    cpu.SetMemory(0x0001, 0x01);
    r.Y = 0x02;
    /************************************************************************
     * Verify that we read from the absolute
     ************************************************************************/
    EXPECT_EQ(cpu.INDIRECT_Y(), 0x0201);
    EXPECT_EQ(cpu.GetTicks(), 4);
}