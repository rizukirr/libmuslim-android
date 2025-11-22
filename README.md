# LibMuslim Android

[![](https://jitpack.io/v/rizukirr/libmuslim-android.svg)](https://jitpack.io/#rizukirr/libmuslim-android)

An Android library for calculating Islamic prayer times using the **Indonesian Ministry of Religious Affairs (Kemenag RI)** calculation method. Built with a native C implementation for optimal performance.

## Features

-  **Accurate Kemenag Calculation**: Uses official Indonesian Ministry of Religious Affairs (Kemenag RI) method
-  **High Performance**: Native C implementation with JNI bindings
-  **Single & Batch Calculations**: Calculate for a specific date or date range
-  **Multi-Architecture Support**: ARM64-v8a (physical devices) and x86_64 (emulators)
-  **Complete Prayer Times**: Fajr, Sunrise, Dhuha, Dzuhr, Asr, Maghrib, and Isha
-  **Precision**: ±1 minute accuracy compared to official Kemenag schedules

## Installation

Add JitPack repository to your project's `settings.gradle.kts`:

```kotlin
dependencyResolutionManagement {
    repositoriesMode.set(RepositoriesMode.FAIL_ON_PROJECT_REPOS)
    repositories {
        google()
        mavenCentral()
        maven { url = uri("https://jitpack.io") }
    }
}
```

Add the dependency to your app's `build.gradle.kts`:

```kotlin
dependencies {
    implementation("com.github.rizukirr:libmuslim-android:1.0.0")
}
```

## Usage

### Basic Usage (Current Date)

Calculate prayer times for today using your current location:

```kotlin
import com.rizukirr.libmuslim.LibMuslim

// Jakarta coordinates as example
val prayerTimes = LibMuslim.prayerTime(
    latitude = -6.2088,
    longitude = 106.8456,
    timezone = 7.0  // WIB (UTC+7)
)

// Access individual prayer times
println("Fajr: ${prayerTimes.fajr.hour}:${String.format("%02d", prayerTimes.fajr.minute)}")
println("Sunrise: ${prayerTimes.sunrise.hour}:${String.format("%02d", prayerTimes.sunrise.minute)}")
println("Dhuha: ${prayerTimes.dhuha.hour}:${String.format("%02d", prayerTimes.dhuha.minute)}")
println("Dzuhr: ${prayerTimes.dzuhr.hour}:${String.format("%02d", prayerTimes.dzuhr.minute)}")
println("Asr: ${prayerTimes.asr.hour}:${String.format("%02d", prayerTimes.asr.minute)}")
println("Maghrib: ${prayerTimes.maghrib.hour}:${String.format("%02d", prayerTimes.maghrib.minute)}")
println("Isha: ${prayerTimes.isha.hour}:${String.format("%02d", prayerTimes.isha.minute)}")
```

### Specific Date

Calculate prayer times for a specific date:

```kotlin
import com.rizukirr.libmuslim.PrayerTimeParam

val param = PrayerTimeParam(
    year = 2025,
    month = 11,
    day = 22,
    latitude = -6.2088,
    longitude = 106.8456,
    timezone = 7.0
)

val prayerTimes = LibMuslim.prayerTime(param)
```

### Date Range (Batch Calculation)

Calculate prayer times for multiple days efficiently:

```kotlin
import com.rizukirr.libmuslim.PrayerTimeDateRangeParam

val param = PrayerTimeDateRangeParam(
    startYear = 2025, startMonth = 11, startDay = 1,
    endYear = 2025, endMonth = 11, endDay = 30,
    latitude = -6.2088,
    longitude = 106.8456,
    timezone = 7.0
)

val prayerTimesList: List<PrayerTimeResult> = LibMuslim.prayerTime(param)

// Iterate through all days
prayerTimesList.forEachIndexed { index, times ->
    println("Day ${index + 1}: Fajr ${times.fajr.hour}:${times.fajr.minute}")
}
```

## Data Classes

### PrayerDateTime

Represents a specific prayer time:

```kotlin
data class PrayerDateTime(
    val hour: Int,   // 0-23
    val minute: Int  // 0-59
)
```

### PrayerTimeResult

Contains all prayer times for a single day:

```kotlin
data class PrayerTimeResult(
    val fajr: PrayerDateTime,      // Subuh/Fajr (dawn)
    val sunrise: PrayerDateTime,   // Syuruq (sunrise)
    val dhuha: PrayerDateTime,     // Dhuha (mid-morning)
    val dzuhr: PrayerDateTime,     // Dzuhur/Dhuhr (noon)
    val asr: PrayerDateTime,       // Ashar/Asr (afternoon)
    val maghrib: PrayerDateTime,   // Maghrib (sunset)
    val isha: PrayerDateTime       // Isya/Isha (night)
)
```

### Parameters

```kotlin
// Single date parameter
data class PrayerTimeParam(
    val year: Int,
    val month: Int,        // 1-12
    val day: Int,          // 1-31
    val latitude: Double,  // -90 to 90 (negative = South)
    val longitude: Double, // -180 to 180 (positive = East)
    val timezone: Double   // UTC offset (e.g., WIB = 7.0)
)

// Date range parameter
data class PrayerTimeDateRangeParam(
    val startYear: Int, startMonth: Int, startDay: Int,
    val endYear: Int, endMonth: Int, endDay: Int,
    val latitude: Double,
    val longitude: Double,
    val timezone: Double
)
```

## Indonesian Timezone Reference

| Zone | Name | UTC Offset | `timezone` Value |
|------|------|------------|------------------|
| WIB  | Waktu Indonesia Barat | UTC+7 | `7.0` |
| WITA | Waktu Indonesia Tengah | UTC+8 | `8.0` |
| WIT  | Waktu Indonesia Timur | UTC+9 | `9.0` |

## Kemenag Calculation Method

This library implements the official **Kementerian Agama Republik Indonesia (Kemenag RI)** calculation method with the following specifications:

### Solar Altitude Angles

| Prayer | Angle | Description |
|--------|-------|-------------|
| **Fajr** | -20° | Sun 20° below eastern horizon |
| **Sunrise/Maghrib** | -0.833° | Atmospheric refraction correction |
| **Isha** | -18° | Sun 18° below western horizon |
| **Asr** | Shadow-based | Shadow = object length + noon shadow (Shafi'i madhhab) |

### Ihtiyat (Precautionary Adjustments)

Kemenag adds **2 minutes** to all prayer times as a safety margin:

| Prayer | Adjustment |
|--------|------------|
| Fajr   | +2 minutes |
| Sunrise | -2 minutes |
| Dhuha  | Based on adjusted sunrise +28 minutes |
| Dzuhr  | +2 minutes |
| Asr    | +2 minutes |
| Maghrib | +2 minutes |
| Isha   | +2 minutes |

### Time Rounding

The library uses **ceiling rounding** (always round up) for minutes, ensuring prayers are never performed before their actual time - in accordance with Islamic jurisprudence principles of caution (*ihtiyat*).

### Astronomical Algorithm

Based on simplified **Jean Meeus** astronomical algorithms:

1. **Julian Day** calculation from Gregorian calendar
2. **Solar position** determination (declination & equation of time)
3. **Transit time** (solar noon) calculation
4. **Hour angle** computation for each prayer
5. **Ihtiyat** adjustment and ceiling rounding

### Accuracy

- **±1 minute** compared to official Kemenag schedules
- **±0.1°** solar position accuracy (~4 minutes)
- Verified against official Kemenag API data (2021-2030)

## Example: Jakarta, November 22, 2025

```kotlin
val jakartaTimes = LibMuslim.prayerTime(
    PrayerTimeParam(
        year = 2025, month = 11, day = 22,
        latitude = -6.1944,
        longitude = 106.8229,
        timezone = 7.0
    )
)

// Expected output (verified with Kemenag):
// Fajr:    04:05
// Sunrise: 05:23
// Dhuha:   05:51
// Dzuhr:   11:42
// Asr:     15:05
// Maghrib: 17:55
// Isha:    19:09
```

## Architecture

### Native Components

- **libmuslim.so**: Core prayer time calculation library (C implementation)
- **libmuslim-jni.so**: JNI wrapper bridging Kotlin to native C code
- Supports: ARM64-v8a (physical devices), x86_64 (emulators)

### Source Repository

The core C library is maintained at: [github.com/rizukirr/libmuslim](https://github.com/rizukirr/libmuslim)

## Building from Source

```bash
# Clone the repository
git clone https://github.com/rizukirr/libmuslim-android.git
cd libmuslim-android

# Build the library
./gradlew assembleRelease

# Run tests
./gradlew connectedAndroidTest

# Output AAR will be at: app/build/outputs/aar/app-release.aar
```

## Testing

The library includes instrumented tests that verify calculations against official Kemenag data:

```bash
./gradlew connectedAndroidTest
```

Tests verify:
- Single date calculations
- Date range calculations
- Accuracy within ±2 minute tolerance
- Comparison with official Kemenag API

## Minimum Requirements

- **Android API 24** (Android 7.0 Nougat) or higher
- **JDK 11** or higher

## License

MIT License

## Credits

- Calculation method: **Kementerian Agama Republik Indonesia (Kemenag RI)**
- Algorithm reference: Jean Meeus, "Astronomical Algorithms" (2nd Edition)
- Verified against: Official Kemenag Bimas Islam prayer schedule API

## Support

For issues, questions, or contributions:
- **GitHub Issues**: [github.com/rizukirr/libmuslim-android/issues](https://github.com/rizukirr/libmuslim-android/issues)
- **Core Library**: [github.com/rizukirr/libmuslim](https://github.com/rizukirr/libmuslim)
if you find this project helpful, consider ☕ [Buy Me a Coffee](https://ko-fi.com/rizukirr)

---

*In the name of Allah, the Most Gracious, the Most Merciful*
