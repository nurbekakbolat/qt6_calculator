#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QListWidget>

class Calculator : public QWidget {
    Q_OBJECT

public:
    explicit Calculator(QWidget *parent = nullptr);
    ~Calculator();

private slots:
    void onButtonClicked();
    void onClearClicked();
    void onCalculateClicked();
    void toggleTheme();
    void loadStyleSheet(const QString &fileName);
    
private:
    QLineEdit *display;
    QGridLayout *layout;
    QString currentExpression;
    QListWidget *historyList;

    void createButtons();
};

#endif
