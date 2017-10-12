#include "util_http.h"
#include <QTextCodec>
#include <iostream>

using namespace std;

const int nHTTP_TIME = 10000; //10秒

HttpUtil::HttpUtil(QObject *parent) :
    QObject(parent)
{
    m_pNetworkManager = new QNetworkAccessManager(this);
    m_pNetworkReply = NULL;
    m_pTimer = new QTimer;
    connect(m_pTimer,SIGNAL(timeout()),this,SLOT(slot_requestTimeout()));//超时信号
}

//发起请求
void HttpUtil::sendGetRequest(const QString &strUrl)
{
    m_strUrl = strUrl;
    QNetworkRequest netRequest;
    netRequest.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    netRequest.setRawHeader("X-LC-Id","FijOSI0Rs8JXUgoL4FdYiMqq-gzGzoHsz");
    netRequest.setRawHeader("X-LC-Key","hUdYc8rE4L6nfTI2XUM6SMc8");
    netRequest.setUrl(QUrl(strUrl)); //地址信息
    //    if(strUrl.toLower().startsWith("https"))//https请求，需ssl支持(下载openssl拷贝libeay32.dll和ssleay32.dll文件至Qt bin目录或程序运行目录)
    //    {
    //        QSslConfiguration sslConfig;
    //        sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    //        sslConfig.setProtocol(QSsl::TlsV1_1);
    //        netRequest.setSslConfiguration(sslConfig);
    //    }
    m_pNetworkReply = m_pNetworkManager->get(netRequest); //发起get请求
    connect(m_pNetworkReply,SIGNAL(finished()),this,SLOT(slot_requestFinished())); //请求完成信号
    m_pTimer->start(nHTTP_TIME);
}

void HttpUtil::sendPostRequest(const QString &strUrl, QString contentType, QByteArray postBody)
{
    m_strUrl = strUrl;
    QNetworkRequest netRequest;
    netRequest.setHeader(QNetworkRequest::ContentTypeHeader,contentType);
    netRequest.setRawHeader("X-LC-Id","FijOSI0Rs8JXUgoL4FdYiMqq-gzGzoHsz");
    netRequest.setRawHeader("X-LC-Key","hUdYc8rE4L6nfTI2XUM6SMc8");
    netRequest.setUrl(QUrl(strUrl)); //地址信息
    //    if(strUrl.toLower().startsWith("https"))//https请求，需ssl支持(下载openssl拷贝libeay32.dll和ssleay32.dll文件至Qt bin目录或程序运行目录)
    //    {
    //        QSslConfiguration sslConfig;
    //        sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    //        sslConfig.setProtocol(QSsl::TlsV1_1);
    //        netRequest.setSslConfiguration(sslConfig);
    //    }
    m_pNetworkReply = m_pNetworkManager->post(netRequest,postBody);//发起post请求
    connect(m_pNetworkReply,SIGNAL(finished()),this,SLOT(slot_requestFinished())); //请求完成信号
    m_pTimer->start(nHTTP_TIME);
}

//请求结束
void HttpUtil::slot_requestFinished()
{
    m_pTimer->stop();//关闭定时器
    QByteArray resultContent = m_pNetworkReply->readAll();
    QTextCodec *pCodec = QTextCodec::codecForName("UTF-8");
    QString strResult = pCodec->toUnicode(resultContent);
    int nHttpCode = m_pNetworkReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();//http返回码
    emit signal_requestFinished(nHttpCode,strResult);//请求成功
    m_pNetworkReply->deleteLater();
    this->deleteLater(); //释放内存
}

//请求超时
void HttpUtil::slot_requestTimeout()
{
    emit signal_requestFinished(0,"timeout");//请求失败
    m_pNetworkReply->deleteLater();
    this->deleteLater();//释放内存
}
