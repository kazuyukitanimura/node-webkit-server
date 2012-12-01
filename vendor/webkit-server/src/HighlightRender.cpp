#include "HighlightRender.h"
#include "WebPage.h"
#include "WebPageManager.h"

HighlightRender::HighlightRender(WebPageManager *manager, QStringList &arguments, QObject *parent) : SocketCommand(manager, arguments, parent) {
}

void HighlightRender::start() {
  int width = arguments()[0].toInt();
  int height = arguments()[1].toInt();
  QStringList keyWords = arguments()[2].split(QRegExp("\\s+"));
  for ( QStringList::Iterator it = keyWords.begin(); it != keyWords.end(); ++it ) {
    page()->findText((*it).toUtf8(), (QWebPage::FindFlags) QWebPage::HighlightAllOccurrences);
  }

  QSize size(width, height);
  page()->setViewportSize(size);

  QSize pageSize = page()->mainFrame()->contentsSize();
  if (pageSize.isEmpty()) {
    emit finished(new Response(false));
  }

  QPainter p;
  QImage buffer(pageSize, QImage::Format_ARGB32);
  buffer.fill(qRgba(255, 255, 255, 0));
  p.begin(&buffer);
  p.setRenderHint( QPainter::Antialiasing,          true);
  p.setRenderHint( QPainter::TextAntialiasing,      true);
  p.setRenderHint( QPainter::SmoothPixmapTransform, true);
  page()->mainFrame()->render(&p);
  p.end();

  // Prepare buffer for writing
  QByteArray ba;
  QBuffer b(&ba);
  bool result = b.open(QIODevice::WriteOnly);
  // Writing image to the buffer, using PNG encoding
  result &= buffer.save(&b, "PNG");
  b.close();

  // The IPC cannot send binary, so use base64
  emit finished(new Response(result, ba.toBase64()));
}
