//a place for for small misc tests

#ifdef C74_UNIT_TESTS

#include "RNBO.h"
#include <JuceHeader.h>
#include <stdexcept>

namespace RNBOUnitTests {
	using namespace RNBO;
	class GeneralTests : public UnitTest {
		public:
			GeneralTests()
				: UnitTest("General Tests")
			{}
			void runTest() override {
				beginTest("array bounds");
				RNBO::array<number, 1> l = { 1.0 };
				expectDoesNotThrow(l[0]);
				expectThrowsType(l[1], std::out_of_range);
				expectThrowsType(l[100], std::out_of_range);

				beginTest("truc");
				for (double v: { 2.0, 2.1, -1.0, -1.1, 6.999999999999998 }) {
					expectEquals(rnbo_trunc(v), (RNBO::Int)std::trunc(v));
				}
			}
	};
	GeneralTests tests;
}
#endif

