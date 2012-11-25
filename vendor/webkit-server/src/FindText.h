#include "SocketCommand.h"

class FindText : public SocketCommand {
  Q_OBJECT

  public:
    FindText(WebPageManager *page, QStringList &arguments, QObject *parent = 0);
    virtual void start();
};

