# AI Chat Application

A Qt-based GUI application that allows you to interact with an AI model via a chat interface. The application uses a Python script (leveraging HuggingFace’s Transformers library) to load and run the AI model. The UI supports model folder selection, displays a continuously updating conversation, and shows a loading animation while waiting for the AI response.

## Features

- **Persistent Conversation:**  
  Each message is appended to the chat window, maintaining a conversation history.

- **Loading Animation:**  
  An animated spinner is shown while waiting for the AI model to process the input.

- **Model Folder Selection:**  
  Easily select the folder containing your AI model directories. The application filters subdirectories and only lists those that contain required model files (e.g., `pytorch_model.bin`, `tf_model.h5`, `model.safetensors`, or `config.json`).

## Directory Structure

```
.
├── AIChatApp.cpp       # Qt UI implementation
├── AIChatApp.h         # Header for the AIChatApp class
├── CMakeLists.txt      # CMake build configuration
├── main.cpp            # Main entry point for the Qt application
├── run_model.py        # Python script to load and run the AI model
└── spinner.gif         # Animated spinner for loading indication
```

## Prerequisites

- **CMake 3.10+**  
- **Qt5 (Widgets module)**  
  - *Ubuntu/Debian:* `sudo apt-get install qtbase5-dev`  
  - *macOS:* Install via Homebrew: `brew install qt@5`  
  - *Windows:* Download via the [Qt installer](https://www.qt.io/download)
- **Python 3**  
- **Python Packages:**  
  ```bash
  pip install transformers torch
  ```

## Building the Application

You can build the project using the following one-liner (run from the project’s root directory):

```bash
rm -rf build && mkdir build && cd build && cmake .. && make
```

This command:
- Removes any existing build directory.
- Creates a new build directory.
- Configures the project using CMake.
- Builds the project.

## Resource Files

The project requires certain files (such as `run_model.py` and `spinner.gif`) to be accessible at runtime. You have two options:

1. **Copy Files During Build:**  
   The provided `CMakeLists.txt` can be configured to copy these resources into the build directory:
   ```cmake
   file(COPY ${CMAKE_SOURCE_DIR}/run_model.py DESTINATION ${CMAKE_BINARY_DIR})
   file(COPY ${CMAKE_SOURCE_DIR}/spinner.gif DESTINATION ${CMAKE_BINARY_DIR})
   ```
   This ensures the executable has access to the necessary files in the same folder.

2. **Set Working Directory at Runtime:**  
   The application’s code sets the working directory for the QProcess that calls the Python script. This lets you keep the resources in the source directory (adjust the path in your code if needed).

## Running the Application

After building, navigate to the build directory and run the executable:

```bash
./AIChat
```

Once running:
- Use the **Select Models Folder** button to choose the folder containing your model directories.
- Enter your message in the input field and click **Send**.
- A loading spinner appears while the Python script processes your request, and then the AI's response is displayed in the conversation.

## Troubleshooting

- **Resource File Not Found:**  
  Ensure that `run_model.py` and `spinner.gif` are either copied to the build directory or that the working directory is correctly set.

- **Python Script Issues:**  
  Test the Python script separately using:
  ```bash
  python run_model.py <model_dir> "<user_input>"
  ```
  Ensure that your Python environment is properly configured and the necessary packages are installed.

- **Qt/Build Errors:**  
  If CMake cannot find your Qt installation, try setting the `CMAKE_PREFIX_PATH` variable:
  ```bash
  cmake -DCMAKE_PREFIX_PATH=/path/to/Qt5 ..
  ```

## License


## Acknowledgments

- [HuggingFace Transformers](https://github.com/huggingface/transformers)
- Qt Framework

Happy coding!
