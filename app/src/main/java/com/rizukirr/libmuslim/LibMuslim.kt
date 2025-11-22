package com.rizukirr.libmuslim

import java.util.Calendar

data class PrayerDateTime(
    val hour: Int,
    val minute: Int,
)

data class PrayerTimeResult(
    val fajr: PrayerDateTime,
    val sunrise: PrayerDateTime,
    val dhuha: PrayerDateTime,
    val dzuhr: PrayerDateTime,
    val asr: PrayerDateTime,
    val maghrib: PrayerDateTime,
    val isha: PrayerDateTime,
)

data class PrayerTimeParam(
    val year: Int,
    val month: Int,
    val day: Int,
    val latitude: Double,
    val longitude: Double,
    val timezone: Double,
)

data class PrayerTimeDateRangeParam(
    val startYear: Int,
    val startMonth: Int,
    val startDay: Int,
    val endYear: Int,
    val endMonth: Int,
    val endDay: Int,
    val latitude: Double,
    val longitude: Double,
    val timezone: Double,
)

object LibMuslim {
    // Used to load the 'muslim-jni' library on application startup.
    init {
        System.loadLibrary("muslim-jni")
        ensureJniRefs()
    }

    fun prayerTime(
        latitude: Double,
        longitude: Double,
        timezone: Double,
    ): PrayerTimeResult {
        val calendar = Calendar.getInstance()
        val year = calendar.get(Calendar.YEAR)
        val month = calendar.get(Calendar.MONTH) + 1
        val day = calendar.get(Calendar.DAY_OF_MONTH)
        return calculatePrayerTimes(year, month, day, latitude, longitude, timezone)
    }

    fun prayerTime(param: PrayerTimeParam): PrayerTimeResult {
        return calculatePrayerTimes(param.year, param.month, param.day, param.latitude, param.longitude, param.timezone)
    }

    fun prayerTime(param: PrayerTimeDateRangeParam): List<PrayerTimeResult> {
        return calculatePrayerTimesForDateRange(
            param.startYear, param.startMonth, param.startDay,
            param.endYear, param.endMonth, param.endDay,
            param.latitude, param.longitude, param.timezone
        )
    }

    private external fun ensureJniRefs()

    private external fun calculatePrayerTimes(
        year: Int,
        month: Int,
        day: Int,
        latitude: Double,
        longitude: Double,
        timezone: Double,
    ): PrayerTimeResult

    private external fun calculatePrayerTimesForDateRange(
        startYear: Int, startMonth: Int, startDay: Int,
        endYear: Int, endMonth: Int, endDay: Int,
        latitude: Double, longitude: Double, timezone: Double
    ): List<PrayerTimeResult>
}
