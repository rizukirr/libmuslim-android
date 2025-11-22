package com.rizukirr.libmuslim

import androidx.test.ext.junit.runners.AndroidJUnit4
import org.junit.Assert.assertEquals
import org.junit.Assert.assertNotNull
import org.junit.Assert.assertTrue
import org.junit.Test
import org.junit.runner.RunWith
import kotlin.math.abs

@RunWith(AndroidJUnit4::class)
class LibMuslimTest {

    // sample taken from https://bimasislam.kemenag.go.id/jadwalshalat

    @Test
    fun testPrayerTime_Jakarta() {
        // Test for a specific location (e.g., Jakarta, Indonesia)
        val jakartaLatitude = -6.1944
        val jakartaLongitude = 106.8229
        val jakartaTimezone = +7.0

        val param = PrayerTimeParam(
            year = 2025,
            month = 11,
            day = 22,
            latitude = jakartaLatitude,
            longitude = jakartaLongitude,
            timezone = jakartaTimezone
        )

        val prayerTimes = LibMuslim.prayerTime(param)

        assertNotNull(prayerTimes)

        // Expected values for Jakarta on 22 November 2025.
        // A tolerance of +/- 2 minutes is applied to the minute checks.
        assertTimeApproxEquals("Fajr", 4, 5, prayerTimes.fajr)
        assertTimeApproxEquals("Sunrise", 5, 23, prayerTimes.sunrise)
        assertTimeApproxEquals("Dhuha", 5, 51, prayerTimes.dhuha)
        assertTimeApproxEquals("Dzuhur", 11, 42, prayerTimes.dzuhr)
        assertTimeApproxEquals("Asr", 15, 5, prayerTimes.asr)
        assertTimeApproxEquals("Magrib", 17, 55, prayerTimes.maghrib)
        assertTimeApproxEquals("Isha", 19, 9, prayerTimes.isha)
    }

    @Test
    fun testPrayerTime_DateRange() {
        // Test for a date range in the same location
        val jakartaLatitude = -6.1944
        val jakartaLongitude = 106.8229
        val jakartaTimezone = +7.0

        val param = PrayerTimeDateRangeParam(
            startYear = 2025, startMonth = 11, startDay = 20,
            endYear = 2025, endMonth = 11, endDay = 22,
            latitude = jakartaLatitude,
            longitude = jakartaLongitude,
            timezone = jakartaTimezone
        )

        val prayerTimeList = LibMuslim.prayerTime(param)

        // The list should contain 3 days of prayer times
        assertEquals(3, prayerTimeList.size)

        // Verify the last day in the range (22 November 2025)
        val firstDayPrayerTimes = prayerTimeList.first()
        // A tolerance of +/- 2 minutes is applied to the minute checks.
        assertTimeApproxEquals("Fajr", 4, 5, firstDayPrayerTimes.fajr)
        assertTimeApproxEquals("Sunrise", 5, 23, firstDayPrayerTimes.sunrise)
        assertTimeApproxEquals("Dhuha", 5, 51, firstDayPrayerTimes.dhuha)
        assertTimeApproxEquals("Dzuhur", 11, 42, firstDayPrayerTimes.dzuhr)
        assertTimeApproxEquals("Asr", 15, 4, firstDayPrayerTimes.asr)
        assertTimeApproxEquals("Magrib", 17, 55, firstDayPrayerTimes.maghrib)
        assertTimeApproxEquals("Isha", 19, 9, firstDayPrayerTimes.isha)

        val lastDayPrayerTimes = prayerTimeList.last()

        // A tolerance of +/- 2 minutes is applied to the minute checks.
        assertTimeApproxEquals("Fajr", 4, 5, lastDayPrayerTimes.fajr)
        assertTimeApproxEquals("Sunrise", 5, 23, lastDayPrayerTimes.sunrise)
        assertTimeApproxEquals("Dhuha", 5, 51, lastDayPrayerTimes.dhuha)
        assertTimeApproxEquals("Dzuhur", 11, 42, lastDayPrayerTimes.dzuhr)
        assertTimeApproxEquals("Asr", 15, 5, lastDayPrayerTimes.asr)
        assertTimeApproxEquals("Magrib", 17, 55, lastDayPrayerTimes.maghrib)
        assertTimeApproxEquals("Isha", 19, 9, lastDayPrayerTimes.isha)
    }

    /**
     * Asserts that the actual prayer time is within a given tolerance of the expected time.
     * It checks for an exact hour match and a minute match within the specified tolerance.
     *
     * @param prayerName The name of the prayer for the assertion message (e.g., "Fajr").
     * @param expectedHour The expected hour of the prayer time.
     * @param expectedMinute The expected minute of the prayer time.
     * @param actualTime The actual [PrayerDateTime] object returned from the library.
     * @param toleranceMinutes The allowed tolerance in minutes (defaults to 2).
     */
    private fun assertTimeApproxEquals(
        prayerName: String,
        expectedHour: Int,
        expectedMinute: Int,
        actualTime: PrayerDateTime,
        toleranceMinutes: Int = 2
    ) {
        assertEquals("Hour for $prayerName does not match", expectedHour, actualTime.hour)

        val minuteDiff = abs(expectedMinute - actualTime.minute)
        assertTrue(
            "Minute for $prayerName is out of tolerance. " +
                    "Expected: $expectedMinute, Actual: ${actualTime.minute}, Tolerance: +/-$toleranceMinutes",
            minuteDiff <= toleranceMinutes
        )
    }
}
