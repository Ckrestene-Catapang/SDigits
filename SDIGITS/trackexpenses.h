#ifndef TRACKEXPENSES_H
#define TRACKEXPENSES_H

#include <QWidget>
#include <QStringList>
#include <QListWidget>
#include <QComboBox>
#include <QTableWidget>
#include <QLineEdit>
#include <QDateEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QDateTime>

class TrackExpenses : public QWidget
{
    Q_OBJECT

public:
    TrackExpenses(QWidget *parent = nullptr);
    ~TrackExpenses();

    QList<QStringList> getContributions() const;
    void addContributionData(const QStringList& data);
    void clearContributions();
    void loadContributions(const QString& filename);
    void saveHistory(const QString& filename);
    void showHistory(); // Remains public

signals:
    void showHistoryRequested();


private:
    QStringList sdgList;
    QList<QStringList> contributions;
    QListWidget *historyListWidget;
    QList<QString> historyLog;
    QLabel *dateLabel;
    QDateEdit *dateEdit;
    QLabel *descriptionLabel;
    QLineEdit *descriptionLineEdit;
    QLabel *amountLabel;
    QLineEdit *amountLineEdit;
    QLabel *unitsLabel;
    QLineEdit *unitsLineEdit;
    QLabel *sdgLabel;
    QListWidget *sdgSelectorWidget;
    QPushButton *addContributionButton;
    QTableWidget *contributionsTable;
    QLabel *filterSdgLabel;
    QComboBox *filterSdgComboBox;
    QPushButton *saveDataButton;
    QPushButton *showHistoryButton;
    QPushButton *backToDashboardButton; // Added back button
    QPushButton *deleteButton;

private slots:
    void addContribution();
    void saveData();
    void filterBySdg(const QString &sdg);
    void deleteContribution();
    void backToDashboard();
};

#endif // TRACKEXPENSES_H
