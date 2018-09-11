//Qt
#include <QImage>
#include <QBuffer>
#include <QStringList>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>

//iNote
#include "inoteutils.h"
#include "inoteglobal.h"
#include "ocrcodec_baidu.h"

OCRCodec_Baidu::OCRCodec_Baidu(QObject *parent)
    : OCRCodec(parent)
    , m_eCurrentRequestType(RequestType_eNone)
{
    connect(&m_cNetworkAccessManager,SIGNAL(finished(QNetworkReply *)),
            this,SLOT(slotReadReply(QNetworkReply *)));
}

OCRCodec_Baidu::~OCRCodec_Baidu()
{

}

bool OCRCodec_Baidu::isValid()
{
    if(m_sAppKey.isEmpty()){
        m_sError = tr("app key is empty");
        return false;
    }

    if(m_sSecretKey.isEmpty()){
        m_sError = tr("secret key is empty");
        return false;
    }

    return true;
}

bool OCRCodec_Baidu::decode(RequestType type,const QString &path)
{
    if(!isValid()){
        finishedDecode("",m_sError);
        return false;
    }

    if(m_eCurrentRequestType != RequestType_eNone){
        finishedDecode("",tr("working..."));
        return false;
    }

    m_eCurrentRequestType = type;
    m_sCurrentRequestFile = path;
    m_cNetworkAccessManager.post(requestHead(),requestBody(path));
    return true;
}

void OCRCodec_Baidu::finishedDecode(const QString &ret,const QString &error)
{
    emit sigFinished(ret,error);

    //reset
    m_eCurrentRequestType = RequestType_eNone;
    m_sCurrentRequestFile.clear();
    m_sError.clear();
}

QNetworkRequest OCRCodec_Baidu::requestHead()
{
    QString sUrl;
    switch (m_eCurrentRequestType) {
    case RequestType_eNone:
        break;
    case RequestType_eAccessToken:
        sUrl = "https://aip.baidubce.com/oauth/2.0/token?grant_type=client_credentials"
               "&client_id=" + m_sAppKey + "&client_secret=" + m_sSecretKey;
        break;

    case RequestType_eGeneral:
        sUrl = "https://aip.baidubce.com/rest/2.0/ocr/v1/general_basic";
        break;

    case RequestType_eWebImage:
        sUrl = "https://aip.baidubce.com/rest/2.0/ocr/v1/webimage";
        break;

    case RequestType_eTable:
        sUrl = "https://aip.baidubce.com/rest/2.0/solution/v1/form_ocr/request";
        break;
    }

    if(m_eCurrentRequestType != RequestType_eAccessToken){
        sUrl += "?access_token=" + m_sAccessToken;
    }

    QNetworkRequest request;
    request.setUrl(QUrl(sUrl));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

//    QSslConfiguration config;
//    config.setPeerVerifyMode(QSslSocket::VerifyNone);
//    config.setProtocol(QSsl::TlsV1SslV3);
//    request.setSslConfiguration(config);

    return request;
}

QByteArray OCRCodec_Baidu::requestBody(const QString &file)
{
    QByteArray body;
    QImage image;
    QBuffer bufferBody(&body);
    bufferBody.open(QIODevice::WriteOnly);

    switch (m_eCurrentRequestType) {
    case RequestType_eNone:
    case RequestType_eAccessToken:
        break;

    case RequestType_eGeneral:
    case RequestType_eTable:
        image = QImage(file);
        image.save(&bufferBody,"png");
        body = QByteArray("image=") + body.toBase64().toPercentEncoding();
        break;

    case RequestType_eWebImage:
        body = QByteArray("url=").append(file);
        break;
    }

    bufferBody.close();
    return body;
}

void OCRCodec_Baidu::slotReadReply(QNetworkReply *reply)
{
    if(reply->error() != QNetworkReply::NoError){
        finishedDecode("",reply->errorString());
    }else{
        parseReply(reply->readAll());
    }
    reply->deleteLater();
}

void OCRCodec_Baidu::parseReply(const QByteArray &data)
{
    DEBUG("reply:" + data);

    if(data.isEmpty()){
        finishedDecode("",tr("reply is empty"));
        return ;
    }

    QJsonParseError error;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data,&error);
    if(error.error != QJsonParseError::NoError){
        finishedDecode("",tr("reply format  error:") + error.errorString());
        return ;
    }

    //parse
    switch (m_eCurrentRequestType) {
    case RequestType_eNone:
        break;

    case RequestType_eAccessToken:
        parseReply_AccessToken(jsonDoc.object());
        break;

    case RequestType_eGeneral:
    case RequestType_eWebImage:
        parseReply_General(jsonDoc.object());
        break;

    case RequestType_eTable:
        parseReply_Table(jsonDoc.object());
        break;
    }
}

void OCRCodec_Baidu::parseReply_AccessToken(const QJsonObject &jsonObj)
{
    if(jsonObj.contains("error")){
        finishedDecode("",jsonObj["error_description"].toString());
        return ;
    }

    m_sAccessToken = jsonObj["access_token"].toString();
    finishedDecode(m_sAccessToken,"");
}

void OCRCodec_Baidu::parseReply_General(const QJsonObject &jsonObj)
{
    if(jsonObj.contains("error_code")){
        finishedDecode("",jsonObj["error_msg"].toString());
        return ;
    }

    QString sRet;
    QJsonArray wordsArray = jsonObj["words_result"].toArray();
    for(int i = 0; i< wordsArray.count(); i++){
        sRet.append(wordsArray[i].toObject()["words"].toString());
        sRet.append("\n");
    }

    finishedDecode(sRet,"");
}

void OCRCodec_Baidu::parseReply_Table(const QJsonObject &jsonObj)
{
    if(jsonObj.contains("error_code")){
        finishedDecode("",jsonObj["error_msg"].toString());
        return ;
    }

    QString sRequestID;


    if(jsonObj["result"].isArray()){
        DEBUG("array");
        QJsonArray resultArray = jsonObj["result"].toArray();
        sRequestID = resultArray[0].toObject()["request_id"].toString();
        emit sigProgress(0);
    }else{
        //check progress
        QJsonObject resultObj = jsonObj["result"].toObject();
        if(resultObj["ret_code"].toInt() != DecodeProgerss_eFinished){
            sRequestID = resultObj["request_id"].toString();
            DEBUG("request id:" + sRequestID);
        }else{
            parseTableData(jsonObj["result_data"].toObject());
        }
        emit sigProgress(static_cast<quint8>(resultObj["persent"].toInt()));
    }


    //unfinished
    if(!sRequestID.isEmpty()){
        iNoteUtils::delay(3 * 1000);

        //head
        QNetworkRequest request;
        request.setUrl(QUrl("https://aip.baidubce.com/rest/2.0/solution/v1/form_ocr/get_request_result"
                            "?access_token=" + m_sAccessToken));
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

//        QSslConfiguration config;
//        config.setPeerVerifyMode(QSslSocket::VerifyNone);
//        config.setProtocol(QSsl::TlsV1SslV3);
//        request.setSslConfiguration(config);

        //body
        QByteArray body = QByteArray("request_id=").append(sRequestID);
        body.append("&result_type=json");
        m_cNetworkAccessManager.post(request,body);
    }
}

void OCRCodec_Baidu::parseTableData(const QJsonObject &jsonObj)
{
    //表格1


    //表格2

    //表格3

    //...
}






