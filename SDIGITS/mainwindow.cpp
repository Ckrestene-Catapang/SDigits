#include "mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QListWidgetItem>
#include <QMap>
#include "trackexpenses.h"
#include "dashboard.h"
#include "profilewidget.h"
#include "createuserwidget.h"

const QString MainWindow::usersFilename = "sdigits_users.txt";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setFixedSize(640, 480);
    loadUsers();

    usernameLabel = new QLabel("Username:");
    usernameLineEdit = new QLineEdit();
    passwordLabel = new QLabel("Password:");
    passwordLineEdit = new QLineEdit();
    passwordLineEdit->setEchoMode(QLineEdit::Password);
    loginButton = new QPushButton("Login");
    registerButton = new QPushButton("Register");
    messageLabel = new QLabel("");

    QVBoxLayout *loginLayout = new QVBoxLayout();
    loginLayout->addWidget(usernameLabel);
    loginLayout->addWidget(usernameLineEdit);
    loginLayout->addWidget(passwordLabel);
    loginLayout->addWidget(passwordLineEdit);
    loginLayout->addWidget(loginButton);
    loginLayout->addWidget(registerButton);
    loginLayout->addWidget(messageLabel);

    QWidget *loginWidget = new QWidget();
    loginWidget->setLayout(loginLayout);
    setCentralWidget(loginWidget);

    dashboardWidget = new Dashboard(this);
    trackExpensesWidget = new TrackExpenses(this);
    profileWidget = new ProfileWidget(this);
    createUserWidget = new CreateUserWidget(this);

    dashboardWidget->hide();
    trackExpensesWidget->hide();
    profileWidget->hide();
    createUserWidget->hide();

    connect(loginButton, &QPushButton::clicked, this, &MainWindow::login);
    connect(registerButton, &QPushButton::clicked, this, static_cast<void (MainWindow::*)()>(&MainWindow::registerUser));

    connect(dashboardWidget, &Dashboard::showTrackExpenses, this, &MainWindow::showTrackExpensesScreen);
    connect(dashboardWidget, &Dashboard::showHistory, this, &MainWindow::showHistoryScreen);
    connect(dashboardWidget, &Dashboard::showProfile, this, &MainWindow::showProfileScreen);
    connect(dashboardWidget, &Dashboard::showCreateUser, this, &MainWindow::showCreateUserScreen);

    connect(createUserWidget, &CreateUserWidget::createUser, this, &MainWindow::handleCreateNewUser);
    connect(createUserWidget, &CreateUserWidget::backToDashboard, this, &MainWindow::goBackToDashboard);
    connect(createUserWidget, &CreateUserWidget::registrationResult, this, &MainWindow::handleRegistrationResult);

    connect(profileWidget, &ProfileWidget::switchToLogin, this, &MainWindow::showLoginScreen);
}

MainWindow::~MainWindow()
{
}

bool MainWindow::loadUsers()
{
    QFile file(usersFilename);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split(",");
            if (parts.size() == 2) {
                users[parts[0].trimmed()] = parts[1].trimmed();
            }
        }
        file.close();
        return true;
    }
    return false;
}

bool MainWindow::saveUsers()
{
    QFile file(usersFilename);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (const QString &username : users.keys()) {
            out << username << "," << users[username] << "\n";
        }
        file.close();
        return true;
    }
    return false;
}

void MainWindow::login()
{
    QString username = usernameLineEdit->text().trimmed();
    QString password = passwordLineEdit->text();

    if (users.contains(username) && users[username] == password) {
        loggedInUser = username;
        messageLabel->setText("Login successful!");
        showDashboardScreen();
        usernameLineEdit->clear();
        passwordLineEdit->clear();
    } else {
        messageLabel->setText("Login failed. Invalid username or password.");
    }
}

void MainWindow::registerUser() {
    QString username = usernameLineEdit->text().trimmed();
    QString password = passwordLineEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        messageLabel->setText("Username and password cannot be empty.");
        return;
    }

    if (users.contains(username)) {
        messageLabel->setText("Username already exists.");
        return;
    }

    users[username] = password;
    if (saveUsers()) {
        messageLabel->setText("Registration successful! You can now log in.");
        QString profileFilename = QString("sdigits_profile_%1.txt").arg(username);
        QFile profileFile(profileFilename);
        if (profileFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&profileFile);
            out << username << "\n";
            out << QDate::currentDate().toString("yyyy-MM-dd") << "\n";
            profileFile.close();
        } else {
            messageLabel->setText("Registration successful! You can now log in. (Could not save profile info.)");
        }
        usernameLineEdit->clear();
        passwordLineEdit->clear();

    } else {
        messageLabel->setText("Registration failed. Could not save user data.");
    }
}

void MainWindow::registerUser(const QString& initialUsername, const QString& initialPassword)
{
    QString username = initialUsername.isEmpty() ? usernameLineEdit->text().trimmed() : initialUsername;
    QString password = initialPassword.isEmpty() ? passwordLineEdit->text() : initialPassword;

    if (username.isEmpty() || password.isEmpty()) {
        createUserWidget->registrationResult(false, "Username and password cannot be empty.");
        return;
    }

    if (users.contains(username)) {
        createUserWidget->registrationResult(false, "Username already exists.");
        return;
    }

    users[username] = password;
    if (saveUsers()) {
        createUserWidget->registrationResult(true, "Registration successful! You can now log in.");
        QString profileFilename = QString("sdigits_profile_%1.txt").arg(username);
        QFile profileFile(profileFilename);
        if (profileFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&profileFile);
            out << username << "\n";
            out << QDate::currentDate().toString("yyyy-MM-dd") << "\n";
            profileFile.close();
        } else {
            createUserWidget->registrationResult(false, "Registration successful! You can now log in. (Could not save profile info.)");
        }
        if (centralWidget() == createUserWidget) {
            goBackToDashboard();
        } else {
            createUserWidget->registrationResult(true, "You can now log in.");
        }
        usernameLineEdit->clear();
        passwordLineEdit->clear();
    } else {
        createUserWidget->registrationResult(false, "Registration failed. Could not save user data.");
    }
}



void MainWindow::showDashboardScreen()
{
    setCentralWidget(dashboardWidget);
    dashboardWidget->show();
}

void MainWindow::showTrackExpensesScreen()
{
    trackExpensesWidget->loadContributions(QString("sdigits_data_%1.txt").arg(loggedInUser));
    setCentralWidget(trackExpensesWidget);
    trackExpensesWidget->show();
}

void MainWindow::showHistoryScreen()
{
    trackExpensesWidget->showHistory();
}

void MainWindow::showProfileScreen()
{
    profileWidget->loadProfile(loggedInUser);
    setCentralWidget(profileWidget);
    profileWidget->show();
}

void MainWindow::showCreateUserScreen()
{
    setCentralWidget(createUserWidget);
    createUserWidget->show();
    createUserWidget->clearInputFields();
}

void MainWindow::handleCreateNewUser(const QString& username, const QString& password)
{
    registerUser(username, password);
}

void MainWindow::goBackToDashboard()
{
    showDashboardScreen();
}

void MainWindow::showLoginScreen()
{
    usernameLineEdit->clear();
    passwordLineEdit->clear();
    messageLabel->clear();

    QWidget *loginWidget = new QWidget();
    QVBoxLayout *loginLayout = new QVBoxLayout(loginWidget);

    usernameLabel = new QLabel("Username:");
    usernameLineEdit = new QLineEdit();
    passwordLabel = new QLabel("Password:");
    passwordLineEdit = new QLineEdit();
    passwordLineEdit->setEchoMode(QLineEdit::Password);
    loginButton = new QPushButton("Login");
    messageLabel = new QLabel("");

    loginLayout->addWidget(usernameLabel);
    loginLayout->addWidget(usernameLineEdit);
    loginLayout->addWidget(passwordLabel);
    loginLayout->addWidget(passwordLineEdit);
    loginLayout->addWidget(loginButton);
    loginLayout->addWidget(messageLabel);

    loginWidget->setLayout(loginLayout);
    setCentralWidget(loginWidget);
    loginWidget->show();

    disconnect(loginButton, &QPushButton::clicked, this, &MainWindow::login);
    connect(loginButton, &QPushButton::clicked, this, &MainWindow::login);
}

void MainWindow::handleRegistrationResult(bool success, const QString& message)
{
    messageLabel->setText(message);
    if (success)
    {
        usernameLineEdit->clear();
        passwordLineEdit->clear();
    }
}
