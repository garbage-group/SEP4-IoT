#include <unity.h>

int main(int argc, char **argv)
{
    UNITY_BEGIN();
    TEST_ASSERT_EQUAL_INT32(3, 3);
    TEST_ASSERT_EQUAL_INT32(3, 4);
    UNITY_END();

    return 0;
}