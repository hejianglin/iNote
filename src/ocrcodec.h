#ifndef OCRCODEC_H
#define OCRCODEC_H

#include <QObject>
#include <QString>

class OCRCodec : public QObject
{
    Q_OBJECT
public:
    enum RequestType
    {
      RequestType_eNone,
      RequestType_eAccessToken,
      RequestType_eGeneral,
      RequestType_eWebImage,
      RequestType_eTable
    };

    explicit OCRCodec(QObject *parent = 0);
    virtual ~OCRCodec() {}

    virtual void setAppKey(const QString &) {}
    virtual QString appKey() const { return QString(); }

    virtual void setSecretKey(const QString &) {}
    virtual QString secretKey() const { return QString(); }

    virtual void setAccessToken(const QString &) {}
    virtual QString accessToken() const { return QString(); }

    virtual bool decode(RequestType,const QString &) { return false; }

    virtual bool isValid() { return false; }
    virtual QString errorString() const { return QString(); }

signals:
    void sigProgress(quint8);
    void sigFinished(const QString &reply,const QString &errorString);
};

#endif // OCRCODEC_H
