#include "HighlightRender.h"
#include "WebPage.h"
#include "WebPageManager.h"

HighlightRender::HighlightRender(WebPageManager *manager, QStringList &arguments, QObject *parent) : SocketCommand(manager, arguments, parent) {
}

void HighlightRender::start() {
  int i = 0;
  //QUrl requestedUrl = QUrl::fromEncoded(arguments()[i++].toUtf8(), QUrl::StrictMode);
  //page()->currentFrame()->load(QUrl(requestedUrl));

  QString url = arguments()[i++];
  int width = arguments()[i++].toInt();
  QStringList keyWords = arguments()[i++].split(QRegExp("\\s+"));

  QByteArray ba = page()->highlightRect(url, keyWords, width);

  // The IPC cannot send binary, so use base64
  //emit finished(new Response(result, ba.toBase64()));
  emit finished(new Response(true, ba.toBase64()));
}
