// -------------------------------------------------------------------------
//    @FileName         ��    NFCNet.h
//    @Author           ��    Ark Game Tech
//    @Date             ��    2013-12-15
//    @Module           ��    NFIPacket
//    @Desc             :     CNet
// -------------------------------------------------------------------------

#ifndef NFC_NET_H
#define NFC_NET_H

#include "NFINet.h"
#include <thread>

#pragma pack(push, 1)


class NFCNet : public NFINet
{
public:
    NFCNet()
    {
        base = NULL;
        listener = NULL;

        mstrIP = "";
        mnPort = 0;
        mnCpuCount = 0;
        mbServer = false;
        mbWorking = false;

        mnSendMsgTotal = 0;
        mnReceiveMsgTotal = 0;
    }

    template<typename BaseType>
    NFCNet(BaseType* pBaseType, void (BaseType::*handleRecieve)(const int, const int, const char*, const uint32_t, const NFGUID&), void (BaseType::*handleEvent)(const int, const NF_NET_EVENT, const NFGUID&, const int))
    {
        base = NULL;
        listener = NULL;

        mRecvCB = std::bind(handleRecieve, pBaseType, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
        mEventCB = std::bind(handleEvent, pBaseType, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
        mstrIP = "";
        mnPort = 0;
        mnCpuCount = 0;
        mbServer = false;
        mbWorking = false;
        
        mnSendMsgTotal = 0;
        mnReceiveMsgTotal = 0;
        mnServerID = 0;

    }

    virtual ~NFCNet() 
    {
        Final();
    };

public:
    virtual bool Execute();

    virtual void Initialization(const char* strIP, const unsigned short nPort, const int nServerID);
    virtual int Initialization(const unsigned int nMaxClient, const unsigned short nPort,  const int nServerID, const int nCpuCount = 4 );
    virtual bool Final();

    //�ް�ͷ���ڲ���װ
    virtual bool SendMsgWithOutHead(const int16_t nMsgID, const char* msg, const uint32_t nLen, const int nSockIndex);
    virtual bool SendMsgWithOutHead(const int16_t nMsgID, const char* msg, const uint32_t nLen, const NFGUID& xClientID);

    //�ް�ͷ���ڲ���װ
    virtual bool SendMsgWithOutHead(const int16_t nMsgID, const char* msg, const uint32_t nLen, const std::list<int>& fdList);

    //�ް�ͷ���ڲ���װ
    virtual bool SendMsgToAllClientWithOutHead(const int16_t nMsgID, const char* msg, const uint32_t nLen);


    virtual bool CloseNetObject(const int nSockIndex);
    virtual bool CloseNetObject(const NFGUID& xClient);

    virtual bool IsServer();
    virtual bool Log(int severity, const char* msg);

private:
    //�Ѵ��ϰ�ͷ
    bool SendMsgToAllClient(const char* msg, const uint32_t nLen);

    //�Ѵ��ϰ�ͷ
    bool SendMsg(const char* msg, const uint32_t nLen, const std::list<int>& fdList);

    //�Ѵ��ϰ�ͷ
    bool SendMsg(const char* msg, const uint32_t nLen, const int nSockIndex);
    bool SendMsg(const char* msg, const uint32_t nLen, const NFGUID& xClient);

    bool AddNetObject(const int nSockIndex, NetObject* pObject);
    NetObject* GetNetObject(const int nSockIndex);


private:
    void ExecuteClose();
    bool CloseSocketAll();

    bool Dismantle(NetObject* pObject);


    int InitClientNet();
    int InitServerNet();
    void CloseObject(const int nSockIndex);

    static void listener_cb(struct evconnlistener* listener, evutil_socket_t fd, struct sockaddr* sa, int socklen, void* user_data);
    static void conn_readcb(struct bufferevent* bev, void* user_data);
    static void conn_writecb(struct bufferevent* bev, void* user_data);
    static void conn_eventcb(struct bufferevent* bev, short events, void* user_data);
    static void log_cb(int severity, const char* msg);

protected:
    int DeCode(const char* strData, const uint32_t unLen, NFCMsgHead& xHead);
    int EnCode(const uint16_t unMsgID, const char* strData, const uint32_t unDataLen, std::string& strOutData);

private:
    //<fd,object>


    std::map<int, NetObject*> mmObject;
    std::vector<int> mvRemoveObject;

    int mnMaxConnect;
    std::string mstrIP;
    int mnPort;
    int mnCpuCount;
    bool mbServer;
    int mnServerID;

    bool mbWorking;

    int64_t mnSendMsgTotal;
    int64_t mnReceiveMsgTotal;

    struct event_base* base;
    struct evconnlistener* listener;
    //////////////////////////////////////////////////////////////////////////

    NET_RECEIVE_FUNCTOR mRecvCB;
    NET_EVENT_FUNCTOR mEventCB;

    //////////////////////////////////////////////////////////////////////////
private:
    static uint64_t mObjectIndex;
    
};

#pragma pack(pop)

#endif
