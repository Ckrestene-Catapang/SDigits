#include "profilewidget.h"
#include "mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QDate>
#include <QMessageBox>
#include <QDebug>

ProfileWidget::ProfileWidget(QWidget *parent)
    : QWidget(parent)
{
    usernameLabel = new QLabel("Username: ");
    displayNameLabel = new QLabel("Display Name: ");
    displayNameEdit = new QLineEdit();
    passwordLabel = new QLabel("Password: ");
    passwordEdit = new QLineEdit();
    passwordEdit->setEchoMode(QLineEdit::Password);
    registrationDateLabel = new QLabel("Registration Date: ");
    messageLabel = new QLabel("");
    BackButton = new QPushButton("Back to Dashboard");
    saveButton = new QPushButton("Save Changes");
    deleteButton = new QPushButton("Delete Account");
    changeUsernameNotSupportedLabel = new QLabel("Changing username is not supported.");

    mainLayout = new QVBoxLayout();
    mainLayout->addWidget(usernameLabel);
    mainLayout->addWidget(displayNameLabel);
    mainLayout->addWidget(displayNameEdit);
    mainLayout->addWidget(passwordLabel);
    mainLayout->addWidget(passwordEdit);
    mainLayout->addWidget(registrationDateLabel);
    mainLayout->addWidget(changeUsernameNotSupportedLabel);
    mainLayout->addWidget(messageLabel);
    mainLayout->addWidget(BackButton);
    mainLayout->addWidget(saveButton);
    mainLayout->addWidget(deleteButton);
    setLayout(mainLayout);

    connect(BackButton, &QPushButton::clicked, this, &ProfileWidget::backToDashboard);
    connect(saveButton, &QPushButton::clicked, this, &ProfileWidget::handleSaveButton);
    connect(deleteButton, &QPushButton::clicked, this, &ProfileWidget::handleDeleteButton);
}

ProfileWidget::~ProfileWidget()
{
}

void ProfileWidget::loadProfile(const QString& username)
{
    QString filename = QString("sdigits_profile_%1.txt").arg(username);
    QFile profileFile(filename);

    if (profileFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&profileFile);
        QString displayName = in.readLine();
        QString registrationDate = in.readLine();
        profileFile.close();

        usernameLabel->setText("Username: " + username);
        displayNameLabel->setText("Display Name: ");
        displayNameEdit->setText(displayName);
        registrationDateLabel->setText("Registration Date: " + registrationDate);
        messageLabel->setText("");
    } else {
        usernameLabel->setText("Username: " + username);
        displayNameLabel->setText("Display Name: ");
        displayNameEdit->setText("Not available");
        registrationDateLabel->setText("Registration Date: Not available");
        messageLabel->setText("Profile file not found.");
    }
}

void ProfileWidget::saveProfile(const QString& username, const QString& displayName, const QString& password)
{
    QString filename = QString("sdigits_profile_%1.txt").arg(username);
    QFile profileFile(filename);

    if (profileFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&profileFile);
        out << displayName << "\n";
        out << registrationDateLabel->text().split(": ")[1] << "\n";
        profileFile.close();

        MainWindow* mainWindow = static_cast<MainWindow*>(parentWidget());
        if (mainWindow) {
            mainWindow->users[username] = password;
            if (mainWindow->saveUsers()) {
                messageLabel->setText("Profile updated successfully!");
            } else {
                messageLabel->setText("Error saving profile (password not saved).");
            }
        } else {
            messageLabel->setText("Error: Could not access main window.");
        }
    } else {
        messageLabel->setText("Error saving profile.");
    }
}

void ProfileWidget::deleteAccount(const QString& username)
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Delete Account",
                                  "Are you sure you want to delete your account? This action cannot be undone.",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        QString profileFilename = QString("sdigits_profile_%1.txt").arg(username);
        QString dataFilename = QString("sdigits_data_%1.txt").arg(username);
        QString historyFilename = QString("sdigits_history_%1.txt").arg(username);

        QFile::remove(profileFilename);
        QFile::remove(dataFilename);
        QFile::remove(historyFilename);

        MainWindow* mainWindow = static_cast<MainWindow*>(parentWidget());
        if (mainWindow) {
            mainWindow->users.remove(username);
            if (mainWindow->saveUsers()) {
                qDebug() << "User deleted from users.txt";
                emit switchToLogin(); // Go back to login screen
            } else {
                QMessageBox::critical(this, "Error", "Failed to delete account. Could not update user list.");
            }
        } else {
            QMessageBox::critical(this, "Error", "Failed to delete account. Could not access main window.");
        }
    }
}

void ProfileWidget::handleSaveButton()
{
    QString username = usernameLabel->text().split(": ")[1];
    QString displayName = displayNameEdit->text().trimmed();
    QString password = passwordEdit->text();
    saveProfile(username, displayName, password);
}

void ProfileWidget::handleDeleteButton()
{
    QString username = usernameLabel->text().split(": ")[1];
    deleteAccount(username);
}

void ProfileWidget::backToDashboard()
{
    this->hide();  // Hide the profile screen
    MainWindow *mainWindow = static_cast<MainWindow*>(parentWidget());
    if (mainWindow) {
        mainWindow->showDashboardScreen();  // Show the dashboard of the current session
    } else {
        qDebug() << "Error: Cannot return to dashboard. Main window is null.";
    }
}
