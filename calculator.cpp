#include "calculator.h"
#include <QVBoxLayout>
#include <QScrollArea>
#include <QJSEngine>
#include <QFile>
#include <QTextStream>

Calculator::Calculator(QWidget *parent) : QWidget(parent) {
    // Toggle Theme Button
    loadStyleSheet("styles.qss");

    QPushButton *toggleThemeButton = new QPushButton("🌙 Toggle Theme", this);
    toggleThemeButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(toggleThemeButton, &QPushButton::clicked, this, &Calculator::toggleTheme);

    // Scrollable history List
    historyList = new QListWidget(this);
    historyList->setFixedHeight(150);  
    historyList->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    historyList->setStyleSheet("font-size: 14px; padding: 10px;");
    historyList->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    historyList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QVBoxLayout *topRowLayout = new QVBoxLayout();
    topRowLayout->addWidget(toggleThemeButton);
    topRowLayout->addWidget(historyList);
    topRowLayout->setStretch(0, 8); 
    topRowLayout->setStretch(1, 2); 

    // Input Display
    display = new QLineEdit(this);
    display->setReadOnly(true);
    display->setAlignment(Qt::AlignRight);
    display->setFixedHeight(70);
    display->setPlaceholderText("0");
    display->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    QVBoxLayout *topLayout = new QVBoxLayout();
    topLayout->addLayout(topRowLayout);
    topLayout->addWidget(display);

    layout = new QGridLayout();
    createButtons();

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(layout);   

    setLayout(mainLayout);
    setWindowTitle("Qt Calculator");
    setMinimumSize(400, 600);
}

void Calculator::loadStyleSheet(const QString &fileName) {
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        qWarning("Failed to load stylesheet");
        return;
    }
    QTextStream stream(&file);
    QString styleSheet = stream.readAll();
    this->setStyleSheet(styleSheet);
}

void Calculator::createButtons() {
    QStringList buttons = {
        "7", "8", "9", "/", 
        "4", "5", "6", "*", 
        "1", "2", "3", "-", 
        "0", "C", "=", "+" 
    };

    int row = 0, col = 0;
    for (const QString &text : buttons) {
        QPushButton *button = new QPushButton(text, this);
        button->setMinimumSize(60, 60);

        layout->addWidget(button, row, col);
        connect(button, &QPushButton::clicked, this, &Calculator::onButtonClicked);

        col++;
        if (col == 4) { col = 0; row++; }
    }
}

void Calculator::onButtonClicked() {
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (!button) return;

    QString buttonText = button->text();
    
    if (buttonText == "C") {
        onClearClicked();
    } else if (buttonText == "=") {
        onCalculateClicked();
    } else {
        currentExpression += buttonText;
        display->setText(currentExpression);
    }
}

void Calculator::onClearClicked() {
    currentExpression.clear();
    display->clear();
}

void Calculator::onCalculateClicked() {
    QJSEngine engine;
    QJSValue result = engine.evaluate(currentExpression);

    if (result.isError()) {
        display->setText("Error");
    } else {
        display->setText(result.toString());
        historyList->insertItem(0, currentExpression + " = " + result.toString());
    }

    currentExpression.clear();
}

void Calculator::toggleTheme() {
    static bool darkMode = false;
    darkMode = !darkMode;

    if (darkMode) {
        loadStyleSheet("dark.qss");
    } else {
        loadStyleSheet("styles.qss");
    }
}

Calculator::~Calculator() {
    delete display;
    delete layout;
}
