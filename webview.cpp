#include "webview.h"

#ifdef QT_BUILD_WITH_OPENGL
#include <QtOpenGL/QGLWidget>
#endif

#ifdef QT_BUILD_WITH_QML_API
#include <QCoreApplication>
#include <QQmlProperty>
#include <QQuickItem>

#define QML_APP_NAME "browser.qml"
#endif

WebView* WebView::webview=NULL;

WebView::WebView()
{}

WebView::WebView(const WebView&)
{}

WebView& WebView::operator=(const WebView&)
{}

WebView& WebView::instance(void)
{
    return *webview;
}

WebView::~WebView()
{
    webview=NULL;
}

WebPage& WebView::page(void)
{
    return _page_;
}

WK1WebView::WK1WebView()
{
    g_view.setScene(new QGraphicsScene(&g_view));
    g_view.scene()->setItemIndexMethod(QGraphicsScene::NoIndex);
    g_view.setAttribute(Qt::WA_DeleteOnClose);
    g_view.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    g_view.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    g_view.setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    g_view.setFrameStyle(QFrame::NoFrame);
#ifdef QT_BUILD_WITH_OPENGL
    g_view.setViewport(new QGLWidget());
#endif
    g_view.showFullScreen();

    g_view.scene()->addItem(&g_webview);
}

WK1WebView& WK1WebView::instance(void)
{
    if (webview==NULL)
        webview = new WK1WebView();

    Q_ASSERT(webview!=NULL);

    return dynamic_cast<WK1WebView&>(WebView::instance()); 
}

WK1WebView::~WK1WebView()
{
}

void WK1WebView::destroy(void)
{
    delete dynamic_cast<WK1WebView*>(webview);
}

void WK1WebView::setViewportUpdateMode(WebView::ViewportUpdateMode mode)
{
    switch(mode)
    {
        case FullViewport :
            g_view.setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
            break;
        default           :;
    }
}

void WK1WebView::resize(const QSize& _size_)
{
    g_view.resize(_size_);
    g_webview.resize(_size_);
}

void WK1WebView::load(const QUrl& _url_)
{
    g_webview.load(_url_);
}

void WK1WebView::show()
{
    g_webview.show();
}

void WK1WebView::hide()
{
    //No implementation
}

void WK1WebView::setFocus()
{
    g_webview.setFocus();
}

#ifdef QT_BUILD_WITH_QML_API
WK2WebView::WK2WebView()
{
    q_view.setSource(QUrl::fromLocalFile((QCoreApplication::applicationDirPath()+"/"+QML_APP_NAME)));
    q_view.setResizeMode(QQuickView::SizeRootObjectToView);

    //Creation from a local file should be instant. Therefore we keep it simple.
    Q_ASSERT(q_view.status() == QQuickView::Ready);
}

WK2WebView& WK2WebView::instance(void)
{
    if (webview==NULL)
        webview = new WK2WebView();

    Q_ASSERT(webview!=NULL);

    return dynamic_cast<WK2WebView&>(WebView::instance()); 
}

WK2WebView::~WK2WebView()
{
}

void WK2WebView::destroy(void)
{
    delete dynamic_cast<WK2WebView*>(webview);
}

void WK2WebView::setViewportUpdateMode(WebView::ViewportUpdateMode mode)
{
    switch(mode)
    {
        case FullViewport :
            //No implementation
            break;
        default           :;
    }
}

void WK2WebView::resize(const QSize& _size_)
{
    QObject* object=dynamic_cast<QObject*>(q_view.rootObject());
    Q_ASSERT(object != NULL);

    QQmlProperty x(object, "x");
    if(x.isValid())
        x.write(0);

    QQmlProperty y(object, "y");
    if(y.isValid())
        y.write(0);

    QQmlProperty with(object, "width");
    if(with.isValid())
        with.write(_size_.width());

    QQmlProperty height(object, "width");
    if(height.isValid())
        height.write(_size_.height());
}

void WK2WebView::load(const QUrl& _url_)
{
    QObject* object=dynamic_cast<QObject*>(q_view.rootObject());
    Q_ASSERT(object != NULL);

    QQmlProperty property(object, "url");
    if(property.isValid())
        property.write(_url_);
}

void WK2WebView::show()
{
    q_view.show();

    QObject* object=dynamic_cast<QObject*>(q_view.rootObject());
    Q_ASSERT(object != NULL);

    QQmlProperty property(object, "visible");
    if(property.isValid())
        property.write(true);
}

void WK2WebView::hide()
{
    //No implementation

/*
    q_view.hide();

    QObject* object=dynamic_cast<QObject*>(q_view.rootObject());
    Q_ASSERT(object != NULL);

    QQmlProperty property(object, "visible");
    if(property.isValid())
        property.write(false);
*/
}

void WK2WebView::setFocus()
{
    QObject* object=dynamic_cast<QObject*>(q_view.rootObject());
    Q_ASSERT(object != NULL);

    QQmlProperty property(object, "focus");
    if(property.isValid())
        property.write(false);
}
#endif
