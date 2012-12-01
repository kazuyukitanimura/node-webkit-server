#include "SocketCommand.h"
#include <QStringList>

class HighlightRender : public SocketCommand {
  Q_OBJECT

  public:
    HighlightRender(WebPageManager *page, QStringList &arguments, QObject *parent = 0);
    virtual void start();
};
