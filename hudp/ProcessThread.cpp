#include "Log.h"
#include "IMsg.h"
#include "ISocket.h"
#include "HudpImpl.h"
#include "ProcessThread.h"
#include "IFilterProcess.h"

using namespace hudp;

CProcessThread::CProcessThread() {

}

CProcessThread::~CProcessThread() {
    Stop();
    Push(nullptr);
    Join();
}

void CProcessThread::Start(std::shared_ptr<CFilterProcess>& filter_process) {
    _filter_process = filter_process;
    CRunnable::Start();
}

void CProcessThread::Run() {
    while (!_stop) {
        auto msg =_Pop();
        // msg not free to pool
        if (msg) {
            auto flag = msg->GetFlag();
            // get a recv msg.
            if (flag & msg_recv) {
                auto sock = msg->GetSocket();
                if (sock) {
                    sock->RecvMessage(msg);
                }

            } else {
                auto sock = msg->GetSocket();
                if (sock) {
                    sock->SendMessage(msg);
                }
            }

        } else {
            base::LOG_WARN("send thread get a null msg.");
        }
    }
}
