#include <jni.h>
#include <string>
#include <vector>
#include <limits> // For std::numeric_limits
#include <android/log.h>
#include "llama.h" // Include llama.cpp headers

// Define a logging macro
#define LOG_TAG "NativeLib"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

// Global variables for the model and context
static llama_model* model = nullptr;
static llama_context* ctx = nullptr;

extern "C" JNIEXPORT jstring JNICALL
Java_com_pangeranwiguan_kizunacoreandroidaiplugingodot_MainActivity_loadModel(JNIEnv* env, jobject /* this */, jstring modelPath) {
    const char* modelPath_cstr = env->GetStringUTFChars(modelPath, nullptr);

    LOGD("Attempting to load GGUF model from path: %s", modelPath_cstr);

    // Check if the file exists
    FILE* file = fopen(modelPath_cstr, "rb");
    if (!file) {
        LOGE("Failed to open GGUF file at path: %s", modelPath_cstr);
        env->ReleaseStringUTFChars(modelPath, modelPath_cstr);
        return env->NewStringUTF("Failed to open GGUF file.");
    }
    fclose(file);

    // Initialize model parameters
    llama_model_params modelParams = llama_model_default_params();
    modelParams.use_mmap = false; // Enable memory mapping for faster loading
    modelParams.use_mlock = false; // Disable locking memory (optional)

    // Load the GGUF model using the updated function
    LOGD("Initializing llama_model_load_from_file...");
    model = llama_model_load_from_file(modelPath_cstr, modelParams);
    if (!model) {
        LOGE("Failed to load GGUF model from path: %s", modelPath_cstr);
        env->ReleaseStringUTFChars(modelPath, modelPath_cstr);
        return env->NewStringUTF("Failed to load model.");
    }

    LOGD("GGUF model loaded successfully.");

    // Initialize context parameters
    llama_context_params ctxParams = llama_context_default_params();
    ctxParams.n_ctx = 1024; // Set the context size (adjust as needed)

    // Create the context
    LOGD("Initializing llama_init_from_model...");
    ctx = llama_init_from_model(model, ctxParams);
    if (!ctx) {
        LOGE("Failed to create context for GGUF model.");
        llama_model_free(model);
        env->ReleaseStringUTFChars(modelPath, modelPath_cstr);
        return env->NewStringUTF("Failed to create context.");
    }

    LOGD("Context created successfully.");

    env->ReleaseStringUTFChars(modelPath, modelPath_cstr);
    return env->NewStringUTF("Model loaded successfully.");
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_pangeranwiguan_kizunacoreandroidaiplugingodot_MainActivity_generateResponse(JNIEnv* env, jobject /* this */, jstring input) {
    if (!ctx) {
        LOGE("Model not loaded. Cannot generate response.");
        return env->NewStringUTF("Model not loaded.");
    }

    const char* input_cstr = env->GetStringUTFChars(input, nullptr);
    LOGD("Generating response for input: %s", input_cstr);

    // Example: Generate a response (replace with actual inference logic)
    std::string response = "Response from AI"; // Replace with actual inference logic

    env->ReleaseStringUTFChars(input, input_cstr);
    LOGD("Generated response: %s", response.c_str());
    return env->NewStringUTF(response.c_str());
}

extern "C" JNIEXPORT void JNICALL
Java_com_pangeranwiguan_kizunacoreandroidaiplugingodot_MainActivity_freeModel(JNIEnv* /* env */, jobject /* this */) {
    if (ctx) {
        LOGD("Freeing context...");
        llama_free(ctx); // Free the context
        ctx = nullptr;
    }
    if (model) {
        LOGD("Freeing model...");
        llama_model_free(model); // Free the model
        model = nullptr;
    }
    LOGD("Model resources freed.");
}