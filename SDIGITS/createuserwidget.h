    #ifndef CREATEUSERWIDGET_H
#define CREATEUSERWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>

class CreateUserWidget : public QWidget {
    Q_OBJECT
public:
    CreateUserWidget(QWidget *parent = nullptr);
    ~CreateUserWidget();

signals:
    void createUser(const QString& username, const QString& password);
    void backToDashboard();

    void registrationResult(bool success, const QString& message); // Signal declaration

private:
    QLabel *usernameLabel;
    QLineEdit *usernameLineEdit;
    QLabel *passwordLabel;
    QLineEdit *passwordLineEdit;
    QPushButton *registerButton;
    QPushButton *backButton;
    QLabel *messageLabel;
    QVBoxLayout *mainLayout;

public slots:
    void registerNewUser();
    void goBack();
    void clearInputFields();
};

#endif // CREATEUSERWIDGET_H
