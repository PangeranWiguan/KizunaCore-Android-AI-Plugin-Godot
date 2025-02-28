#include <jni.h>
#include <string>
#include <vector>
#include <limits> // For std::numeric_limits
#include "llama.h" // Include llama.cpp headers

// Global variables for the model and context
static llama_model* model = nullptr;
static llama_context* ctx = nullptr;

extern "C" JNIEXPORT jstring JNICALL
Java_com_pangeranwiguan_kizunacoreandroidaiplugingodot_MainActivity_loadModel(JNIEnv* env, jobject /* this */, jstring modelPath) {
    const char* modelPath_cstr = env->GetStringUTFChars(modelPath, nullptr);

    // Initialize model parameters
    llama_model_params modelParams = llama_model_default_params();
    modelParams.use_mmap = true; // Enable memory mapping for faster loading
    modelParams.use_mlock = false; // Disable locking memory (optional)

    // Load the GGUF model using the updated function
    model = llama_model_load_from_file(modelPath_cstr, modelParams);
    if (!model) {
        env->ReleaseStringUTFChars(modelPath, modelPath_cstr);
        return env->NewStringUTF("Failed to load model.");
    }

    // Initialize context parameters
    llama_context_params ctxParams = llama_context_default_params();
    ctxParams.n_ctx = 2048; // Set the context size (adjust as needed)

    // Create the context
    ctx = llama_init_from_model(model, ctxParams);
    if (!ctx) {
        llama_model_free(model);
        env->ReleaseStringUTFChars(modelPath, modelPath_cstr);
        return env->NewStringUTF("Failed to create context.");
    }

    env->ReleaseStringUTFChars(modelPath, modelPath_cstr);
    return env->NewStringUTF("Model loaded successfully.");
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_pangeranwiguan_kizunacoreandroidaiplugingodot_MainActivity_generateResponse(JNIEnv* env, jobject /* this */, jstring input) {
    if (!ctx) {
        return env->NewStringUTF("Model not loaded.");
    }

    const char* input_cstr = env->GetStringUTFChars(input, nullptr);

    // Get the vocabulary from the model
    const llama_vocab* vocab = llama_model_get_vocab(model);

    // Tokenize the input
    size_t token_count = -llama_tokenize(vocab, input_cstr, strlen(input_cstr), nullptr, 0, true, true);
    if (token_count > static_cast<size_t>(std::numeric_limits<int>::max())) {
        env->ReleaseStringUTFChars(input, input_cstr);
        return env->NewStringUTF("Token count exceeds maximum allowed value.");
    }
    int n_tokens_max = static_cast<int>(token_count);

    std::vector<llama_token> tokens(n_tokens_max);
    if (llama_tokenize(vocab, input_cstr, strlen(input_cstr), tokens.data(), tokens.size(), true, true) < 0) {
        env->ReleaseStringUTFChars(input, input_cstr);
        return env->NewStringUTF("Failed to tokenize input.");
    }

    // Example: Generate a response (replace with actual inference logic)
    std::string response = "Response from AI"; // Replace with actual inference logic

    env->ReleaseStringUTFChars(input, input_cstr);
    return env->NewStringUTF(response.c_str());
}

extern "C" JNIEXPORT void JNICALL
Java_com_pangeranwiguan_kizunacoreandroidaiplugingodot_MainActivity_freeModel(JNIEnv* /* env */, jobject /* this */) {
    if (ctx) {
        llama_free(ctx); // Free the context
        ctx = nullptr;
    }
    if (model) {
        llama_model_free(model); // Free the model
        model = nullptr;
    }
}