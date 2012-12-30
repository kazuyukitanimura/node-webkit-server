#include "HighlightRender.h"
#include "WebPage.h"
#include "WebPageManager.h"

HighlightRender::HighlightRender(WebPageManager *manager, QStringList &arguments, QObject *parent) : SocketCommand(manager, arguments, parent) {
}

void HighlightRender::start() {
  int i = 0;

  int width = arguments()[i++].toInt();
  QStringList keyWords = arguments()[i++].split(QRegExp("\\s+"));

  QByteArray ba = page()->highlightRect(keyWords, width);

  // The IPC cannot send binary, so use base64
  //emit finished(new Response(result, ba.toBase64()));
  emit finished(new Response(true, ba.toBase64()));
}
