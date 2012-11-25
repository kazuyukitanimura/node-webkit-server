#include "FindText.h"
#include "SocketCommand.h"
#include "WebPage.h"
#include "WebPageManager.h"

FindText::FindText(WebPageManager *manager, QStringList &arguments, QObject *parent) : SocketCommand(manager, arguments, parent) {
}

void FindText::start() {
  page()->findText(arguments()[0].toUtf8(), (QWebPage::FindFlags) QWebPage::HighlightAllOccurrences);
  emit finished(new Response(true));
}

