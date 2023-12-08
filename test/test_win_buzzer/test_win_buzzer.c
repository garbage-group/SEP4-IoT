#include "unity.h"

#include "activate_buzzer.h"
#include "tone.h"
#include "../fff.h"

DEFINE_FFF_GLOBALS;
// Declare a fake function for tone_play_starwars_tuned
FAKE_VOID_FUNC(tone_play_starwars_tuned);

void setUp(void)
{
    // Reset the fakes before each test
    RESET_FAKE(tone_play_starwars_tuned);
    FFF_RESET_HISTORY();
}

void tearDown(void)
{
    // Any teardown can be done here
}

void test_activateBuzzer_ShouldCallTonePlayStarWarsTuned(void)
{
    // Act
    activateBuzzer();

    // Assert
    TEST_ASSERT_EQUAL(1, tone_play_starwars_tuned_fake.call_count);
}

void test_activateBuzzer_ShouldCallTonePlayStarWarsTunedTwice(void)
{
    // Act
    activateBuzzer();
    activateBuzzer();

    // Assert
    // Verify that tone_play_starwars_tuned is called exactly once
    TEST_ASSERT_EQUAL(2, tone_play_starwars_tuned_fake.call_count);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_activateBuzzer_ShouldCallTonePlayStarWarsTuned);
    RUN_TEST(test_activateBuzzer_ShouldCallTonePlayStarWarsTunedTwice);
    UNITY_END();
}
