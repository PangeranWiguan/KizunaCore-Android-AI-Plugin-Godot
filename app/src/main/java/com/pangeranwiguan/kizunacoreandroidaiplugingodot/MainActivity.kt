package com.pangeranwiguan.kizunacoreandroidaiplugingodot

import android.os.Bundle
import android.util.Log
import androidx.appcompat.app.AppCompatActivity
import java.io.File

class MainActivity : AppCompatActivity() {

    companion object {
        private const val TAG = "MainActivity" // Tag for logging

        init {
            Log.d(TAG, "Loading native library...")
            System.loadLibrary("kizunacoreandroidaiplugingodot") // Load the compiled .so file
            Log.d(TAG, "Native library loaded successfully.")
        }
    }

    // Declare the native methods
    private external fun loadModel(modelPath: String): String
    private external fun generateResponse(input: String): String
    private external fun freeModel()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        // Copy the GGUF model from assets to internal storage
        Log.d(TAG, "Copying GGUF model from assets to internal storage...")
        val assetManager = applicationContext.assets
        val inputStream = assetManager.open("qwen2.5-0.5b-instruct-q4_k_m.gguf")
        val outFile = File(filesDir, "qwen2.5-0.5b-instruct-q4_k_m.gguf")
        if (!outFile.exists()) {
            Log.e(TAG, "GGUF file does not exist at path: ${outFile.absolutePath}")
            return
        }
        outFile.outputStream().use { outputStream ->
            inputStream.copyTo(outputStream)
        }
        Log.d(TAG, "GGUF model copied to internal storage.")

        // Load the GGUF model
        val modelPath = outFile.absolutePath
        Log.d(TAG, "Loading GGUF model from path: $modelPath")
        val loadResult = loadModel(modelPath)
        Log.d(TAG, "Load model result: $loadResult")

        // Generate a response
        val userMessage = "Hello! How can I assist you today?"
        Log.d(TAG, "Generating response for input: $userMessage")
        val aiResponse = generateResponse(userMessage)
        Log.d(TAG, "AI Response: $aiResponse")
    }

    override fun onDestroy() {
        super.onDestroy()

        // Free the model when the activity is destroyed
        Log.d(TAG, "Freeing model resources...")
        freeModel()
        Log.d(TAG, "Model resources freed.")
    }
}