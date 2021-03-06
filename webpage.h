#ifndef __WEBPAGE__
#define __WEBPAGE__

#include <QString>
#include <QWebPage>
#include <QWebFrame>
#include <QUrl>

class WebPage : public QWebPage
{
public:
    WebPage();
    void setDefaultUserAgent(QString agent);

protected:
    void javaScriptConsoleMessage(const QString& message, int lineNumber, const QString& source);
    void javaScriptAlert(QWebFrame*, const QString& message);
    bool shouldInterruptJavaScript();
    QString userAgentForUrl(const QUrl& url) const;

private:
    QString defaultUserAgent;
    QHash<QString, QString> userAgents;
};

#endif // __WEBPAGE__
