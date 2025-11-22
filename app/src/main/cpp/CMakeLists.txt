cmake_minimum_required(VERSION 3.22.1)

project("libmuslim")

# Build the JNI wrapper library that will be loaded by the app
add_library(muslim-jni SHARED
        native-lib.cpp)

# Include directory for prayertimes.h
target_include_directories(muslim-jni PRIVATE
        ${CMAKE_SOURCE_DIR}/include)

# Set the path to the prebuilt libraries
set(prebuilt_libs_dir ${CMAKE_SOURCE_DIR}/../jniLibs)

# Import the prebuilt libmuslim.so that contains prayer calculation functions
add_library(muslim-core SHARED IMPORTED)
set_target_properties(muslim-core PROPERTIES
        IMPORTED_LOCATION ${prebuilt_libs_dir}/${ANDROID_ABI}/libmuslim.so)

# Add the jniLibs directory to the link directories
link_directories(${prebuilt_libs_dir}/${ANDROID_ABI})


# Link the JNI wrapper against the prebuilt library
target_link_libraries(muslim-jni
        muslim-core
        android
        log)

# Set the output name to produce libmuslim-jni.so
set_target_properties(muslim-jni PROPERTIES
        OUTPUT_NAME "muslim-jni")
