#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    // 1. Create the Network Manager
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    // 2. Define the URL and Request
    QUrl url("http://localhost:11434/api/generate");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // 3. Create the JSON Body
    QJsonObject json;
    json["model"] = "SpeakLeash/bielik-11b-v3.0-instruct:Q4_K_M";
    json["prompt"] = ui->plainTextEdit_prompt->toPlainText();
    json["system"] = "You are a sailor, who hates sea but has no money.";
    json["stream"] = false;

    QJsonDocument doc(json);
    QByteArray data = doc.toJson();

    // 4. Send the PUT Request
    QNetworkReply *reply = manager->post(request, data);

    // 5. Handle the Response (wiat for response)
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec(); // Execution pauses here until 'finished' is emitted

    // Parse output to json and put to window
    QJsonDocument reply_json = QJsonDocument::fromJson(reply->readAll());
    ui->plainTextEdit_resp->setPlainText(reply_json.object()["response"].toString());
    // Cleanup
    reply->deleteLater();
    manager->deleteLater();

}

void MainWindow::on_pushButton_clear_clicked()
{
    ui->plainTextEdit_prompt->clear();
}

