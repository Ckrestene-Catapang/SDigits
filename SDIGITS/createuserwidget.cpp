    #include "createuserwidget.h"
#include <QMessageBox> // Include this if you use QMessageBox

CreateUserWidget::CreateUserWidget(QWidget *parent)
    : QWidget(parent) {
    usernameLabel = new QLabel("New Username:");
    usernameLineEdit = new QLineEdit();
    passwordLabel = new QLabel("New Password:");
    passwordLineEdit = new QLineEdit();
    passwordLineEdit->setEchoMode(QLineEdit::Password);
    registerButton = new QPushButton("Create User");
    backButton = new QPushButton("Back to Dashboard");
    messageLabel = new QLabel("");

    mainLayout = new QVBoxLayout();
    mainLayout->addWidget(usernameLabel);
    mainLayout->addWidget(usernameLineEdit);
    mainLayout->addWidget(passwordLabel);
    mainLayout->addWidget(passwordLineEdit);
    mainLayout->addWidget(registerButton);
    mainLayout->addWidget(backButton);
    mainLayout->addWidget(messageLabel);
    setLayout(mainLayout);

    connect(registerButton, &QPushButton::clicked, this, &CreateUserWidget::registerNewUser);
    connect(backButton, &QPushButton::clicked, this, &CreateUserWidget::goBack);
}

CreateUserWidget::~CreateUserWidget() {
}

void CreateUserWidget::registerNewUser() {
    QString username = usernameLineEdit->text().trimmed();
    QString password = passwordLineEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        emit registrationResult(false, "Username and password cannot be empty.");
        return;
    }

    emit createUser(username, password); // Emit the createUser signal
}

void CreateUserWidget::goBack() {
    emit backToDashboard();
}

void CreateUserWidget::clearInputFields() {
    usernameLineEdit->clear();
    passwordLineEdit->clear();
    messageLabel->clear();
}
