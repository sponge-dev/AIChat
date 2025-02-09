#ifndef AICHATAPP_H
#define AICHATAPP_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QDir>
#include <QProcess>
#include <QLabel>
#include <QMovie>
#include <QFileDialog>

class AIChatApp : public QWidget {
    Q_OBJECT

public:
    explicit AIChatApp(QWidget *parent = nullptr);
    ~AIChatApp() {}

private:
    // Layouts
    QVBoxLayout *mainLayout;
    QHBoxLayout *inputLayout;

    // Chat display & input
    QTextEdit *chatBox;
    QLineEdit *inputBox;
    QPushButton *sendButton;

    // Model selection widgets
    QComboBox *modelSelector;
    QPushButton *selectFolderButton;
    QString selectedModel;
    QString modelsFolder; // Path to the folder that contains model subdirectories

    // Loading animation
    QLabel *loadingLabel;
    QMovie *loadingMovie;

private slots:
    void loadModelList(const QString &folderPath);
    void setModel(const QString &modelName);
    void sendMessage();
    void selectModelFolder();
};

#endif // AICHATAPP_H
