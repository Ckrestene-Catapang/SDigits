#include "dashboard.h"

Dashboard::Dashboard(QWidget *parent)
    : QWidget(parent)
{
    trackExpensesButton = new QPushButton("Track Expenses");
    historyButton = new QPushButton("View History");
    profileButton = new QPushButton("View Profile");
    createUserButton = new QPushButton("Create User");
    backToLoginButton = new QPushButton("Back to Login"); // Initialize the back button

    mainLayout = new QVBoxLayout();
    mainLayout->addWidget(trackExpensesButton);
    mainLayout->addWidget(historyButton);
    mainLayout->addWidget(profileButton);
    mainLayout->addWidget(createUserButton);
    mainLayout->addWidget(backToLoginButton); // Add the back button to the layout
    setLayout(mainLayout);

    connect(trackExpensesButton, &QPushButton::clicked, this, &Dashboard::showTrackExpenses);
    connect(historyButton, &QPushButton::clicked, this, &Dashboard::showHistory);
    connect(profileButton, &QPushButton::clicked, this, &Dashboard::showProfile);
    connect(createUserButton, &QPushButton::clicked, this, &Dashboard::showCreateUser);
    connect(backToLoginButton, &QPushButton::clicked, this, &Dashboard::showLogin); // Connect back button
}

Dashboard::~Dashboard()
{
}


void Dashboard::goToTrackExpenses()
{
    emit showTrackExpenses();
}

void Dashboard::goToHistory()
{
    emit showHistory();
}

void Dashboard::goToProfile()
{
    emit showProfile();
}

void Dashboard::goToCreateUser()
{
    emit showCreateUser();
}

void Dashboard::goToSwitchAccount()
{
    emit showLogin();
}
