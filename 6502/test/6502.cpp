#include <gtest/gtest.h>
#include <gtest/gtest-typed-test.h>
#include <iostream>
#include <6502.hpp>
#include <cstring>

class CPUTest : public virtual MOS6502::CPU
{
public:
    CPUTest() : MOS6502::CPU() {
        m_ticks  = 0;
        std::memset(m_memory, 0, 1 << 16);
    }
    ~CPUTest() {

    }
    uint8_t Read (uint16_t addr) override { 
        return m_memory[ addr ];
    }
    void    Write(uint16_t addr, uint8_t v) override {
        m_memory[ addr ] = v;
    }
    void    Tick () override {
        m_ticks++;
    }

    inline uint32_t & GetTicks() {
        return m_ticks;
    }

    inline uint8_t*   GetMemory() {
        return m_memory;
    }
private:
    uint32_t m_ticks;
    uint8_t m_memory[1 << 16];
};

TEST(ADC, ACCUMULATOR) {
    CPUTest t;
    t.Step();
    EXPECT_EQ(1, 1);
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}