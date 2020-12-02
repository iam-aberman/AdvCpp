#ifndef SERVICE_LISTENER_H
#define SERVICE_LISTENER_H

#include "net_exception.h"
#include <iostream>

namespace net {

    class IServiceListener {
    public:
        virtual void OnNewConnection(BufferedConnection& con) = 0;
        virtual void OnWriteDone(BufferedConnection& con) = 0;
        virtual void OnReadAvailible(BufferedConnection& con) = 0;
        virtual void OnError(BufferedConnection& con) = 0;
        virtual void OnClose(BufferedConnection& con) = 0;
    };

} // namespace net

#endif //SERVICE_LISTENER_H
