# JitPack Publishing Guide

This document explains how to publish libmuslim to JitPack.

## Prerequisites

1. GitHub repository with the code
2. Git tags/releases for versioning

## Files Configured for JitPack

### 1. `jitpack.yml`
Specifies the JDK version for building on JitPack servers.

```yaml
jdk:
  - openjdk17
before_install:
  - sdk install java 17.0.1-open
  - sdk use java 17.0.1-open
```

### 2. `app/build.gradle.kts`
Added Maven publishing configuration:

```kotlin
plugins {
    alias(libs.plugins.android.library)
    alias(libs.plugins.kotlin.android)
    id("maven-publish")  // Added for JitPack
}

// Maven publishing configuration
afterEvaluate {
    publishing {
        publications {
            create<MavenPublication>("release") {
                from(components["release"])
                groupId = "com.github.rizukirr"
                artifactId = "libmuslim"
                version = "1.0.0"
            }
        }
    }
}
```

## Publishing Steps

### 1. Commit and Push Changes

```bash
git add .
git commit -m "Add JitPack configuration"
git push origin main
```

### 2. Create a Git Tag

```bash
# Create a tag for version 1.0.0
git tag -a v1.0.0 -m "Release version 1.0.0"
git push origin v1.0.0
```

Or create a release through GitHub UI:
1. Go to your repository on GitHub
2. Click "Releases" → "Create a new release"
3. Enter tag version (e.g., `v1.0.0`)
4. Add release notes
5. Publish release

### 3. Build on JitPack

Visit: `https://jitpack.io/#rizukirr/libmuslim`

JitPack will automatically detect the tag and start building. You can:
- View build logs
- Check build status
- Get the dependency coordinates

### 4. Using the Library

Add to your app's `build.gradle.kts`:

```kotlin
dependencies {
    implementation("com.github.rizukirr:libmuslim:1.0.0")
}
```

And ensure JitPack repository is added in `settings.gradle.kts`:

```kotlin
dependencyResolutionManagement {
    repositories {
        google()
        mavenCentral()
        maven { url = uri("https://jitpack.io") }
    }
}
```

## Version Management

To release a new version:

1. Update version in `app/build.gradle.kts` (line 64):
   ```kotlin
   version = "1.1.0"  // Update version number
   ```

2. Create a new tag:
   ```bash
   git tag -a v1.1.0 -m "Release version 1.1.0"
   git push origin v1.1.0
   ```

3. JitPack will automatically build the new version

## Troubleshooting

### Build Fails on JitPack

1. Check the build log on JitPack.io
2. Verify `jitpack.yml` has correct JDK version
3. Ensure all dependencies are available on Maven Central or JCenter
4. Make sure NDK is not required (native libs should be prebuilt and included in jniLibs)

### Native Libraries

The native libraries (`android-libmuslim.so` and `libmuslim-jni.so`) must be:
1. Pre-built using `bash android.sh` in the muslimify project
2. Copied to `app/src/main/jniLibs/{abi}/` before committing
3. Included in the repository (not built on JitPack)

This is because JitPack doesn't have Android NDK configured by default.

## Notes

- The library includes native C libraries that are pre-built
- Supported architectures: ARM64 (arm64-v8a) and x86_64
- Minimum SDK: 24 (Android 7.0)
- Compile SDK: 36

## Artifact Information

- **Group ID**: `com.github.rizukirr`
- **Artifact ID**: `libmuslim`
- **Version**: Based on git tag (e.g., `1.0.0`)
- **Full coordinate**: `com.github.rizukirr:libmuslim:1.0.0`

## Published Artifacts

When published, JitPack provides:
- `libmuslim-{version}.aar` - The Android library
- `libmuslim-{version}.pom` - Maven metadata
- `libmuslim-{version}-sources.jar` - Source code (optional)
- `libmuslim-{version}.module` - Gradle module metadata
