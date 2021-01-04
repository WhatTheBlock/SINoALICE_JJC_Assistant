/*
//   哈囉大家好：）我是台版玩家【小二】
//   寫這個工具是因為算時間反應不過來只好用程式輔助XD
//   如果有哪邊希望改進的部分歡迎到巴哈小屋找我，或是在Github提issue也可以唷
//   歡迎加巴哈好友，巴哈小屋：https://home.gamer.com.tw/evildjkay
//   巴哈文章：https://forum.gamer.com.tw/C.php?bsn=31743&snA=4390
//   夢魘圖片來源：https://sinoalice.game-db.tw/
*/

#include "widget.h"
#include "ui_widget.h"

#define magicValue 1198 //競技場總時長只有1198秒
constexpr const char* const Widget::jjcTime[];

//初始化
Widget::Widget(QWidget *parent): QWidget(parent), ui(new Ui::Widget) {
    //設定UI介面
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    ui->setupUi(this);

    //設定TextEdit的鼠標為標準箭頭
    ui->background->viewport()->setCursor(Qt::ArrowCursor);

    //初始化時間
    ui->currentTime->setTime(QTime::currentTime());
    //ui->currentTime->setTime(QTime::fromString("20:59:50"));
    //ui->currentTime->setTime(QTime::fromString("21:19:50"));

    //計時器(1秒執行1次)
    oneSecTimer = new QTimer(this);
    connect(oneSecTimer, SIGNAL(timeout()), this, SLOT(oneSec()));
    oneSecTimer->start(1000);
}

Widget::~Widget() { delete ui; }

//計時器(1秒執行1次)
void Widget::oneSec() {
    //更新當前時間
    ui->currentTime->setTime(QTime::currentTime());
    //ui->currentTime->setTime(ui->currentTime->time().addSecs(1));

    //若競技場已開戰
    if(jjcStarted) {
        //若競技場已結束或尚未開始 (開戰途中修改時段設定)
        if(operator<(ui->currentTime->time(), QTime::fromString(jjcTime[jjcTimeIndex])) ||
                operator>(ui->currentTime->time(), QTime::fromString(jjcTime[jjcTimeIndex]).addSecs(magicValue))) {
            jjcStarted = false;
            ui->debuff_both_lv3->setEnabled(false);
            ui->debuff_both_lv4->setEnabled(false);
            ui->debuff_single_lv3->setEnabled(false);
            ui->debuff_single_lv4->setEnabled(false);
            ui->remainingTime->setEnabled(false);
            ui->switchTime->setEnabled(false);
            ui->remainingTime->setTime(QTime::fromString("20:00", "mm:ss"));
            ui->switchTime->setTime(QTime::fromString("20:00", "mm:ss"));
        }
        //計算競技場剩餘時間
        else {
            ui->remainingTime->setTime(
                        QTime(0, 0).addSecs(
                            ui->currentTime->time().secsTo(
                                QTime::fromString(jjcTime[jjcTimeIndex]).addSecs(magicValue))
                            )
                        );
        }
    }
    //判斷競技場開戰狀態
    else if(operator>=(ui->currentTime->time(), QTime::fromString(jjcTime[jjcTimeIndex])) &&
            operator<(ui->currentTime->time(), QTime::fromString(jjcTime[jjcTimeIndex]).addSecs(magicValue))) {
        jjcStarted = true;
        ui->debuff_both_lv3->setEnabled(true);
        ui->debuff_both_lv4->setEnabled(true);
        ui->debuff_single_lv3->setEnabled(true);
        ui->debuff_single_lv4->setEnabled(true);
        ui->remainingTime->setEnabled(true);
        ui->switchTime->setEnabled(true);
    }
}

//計算換位時間
void Widget::calculate(int time) {
    int delay = ui->reserveTime->value() + ui->determineTime->value();
    ui->switchTime->setTime(ui->remainingTime->time().addSecs(-(time - delay)));
    
    //複製換位時間訊息到剪貼簿
    clipboardText = QString("換位時間：%1").arg(ui->switchTime->time().toString("mm:ss"));
    clipboard->setText(clipboardText);
}

//=========觸發事件=========
//設定競技場時段
void Widget::on_JJC_Time_currentIndexChanged(int index) { jjcTimeIndex = index; }
//降單攻/防 Ⅲ (70秒)
void Widget::on_debuff_single_lv3_clicked() { calculate(70); }
//降單攻/防 Ⅳ (60秒)
void Widget::on_debuff_single_lv4_clicked() { calculate(60); }
//降雙攻/防 Ⅲ (50秒)
void Widget::on_debuff_both_lv3_clicked() { calculate(50); }
//降雙攻/防 Ⅳ (40秒)
void Widget::on_debuff_both_lv4_clicked() { calculate(40); }
