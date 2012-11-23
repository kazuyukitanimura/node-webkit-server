#include "SetHtml.h"
#include "SocketCommand.h"
#include "WebPage.h"
#include "WebPageManager.h"

SetHtml::SetHtml(WebPageManager *manager, QStringList &arguments, QObject *parent) : SocketCommand(manager, arguments, parent) {
}

void SetHtml::start() {
  if (arguments().size() > 1) {
    QUrl baseUrl = QUrl::fromEncoded(arguments()[1].toUtf8(), QUrl::StrictMode);
    page()->currentFrame()->setHtml(arguments()[0], baseUrl);
  } else {
    page()->currentFrame()->setHtml(arguments()[0]);
  }
  emit finished(new Response(true));
}
