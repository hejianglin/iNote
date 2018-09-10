//Qt
#include <QApplication>

//iNote
#include "inoteconfig.h"

iNoteConfig::iNoteConfig()
{
    QString sConfigFile = QApplication::applicationDirPath() + "/config.ini";
    m_cSettings = new QSettings(sConfigFile,QSettings::IniFormat);
}

iNoteConfig::~iNoteConfig()
{
    delete m_cSettings;
}

void iNoteConfig::setAppKey(const QString &appKey)
{
    setValue("User/AppKey",appKey);
}

QString iNoteConfig::appKey() const
{
    return value("User/AppKey").toString();
}

void iNoteConfig::setSecretKey(const QString &secretKey)
{
    setValue("User/SecretKey",secretKey);
}

QString iNoteConfig::secretKey() const
{
    return value("User/SecretKey").toString();
}

void iNoteConfig::setAccessToken(const QString &accessToken)
{
    setValue("User/AccessToken",accessToken);
}

QString iNoteConfig::accessToken() const
{
    return value("User/AccessToken").toString();
}

void iNoteConfig::setValue(const QString &key,const QVariant &value)
{
    m_cSettings->setValue(key,value);
    m_cSettings->sync();
}

QVariant iNoteConfig::value(const QString &key, const QVariant &defaultValue) const
{
    return m_cSettings->value(key,defaultValue);
}




