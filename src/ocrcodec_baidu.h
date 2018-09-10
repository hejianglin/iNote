#ifndef OCRCODEC_BAIDU_H
#define OCRCODEC_BAIDU_H

//Qt
#include <QObject>
#include <QString>
#include <QByteArray>
#include <QJsonObject>

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>

//iNote
#include "ocrcodec.h"

class OCRCodec_Baidu : public OCRCodec
{
    Q_OBJECT

public:
    OCRCodec_Baidu(QObject *parent  = 0);
    ~OCRCodec_Baidu();

    void setAppKey(const QString &appKey) override { m_sAppKey = appKey; }
    QString appKey() const override { return m_sAppKey; }

    void setSecretKey(const QString &secretkey) override { m_sSecretKey = secretkey; }
    QString secretKey() const override { return m_sSecretKey; }

    void setAccessToken(const QString &accessToken) override { m_sAccessToken = accessToken; }
    QString accessToken() const override { return m_sAccessToken; }

    bool decode(RequestType,const QString &) override;
    bool isDecode() const override { return m_eCurrentRequestType != RequestType_eNone; }

    bool isValid() override;
    QString errorString() const override { return m_sError; }

private slots:
    void slotReadReply(QNetworkReply *);

private:
    enum DecodeProgerss
    {
        DecodeProgerss_eStart = 1,
        DecodeProgerss_eOnGoing,
        DecodeProgerss_eFinished
    };

    //Function
    QNetworkRequest requestHead();
    QByteArray requestBody(const QString &file);

    void parseReply(const QByteArray &);
    void parseReply_General(const QJsonObject &);
    void parseReply_AccessToken(const QJsonObject &);
    void parseReply_Table(const QJsonObject &);
    void parseTableData(const QJsonObject &);

    void finishedDecode(const QString &ret,const QString &error);

    //Data
    QString m_sError;
    QString m_sAppKey;
    QString m_sSecretKey;
    QString m_sAccessToken;

    RequestType m_eCurrentRequestType;
    QNetworkAccessManager m_cNetworkAccessManager;
};

#endif // OCRCODEC_BAIDU_H
