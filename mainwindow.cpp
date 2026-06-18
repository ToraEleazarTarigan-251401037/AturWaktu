#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTime>
#include <QMessageBox>
#include "alarmcardwidget.h"

// ======================================================
// Constructor Main Window
// ======================================================


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // ==================================================
    // Load Data Alarm
    // ==================================================

    loadAlarms();

    // ==================================================
    // Inisialisasi Input Alarm
    // ==================================================

    ui->dateEditAlarm->setDate(QDate::currentDate());
    ui->dateEditAlarm->setMinimumDate(QDate::currentDate());
    ui->timeEditAlarm->setTime(QTime::currentTime());

    // ==================================================
    // Styling Tab Widget
    // ==================================================

    ui->tabAturWaktu->setStyleSheet(R"(
        QTabWidget::pane {
            border: none;
            background-color: #f8f7fc;
        }
        QTabBar {
            alignment: center;
        }
        QTabBar::tab {
            background-color: transparent;
            color: #a09eaa;
            font-size: 14px;
            font-weight: 500;
            padding: 10px 26px;
            margin: 6px 4px;
            border-radius: 12px;
        }
        QTabBar::tab:selected {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #b67bfb, stop:1 #f57db1);
            color: #ffffff;
            font-weight: bold;
        }
        QTabBar::tab:hover:!selected {
            background-color: #ede9f8;
            color: #6b6880;
        }
    )");

    this->centralWidget()->setStyleSheet("background-color: #f8f7fc;");

    // ==================================================
    // Styling Label
    // ==================================================

    ui->labelTitle->setStyleSheet("font-size: 22px; font-weight: bold; color: #14121e;");
    ui->labelStopwatch->setStyleSheet("font-size: 52px; font-weight: bold; color: #14121e;");
    ui->labelTimer->setStyleSheet("font-size: 52px; font-weight: bold; color: #14121e;");
    ui->labelTimerRemaining->setStyleSheet("font-size: 12px; color: #a09eaa;");
    ui->labelSWHeader->setStyleSheet("font-size: 12px; font-weight: bold; color: #a09eaa; letter-spacing: 1px;");
    ui->labelTimerHeader->setStyleSheet("font-size: 12px; font-weight: bold; color: #a09eaa; letter-spacing: 1px;");
    ui->lineEditAlarmTitle->setStyleSheet(
        "background:white;"
        "color:#14121e;"
        "border:2px solid #d9c8ff;"
        "border-radius:10px;"
        "padding:6px;");

    // ==================================================
    // Styling Tombol Utama
    // ==================================================

    QString gradientPrimaryStyle =
        "QPushButton {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #b67bfb, stop:1 #f57db1);"
        "   color: white; border: none; border-radius: 14px; padding: 12px 24px; font-weight: bold;"
        "}"
        "QPushButton:hover { opacity: 0.9; }";


    //percantik ui nya button alarm
    ui->btnStartSW->setStyleSheet(gradientPrimaryStyle);
    ui->btnStartTimer->setStyleSheet(gradientPrimaryStyle);
    ui->btnAddAlarm->setStyleSheet(gradientPrimaryStyle);
    ui->btnEditAlarm->setStyleSheet(gradientPrimaryStyle);


    ui->btnLapSW->setStyleSheet("background-color: #fff0f6; color: #fa73a6; border: 1px solid #fac0d5; border-radius: 10px; padding: 10px 20px; font-weight: bold;");
    ui->btnResetSW->setStyleSheet("background-color: #ede9f8; color: #8c73f2; border: 1px solid #c9bff5; border-radius: 10px; padding: 10px 20px; font-weight: bold;");
    ui->btnResetTimer->setStyleSheet("background-color: #ede9f8; color: #8c73f2; border: 1px solid #c9bff5; border-radius: 10px; padding: 10px 20px; font-weight: bold;");
    ui->btnDeleteAlarm->setStyleSheet("background-color: #ede9f8; color: #8c73f2; border: 1px solid #c9bff5; border-radius: 10px; padding: 10px 20px; font-weight: bold;");


    QString presetStyle = "background-color: #ede9f8; color: #8c73f2; border: 1px solid #c9bff5; border-radius: 16px; padding: 6px 14px; font-weight: bold;";
    ui->btnPreset5->setStyleSheet(presetStyle);
    ui->btnPreset10->setStyleSheet(presetStyle);
    ui->btnPreset15->setStyleSheet(presetStyle);
    ui->btnPreset30->setStyleSheet(presetStyle);


    ui->listAlarm->setStyleSheet("QListWidget { background-color: transparent; border: none; } QListWidget::item { background-color: transparent; } QListWidget::item:selected { background-color: transparent; }");
    ui->listLap->setStyleSheet("QListWidget { background-color: #ffffff; border: 1px solid #e0ddf0; border-radius: 12px; padding: 4px; color: #14121e; } QListWidget::item { padding: 6px 10px; border-radius: 8px; } QListWidget::item:selected { background-color: #fff0f6; color: #fa73a6; } QListWidget::item:hover { background-color: #f3f0fb; }");

    ui->dateEditAlarm->setStyleSheet(
        "QDateEdit {"
        "   background-color: white;"
        "   border: 2px solid #d9c8ff;"
        "   border-radius: 12px;"
        "   padding: 4px 8px;"
        "   color: #5d4db3;"
        "   font-size: 16px;"
        "   font-weight: bold;"
        "}"
        "QCalendarWidget QWidget {"
        "   background-color: white;"
        "   color: black;"
        "}"
        );

    ui->timeEditAlarm->setStyleSheet(
        "QTimeEdit {"
        "   background-color: white;"
        "   border: 2px solid #d9c8ff;"
        "   border-radius: 14px;"
        "   padding: 8px;"
        "   color: #5d4db3;"
        "   font-size: 28px;"
        "   font-weight: bold;"
        "}"
        "QTimeEdit:focus {"
        "   border: 2px solid #8c73f2;"
        "}"
        );

    // ==================================================
    // Stopwatch
    // ==================================================

    // Timer stopwatch berjalan setiap 10 ms
    timer = new QTimer(this);
    timer->setInterval(10);
    // Hubungkan timer ke fungsi update tampilan stopwatch
    connect(timer, &QTimer::timeout, this, &MainWindow::updateWaktu);


    // Tombol Start/Pause Stopwatch
    connect(ui->btnStartSW, &QPushButton::clicked, this, [=]() {
        // Jika stopwatch sedang berjalan maka hentikan
        if (isRunning) {
            timer->stop();
            ui->btnStartSW->setText("Start");
            isRunning = false;
        // Jika stopwatch berhenti maka mulai kembali
        } else {
            timer->start();
            ui->btnStartSW->setText("Pause");
            isRunning = true;
        }
    });

    connect(ui->btnResetSW, &QPushButton::clicked, this, [=]() {
        timer->stop();
        isRunning = false;
        detik     = 0;
        lapCount  = 0;
        ui->btnStartSW->setText("Start");
        ui->labelStopwatch->setText("00:00.00");
        ui->listLap->clear();
    });

    connect(ui->btnLapSW, &QPushButton::clicked, this, [=]() {
        if (!isRunning && detik == 0) return;
        lapCount++;
        int cs   =  detik % 100;
        int secs = (detik / 100) % 60;
        int mins = (detik / 100) / 60;
        QString teks = QString("Lap %1    %2:%3.%4").arg(lapCount,2,10,QChar('0')).arg(mins,2,10,QChar('0')).arg(secs,2,10,QChar('0')).arg(cs,2,10,QChar('0'));
        ui->listLap->addItem(teks);
        ui->listLap->scrollToBottom();
    });



    // ==================================================
    // Countdown Timer
    // ==================================================

    countdownTimer = new QTimer(this);
    countdownTimer->setInterval(1000);
    connect(countdownTimer, &QTimer::timeout, this, &MainWindow::updateTimer);

    connect(ui->btnStartTimer, &QPushButton::clicked, this, [=]() {
        if (isTimerRunning) {
            countdownTimer->stop();
            ui->btnStartTimer->setText("Start");
            isTimerRunning = false;
        } else {
            if (sisaDetik == 0) {
                QTime w = ui->timeEditTimer->time();
                sisaDetik = w.hour()*3600 + w.minute()*60 + w.second();
                if (sisaDetik == 0) return;
            }
            countdownTimer->start();
            ui->btnStartTimer->setText("Pause");
            isTimerRunning = true;
        }
    });

    connect(ui->btnResetTimer, &QPushButton::clicked, this, [=]() {
        countdownTimer->stop();
        isTimerRunning = false;
        sisaDetik = 0;
        ui->btnStartTimer->setText("Start");
        ui->labelTimer->setText("00:00:00");
        ui->labelTimerRemaining->setText("remaining");
    });

    auto setPreset = [=](int menit) {
        countdownTimer->stop();
        isTimerRunning = false;
        sisaDetik = menit * 60;
        ui->btnStartTimer->setText("Start");
        ui->timeEditTimer->setTime(QTime(0, menit, 0));
        QTime t(0, 0);
        t = t.addSecs(sisaDetik);
        ui->labelTimer->setText(t.toString("HH:mm:ss"));
        ui->labelTimerRemaining->setText("remaining");
    };
    connect(ui->btnPreset5,  &QPushButton::clicked, this, [=]() { setPreset(5);  });
    connect(ui->btnPreset10, &QPushButton::clicked, this, [=]() { setPreset(10); });
    connect(ui->btnPreset15, &QPushButton::clicked, this, [=]() { setPreset(15); });
    connect(ui->btnPreset30, &QPushButton::clicked, this, [=]() { setPreset(30); });


    alarmSound = new QSoundEffect(this);

    alarmSound->setSource(
        QUrl("qrc:/Alarm.wav"));

    alarmSound->setLoopCount(QSoundEffect::Infinite);

    alarmSound->setVolume(1.0f);

    connect(ui->btnAddAlarm,    &QPushButton::clicked, this, &MainWindow::addAlarm);
    connect(ui->btnDeleteAlarm, &QPushButton::clicked, this, &MainWindow::deleteAlarm);
    connect(ui->btnEditAlarm, &QPushButton::clicked, this, &MainWindow::editAlarm);

    alarmTimer = new QTimer(this);
    alarmTimer->setInterval(1000);
    alarmTimer->start();
    connect(alarmTimer, &QTimer::timeout, this, &MainWindow::checkAlarm);
    connect(ui->listAlarm,
            &QListWidget::itemClicked,
            this,
            [=](QListWidgetItem *item)
            {
                selectedAlarmItem = item;

                AlarmCardWidget *card =
                    qobject_cast<AlarmCardWidget*>(
                        ui->listAlarm->itemWidget(item));

                if(card)
                {
                    ui->timeEditAlarm->setTime(
                        QTime::fromString(
                            card->getTimeText(),
                            "HH:mm:ss"));
                }
            });


}


void MainWindow::updateWaktu()
{   // Menambah waktu stopwatch setiap 10 ms
    detik++;
    // Menghitung centisecond (1/100 detik)
    int cs   =  detik % 100;
    // Menghitung detik (0 - 59)
    int secs = (detik / 100) % 60;
    // Menghitung menit
    int mins = (detik / 100) / 60;
    ui->labelStopwatch->setText(QString("%1:%2.%3").arg(mins, 2, 10, QChar('0')).arg(secs, 2, 10, QChar('0')).arg(cs, 2, 10, QChar('0')));
}

void MainWindow::updateTimer()
{
    if (sisaDetik > 0) {
        sisaDetik--;
        QTime t(0, 0);
        t = t.addSecs(sisaDetik);
        ui->labelTimer->setText(t.toString("HH:mm:ss"));
    } else {
        countdownTimer->stop();
        isTimerRunning = false;
        ui->btnStartTimer->setText("Start");
        ui->labelTimer->setText("SELESAI!");
        ui->labelTimerRemaining->setText("");

        if (alarmSound->isPlaying()) {
            alarmSound->stop();
        }
        
        alarmSound->play();

        QMessageBox msg(this);
        msg.setWindowTitle("Timer");
        msg.setText("⏳ WAKTU TIMER HABIS!");
        msg.exec();

        alarmSound->stop();
    }
}

void MainWindow::saveAlarms()
{
    QJsonArray alarmArray;

    for(int i = 0; i < ui->listAlarm->count(); i++)
    {
        QListWidgetItem *item = ui->listAlarm->item(i);

        AlarmCardWidget *card =
            qobject_cast<AlarmCardWidget*>(
                ui->listAlarm->itemWidget(item));

        if(!card)
            continue;

        QJsonObject alarm;

        alarm["title"] = card->getTitleText();
        alarm["time"] = card->getTimeText();
        alarm["date"] = card->getDaysText();
        alarm["active"] = card->isAlarmActive();

        alarmArray.append(alarm);
    }

    QFile file("alarms.json");

    if(file.open(QIODevice::WriteOnly))
    {
        file.write(
            QJsonDocument(alarmArray).toJson());

        file.close();
    }
}

void MainWindow::loadAlarms()
{
    QFile file("alarms.json");

    if(!file.open(QIODevice::ReadOnly))
        return;

    QByteArray data = file.readAll();

    file.close();

    QJsonArray alarmArray =
        QJsonDocument::fromJson(data).array();

    for(auto value : alarmArray)
    {
        QJsonObject alarm =
            value.toObject();

        QListWidgetItem *item =
            new QListWidgetItem();

        AlarmCardWidget *card =
            new AlarmCardWidget(
                alarm["title"].toString(),
                alarm["time"].toString(),
                alarm["date"].toString(),
                alarm["active"].toBool(),
                this);
        item->setSizeHint(card->sizeHint());

        ui->listAlarm->addItem(item);

        ui->listAlarm->setItemWidget(
            item,
            card);

        connect(card,
                &AlarmCardWidget::deletePressed,
                [=]()
                {
                    delete item;
                    saveAlarms();
                });
    }
}

void MainWindow::addAlarm()
{
    QTime waktu = ui->timeEditAlarm->time();
    QString timeStr = waktu.toString("HH:mm:ss");

    QDate tanggal = ui->dateEditAlarm->date();
    QString dateStr = tanggal.toString("dd/MM/yyyy");

    QString daysStr = "📅 " + dateStr;

    QListWidgetItem *item =
        new QListWidgetItem(ui->listAlarm);

    QString title =
        ui->lineEditAlarmTitle->text();

    if(title.trimmed().isEmpty())
    {
        title = "Alarm";
    }

    AlarmCardWidget *card =
        new AlarmCardWidget(
            title,
            timeStr,
            daysStr,
            true,
            this);

    item->setSizeHint(card->sizeHint());

    ui->listAlarm->addItem(item);
    ui->listAlarm->setItemWidget(item, card);

    connect(card,
            &AlarmCardWidget::deletePressed,
            [=]()
            {
                delete item;
                saveAlarms();
            });

    connect(card,
            &AlarmCardWidget::alarmStateChanged,
            this,
            [=]()
            {
                saveAlarms();
            });
    ui->lineEditAlarmTitle->clear();
    saveAlarms(); // simpan setelah tambah
}
void MainWindow::checkAlarm()
{
    QString sekarangWaktu = QTime::currentTime().toString("HH:mm:ss");
    QString sekarangTanggal = QDate::currentDate().toString("dd/MM/yyyy");

    for (int i = 0; i < ui->listAlarm->count(); i++)
    {
        QListWidgetItem *item = ui->listAlarm->item(i);
        AlarmCardWidget *card = qobject_cast<AlarmCardWidget*>(ui->listAlarm->itemWidget(item));

        if (!card) continue;

        QString tanggalAlarm = card->getDaysText();
        tanggalAlarm.remove("📅 ");
        tanggalAlarm = tanggalAlarm.trimmed();

        QString alarmKey = tanggalAlarm + "_" + card->getTimeText();

        // 1. Kondisi ketika alarm HARUS BUNYI
        if (card->getTimeText() == sekarangWaktu &&
            tanggalAlarm == sekarangTanggal)
        {
            if (card->isAlarmActive() && !alarmSudahBunyi.contains(alarmKey))
            {
                alarmSudahBunyi.insert(alarmKey);

                alarmSound->play();

                QMessageBox msg(this);
                msg.setWindowTitle("Alarm");
                msg.setText("🔔 WAKTU SUDAH TIBA!");
                msg.exec();

                alarmSound->stop();
            }
        }
        else 
        {
            if (alarmSudahBunyi.contains(alarmKey)) {
                alarmSudahBunyi.remove(alarmKey);
            }
        }
    }
}void MainWindow::deleteAlarm()
{
    QListWidgetItem *item = ui->listAlarm->currentItem();
    if (!item) return;
    delete item;
}

void MainWindow::editAlarm()
{
    QListWidgetItem *item =
        selectedAlarmItem;

    if (!item)
    {
        QMessageBox::warning(
            this,
            "Edit Alarm",
            "Pilih alarm terlebih dahulu!");
        return;
    }

    AlarmCardWidget *card =
        qobject_cast<AlarmCardWidget*>(
            ui->listAlarm->itemWidget(item));

    if (!card)
        return;

    QString waktuBaru =
        ui->timeEditAlarm->time()
            .toString("HH:mm:ss");

    card->setTimeText(waktuBaru);
}
MainWindow::~MainWindow()
{
    delete ui;
}
