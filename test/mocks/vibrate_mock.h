#ifndef VIBRATE_MOCK_H_
#define VIBRATE_MOCK_H_

/**
 * @brief Resets the state of the mock.
 */
void reset_vibrate_mock(void);

/**
 * @brief Gets the number of times the vibrate function was called.
 * @return The total call count.
 */
int get_vibrate_call_count(void);

/**
 * @brief Gets the 'count' argument from the last call to vibrate.
 * @return The last value passed for the 'count' parameter.
 */
int get_last_vibration_count_arg(void);

/**
 * @brief Gets the 'duration' argument from the last call to vibrate.
 * @return The last value passed for the 'duration' parameter.
 */
double get_last_duration_arg(void);

/**
 * @brief Gets the 'delay' argument from the last call to vibrate.
 * @return The last value passed for the 'delay' parameter.
 */
double get_last_delay_arg(void);

#endif