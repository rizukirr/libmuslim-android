# libmuslim - Android Prayer Times Library

An Android library for calculating Islamic prayer times based on Indonesian Kemenag standards.

## Features

- Calculate prayer times (Fajr, Sunrise, Dhuha, Dzuhr, Asr, Maghrib, Isha)
- Based on Indonesian Kemenag calculation method
- Native C implementation for performance
- Supports ARM64 and x86_64 architectures

## Installation

### Using JitPack (Recommended)

1. Add JitPack repository to your project's `settings.gradle.kts`:

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

2. Add the dependency to your app's `build.gradle.kts`:

```kotlin
dependencies {
    implementation("com.github.rizukirr:libmuslim:1.0.0")
}
```

### Using AAR File

1. Copy `app/build/outputs/aar/app-release.aar` to your Android project's `libs/` directory
2. Add to your app's `build.gradle.kts`:

```kotlin
dependencies {
    implementation(files("libs/app-release.aar"))
}
```

### Building from Source

```bash
./gradlew assembleRelease
```

The AAR file will be generated at `app/build/outputs/aar/app-release.aar`

## Publishing to JitPack

1. Create a new release/tag on GitHub (e.g., `v1.0.0`)
2. JitPack will automatically build the library
3. Visit `https://jitpack.io/#rizukirr/libmuslim` to check build status

## Usage

```kotlin
import com.rizukirr.libmuslim.LibMuslim
import com.rizukirr.libmuslim.PrayerDateTime
import com.rizukirr.libmuslim.PrayerTimeResult
import com.rizukirr.libmuslim.PrayerTimeParam

// Calculate prayer times for current date
val prayerTimes = LibMuslim.prayerTime(
    latitude = -6.2088,    // Jakarta latitude
    longitude = 106.8456,  // Jakarta longitude
    timezone = 7.0         // WIB (UTC+7)
)

// Access individual prayer times
println("Fajr: ${prayerTimes.fajr.hour}:${prayerTimes.fajr.minute}")
println("Sunrise: ${prayerTimes.sunrise.hour}:${prayerTimes.sunrise.minute}")
println("Dhuha: ${prayerTimes.dhuha.hour}:${prayerTimes.dhuha.minute}")
println("Dzuhr: ${prayerTimes.dzuhr.hour}:${prayerTimes.dzuhr.minute}")
println("Asr: ${prayerTimes.asr.hour}:${prayerTimes.asr.minute}")
println("Maghrib: ${prayerTimes.maghrib.hour}:${prayerTimes.maghrib.minute}")
println("Isha: ${prayerTimes.isha.hour}:${prayerTimes.isha.minute}")

// Calculate for a specific date
val param = PrayerTimeParam(
    year = 2025,
    month = 11,
    day = 21,
    latitude = -6.2088,
    longitude = 106.8456,
    timezone = 7.0
)
val customDateTimes = LibMuslim.prayerTime(param)
```

## Data Classes

### PrayerDateTime
```kotlin
data class PrayerDateTime(
    val hour: Int,
    val minute: Int
)
```

### PrayerTimeResult
```kotlin
data class PrayerTimeResult(
    val fajr: PrayerDateTime,
    val sunrise: PrayerDateTime,
    val dhuha: PrayerDateTime,
    val dzuhr: PrayerDateTime,
    val asr: PrayerDateTime,
    val maghrib: PrayerDateTime,
    val isha: PrayerDateTime
)
```

### PrayerTimeParam
```kotlin
data class PrayerTimeParam(
    val year: Int,
    val month: Int,
    val day: Int,
    val latitude: Double,
    val longitude: Double,
    val timezone: Double
)
```

## Native Library

This library uses a native C implementation built from the `muslimify` project:
- Core calculation library: `android-libmuslim.so` (contains the prayer time algorithms)
- JNI wrapper: `libmuslim-jni.so` (bridges Kotlin to native C code)

### Rebuilding Native Libraries

From the `muslimify` project directory:

```bash
bash android.sh
```

This will build the native libraries for ARM64 and x86_64 architectures and place them in `libs/`.

## Supported Architectures

- ARM64 (arm64-v8a) - Physical devices
- x86_64 - Android emulators

## License

[Your License Here]

## Credits

Based on Kemenag Indonesia prayer time calculation standards.
