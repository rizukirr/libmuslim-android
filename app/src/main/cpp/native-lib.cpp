#include "prayertimes.h"
#include <jni.h>

// ---- Macros for class paths and signatures ----

#define PKG_PATH "com/rizukirr/libmuslim"

#define CLASS_PrayerDateTime PKG_PATH "/PrayerDateTime"
#define CLASS_PrayerTimeResult PKG_PATH "/PrayerTimeResult"
#define CLASS_ArrayList "java/util/ArrayList"

#define SIG_PrayerDateTime_CTOR "(II)V"

// Constructor of PrayerTimeResult takes 7 PrayerDateTime objects
#define SIG_PrayerTimeResult_CTOR                                              \
  "(L" CLASS_PrayerDateTime ";"                                                \
  "L" CLASS_PrayerDateTime ";"                                                 \
  "L" CLASS_PrayerDateTime ";"                                                 \
  "L" CLASS_PrayerDateTime ";"                                                 \
  "L" CLASS_PrayerDateTime ";"                                                 \
  "L" CLASS_PrayerDateTime ";"                                                 \
  "L" CLASS_PrayerDateTime ";)V"

#define SIG_ArrayList_CTOR "()V"
#define SIG_ArrayList_ADD "(Ljava/lang/Object;)Z"

// ---- Cached references ----

static jclass gCls_PrayerDateTime = nullptr;
static jmethodID gMtd_PrayerDateTime_ctor = nullptr;

static jclass gCls_PrayerTimeResult = nullptr;
static jmethodID gMtd_PrayerTimeResult_ctor = nullptr;

static jclass gCls_ArrayList = nullptr;
static jmethodID gMtd_ArrayList_ctor = nullptr;
static jmethodID gMtd_ArrayList_add = nullptr;

// ---- Date utilities for efficient iteration ----

static inline bool is_leap(int year) {
    return (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0));
}

static inline int days_in_month(int year, int month) {
    if (month == 2) return is_leap(year) ? 29 : 28;
    if (month == 4 || month == 6 || month == 9 || month == 11) return 30;
    return 31;
}

// ---- Helper to cache class & method IDs ----
extern "C" JNIEXPORT void JNICALL
Java_com_rizukirr_libmuslim_LibMuslim_ensureJniRefs(JNIEnv *env,
        jobject /* thiz */) {
    // Cache PrayerDateTime class and constructor
    if (gCls_PrayerDateTime == nullptr) {
        jclass local = env->FindClass(CLASS_PrayerDateTime);
        gCls_PrayerDateTime = (jclass) env->NewGlobalRef(local);
        env->DeleteLocalRef(local);
        gMtd_PrayerDateTime_ctor = env->GetMethodID(gCls_PrayerDateTime, "<init>",
                SIG_PrayerDateTime_CTOR);
    }

    // Cache PrayerTimeResult class and constructor
    if (gCls_PrayerTimeResult == nullptr) {
        jclass local = env->FindClass(CLASS_PrayerTimeResult);
        gCls_PrayerTimeResult = (jclass) env->NewGlobalRef(local);
        env->DeleteLocalRef(local);
        gMtd_PrayerTimeResult_ctor = env->GetMethodID(
                gCls_PrayerTimeResult, "<init>", SIG_PrayerTimeResult_CTOR);
    }

    // Cache ArrayList class, constructor, and add method
    if (gCls_ArrayList == nullptr) {
        jclass local = env->FindClass(CLASS_ArrayList);
        gCls_ArrayList = (jclass) env->NewGlobalRef(local);
        env->DeleteLocalRef(local);
        gMtd_ArrayList_ctor = env->GetMethodID(gCls_ArrayList, "<init>", SIG_ArrayList_CTOR);
        gMtd_ArrayList_add =
                env->GetMethodID(gCls_ArrayList, "add", SIG_ArrayList_ADD);
    }
}

// ---- Helper to create PrayerDateTime object ----

static jobject newPrayerDateTime(JNIEnv *env, double timeHours) {
    double hours = timeHours;
    double minutes = ceil((timeHours - hours) * 60 + 0.5);
    return env->NewObject(gCls_PrayerDateTime, gMtd_PrayerDateTime_ctor, (int) hours,
            (int) minutes);
}

// ---- Single date JNI method ----

extern "C" JNIEXPORT jobject JNICALL
Java_com_rizukirr_libmuslim_LibMuslim_calculatePrayerTimes(
        JNIEnv *env, jobject /* thiz */, jint year, jint month, jint day,
        jdouble latitude, jdouble longitude, jdouble timezone) {

    if (gCls_PrayerTimeResult == nullptr) return nullptr; // JNI refs not ready

    struct PrayerTimes pt =
            calculate_prayer_times(year, month, day, latitude, longitude, timezone);

    jobject fajr = newPrayerDateTime(env, pt.fajr);
    jobject sunrise = newPrayerDateTime(env, pt.sunrise);
    jobject dhuha = newPrayerDateTime(env, pt.dhuha);
    jobject dhuhr = newPrayerDateTime(env, pt.dhuhr);
    jobject asr = newPrayerDateTime(env, pt.asr);
    jobject maghrib = newPrayerDateTime(env, pt.maghrib);
    jobject isha = newPrayerDateTime(env, pt.isha);

    jobject result = env->NewObject(gCls_PrayerTimeResult, gMtd_PrayerTimeResult_ctor,
            fajr, sunrise, dhuha, dhuhr, asr, maghrib, isha);

    // Clean up all local references
    env->DeleteLocalRef(fajr);
    env->DeleteLocalRef(sunrise);
    env->DeleteLocalRef(dhuha);
    env->DeleteLocalRef(dhuhr);
    env->DeleteLocalRef(asr);
    env->DeleteLocalRef(maghrib);
    env->DeleteLocalRef(isha);

    return result;
}

// ---- Date range JNI method (memory and speed optimized) ----

extern "C" JNIEXPORT jobject JNICALL
Java_com_rizukirr_libmuslim_LibMuslim_calculatePrayerTimesForDateRange(
        JNIEnv *env, jobject /* thiz */, jint startYear, jint startMonth,
        jint startDay, jint endYear, jint endMonth, jint endDay, jdouble latitude,
        jdouble longitude, jdouble timezone) {

    if (gCls_ArrayList == nullptr) return nullptr; // JNI refs not ready

    jobject prayerTimeList = env->NewObject(gCls_ArrayList, gMtd_ArrayList_ctor);

    int y = startYear, m = startMonth, d = startDay;

    while (y < endYear || (y == endYear && m < endMonth) ||
            (y == endYear && m == endMonth && d <= endDay)) {

        struct PrayerTimes pt = calculate_prayer_times(y, m, d, latitude, longitude, timezone);

        jobject fajr = newPrayerDateTime(env, pt.fajr);
        jobject sunrise = newPrayerDateTime(env, pt.sunrise);
        jobject dhuha = newPrayerDateTime(env, pt.dhuha);
        jobject dhuhr = newPrayerDateTime(env, pt.dhuhr);
        jobject asr = newPrayerDateTime(env, pt.asr);
        jobject maghrib = newPrayerDateTime(env, pt.maghrib);
        jobject isha = newPrayerDateTime(env, pt.isha);

        jobject resultForDay = env->NewObject(
                gCls_PrayerTimeResult, gMtd_PrayerTimeResult_ctor, fajr, sunrise, dhuha,
                dhuhr, asr, maghrib, isha);

        env->CallBooleanMethod(prayerTimeList, gMtd_ArrayList_add, resultForDay);

        // CRITICAL: Clean up all local references created inside the loop
        env->DeleteLocalRef(fajr);
        env->DeleteLocalRef(sunrise);
        env->DeleteLocalRef(dhuha);
        env->DeleteLocalRef(dhuhr);
        env->DeleteLocalRef(asr);
        env->DeleteLocalRef(maghrib);
        env->DeleteLocalRef(isha);
        env->DeleteLocalRef(resultForDay);

        d++;
        if (d > days_in_month(y, m)) {
            d = 1;
            m++;
            if (m > 12) {
                m = 1;
                y++;
            }
        }
    }

    return prayerTimeList;
}
