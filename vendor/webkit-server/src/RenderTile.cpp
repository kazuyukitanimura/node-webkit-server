#include "RenderTile.h"
#include "WebPage.h"
#include "WebPageManager.h"

RenderTile::RenderTile(WebPageManager *manager, QStringList &arguments, QObject *parent) : SocketCommand(manager, arguments, parent) {
}

void RenderTile::start() {
  int width      = arguments()[0].toInt();
  int height     = arguments()[1].toInt();
  int tileWidth  = arguments()[2].toInt();
  int tileHeight = arguments()[3].toInt();

  // https://github.com/ariya/phantomjs/blob/master/src/webpage.cpp

  QSize viewportSize(width, height);
  page()->setViewportSize(viewportSize);

  QSize pageSize = page()->mainFrame()->contentsSize();
  if (pageSize.isEmpty()) {
    emit finished(new Response(false));
  }
  QRect frameRect = QRect(QPoint(0, 0), pageSize);

  QPainter p;

  int vtiles = (pageSize.height() + tileHeight - 1) / tileHeight;
  int htiles = (pageSize.width() + tileWidth - 1) / tileWidth;
  for (int y = 0; y < vtiles; ++y) {
    for (int x = 0; x < htiles; ++x) {
      QImage tileBuffer(tileWidth, tileHeight, QImage::Format_ARGB32);
      tileBuffer.fill(qRgba(255, 255, 255, 0));
      p.begin(&tileBuffer);
      p.setRenderHint( QPainter::Antialiasing,          true);
      p.setRenderHint( QPainter::TextAntialiasing,      true);
      p.setRenderHint( QPainter::SmoothPixmapTransform, true);
      p.translate(-frameRect.left(), -frameRect.top());
      p.translate(-x * tileWidth, -y * tileHeight);
      page()->mainFrame()->render(&p, QRegion(frameRect));
      p.end();

      // Prepare buffer for writing
      QByteArray ba;
      QBuffer b(&ba);
      bool result = b.open(QIODevice::WriteOnly);
      // Writing image to the buffer, using PNG encoding
      result &= tileBuffer.save(&b, "PNG");
      b.close();

      // The current IPC read binary as utf8, so use base64 for now
      emit finished(new Response(result, ba.toBase64()));
    }
  }
}
