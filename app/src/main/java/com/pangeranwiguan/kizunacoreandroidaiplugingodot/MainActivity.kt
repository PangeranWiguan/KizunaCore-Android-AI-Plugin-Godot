package com.pangeranwiguan.kizunacoreandroidaiplugingodot

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import java.io.File

class MainActivity : AppCompatActivity() {

    companion object {
        init {
            System.loadLibrary("kizunacoreandroidaiplugingodot") // Load the compiled .so file
        }
    }

    // Declare the native methods
    private external fun loadModel(modelPath: String): String
    private external fun generateResponse(input: String): String
    private external fun freeModel()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        // Copy the GGUF model from assets to internal storage
        val assetManager = applicationContext.assets
        val inputStream = assetManager.open("qwen2.5-0.5b-instruct-q4_k_m.gguf")
        val outFile = File(filesDir, "qwen2.5-0.5b-instruct-q4_k_m.gguf")
        outFile.outputStream().use { outputStream ->
            inputStream.copyTo(outputStream)
        }

        // Load the GGUF model
        val modelPath = outFile.absolutePath
        val loadResult = loadModel(modelPath)
        println(loadResult)

        // Generate a response
        val userMessage = "Hello! How can I assist you today?"
        val aiResponse = generateResponse(userMessage)
        println(aiResponse)
    }

    override fun onDestroy() {
        super.onDestroy()

        // Free the model when the activity is destroyed
        freeModel()
    }
}