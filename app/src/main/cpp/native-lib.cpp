#include <jni.h>
#include <string>
#include "llama.h"

extern "C" JNIEXPORT jstring JNICALL
Java_com_pangeranwiguan_kizunacoreandroidaiplugingodot_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++ for KizunaCore by Pangeran Wiguan.";
    return env->NewStringUTF(hello.c_str());
}