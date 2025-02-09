#include "AIChatApp.h"
#include <QDebug>
#include <QScrollBar>

AIChatApp::AIChatApp(QWidget *parent) : QWidget(parent) {
    // Set up main layout
    mainLayout = new QVBoxLayout(this);

    // Chat display area
    chatBox = new QTextEdit(this);
    chatBox->setReadOnly(true);
    mainLayout->addWidget(chatBox);

    // Model selection area
    QHBoxLayout *modelLayout = new QHBoxLayout();
    modelSelector = new QComboBox(this);
    modelLayout->addWidget(modelSelector);

    selectFolderButton = new QPushButton("Select Models Folder", this);
    modelLayout->addWidget(selectFolderButton);
    connect(selectFolderButton, &QPushButton::clicked, this, &AIChatApp::selectModelFolder);

    mainLayout->addLayout(modelLayout);

    // Input layout for user message and send button
    inputLayout = new QHBoxLayout();
    inputBox = new QLineEdit(this);
    inputLayout->addWidget(inputBox);
    sendButton = new QPushButton("Send", this);
    inputLayout->addWidget(sendButton);
    mainLayout->addLayout(inputLayout);
    connect(sendButton, &QPushButton::clicked, this, &AIChatApp::sendMessage);

    // Loading animation label (hidden by default)
    loadingLabel = new QLabel(this);
    loadingMovie = new QMovie("spinner.gif"); // Ensure spinner.gif is available or update the path.
    loadingLabel->setMovie(loadingMovie);
    loadingLabel->setVisible(false);
    mainLayout->addWidget(loadingLabel);

    // Set initial models folder to the current directory and load available models.
    modelsFolder = QDir::currentPath();
    loadModelList(modelsFolder);

    // Connect model selector to update the selected model.
    connect(modelSelector, &QComboBox::currentTextChanged, this, &AIChatApp::setModel);
}

void AIChatApp::loadModelList(const QString &folderPath) {
    // Clear current model list
    modelSelector->clear();

    QDir dir(folderPath);
    if (!dir.exists()) {
        chatBox->append("Models folder does not exist: " + folderPath);
        chatBox->verticalScrollBar()->setValue(chatBox->verticalScrollBar()->maximum());
        return;
    }
    QStringList allDirs = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    // Files that indicate a valid model directory
    QStringList validModelFiles = {"pytorch_model.bin", "tf_model.h5", "model.safetensors", "config.json"};
    QStringList validDirs;
    for (const QString &subDirName : allDirs) {
        QDir subDir(dir.filePath(subDirName));
        bool hasModelFile = false;
        for (const QString &modelFile : validModelFiles) {
            if (subDir.exists(modelFile)) {
                hasModelFile = true;
                break;
            }
        }
        if (hasModelFile) {
            validDirs.append(subDirName);
        }
    }

    if (validDirs.isEmpty()) {
        chatBox->append("No valid AI model directories found in " + folderPath);
    } else {
        modelSelector->addItems(validDirs);
        selectedModel = validDirs.first();
    }
    chatBox->verticalScrollBar()->setValue(chatBox->verticalScrollBar()->maximum());
}

void AIChatApp::setModel(const QString &modelName) {
    selectedModel = modelName;
    chatBox->append("<b>Selected Model:</b> " + modelName);
    chatBox->verticalScrollBar()->setValue(chatBox->verticalScrollBar()->maximum());
}

void AIChatApp::selectModelFolder() {
    QString folder = QFileDialog::getExistingDirectory(this, "Select Models Folder", modelsFolder);
    if (!folder.isEmpty()) {
        modelsFolder = folder;
        chatBox->append("<b>Selected models folder:</b> " + modelsFolder);
        chatBox->verticalScrollBar()->setValue(chatBox->verticalScrollBar()->maximum());
        loadModelList(modelsFolder);
    }
}

void AIChatApp::sendMessage() {
    QString userInput = inputBox->text().trimmed();
    if (userInput.isEmpty())
        return;

    // Append user message to conversation
    chatBox->append("<b>You:</b> " + userInput);
    chatBox->verticalScrollBar()->setValue(chatBox->verticalScrollBar()->maximum());
    inputBox->clear();

    // Show loading animation and disable input controls while waiting
    loadingLabel->setVisible(true);
    loadingMovie->start();
    sendButton->setEnabled(false);
    inputBox->setEnabled(false);

    // Construct the full model path using the selected folder and model subdirectory
    QString modelPath = modelsFolder + "/" + selectedModel;

    // Prepare to call the Python script
    QString program = "python"; // Use "python3" if needed
    QStringList arguments;
    arguments << "run_model.py" << modelPath << userInput;

    QProcess *process = new QProcess(this);
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            [this, process](int exitCode, QProcess::ExitStatus exitStatus) {
                // Stop loading animation and re-enable controls
                loadingMovie->stop();
                loadingLabel->setVisible(false);
                sendButton->setEnabled(true);
                inputBox->setEnabled(true);

                if (exitStatus == QProcess::CrashExit) {
                    chatBox->append("<b>AI:</b> [Error: Process crashed]");
                } else {
                    QString output = process->readAllStandardOutput().trimmed();
                    chatBox->append("<b>AI:</b> " + output);
                }
                chatBox->verticalScrollBar()->setValue(chatBox->verticalScrollBar()->maximum());
                process->deleteLater();
            });

    process->start(program, arguments);
}
