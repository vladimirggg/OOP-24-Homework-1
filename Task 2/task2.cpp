#include "ModifiableIntegerFunction.h"

void runTest(const char* testName, bool condition) {
    std::cout << testName << ": " << (condition ? "PASS" : "FAIL") << std::endl;
}

int16_t doubleFunction(int16_t x) {
    return 2 * x;
}

int main() {
    // Create a ModifiableIntegerFunction with a simple doubling function
    ModifiableIntegerFunction f(doubleFunction);
    ModifiableIntegerFunction f2(f);

    // Test basic functionality
    runTest("Test Invocation", f.invoke(5) == 10);
    f.setCustomResult(5, 20);
    runTest("Test Custom Result", f.invoke(5) == 20);
    f.disable(6);
    runTest("Test Disable", f.isDisabled(6));
    runTest("Test Disabled Invocation", !f.isDisabled(5) && f.invoke(5) == 20 && f.isDisabled(6));

    // Test serialization and deserialization
    f.serialize("function_data.bin");
    ModifiableIntegerFunction g;
    g.deserialize("function_data.bin");
    runTest("Test Serialization/Deserialization", f == g);

    // Test composition of functions
    ModifiableIntegerFunction h(doubleFunction);
    ModifiableIntegerFunction composed = f * h;
    runTest("Test Composition", composed.invoke(5) == 20);

    // Test bijective/injective/surjective properties
    runTest("Test Bijective", f.isBijective());
    runTest("Test Injective", h.isInjective());
    runTest("Test Surjective", h.isSurjective());

    // Test parallel functions
    ModifiableIntegerFunction parallel(doubleFunction);
    runTest("Test Parallel Functions", f.areParallel(parallel));

    // Test comparison operators
    ModifiableIntegerFunction less(doubleFunction);
    less.disable(5);
    runTest("Test Less Than", less < f);
    runTest("Test Greater Than", f > less);

    // Test power operator
    ModifiableIntegerFunction power(doubleFunction);
    ModifiableIntegerFunction power2 = power ^ 2;
    runTest("Test Power Operator", power2.invoke(5) == 20);
    std::cout << "All tests completed." << std::endl;

    return 0;
}