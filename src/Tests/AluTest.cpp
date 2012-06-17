#include "Alu.h"
#include "MiniUnit.hpp"

#include <iostream>

namespace Minirechner2i {
namespace Tests {

using std::bitset;

class Alu : public MiniUnit::TestCase <Alu> {
    public:
        void testSummation() {
            bitset<4> add("0100"), addo("0101"), addc("0110"), addci("0111");
            unsigned long a = 47, b = 19, c = 236;
            bitset<3> flags;

            // Cin = 0, a = 47, b = 19  -> a + b = 66, Ca = 0
            MiniUnitAssertEqual(alu->calculate(add, a, b, flags), bitset<8>(66));
            // Ca must be 0
            MiniUnitAssertEqual(flags[0], 0);
            // Cin = 0, a = 47, b = 19  -> a + b + 1 = 67, Ca = 1
            MiniUnitAssertEqual(alu->calculate(addo, a, b, flags), bitset<8>(67));
            // Ca must be 1
            MiniUnitAssertEqual(flags[0], 1);
            // Cin = 1, a = 47, b = 19  -> a + b + Cin = 67, Ca = 0
            MiniUnitAssertEqual(alu->calculate(addc, a, b, flags), bitset<8>(67));
            // Ca must be 0
            MiniUnitAssertEqual(flags[0], 0);
            // Cin = 0, a = 47, b = 19  -> a + b + ~Cin = 67, Ca = 1
            MiniUnitAssertEqual(alu->calculate(addci, a, b, flags), bitset<8>(67));
            // Ca must be 1
            MiniUnitAssertEqual(flags[0], 1);
            // Cin = 1, a = 47, c = 236  -> a + c + Cin = 28 (overflow), Ca = 1
            MiniUnitAssertEqual(alu->calculate(addc, a, c, flags), bitset<8>(28));
            // Ca must be 1
            MiniUnitAssertEqual(flags[0], 1);
        }

        void testShift() {
            bitset<4> lsr("1000"), rr("1001"), rrc("1010"), asr("1011");
            bitset<8> a("11010110"), b("00101101"), c;
            bitset<3> flags;

            // Cin = 0, a = 11010110  -> a >> 1 = 01101011, Ca = 0
            MiniUnitAssertEqual(alu->calculate(lsr, a, c, flags), bitset<8>("01101011"));
            // Ca must be 0
            MiniUnitAssertEqual(flags[0], 0);
            // Cin = 0, b = 00101101  -> b RR 1 = 10010110, Ca = 1
            MiniUnitAssertEqual(alu->calculate(rr, b, c, flags), bitset<8>("10010110"));
            // Ca must be 1
            MiniUnitAssertEqual(flags[0], 1);
            // Cin = 1, a = 11010110  -> a RRC 1 = 11101011, Ca = 0
            MiniUnitAssertEqual(alu->calculate(rrc, a, c, flags), bitset<8>("11101011"));
            // Ca must be 0
            MiniUnitAssertEqual(flags[0], 0);
            // Cin = 0, b = 00101101  -> b >>A 1 = 00010110, Ca = 1
            MiniUnitAssertEqual(alu->calculate(asr, b, c, flags), bitset<8>("00010110"));
            // Ca must be 1
            MiniUnitAssertEqual(flags[0], 1);
        }

        void initialize() {
            alu = new Minirechner2i::Alu;
        }

        void clean() {
            delete alu;
            alu = NULL;
        }

    private:
        Minirechner2i::Alu * alu;
};

}
}

int main(int argc, char** args) {
    typedef Minirechner2i::Tests::Alu AluTest;

    AluTest test;

    test.addTest(&AluTest::testSummation, "summation");
    test.addTest(&AluTest::testShift, "right shift");
    std::cout << test.run();

    return 0;
}
