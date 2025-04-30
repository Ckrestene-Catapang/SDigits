#ifndef PROFILEWIDGET_H
#define PROFILEWIDGET_H


#include "mainwindow.h"
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QString>
#include <QFile>
#include <QDate>
#include <QPushButton>

class ProfileWidget : public QWidget
{
    Q_OBJECT
public slots:
    void backToDashboard();


public:
    ProfileWidget(QWidget *parent = nullptr);
    ~ProfileWidget();

    void loadProfile(const QString& username);
    void saveProfile(const QString& username, const QString& displayName, const QString& password);
    void deleteAccount(const QString& username);

signals:
    void switchToLogin();

private:
    QLabel *usernameLabel;
    QLabel *displayNameLabel;
    QLineEdit *displayNameEdit;
    QLabel *passwordLabel;
    QLineEdit *passwordEdit;
    QLabel *registrationDateLabel;
    QLabel *messageLabel;
    QPushButton *saveButton;
    QPushButton *deleteButton;
     QPushButton *BackButton;
    QVBoxLayout *mainLayout;
    QLabel *changeUsernameNotSupportedLabel;
    QPushButton *backToLoginButton;
private slots:
    void handleSaveButton();
    void handleDeleteButton();
};

#endif // PROFILEWIDGET_H
