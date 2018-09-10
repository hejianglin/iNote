#ifndef INOTECONFIG_H
#define INOTECONFIG_H

#include <QObject>
#include <QSettings>


class iNoteConfig
{
public:
    enum TableTransFormat
    {
      TableTransFormat_eMarkdown,
      TableTransFormat_eHtml
    };

    iNoteConfig();
    ~iNoteConfig();

    //user
    void setAppKey(const QString &);
    QString appKey() const;

    void setSecretKey(const QString &);
    QString secretKey() const;

    void setAccessToken(const QString &);
    QString accessToken() const;

    void setValue(const QString &key,const QVariant &value);
    QVariant value(const QString &key,const QVariant &defaultValue = QVariant()) const;

private:
    QSettings *m_cSettings;
};

#endif // INOTECONFIG_H
