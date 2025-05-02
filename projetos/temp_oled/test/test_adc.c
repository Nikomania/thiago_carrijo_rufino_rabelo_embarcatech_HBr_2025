#include "unity.h"
#include "temperature.h"

void setUp(void) {}
void tearDown(void) {}

void test_adc_to_celsius_zero(void) {
    float actual = adc_to_celsius(0);
    float expected = 27.0f - (((0 * (3.3f/4096)) - 0.706f) / 0.001721f);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, expected, actual);
}

void test_adc_to_celsius_mid(void) {
    uint16_t mid = 2048;
    float actual = adc_to_celsius(mid);
    float expected = 27.0f - (((mid * (3.3f/4096)) - 0.706f) / 0.001721f);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, expected, actual);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_adc_to_celsius_zero);
    RUN_TEST(test_adc_to_celsius_mid);
    return UNITY_END();
}
