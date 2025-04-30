#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>

class Dashboard : public QWidget
{
    Q_OBJECT

public:
    Dashboard(QWidget *parent = nullptr);
    ~Dashboard();

signals:
    void showTrackExpenses();
    void showHistory();
    void showProfile();
    void showCreateUser();
    void showLogin();

private:
    QPushButton *trackExpensesButton;
    QPushButton *historyButton;
    QPushButton *profileButton;
    QPushButton *createUserButton;
    QPushButton *switchAccountButton;
    QVBoxLayout *mainLayout;
    QPushButton *backToLoginButton;

private slots:
    void goToTrackExpenses();
    void goToHistory();
    void goToProfile();
    void goToCreateUser();
    void goToSwitchAccount();
};

#endif // DASHBOARD_H
