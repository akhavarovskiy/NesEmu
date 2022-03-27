#include <iostream>
#include <cstring>
#include <gtest/gtest.h>
#include <gtest/gtest_prod.h>
#include <6502.hpp>
#include <6502def.hpp>

using namespace MOS6502;

TEST(ADC, IMMEDIATE)
{
    TestCPU t;
    t.SetBootAddress(0x100);
    t.SetMemory(0x100, MOS6502_ADC_IMMEDIATE);  /* Set the instruction at boot address */
    t.Reset();                                  /* Reset the CPU */
    /* Verify boot address*/
    MOS6502::REG & r = t.Register();            /* Get the registers */
    EXPECT_EQ(r.PC, 0x100);                     /* Verify that the program counter is at 0x100 */
    /*
    ==================================================
        Verify the carry and negative flags
    ==================================================*/
    t.SetTicks(0);
    r.A    = 0xFF;              /* Set the accumulator */
    r.PS.C = 0x01;
    t.SetMemory(0x101, 0xFF);    /* Set the operand */
    t.Step();                   /* Run Single instruction */
    EXPECT_EQ(r.A, 0xFF);       /* Verify Accumulator */
    EXPECT_EQ(r.PS.Z, 0);       /* Verify Flags */
    EXPECT_EQ(r.PS.N, 1);
    EXPECT_EQ(r.PS.C, 1);
    EXPECT_EQ(r.PS.V, 0);
    EXPECT_EQ(t.GetTicks(), 2); /* Verify Cycle Count */

    /*
    ==================================================
        Verify the overflow flag
    ==================================================*/
    t.Reset();
    t.SetTicks(0);
    r.A    = 0x7F;              /* Set the accumulator */
    r.PS.C = 0x01;
    t.SetMemory(0x101, 0x02);   /* Set the operand */
    t.Step();                   /* Run Single instruction */
    EXPECT_EQ(r.A, 0x82);       /* Verify Accumulator */
    EXPECT_EQ(r.PS.Z, 0);       /* Verify Flags */
    EXPECT_EQ(r.PS.N, 1);
    EXPECT_EQ(r.PS.C, 0);
    EXPECT_EQ(r.PS.V, 1);
    EXPECT_EQ(t.GetTicks(), 2); /* Verify Cycle Count */

    /*
    ==================================================
        Verify the zero flags
    ==================================================*/
    t.Reset();
    t.SetTicks(0);
    r.A    = 0x00;              /* Set the accumulator */
    r.PS.C = 0x00;
    t.SetMemory(0x101, 0x00);   /* Set the operand */
    t.Step();                   /* Run Single instruction */
    EXPECT_EQ(r.A, 0x00);       /* Verify Accumulator */
    EXPECT_EQ(r.PS.Z, 1);       /* Verify Flags */
    EXPECT_EQ(r.PS.N, 0);
    EXPECT_EQ(r.PS.C, 0);
    EXPECT_EQ(r.PS.V, 0);
    EXPECT_EQ(t.GetTicks(), 2); /* Verify Cycle Count */
    /*
    ============================================
        Verify the negative flag
    ============================================*/
    t.Reset();
    t.SetTicks(0);
    r.A    = 0x80;              /* Set the accumulator */
    r.PS.C = 0x00;
    t.SetMemory(0x101, 0x00);   /* Set the operand */
    t.Step();                   /* Run Single instruction */
    EXPECT_EQ(r.A, 0x80);       /* Verify Accumulator */
    EXPECT_EQ(r.PS.Z, 0);       /* Verify Flags */
    EXPECT_EQ(r.PS.N, 1);
    EXPECT_EQ(r.PS.C, 0);
    EXPECT_EQ(r.PS.V, 0);
    EXPECT_EQ(t.GetTicks(), 2); /* Verify Cycle Count */
}

TEST(ADC, ZEROPAGE)
{
    TestCPU t;
    t.SetBootAddress(0x100);
    t.SetMemory(0x100, MOS6502_ADC_ZEROPAGE);   /* Set the instruction at boot address */
    t.Reset();                                  /* Reset the CPU */
    /* Verify boot address*/
    MOS6502::REG & r = t.Register();            /* Get the registers */
    EXPECT_EQ(r.PC, 0x100);                     /* Verify that the program counter is at 0x100 */
    /*
    ==================================================
        Verify the carry and negative flags
    ==================================================*/
    t.SetTicks(0);
    r.A    = 0xFF;              /* Set the accumulator */
    r.PS.C = 0x01;
    t.SetMemory(0x101, 0x00);   /* Set the zero page address */
    t.SetMemory(0x000, 0xFF);   /* Set the data at the address */
    t.Step();                   /* Run Single instruction */
    EXPECT_EQ(r.A, 0xFF);       /* Verify Accumulator */
    EXPECT_EQ(r.PS.Z, 0);       /* Verify Flags */
    EXPECT_EQ(r.PS.N, 1);
    EXPECT_EQ(r.PS.C, 1);
    EXPECT_EQ(r.PS.V, 0);
    EXPECT_EQ(t.GetTicks(), 3); /* Verify Cycle Count */

    /*
    ==================================================
        Verify the overflow flag
    ==================================================*/
    t.Reset();
    t.SetTicks(0);
    r.A    = 0x7F;              /* Set the accumulator */
    r.PS.C = 0x01;
    t.SetMemory(0x101, 0x00);   /* Set the zero page address */
    t.SetMemory(0x000, 0x02);   /* Set the data at the address */
    t.Step();                   /* Run Single instruction */
    EXPECT_EQ(r.A, 0x82);       /* Verify Accumulator */
    EXPECT_EQ(r.PS.Z, 0);       /* Verify Flags */
    EXPECT_EQ(r.PS.N, 1);
    EXPECT_EQ(r.PS.C, 0);
    EXPECT_EQ(r.PS.V, 1);
    EXPECT_EQ(t.GetTicks(), 3); /* Verify Cycle Count */

    /*
    ==================================================
        Verify the zero flags
    ==================================================*/
    t.Reset();
    t.SetTicks(0);
    r.A    = 0x00;              /* Set the accumulator */
    r.PS.C = 0x00;
    t.SetMemory(0x101, 0x00);   /* Set the zero page address */
    t.SetMemory(0x000, 0x00);   /* Set the data at the address */
    t.Step();                   /* Run Single instruction */
    EXPECT_EQ(r.A, 0x00);       /* Verify Accumulator */
    EXPECT_EQ(r.PS.Z, 1);       /* Verify Flags */
    EXPECT_EQ(r.PS.N, 0);
    EXPECT_EQ(r.PS.C, 0);
    EXPECT_EQ(r.PS.V, 0);
    EXPECT_EQ(t.GetTicks(), 3); /* Verify Cycle Count */
    /*
    ============================================
        Verify the negative flag
    ============================================*/
    t.Reset();
    t.SetTicks(0);
    r.A    = 0x80;              /* Set the accumulator */
    r.PS.C = 0x01;
    t.SetMemory(0x101, 0x00);   /* Set the zero page address */
    t.SetMemory(0x000, 0x01);   /* Set the data at the address */
    t.Step();                   /* Run Single instruction */
    EXPECT_EQ(r.A, 0x82);       /* Verify Accumulator */
    EXPECT_EQ(r.PS.Z, 0);       /* Verify Flags */
    EXPECT_EQ(r.PS.N, 1);
    EXPECT_EQ(r.PS.C, 0);
    EXPECT_EQ(r.PS.V, 0);
    EXPECT_EQ(t.GetTicks(), 3); /* Verify Cycle Count */
}

TEST(ADC, ZEROPAGE_X)
{
    TestCPU t;
    t.SetBootAddress(0x100);
    t.SetMemory(0x100, MOS6502_ADC_ZEROPAGE_X);   /* Set the instruction at boot address */
    t.Reset();                                  /* Reset the CPU */
    /* Verify boot address*/
    MOS6502::REG & r = t.Register();            /* Get the registers */
    EXPECT_EQ(r.PC, 0x100);                     /* Verify that the program counter is at 0x100 */
    /*
    ==================================================
        Verify the carry and negative flags
    ==================================================*/
    t.SetTicks(0);
    r.A    = 0xFF;              /* Set the accumulator */
    r.PS.C = 0x01;
    r.X    = 0xF0;              /* Set the zero page offset */
    t.SetMemory(0x101, 0x20);   /* Set the zero page address */
    t.SetMemory(0x010, 0xFF);   /* Set the data at the address */
    t.Step();                   /* Run Single instruction */
    EXPECT_EQ(r.A, 0xFF);       /* Verify Accumulator */
    EXPECT_EQ(r.PS.Z, 0);       /* Verify Flags */
    EXPECT_EQ(r.PS.N, 1);
    EXPECT_EQ(r.PS.C, 1);
    EXPECT_EQ(r.PS.V, 0);
    EXPECT_EQ(t.GetTicks(), 4); /* Verify Cycle Count */

    /*
    ==================================================
        Verify the overflow flag
    ==================================================*/
    t.Reset();
    t.SetTicks(0);
    r.A    = 0x7F;              /* Set the accumulator */
    r.PS.C = 0x01;
    r.X    = 0xF0;              /* Set the zero page offset */
    t.SetMemory(0x101, 0x20);   /* Set the zero page address */
    t.SetMemory(0x010, 0x02);   /* Set the data at the address */
    t.Step();                   /* Run Single instruction */
    EXPECT_EQ(r.A, 0x82);       /* Verify Accumulator */
    EXPECT_EQ(r.PS.Z, 0);       /* Verify Flags */
    EXPECT_EQ(r.PS.N, 1);
    EXPECT_EQ(r.PS.C, 0);
    EXPECT_EQ(r.PS.V, 1);
    EXPECT_EQ(t.GetTicks(), 4); /* Verify Cycle Count */

    /*
    ==================================================
        Verify the zero flags
    ==================================================*/
    t.Reset();
    t.SetTicks(0);
    r.A    = 0x00;              /* Set the accumulator */
    r.PS.C = 0x00;
    r.X    = 0xF0;              /* Set the zero page offset */
    t.SetMemory(0x101, 0x20);   /* Set the zero page address */
    t.SetMemory(0x010, 0x00);   /* Set the data at the address */
    t.Step();                   /* Run Single instruction */
    EXPECT_EQ(r.A, 0x00);       /* Verify Accumulator */
    EXPECT_EQ(r.PS.Z, 1);       /* Verify Flags */
    EXPECT_EQ(r.PS.N, 0);
    EXPECT_EQ(r.PS.C, 0);
    EXPECT_EQ(r.PS.V, 0);
    EXPECT_EQ(t.GetTicks(), 4); /* Verify Cycle Count */
    /*
    ============================================
        Verify the negative flag
    ============================================*/
    t.Reset();
    t.SetTicks(0);
    r.A    = 0x80;              /* Set the accumulator */
    r.PS.C = 0x00;
    r.X    = 0xF0;              /* Set the zero page offset */
    t.SetMemory(0x101, 0x20);   /* Set the zero page address */
    t.SetMemory(0x010, 0x00);   /* Set the data at the address */
    t.Step();                   /* Run Single instruction */
    EXPECT_EQ(r.A, 0x80);       /* Verify Accumulator */
    EXPECT_EQ(r.PS.Z, 0);       /* Verify Flags */
    EXPECT_EQ(r.PS.N, 1);
    EXPECT_EQ(r.PS.C, 0);
    EXPECT_EQ(r.PS.V, 0);
    EXPECT_EQ(t.GetTicks(), 4); /* Verify Cycle Count */
}

TEST(ADC, ABSOLUTE)
{
    TestCPU t;
    t.SetBootAddress(0x100);
    t.SetMemory(0x100, MOS6502_ADC_ABSOLUTE);   /* Set the instruction at boot address */
    t.Reset();                                  /* Reset the CPU */
    /* Verify boot address*/
    MOS6502::REG & r = t.Register();            /* Get the registers */
    EXPECT_EQ(r.PC, 0x100);                     /* Verify that the program counter is at 0x100 */
    /*
    ==================================================
        Verify the carry and negative flags
    ==================================================*/
    t.SetTicks(0);
    r.A    = 0xFF;              /* Set the accumulator */
    r.PS.C = 0x01;
    t.SetMemory(0x101, 0x20);   /* Set the abosolute address */
    t.SetMemory(0x102, 0x00);   
    t.SetMemory(0x020, 0xFF);   /* Set the data at the address */
    t.Step();                   /* Run Single instruction */
    EXPECT_EQ(r.A, 0xFF);       /* Verify Accumulator */
    EXPECT_EQ(r.PS.Z, 0);       /* Verify Flags */
    EXPECT_EQ(r.PS.N, 1);
    EXPECT_EQ(r.PS.C, 1);
    EXPECT_EQ(r.PS.V, 0);
    EXPECT_EQ(t.GetTicks(), 4); /* Verify Cycle Count */

    /*
    ==================================================
        Verify the overflow flag
    ==================================================*/
    t.Reset();
    t.SetTicks(0);
    r.A    = 0x7F;              /* Set the accumulator */
    r.PS.C = 0x01;
    t.SetMemory(0x101, 0x20);   /* Set the abosolute address */
    t.SetMemory(0x102, 0x00);   
    t.SetMemory(0x020, 0x02);   /* Set the data at the address */
    t.Step();                   /* Run Single instruction */
    EXPECT_EQ(r.A, 0x82);       /* Verify Accumulator */
    EXPECT_EQ(r.PS.Z, 0);       /* Verify Flags */
    EXPECT_EQ(r.PS.N, 1);
    EXPECT_EQ(r.PS.C, 0);
    EXPECT_EQ(r.PS.V, 1);
    EXPECT_EQ(t.GetTicks(), 4); /* Verify Cycle Count */

    /*
    ==================================================
        Verify the zero flags
    ==================================================*/
    t.Reset();
    t.SetTicks(0);
    r.A    = 0x00;              /* Set the accumulator */
    r.PS.C = 0x00;
    t.SetMemory(0x101, 0x20);   /* Set the abosolute address */
    t.SetMemory(0x102, 0x00);   
    t.SetMemory(0x020, 0x00);   /* Set the data at the address */
    t.Step();                   /* Run Single instruction */
    EXPECT_EQ(r.A, 0x00);       /* Verify Accumulator */
    EXPECT_EQ(r.PS.Z, 1);       /* Verify Flags */
    EXPECT_EQ(r.PS.N, 0);
    EXPECT_EQ(r.PS.C, 0);
    EXPECT_EQ(r.PS.V, 0);
    EXPECT_EQ(t.GetTicks(), 4); /* Verify Cycle Count */
    /*
    ============================================
        Verify the negative flag
    ============================================*/
    t.Reset();
    t.SetTicks(0);
    r.A    = 0x80;              /* Set the accumulator */
    r.PS.C = 0x00;
    t.SetMemory(0x101, 0x20);   /* Set the abosolute address */
    t.SetMemory(0x102, 0x00);   
    t.SetMemory(0x020, 0x00);   /* Set the data at the address */
    t.Step();                   /* Run Single instruction */
    EXPECT_EQ(r.A, 0x80);       /* Verify Accumulator */
    EXPECT_EQ(r.PS.Z, 0);       /* Verify Flags */
    EXPECT_EQ(r.PS.N, 1);
    EXPECT_EQ(r.PS.C, 0);
    EXPECT_EQ(r.PS.V, 0);
    EXPECT_EQ(t.GetTicks(), 4); /* Verify Cycle Count */
}
