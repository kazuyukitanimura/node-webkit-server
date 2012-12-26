#include "HighlightRender.h"
#include "WebPage.h"
#include "WebPageManager.h"

HighlightRender::HighlightRender(WebPageManager *manager, QStringList &arguments, QObject *parent) : SocketCommand(manager, arguments, parent) {
}

void HighlightRender::start() {
  QString url = arguments()[0];
  int width = arguments()[1].toInt();
  QStringList keyWords = arguments()[2].split(QRegExp("\\s+"));

  // Turn off the scroll bars
  //page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
  //page()->mainFrame()->setScrollBarPolicy(Qt::Vertical,   Qt::ScrollBarAlwaysOff);
  // Set white background
  //page()->setPalette(QPalette(Qt::white));

  // Highlight the keywords
  //for ( QStringList::Iterator it = keyWords.begin(); it != keyWords.end(); ++it ) {
  //  page()->findText((*it).toUtf8(), (QWebPage::FindFlags) QWebPage::HighlightAllOccurrences);
  //}
  QByteArray ba = page()->highlightRect(url, keyWords, width);

  //QSize pageSize = page()->mainFrame()->contentsSize();
  //if (pageSize.isEmpty()) {
  //  emit finished(new Response(false));
  //}
  //pageSize.setWidth(width);
  //page()->setViewportSize(pageSize);
  //QSize pageSize = page()->viewportSize();

  //QPainter p;
  //QImage buffer(pageSize, QImage::Format_ARGB32);
  //p.begin(&buffer);
  //p.setRenderHint( QPainter::Antialiasing,          true);
  //p.setRenderHint( QPainter::TextAntialiasing,      true);
  //p.setRenderHint( QPainter::SmoothPixmapTransform, true);
  //page()->mainFrame()->render(&p);
  //p.end();

  //// Prepare buffer for writing
  //QByteArray ba;
  //QBuffer b(&ba);
  //bool result = b.open(QIODevice::WriteOnly);
  //// Writing image to the buffer, using PNG encoding
  //result &= buffer.save(&b, "PNG");
  //b.close();

  // The IPC cannot send binary, so use base64
  //emit finished(new Response(result, ba.toBase64()));
  emit finished(new Response(true, ba.toBase64()));
}
