#include "trackexpenses.h"
#include "mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDialog>
#include <QVBoxLayout>
#include <QListWidget>

TrackExpenses::TrackExpenses(QWidget *parent)
    : QWidget(parent)
{
    sdgList = {
        "No Poverty",
        "Zero Hunger",
        "Good Health and Well-being",
        "Quality Education",
        "Gender Equality",
        "Clean Water and Sanitation",
        "Affordable and Clean Energy",
        "Decent Work and Economic Growth",
        "Industry, Innovation and Infrastructure",
        "Reduced Inequalities",
        "Sustainable Cities and Communities",
        "Responsible Consumption and Production",
        "Climate Action",
        "Life Below Water",
        "Life on Land",
        "Peace, Justice and Strong Institutions",
        "Partnerships for the Goals"
    };

    // Initialize UI elements
    dateLabel = new QLabel("Date:");
    dateEdit = new QDateEdit(QDate::currentDate());
    descriptionLabel = new QLabel("Description:");
    descriptionLineEdit = new QLineEdit();
    amountLabel = new QLabel("Amount:");
    amountLineEdit = new QLineEdit();
    unitsLabel = new QLabel("Units:");
    unitsLineEdit = new QLineEdit();
    sdgLabel = new QLabel("SDG(s):");
    sdgSelectorWidget = new QListWidget();
    sdgSelectorWidget->addItems(sdgList);
    sdgSelectorWidget->setSelectionMode(QAbstractItemView::MultiSelection);
    addContributionButton = new QPushButton("Add Contribution");
    contributionsTable = new QTableWidget();
    contributionsTable->setColumnCount(5);
    contributionsTable->setHorizontalHeaderLabels({"Date", "Description", "Amount", "Units", "SDGs"});
    filterSdgLabel = new QLabel("Filter by SDG:");
    filterSdgComboBox = new QComboBox();
    filterSdgComboBox->addItem("All SDGs");
    filterSdgComboBox->addItems(sdgList);
    saveDataButton = new QPushButton("Save Data");
    showHistoryButton = new QPushButton("Show History");
    deleteButton = new QPushButton("Delete");
    backToDashboardButton = new QPushButton("Back to Dashboard");

    // Create Layouts
    QFormLayout *inputLayout = new QFormLayout();
    inputLayout->addRow(dateLabel, dateEdit);
    inputLayout->addRow(descriptionLabel, descriptionLineEdit);
    inputLayout->addRow(amountLabel, amountLineEdit);
    inputLayout->addRow(unitsLabel, unitsLineEdit);
    inputLayout->addRow(sdgLabel, sdgSelectorWidget);
    inputLayout->addWidget(deleteButton);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(addContributionButton);
    buttonLayout->addWidget(saveDataButton);
    buttonLayout->addWidget(showHistoryButton);
    buttonLayout->addWidget(backToDashboardButton);

    QHBoxLayout *filterLayout = new QHBoxLayout();
    filterLayout->addWidget(filterSdgLabel);
    filterLayout->addWidget(filterSdgComboBox);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(inputLayout);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addLayout(filterLayout);
    mainLayout->addWidget(contributionsTable);
    mainLayout->addWidget(backToDashboardButton);
    setLayout(mainLayout);

    // Connect signals and slots
    connect(addContributionButton, &QPushButton::clicked, this, &TrackExpenses::addContribution);
    connect(saveDataButton, &QPushButton::clicked, this, &TrackExpenses::saveData);
    connect(filterSdgComboBox, &QComboBox::currentTextChanged, this, &TrackExpenses::filterBySdg);
    connect(showHistoryButton, &QPushButton::clicked, this, &TrackExpenses::showHistory);
    connect(deleteButton, &QPushButton::clicked, this, &TrackExpenses::deleteContribution);
    connect(backToDashboardButton, &QPushButton::clicked, this, &TrackExpenses::backToDashboard); // Connect back button

    historyListWidget = nullptr;
}

TrackExpenses::~TrackExpenses()
{
    if (historyListWidget) delete historyListWidget;
}

QList<QStringList> TrackExpenses::getContributions() const
{
    return contributions;
}

void TrackExpenses::addContributionData(const QStringList& data)
{
    contributions.append(data);
    int row = contributionsTable->rowCount();
    contributionsTable->insertRow(row);
    for (int i = 0; i < data.size(); ++i) {
        contributionsTable->setItem(row, i, new QTableWidgetItem(data[i]));
    }
}

void TrackExpenses::clearContributions()
{
    contributions.clear();
    contributionsTable->clearContents();
    contributionsTable->setRowCount(0);
}

void TrackExpenses::loadContributions(const QString& filename)
{
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        clearContributions();
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split(",");
            if (parts.size() == 5) {
                addContributionData(parts);
            }
        }
        file.close();
    }
    // We no longer call loadHistory here for display
}

void TrackExpenses::saveHistory(const QString& filename)
{
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        // We still write to the file based on the historyLog
        for (const QString& event : historyLog) {
            out << event << "\n";
        }
        file.close();
    }
}

void TrackExpenses::addContribution()
{
    QDate date = dateEdit->date();
    QString description = descriptionLineEdit->text();
    QString amount = amountLineEdit->text();
    QString units = unitsLineEdit->text();
    QList<QListWidgetItem*> selectedSdgItems = sdgSelectorWidget->selectedItems();
    QStringList selectedSdgs;

    if (selectedSdgItems.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please select at least one SDG.");
        return;
    }

    for (QListWidgetItem *item : selectedSdgItems) {
        selectedSdgs << item->text();
    }

    QStringList newContribution;
    newContribution << date.toString("yyyy-MM-dd") << description << amount << units << selectedSdgs.join(", ");
    addContributionData(newContribution);
    contributions.append(newContribution);

    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    historyLog.append(QString("[%1] - Added: %2 (%3 %4) - SDGs: %5").arg(timestamp).arg(description).arg(amount).arg(units).arg(selectedSdgs.join(", ")));
    saveHistory(QString("sdigits_history_%1.txt").arg(static_cast<MainWindow*>(parentWidget())->loggedInUser));

    dateEdit->setDate(QDate::currentDate());
    descriptionLineEdit->clear();
    amountLineEdit->clear();
    unitsLineEdit->clear();
    sdgSelectorWidget->clearSelection();
}

void TrackExpenses::saveData()
{
    QString filename = QString("sdigits_data_%1.txt").arg(static_cast<MainWindow*>(parentWidget())->loggedInUser);
    QFile file(filename);

    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (const QStringList& contribution : contributions) {
            out << contribution.join(",") << "\n";
        }
        file.close();
        QMessageBox::information(this, "Success", "Data saved!");
    } else {
        QMessageBox::critical(this, "Error", "Could not open file for saving data.");
    }
    saveHistory(filename.replace("sdigits_data", "sdigits_history"));
}

void TrackExpenses::filterBySdg(const QString &sdg)
{
    for (int row = 0; row < contributionsTable->rowCount(); ++row) {
        bool match = false;
        if (sdg == "All SDGs") {
            match = true;
        } else {
            QString sdgsInRow = contributionsTable->item(row, 4)->text();
            if (sdgsInRow.contains(sdg)) {
                match = true;
            }
        }
        contributionsTable->setRowHidden(row, !match);
    }
}

void TrackExpenses::showHistory()
{
    QDialog *historyDialog = new QDialog(this);
    historyDialog->setWindowTitle("Activity History");
    QVBoxLayout *layout = new QVBoxLayout(historyDialog);
    QListWidget *historyDisplay = new QListWidget();

    QString filename = QString("sdigits_history_%1.txt").arg(static_cast<MainWindow*>(parentWidget())->loggedInUser);
    QFile historyFile(filename);

    if (historyFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&historyFile);
        while (!in.atEnd()) {
            QString line = in.readLine();
            historyDisplay->addItem(line);
        }
        historyFile.close();
    } else {
        historyDisplay->addItem("No history available.");
    }

    layout->addWidget(historyDisplay);
    historyDialog->setLayout(layout);
    historyDialog->setModal(true);
    historyDialog->exec();
}

void TrackExpenses::backToDashboard()
{
    // 1.  Check the parent
    if (parentWidget()) {
        qDebug() << "TrackExpenses::backToDashboard: Parent is: " << parentWidget();

        // 2. Cast the parent
        MainWindow* mainWindow = dynamic_cast<MainWindow*>(parentWidget());
        if (mainWindow) {
            qDebug() << "TrackExpenses::backToDashboard: cast was successful";
            // 3. Access the central widget and set it.
            mainWindow->showDashboardScreen(); // Use the public method
        }
        else {
            qDebug() << "TrackExpenses::backToDashboard: cast failed";
        }
    } else {
        qDebug() << "TrackExpenses::backToDashboard: Parent is null";
    }
}

void TrackExpenses::deleteContribution()
{
    int row = contributionsTable->currentRow();
    if (row == -1) {
        QMessageBox::information(this, "No Item Selected", "Please select an item to delete.");
        return;
    }

    QString filename = QString("sdigits_data_%1.txt").arg(static_cast<MainWindow*>(parentWidget())->loggedInUser);
    QFile file(filename);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Could not open file.");
        return;
    }
    QTextStream in(&file);
    QStringList lines;
    while (!in.atEnd()) {
        lines.append(in.readLine());
    }
    file.resize(0);
    QTextStream out(&file);
    for (int i = 0; i < lines.size(); ++i) {
        if (i != row) {
            out << lines[i] << "\n";
        }
    }
    file.close();
    loadContributions(filename);
    QMessageBox::information(this, "Success", "Selected item deleted successfully");
}
