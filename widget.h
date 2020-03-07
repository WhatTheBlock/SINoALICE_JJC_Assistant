#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QStyleFactory>
#include <QDebug>
#include <QtCore>
#include <QLineEdit>
#include <QTimer>
#include <QApplication>
#include <QClipboard>
#include <cmath>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget {
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    //計時器
    QTimer *oneSecTimer;

    //剪貼簿
    QClipboard *clipboard = QApplication::clipboard();
    QString clipboardText;

    //競技場時段
    static const constexpr char* const jjcTime[] = {
        "08:00:00", "12:30:00", "18:00:00", "19:00:00", "20:00:00", "21:00:00", "22:00:00", "23:00:00"
    };

    //競技場時段設定
    int jjcTimeIndex = 5;

    //競技場開戰狀態
    bool jjcStarted = false;

    //計算換位時間
    void calculate(int time);


public slots:
    void oneSec();

private slots:
    void on_JJC_Time_currentIndexChanged(int index);

    void on_debuff_single_lv3_clicked();

    void on_debuff_single_lv4_clicked();

    void on_debuff_both_lv3_clicked();

    void on_debuff_both_lv4_clicked();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
