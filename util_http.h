
#ifndef HTTPFUN_H
#define HTTPFUN_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QTimer>
//#include <QSslConfiQguration>
#include <QString>
#include <QByteArray>

//http请求类
class HttpUtil : public QObject
{
    Q_OBJECT
public:
    explicit HttpUtil(QObject *parent = 0);
    void sendGetRequest(const QString& strUrl);//根据url发起http请求
    void sendPostRequest(const QString& strUrl,QString contentType, QByteArray postBody);
signals:
    void signal_requestFinished(int code,const QString& strResult); //http请求结束
public slots:
    void slot_requestFinished(); //http请求结束
    void slot_requestTimeout();  //请求超时

private:
    QNetworkAccessManager* m_pNetworkManager;//网络管理类
    QNetworkReply* m_pNetworkReply; //封装请求返回信息
    QTimer* m_pTimer; //请求超时计时器
    QString m_strUrl; //记录当前请求的url
};

#endif // HTTPFUN_H
