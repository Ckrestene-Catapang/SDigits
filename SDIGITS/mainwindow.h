#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringList>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QFile>
#include "trackexpenses.h"
#include "dashboard.h"
#include "profilewidget.h"
#include "createuserwidget.h"

class ProfileWidget;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QString loggedInUser;
    QMap<QString, QString> users;
    bool saveUsers();

public slots:
    void login();
    void registerUser(); // Changed: No parameters
    void registerUser(const QString& initialUsername, const QString& initialPassword); // Added: Overload for CreateUserWidget
    void showDashboardScreen();
    void showTrackExpensesScreen();
    void showHistoryScreen();
    void showProfileScreen();
    void showCreateUserScreen();
    void handleCreateNewUser(const QString& username, const QString& password);
    void goBackToDashboard();
    void showLoginScreen();
    void handleRegistrationResult(bool success, const QString& message);

private:
    bool loadUsers();
    static const QString usersFilename;
    QLabel *usernameLabel;
    QLineEdit *usernameLineEdit;
    QLabel *passwordLabel;
    QLineEdit *passwordLineEdit;
    QPushButton *loginButton;
    QPushButton *registerButton;
    QLabel *messageLabel;
    Dashboard *dashboardWidget;
    TrackExpenses *trackExpensesWidget;
    ProfileWidget *profileWidget;
    CreateUserWidget *createUserWidget;
};

#endif // MAINWINDOW_H
