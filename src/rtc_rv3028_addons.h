/**
 * @warning
 * This add-on library depends on the internal implementation details of the
 * Zephyr RV3028 RTC driver (`drivers/rtc/rtc_rv3028.c`).
 *
 * Compatibility is verified only for Zephyr versions 3.7 through 4.4.
 * Compatibility with other Zephyr versions is not guaranteed.
 *
 * In particular, this library assumes that `struct rv3028_data` begins with:
 *
 * @code
 * struct k_sem lock;
 * @endcode
 *
 * If the layout of `struct rv3028_data` changes, this library may exhibit
 * undefined behavior or fail at runtime.
 *
 * Before using with a different Zephyr version, verify the structure layout
 * in:
 *
 * @code
 * drivers/rtc/rtc_rv3028.c
 * @endcode
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>

struct device;

/**
 * @brief Timer frequency selection: 4096 Hz.
 */
#define RV3028_TIMER_FREQ_4096HZ 0

/**
 * @brief Timer frequency selection: 64 Hz.
 */
#define RV3028_TIMER_FREQ_64HZ 1

/**
 * @brief Timer frequency selection: 1 Hz.
 */
#define RV3028_TIMER_FREQ_1HZ 2

/**
 * @brief Timer frequency selection: 1 minute.
 */
#define RV3028_TIMER_FREQ_MINUTE 3

/**
 * @brief Perform minimal initialization of the RV3028 device.
 *
 * Performs minimal driver initialization.
 *
 * Intended for cases where the RTC is already configured, such as after
 * waking from a System OFF state.
 *
 * This avoids the full setup sequence, which may unintentionally align the
 * timer state to a second boundary.
 *
 * @param dev Pointer to the device instance.
 *
 * @retval 0 Success.
 * @retval Negative error code on failure.
 */
int rv3028_init_minimal(const struct device *dev);

/**
 * @brief Enable the periodic timer interrupt.
 *
 * Configures and enables the periodic interrupt timer.
 *
 * The @p freq parameter selects the timer resolution using one of the
 * `RV3028_TIMER_FREQ_*` constants. The @p period value is a 12-bit value
 * interpreted in the selected frequency domain.
 *
 * @param dev Pointer to the device instance.
 * @param freq Timer frequency selection.
 * @param period Timer period value (12-bit).
 *
 * @retval 0 Success.
 * @retval Negative error code on failure.
 */
int rv3028_enable_periodic_interrupt(
    const struct device *dev,
    uint8_t freq,
    uint16_t period);

/**
 * @brief Read the timer flag (TF) status.
 *
 * Retrieves the current state of the TF flag.
 *
 * @param dev Pointer to the device instance.
 * @param tf Pointer to storage for the TF flag value.
 *
 * @retval 0 Success.
 * @retval Negative error code on failure.
 */
int rv3028_get_tf(const struct device *dev, bool *tf);

/**
 * @brief Clear the timer flag (TF).
 *
 * Clears the TF status flag in the device.
 *
 * @param dev Pointer to the device instance.
 *
 * @retval 0 Success.
 * @retval Negative error code on failure.
 */
int rv3028_clear_tf(const struct device *dev);


/**
 * @brief Read the RV3028 timer status value.
 *
 * Reads the timer status registers TS0 and TS1 and returns the combined
 * timer status value.
 *
 * @param dev Pointer to the device instance.
 * @param timer_status Pointer to storage for the timer status value.
 *
 * @retval 0 Success.
 * @retval Negative error code on failure.
 */
int rv3028_get_timer_status(const struct device *dev, uint16_t *timer_status);