#include <iostream>
#include <cstring>
#include <gtest/gtest.h>
#include <gtest/gtest_prod.h>
#include <6502.hpp>
#include <6502def.hpp>

using namespace MOS6502;

TEST(ASL, ACCUMULATOR) {
    TestCPU t;
    t.SetBootAddress(0x100);
    t.SetMemory(0x100, MOS6502_ASL_ACCUMULATOR); /* Set the instruction at boot address */
    t.Reset();                                   /* Reset the CPU */

    MOS6502::REG & r = t.Register();             /* Get the registers */
    EXPECT_EQ(r.PC, 0x100);                      /* Verify that the program counter is at 0x100 */
    /*
    ============================================
        Sub Test 1
    ============================================*/
    t.SetTicks(0);
    r.A = 0x01;                 /* Set accumulator argument */
    t.Step();                   /* Run Single instruction */
    EXPECT_EQ(r.A, 0x02);       /* Verify Accumulator */
    EXPECT_EQ(r.PS.Z, 0);       /* Verify Flags */
    EXPECT_EQ(r.PS.N, 0);
    EXPECT_EQ(r.PS.C, 0);
    EXPECT_EQ(t.GetTicks(), 2); /* Verify Cycle Count */
    /*
    ============================================
        Sub Test 2
    ============================================*/
    t.Reset();                  /* Reset the CPU between sub tests */
    t.SetTicks(0);
    r.A = 0xFF;                 /* Set accumulator argument */
    t.Step();                   /* Run Single instruction */
    EXPECT_EQ(r.A, 0xFE);       /* Verify Accumulator */
    EXPECT_EQ(r.PS.Z, 0);       /* Verify Flags */
    EXPECT_EQ(r.PS.N, 1);
    EXPECT_EQ(r.PS.C, 1);
    EXPECT_EQ(t.GetTicks(), 2);  /* Verify Cycle Count */
    /*
    ============================================
        Sub Test 3
    ============================================*/
    t.Reset();                  /* Reset the CPU between sub tests */
    t.SetTicks(0);
    r.A = 0x80;                 /* Set accumulator argument */
    t.Step();                   /* Run Single instruction */
    EXPECT_EQ(r.A, 0x00);       /* Verify Accumulator */
    EXPECT_EQ(r.PS.Z, 1);       /* Verify Flags */
    EXPECT_EQ(r.PS.N, 0);
    EXPECT_EQ(r.PS.C, 1);
    EXPECT_EQ(t.GetTicks(), 2); /* Verify Cycle Count */
}