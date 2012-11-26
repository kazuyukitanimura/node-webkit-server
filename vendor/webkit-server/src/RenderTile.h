#include "SocketCommand.h"
#include <QStringList>

class RenderTile : public SocketCommand {
  Q_OBJECT

  public:
    RenderTile(WebPageManager *page, QStringList &arguments, QObject *parent = 0);
    virtual void start();
};
