#include "OrderServantImp.h"
#include "servant/Application.h"
#include "uuid.h"
#include "DataProxyProto.h"
#include "ServiceUtil.h"
#include "LogComm.h"
#include "globe.h"
#include "OrderServer.h"
#include "util/tc_hash_fun.h"
#include "Request.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "HallServant.h"
#include <jwt-cpp/jwt.h>
#include <iostream>
#include <chrono>
#include "LogDefine.h"
#include "RsaEncrypt.h"
#include "Processor.h"
#include "DBOperator.h"
#include "util/tc_md5.h"
#include "JFGameHttpProto.h"
#include "CommonCode.pb.h"

//
using namespace std;

//
using namespace dataproxy;
using namespace rapidjson;
using namespace JFGameHttpProto;
using namespace XGameRetCode;

namespace
{
     const Value &getValue(const Value &d, const string &key)
    {
        if (!d.HasMember(key.c_str()))
        {
            throw logic_error("property " + key + " not exsist.");
        }

        return d[key.c_str()];
    }

    template<typename T>
    T parseValue(const Value &v)
    {
        throw logic_error("must pass value type.");
    }

    template<>
    int parseValue<int>(const Value &v)
    {
        if (!v.IsInt())
        {
            throw logic_error("Not integer Type.");
        }

        return v.GetInt();
    }

    template<>
    string parseValue<string>(const Value &v)
    {
        if (!v.IsString())
        {
            throw logic_error("Not String Type.");
        }

        return v.GetString();
    }

    //产生uuid串
    string generateUUIDStr()
    {
        uuid_t uuid;
        uuid_generate(uuid);

        char buf[1024];
        memset(buf, 0, sizeof(buf));

        uuid_unparse(uuid, buf);

        string strRet;
        strRet.assign(buf, strlen(buf));

        return strRet;
    }

    // 订单日志相关
    std::string GetTimeFormat()
    {
        std::string sFormat("%Y-%m-%d %H:%M:%S");
        time_t t = time(NULL);
        struct tm *pTm = localtime(&t);
        if (pTm == NULL)
        {
            return "";
        }

        ///
        char sTimeString[255] = "\0";
        strftime(sTimeString, sizeof(sTimeString), sFormat.c_str(), pTm);
        return std::string(sTimeString);
    }

    //几天之前的时间
    std::string GetTimeDayLater(int days)
    {
        std::string sFormat("%Y-%m-%d %H:%M:%S");
        time_t t = time(NULL) - 24 * 3600 * days;
        struct tm *pTm = localtime(&t);
        if (pTm == NULL)
        {
            return "";
        }

        ///
        char sTimeString[255] = "\0";
        strftime(sTimeString, sizeof(sTimeString), sFormat.c_str(), pTm);
        return std::string(sTimeString);
    }
}

//////////////////////////////////////////////////////
void OrderServantImp::initialize()
{
    //initialize servant here:

/*    string str = "[{\"amount\":28.0114560,\"fromAddress\":\"TVQrm58BBn4N8RTaUpcsvJA5hUvgJJPazP\",\"markStatus\":2,\"toAddress\":\"TZ3huYXxbB2rtK52PzYNnHfa4cJsT8EpKM\",\"txId\":\"16989147717031000021110\"}]";
    walletOrderCallback(str, 2);
*/
    //初始化收银台机器人账户地址
    auto vecRobot = g_app.getOuterFactoryPtr()->split(g_app.getOuterFactoryPtr()->getRobotId(), "|");
    for(auto robotId : vecRobot)
    {
        Json::Value postJson;
        //获取充玩家钱包信息
        if(getWalletAddressAndKey(S2L(robotId), postJson) != 0)
        {
            ROLLLOG_ERROR << "select wallet address and key err. robotId:"<< robotId << endl;
            continue;
        }
    }
}

//////////////////////////////////////////////////////
void OrderServantImp::destroy()
{
    //destroy servant here:
    //...
}

//人工扣除
tars::Int32 OrderServantImp::takeRecharge(tars::Int64 lUid, const string &sDesc, const string &sOptUser, const vector<tars::Int64> &vecUid, const map<tars::Int64, tars::Int64> &mapRecharge, tars::TarsCurrentPtr current)
{
    FUNC_ENTRY("");

    int iRet = 0;

    __TRY__

    ROLLLOG_DEBUG << "lUid: "<< lUid << "vecUid isze: " << vecUid.size() << ", mapRealRecharge size: "<< mapRecharge.size() << endl;
    
    if (lUid != 0)
    {
        return iRet;
    }

    for(auto rechange : mapRecharge)
    {
        LOG_DEBUG << "rechange first: "<< rechange.first << ", rechange second: "<< rechange.second << endl;
        long lPropsId = rechange.first;
        long lCount = rechange.second;
        bool bBag = false;
        if (lCount > 0)
        {
            continue;
        }

        for (auto uid : vecUid)
        {
            userinfo::ModifyUserWealthReq  modifyUserWealthReq;
            modifyUserWealthReq.uid = uid;
            if(lPropsId == 10000)
            {
                modifyUserWealthReq.diamondChange = lCount;
            }
            else if(lPropsId == 20000)
            {
                modifyUserWealthReq.goldChange = lCount;
            }
            else if (lPropsId == 70000)
            {
                modifyUserWealthReq.pointChange = lCount;
            }
            else
            {
                bBag = true;
            }

            if (bBag)
            {
                userinfo::ModifyUserBagInfo info;
                info.uid = uid;
                info.pid = lPropsId;
                info.count = lCount;
                info.changeType = XGameProto::GOLDFLOW::GOLDFLOW_ID_BACK_SYS_OUT;
                info.relatedId = L2S(lUid) + "|" + sOptUser + "|" + sDesc;

                userinfo::ModifyUserBagReq req;
                req.updateInfo.push_back(info);
                g_app.getOuterFactoryPtr()->getHallServantPrx(uid)->async_modifyUserBag(NULL, req);
            }
            else
            {
                modifyUserWealthReq.changeType = XGameProto::GOLDFLOW::GOLDFLOW_ID_BACK_SYS_OUT;
                modifyUserWealthReq.relateId = L2S(lUid) + "|" + sOptUser + "|" + sDesc;

                userinfo::ModifyUserWealthResp  modifyUserWealthResp;
                iRet = g_app.getOuterFactoryPtr()->getHallServantPrx(uid)->modifyUserWealth(modifyUserWealthReq, modifyUserWealthResp);
            }
            if (iRet != 0)
            {
                LOG_ERROR << "cost gold err. iRet: " << iRet << endl;
                iRet = -3;
            }
        }
    }

    __CATCH__
    FUNC_EXIT("", iRet);
    return iRet;
}

//人工充值
tars::Int32 OrderServantImp::giveRecharge(tars::Int64 lUid, const string &sDesc, const string &sOptUser, const vector<tars::Int64> &vecUid, const map<tars::Int64, tars::Int64> &mapRecharge, tars::TarsCurrentPtr current)
{
    FUNC_ENTRY("");

    int iRet = 0;

    __TRY__

    ROLLLOG_DEBUG << "lUid: "<< lUid << "vecUid isze: " << vecUid.size() << ", mapRealRecharge size: "<< mapRecharge.size() << endl;
    
    for(auto rechange : mapRecharge)
    {
        LOG_DEBUG << "rechange first: "<< rechange.first << ", rechange second: "<< rechange.second << endl;
        long lPropsId = rechange.first;
        long lCount = rechange.second;
        bool bBag = false;
        if (lUid > 0)
        {
            long lTotalCount = lCount * vecUid.size();
            userinfo::ModifyUserWealthReq  modifyUserWealthReq;
            modifyUserWealthReq.uid = lUid;
            if (lPropsId == 10000)
            {
                modifyUserWealthReq.diamondChange = -lTotalCount;
            }
            else if (lPropsId == 20000)
            {
                modifyUserWealthReq.goldChange = -lTotalCount;
            }
            else if (lPropsId == 70000)
            {
                modifyUserWealthReq.pointChange = -lTotalCount;
            }
            else
            {
                bBag = true;
                if (g_app.getOuterFactoryPtr()->getHallServantPrx(lUid)->getUserBagById(lUid, lPropsId) < lTotalCount)
                {
                    ROLLLOG_DEBUG << "giveRecharge totalCount:" << lTotalCount << ", lUid: "<< lUid << "vecUid isze: " << vecUid.size() << endl;
                    iRet = -1;
                    continue;
                }

                userinfo::ModifyUserBagInfo info;
                info.uid = lUid;
                info.pid = lPropsId;
                info.count = -lTotalCount;
                info.changeType = XGameProto::GOLDFLOW::GOLDFLOW_ID_BACK_PRT_OUT;
                info.relatedId = L2S(lUid) + "|" + sOptUser + "|" + sDesc;

                userinfo::ModifyUserBagReq req;
                req.updateInfo.push_back(info);
                g_app.getOuterFactoryPtr()->getHallServantPrx(lUid)->async_modifyUserBag(NULL, req);
            }

            if (bBag == false)
            {
                modifyUserWealthReq.changeType = XGameProto::GOLDFLOW::GOLDFLOW_ID_BACK_PRT_OUT;
                modifyUserWealthReq.relateId = L2S(lUid) + "|" + sOptUser + "|" + sDesc;

                userinfo::ModifyUserWealthResp  modifyUserWealthResp;
                iRet = g_app.getOuterFactoryPtr()->getHallServantPrx(lUid)->modifyUserWealth(modifyUserWealthReq, modifyUserWealthResp);
                if (iRet != 0)
                {
                    LOG_ERROR << "cost gold err. iRet: " << iRet << endl;
                    iRet = -2;
                    continue;
                }
            }
        }
        for (auto uid : vecUid)
        {
            userinfo::ModifyUserWealthReq  modifyUserWealthReq;
            modifyUserWealthReq.uid = uid;
            if(lPropsId == 10000)
            {
                modifyUserWealthReq.diamondChange = lCount;
            }
            else if(lPropsId == 20000)
            {
                modifyUserWealthReq.goldChange = lCount;
            }
            else if (lPropsId == 70000)
            {
                modifyUserWealthReq.pointChange = lCount;
            }
            else
            {
                bBag = true;
            }

            if (bBag)
            {
                userinfo::ModifyUserBagInfo info;
                info.uid = uid;
                info.pid = lPropsId;
                info.count = lCount;
                info.changeType = lUid == 0 ? XGameProto::GOLDFLOW::GOLDFLOW_ID_BACK_SYS_IN : XGameProto::GOLDFLOW::GOLDFLOW_ID_BACK_PRT_IN;
                info.relatedId = L2S(lUid) + "|" + sOptUser + "|" + sDesc;

                userinfo::ModifyUserBagReq req;
                req.updateInfo.push_back(info);
                g_app.getOuterFactoryPtr()->getHallServantPrx(uid)->async_modifyUserBag(NULL, req);
            }
            else
            {
                modifyUserWealthReq.changeType = lUid == 0 ? XGameProto::GOLDFLOW::GOLDFLOW_ID_BACK_SYS_IN : XGameProto::GOLDFLOW::GOLDFLOW_ID_BACK_PRT_IN;;
                modifyUserWealthReq.relateId = L2S(lUid) + "|" + sOptUser + "|" + sDesc;

                userinfo::ModifyUserWealthResp  modifyUserWealthResp;
                iRet = g_app.getOuterFactoryPtr()->getHallServantPrx(uid)->modifyUserWealth(modifyUserWealthReq, modifyUserWealthResp);
            }
            if (iRet != 0)
            {
                LOG_ERROR << "cost gold err. iRet: " << iRet << endl;
                iRet = -3;
            }
        }
    }

    __CATCH__
    FUNC_EXIT("", iRet);
    return iRet;
}

//人工充值
tars::Int32 OrderServantImp::manualRecharge(tars::Int64 lUin, tars::Int64 lCid, const map<tars::Int64, tars::Int64> &mapRecharge, const map<tars::Int64, tars::Int64> &mapRealRecharge, tars::TarsCurrentPtr current)
{
    FUNC_ENTRY("");

    int iRet = 0;

    __TRY__

    ROLLLOG_DEBUG << "lUin: "<< lUin << "lCid : " << lCid << ", mapRealRecharge size: "<< mapRealRecharge.size() << endl;
    
    //单位 分， 钻石 1：100  基金 1：10
    for(auto rechange : mapRealRecharge)
    {
        LOG_DEBUG << "rechange first: "<< rechange.first << ", rechange second: "<< rechange.second << endl;
        long applyAmount = rechange.second;
        auto it = mapRecharge.find(rechange.first);
        if(it != mapRecharge.end())
        {
            applyAmount = it->second;
        }

        string merchantOrderId = L2S(TNOWMS) + L2S(lUin) + I2S(rand() % 10 + 1);//20位 唯一订单号(时间戳 + UID + 随机数)

        WalletOrderInfo order_info;
        order_info.uid = lUin;
        order_info.inOrderID = merchantOrderId;
        order_info.outOrderID = "";
        order_info.tradeType = 1;
        order_info.channelType = 2;
        order_info.status = 3;
        order_info.applyAmount = I2S(applyAmount);
        order_info.amount = I2S(rechange.second);
        order_info.clubId = lCid;
        order_info.productID = rechange.first == 10000 ? "1001" : (rechange.first == 20000 ? "1002" : "1003") ;
        order_info.payTime = g_app.getOuterFactoryPtr()->GetTimeFormat();
        order_info.createTime = g_app.getOuterFactoryPtr()->GetTimeFormat();
        iRet = ProcessorSingleton::getInstance()->createWalletOrder(merchantOrderId, order_info);
        if(iRet != 0)
        {
            ROLLLOG_ERROR<<" create rcaharge order err."<< endl;
            iRet = -2;
        }

        if(lCid > 0)
        {
            long amount = rechange.second * 10;//(rechange.second / 100) * 10 * 100
            g_app.getOuterFactoryPtr()->updateClubCoinByManual(lUin, lCid, amount);
        }
        else
        {
            long amount = rechange.second * 100 ;//(rechange.second / 100) * 100
            userinfo::ModifyUserWealthReq  modifyUserWealthReq;
            modifyUserWealthReq.uid = lUin;
            if(rechange.first == 10000)
            {
                modifyUserWealthReq.diamondChange = amount;
            }
            else if(rechange.first == 20000)
            {
                modifyUserWealthReq.goldChange = amount;
            }
            else
            {
                modifyUserWealthReq.pointChange = amount;
            }

            modifyUserWealthReq.changeType = XGameProto::GOLDFLOW::GOLDFLOW_ID_WALLET_RECHARGE;
            modifyUserWealthReq.relateId = merchantOrderId;

            userinfo::ModifyUserWealthResp  modifyUserWealthResp;
            iRet = g_app.getOuterFactoryPtr()->getHallServantPrx(lUin)->modifyUserWealth(modifyUserWealthReq, modifyUserWealthResp);
            if (iRet != 0)
            {
                LOG_ERROR << "cost gold err. iRet: " << iRet << endl;
                iRet = -3;
            }
        }
    }

    __CATCH__
    FUNC_EXIT("", iRet);
    return iRet;
}

//创建订单
tars::Int32 OrderServantImp::createOrder(const map<string, string> &mapOrderInfo, tars::TarsCurrentPtr current)
{
    int iRet = 0;

    map<string, string> mapInfo = const_cast<map<string, string>&>(mapOrderInfo);
    long uid = DBOperatorSingleton::getInstance()->getUidByWalletAddress(mapInfo["fromAddr"]);
    if(uid <= 0)
    {
        LOG_ERROR << "select uid err. toAddress:"<< mapInfo["fromAddr"] << endl;
        return -1;
    }

    userinfo::GetUserAccountReq userAccountReq;
    userinfo::GetUserAccountResp userAccountResp;
    userAccountReq.uid = uid;
    iRet = g_app.getOuterFactoryPtr()->getHallServantPrx(userAccountReq.uid)->getUserAccount(userAccountReq, userAccountResp);
    if (iRet != 0)
    {
        ROLLLOG_ERROR << "getUserAccount failed, uid: " << userAccountReq.uid << endl;
        return -2;
    }

    WalletOrderInfo order_info;
    order_info.uid = uid;
    order_info.inOrderID = L2S(TNOWMS) + L2S(uid) + I2S(rand() % 10 + 1);
    order_info.outOrderID = mapInfo["outOrderID"];
    order_info.tradeType = 1;
    order_info.channelType = 1;
    order_info.orderType = S2I(mapInfo["orderType"]);
    order_info.applyAmount = mapInfo["amount"];
    order_info.amount = mapInfo["amount"];
    order_info.addrType = 1;
    order_info.sandbox = userAccountResp.useraccount.isinwhitelist;
    order_info.status = 0;
    order_info.productID = "";
    order_info.clubId = 0;
    order_info.fromAddr = mapInfo["fromAddr"];
    order_info.toAddr = mapInfo["toAddr"];
    order_info.createTime = g_app.getOuterFactoryPtr()->GetTimeFormat();

    iRet = ProcessorSingleton::getInstance()->createWalletOrder(order_info.outOrderID, order_info);
    if(iRet != 0)
    {
        ROLLLOG_ERROR<<" create recharge order err."<< endl;
        return iRet;
    }
    return 0;
}

//更新订单
tars::Int32 OrderServantImp::updateOrder(const map<string, string> &mapOrderInfo, tars::TarsCurrentPtr current)
{
    return ProcessorSingleton::getInstance()->updateOrder(mapOrderInfo);
}

tars::Int32 OrderServantImp::modifyWalletBalance(const order::ModifyWalletBalanceReq &req, order::ModifyWalletBalanceResp &resp, tars::TarsCurrentPtr current)
{
    FUNC_ENTRY("");

    int iRet = 0;

    __TRY__

    long balance = 0;
    iRet = updateTgBalance(req.uid, req.type, req.amount, balance);
    if(iRet != 0)
    {
        ROLLLOG_ERROR << "update balance err. iRet: "<< iRet << endl;
    }

    resp.ret = iRet;
    resp.uid = req.uid;
    resp.amount = req.amount;
    resp.balance = balance;

    ROLLLOG_DEBUG << "resp: "<< printTars(resp) << endl;

    __CATCH__
    FUNC_EXIT("", iRet);
    return iRet;
}

tars::Int32 OrderServantImp::bindWalletNotify(tars::Int64 lUin, tars::TarsCurrentPtr current)
{
    FUNC_ENTRY("");

    int iRet = 0;

    __TRY__

    userinfo::GetUserAccountReq userAccountReq;
    userinfo::GetUserAccountResp userAccountResp;
    userAccountReq.uid = lUin;
    iRet = g_app.getOuterFactoryPtr()->getHallServantPrx(userAccountReq.uid)->getUserAccount(userAccountReq, userAccountResp);
    if (iRet != 0)
    {
        ROLLLOG_ERROR << "getUserAccount failed, uid: " << userAccountReq.uid << endl;
        return -1;
    }

    //推送通知
    push::PushMsgReq pushMsgReq;
    push::PushMsg pushMsg;
    pushMsg.uid = lUin;
    pushMsg.msgType = push::E_PUSH_MSG_TYPE_BLIND_TG_NOTIFY;

    push::TGBindCallBackNotify tGBindCallBackNotify;
    tGBindCallBackNotify.tgId = userAccountResp.useraccount.bindTgId;
    tGBindCallBackNotify.tgName = userAccountResp.useraccount.tgName;
    tobuffer(tGBindCallBackNotify, pushMsg.vecData);
    pushMsgReq.msg.push_back(pushMsg);
    g_app.getOuterFactoryPtr()->asyncPushBindTg(pushMsg.uid, pushMsgReq);

    ROLLLOG_DEBUG << "push bind tg, data: "<< printTars(tGBindCallBackNotify)<< endl;

    //同步余额
    userinfo::GetUserBasicReq basicReq;
    basicReq.uid = lUin;
    userinfo::GetUserBasicResp basicResp;
    iRet = g_app.getOuterFactoryPtr()->getHallServantPrx(basicReq.uid)->getUserBasic(basicReq, basicResp);
    if (iRet != 0)
    {
        ROLLLOG_ERROR << " getUserBasic err. iRet:  " << iRet  << endl;
        return iRet;
    }

    if(basicResp.userinfo.gold > 0)
    {
        long balance = 0;
        iRet = updateTgBalance(lUin, 725, basicResp.userinfo.gold, balance);
        if(iRet != 0)
        {
            ROLLLOG_ERROR << "update balance err." << endl;
            return iRet;
        }

        userinfo::ModifyUserWealthReq  modifyUserWealthReq;
        modifyUserWealthReq.uid = lUin;
        modifyUserWealthReq.relateId = userAccountResp.useraccount.tgName;
        modifyUserWealthReq.goldChange = -basicResp.userinfo.gold;
        modifyUserWealthReq.changeType = XGameProto::GOLDFLOW::GOLDFLOW_ID_BLIND_TG;

        userinfo::ModifyUserWealthResp modifyUserWealthResp;
        iRet = g_app.getOuterFactoryPtr()->getHallServantPrx(lUin)->modifyUserWealth(modifyUserWealthReq, modifyUserWealthResp);
        if (iRet != 0)
        {
            LOG_ERROR << "update wealth err. iRet: " << iRet << endl;
            iRet = -2;
        }
    }

    __CATCH__
    FUNC_EXIT("", iRet);
    return iRet;
}

tars::Int64 OrderServantImp::selectWalletBalance(tars::Int64 lUin, tars::TarsCurrentPtr current)
{
    FUNC_ENTRY("");

    int iRet = 0;
    long balance = 0;
    __TRY__

    iRet = getTgBalance(lUin, balance);
    if(iRet != 0)
    {
        ROLLLOG_ERROR << "select balance err. iRet: "<< iRet << endl;
    }

    __CATCH__
    FUNC_EXIT("", iRet);
    return balance;
}


//http请求处理接口
tars::Int32 OrderServantImp::doRequestJson(const string &reqBuf, const string &urlKey, string &rspBuf, tars::TarsCurrentPtr current)
{
    FUNC_ENTRY("");

    int iRet = 0;

    __TRY__

    ROLLLOG_DEBUG << "recive request, reqBuf size : " << reqBuf.size() << ", urlKey: "<< urlKey << endl;

    if (reqBuf.empty())
    {
        iRet = -1;
        return iRet;
    }


    long uid = 0;
    if(urlKey.find("/wallet") != string::npos)//钱包订单回调
    {
        int order_type = urlKey.find("/recharge") != string::npos ? 1 : ( urlKey.find("/withdraw") != string::npos ? 2 : 0);
        iRet = walletOrderCallback(reqBuf, order_type);
    }
    else if(urlKey.find("/user/bindTg") != string::npos || urlKey.find("/user/bindtg") != string::npos)//绑定TG回调
    {
        iRet = bindTgCallback(reqBuf, uid);
    }
    else
    {
        iRet = -2;
    }

    rspBuf = uid > 0 ? L2S(uid) : "";
    ROLLLOG_DEBUG << "response buff size: " << rspBuf << endl;

    __CATCH__
    FUNC_EXIT("", iRet);
    return iRet;
}

//tcp请求处理接口
tars::Int32 OrderServantImp::onRequest(tars::Int64 lUin, const std::string &sMsgPack, const std::string &sCurServrantAddr, const JFGame::TClientParam &stClientParam, const JFGame::UserBaseInfoExt &stUserBaseInfo, tars::TarsCurrentPtr current)
{
    int iRet = 0;

    try
    {
        ROLLLOG_DEBUG << "recv msg, uid : " << lUin << ", addr : " << stClientParam.sAddr << endl;

        OrderServantImp::async_response_onRequest(current, 0);

        XGameComm::TPackage pkg;
        pbToObj(sMsgPack, pkg);
        if (pkg.vecmsghead_size() == 0)
        {
            ROLLLOG_DEBUG << "package empty." << endl;
            return -1;
        }

        ROLLLOG_DEBUG << "recv msg, uid : " << lUin << ", msg : " << logPb(pkg) << endl;

        _userInfo[lUin] = stUserBaseInfo;
        _userIp[lUin] = stClientParam.sAddr;
        for (int i = 0; i < pkg.vecmsghead_size(); ++i)
        {
            int64_t ms1 = TNOWMS;

            auto &head = pkg.vecmsghead(i);
            switch (head.nmsgid())
            {
            case XGameProto::ActionName::WALLET_CREATE_RECHARGE_ORDER: //// 创建钱包充值订单
            {
                orderProto::CreateWalletRechargeOrderReq walletrechatgeorderReq;
                pbToObj(pkg.vecmsgdata(i), walletrechatgeorderReq);
                iRet = onCreateWalletRechargeOrder(pkg, sCurServrantAddr, walletrechatgeorderReq, current);
                break;
            }
            case XGameProto::ActionName::WALLET_ORDER_STATUS_QUERY: //// 钱包订单充值状态
            {
                orderProto::QueryWalletOrderStatusReq queryWalletOrderStatusReq;
                pbToObj(pkg.vecmsgdata(i), queryWalletOrderStatusReq);
                iRet = onQueryWalletOrderStatus(pkg, sCurServrantAddr, queryWalletOrderStatusReq, current);
                break;
            }
            case XGameProto::ActionName::WALLET_WITHDRAW_CFG_QUERY: //// 钱包提现配置请求
            {
                iRet = onQueryWalletWithdrawCfg(pkg, sCurServrantAddr, current);
                break;
            }
            case XGameProto::ActionName::WALLET_CREATE_WITHDRAW_ORDER: //// 创建钱包提现订单
            {
                orderProto::CreateWalletWithdrawOrderReq walletwithdraworderReq;
                pbToObj(pkg.vecmsgdata(i), walletwithdraworderReq);
                iRet = onCreateWalletWithdrawOrder(pkg, sCurServrantAddr, walletwithdraworderReq, current);
                break;
            }
            case XGameProto::ActionName::WALLET_ORDER_LIST_QUERY: //// 钱包订单记录
            {
                orderProto::WalletOrderListReq walletorderlistReq;
                pbToObj(pkg.vecmsgdata(i), walletorderlistReq);
                iRet = onQueryWalletOrderList(pkg, sCurServrantAddr, walletorderlistReq, current);
                break;
            }
            case XGameProto::ActionName::FLOW_HISTORY_RECORD_LIST: //// 历史流水记录
            {
                orderProto::FlowRecordReq flowRecordReq;
                pbToObj(pkg.vecmsgdata(i), flowRecordReq);
                iRet = onQueryFlowRecordList(pkg, sCurServrantAddr, flowRecordReq, current);
                break;
            }
            case XGameProto::ActionName::CALL_ME_INFO: //// 客服
            {
                iRet = onQueryCallMeInfo(pkg, sCurServrantAddr, current);
                break;
            }
            case XGameProto::ActionName::BIND_TG: //// 绑定TG
            {
                orderProto::BindTGReq bindTGReq;
                pbToObj(pkg.vecmsgdata(i), bindTGReq);
                iRet = onBindTG(pkg, sCurServrantAddr, bindTGReq, current);
                break;
            }
            case XGameProto::ActionName::TG_BALANCE: //// TG 余额
            {
                iRet = onGetTgBalance(pkg, sCurServrantAddr, current);
                break;
            }
            case XGameProto::ActionName::MALL_EXCHANGE:
            {
                orderProto::MallExchangeReq mallExchangeReq;
                pbToObj(pkg.vecmsgdata(i), mallExchangeReq);
                iRet = onCoinExchange(pkg, mallExchangeReq, sCurServrantAddr, current);
                break;
            }
            case XGameProto::ActionName::AGENCY_BILL:
            {
                orderProto::AgencyBillReq agencyBillReq;
                pbToObj(pkg.vecmsgdata(i), agencyBillReq);
                iRet = onGetAgencyBill(pkg, agencyBillReq, sCurServrantAddr, current);
                break;
            }
            case XGameProto::ActionName::GOODS_TRANSFER:
            {
                orderProto::GoodsTransferReq goodsTransferReq;
                pbToObj(pkg.vecmsgdata(i), goodsTransferReq);
                iRet = onGoodsTransfer(pkg, goodsTransferReq, sCurServrantAddr, current);
                break;
            }
            case XGameProto::ActionName::ORDER_YIELD:  //// 生成订单号
            {
                orderProto::OrderYieldReq orderYieldReq;
                pbToObj(pkg.vecmsgdata(i), orderYieldReq);
                iRet = onOrderYield(pkg, sCurServrantAddr, orderYieldReq, current);
                break;
            }
            case XGameProto::ActionName::ORDER_VERRITY:  //// 校验订单号
            {
                orderProto::OrderVerifyReq orderVerifyReq;
                pbToObj(pkg.vecmsgdata(i), orderVerifyReq);
                iRet = onOrderVerify(pkg, sCurServrantAddr, orderVerifyReq, current);
                break;
            }
            default:
            {
                ROLLLOG_ERROR << "invalid msg id, uid: " << lUin << ", msg id: " << head.nmsgid() << endl;
                break;
            }
            }

            if (iRet != 0)
            {
                ROLLLOG_ERROR << "msg process fail, uid: " << lUin << ", msg id: " << head.nmsgid() << ", iRet: " << iRet << endl;
            }

            int64_t ms2 = TNOWMS;
            if ((ms2 - ms1) > COST_MS)
            {
                ROLLLOG_WARN << "@Performance, msgid:" << head.nmsgid() << ", costTime:" << (ms2 - ms1) << endl;
            }
        }
    }
    catch (const std::exception &e)
    {
        ROLLLOG_ERROR << e.what() << endl;
        iRet = -1;
    }

    return iRet;
}

// api/wallet
int OrderServantImp::walletOrderCallback(const string& reqData, const int order_type)
{
    __TRY__
    int iRet = 0;
    Json::Value reqJson;
    Json::Reader Reader;
    Reader.parse(reqData, reqJson);
    ROLLLOG_DEBUG << " reqJson: "<< reqJson << endl;

    //数组的读取
    for(unsigned int i = 0;i < reqJson.size();i++)
    {
        ROLLLOG_DEBUG << " reqJson[i] : "<< reqJson[i] << endl;

        if( reqJson[i]["txId"].isNull() || reqJson[i]["markStatus"].isNull() || reqJson[i]["toAddress"].isNull() ||
            reqJson[i]["amount"].isNull() || reqJson[i]["txId"].asString().empty())
        {
           continue;
        }

        string txId = reqJson[i]["txId"].asString();

        WalletOrderInfo orderInfo;
        iRet = ProcessorSingleton::getInstance()->selectWalletOrder(txId, orderInfo);
        if(iRet != 0)
        {
            LOG_ERROR << "select order err. txId:"<< txId << endl;
            return -1;
        }

        if(!orderInfo.outOrderID.empty())//后台转账订单
        {
            LOG_DEBUG << "update order info. txId:"<< txId << endl;
            Json::Value dataJson;
            dataJson["amount"] = reqJson[i]["amount"];
            dataJson["status"] = Json::UInt(3);
            dataJson["payTime"] = g_app.getOuterFactoryPtr()->GetTimeFormat();
            iRet = ProcessorSingleton::getInstance()->updateWalletOrder(txId, dataJson);
            continue;
        }

        long uid = DBOperatorSingleton::getInstance()->getUidByWalletAddress(reqJson[i]["toAddress"].asString());
        if(uid <= 0)
        {
            LOG_ERROR << "select uid err. toAddress:"<< reqJson[i]["toAddress"].asString() << endl;
            return -2;
        }

        userinfo::GetUserAccountReq userAccountReq;
        userinfo::GetUserAccountResp userAccountResp;
        userAccountReq.uid = uid;
        iRet = g_app.getOuterFactoryPtr()->getHallServantPrx(userAccountReq.uid)->getUserAccount(userAccountReq, userAccountResp);
        if (iRet != 0)
        {
            ROLLLOG_ERROR << "getUserAccount failed, uid: " << userAccountReq.uid << endl;
            continue;
        }

        srand(TNOWMS);
        string inOrderId = L2S(TNOWMS) + L2S(uid) + I2S(rand() % 10 + 1);//20位 唯一订单号(时间戳 + UID + 随机数)

        WalletOrderInfo order_info;
        order_info.uid = uid;
        order_info.inOrderID = inOrderId;
        order_info.outOrderID = txId;
        order_info.tradeType = 1;
        order_info.channelType = 1;
        order_info.orderType = 0;
        order_info.applyAmount = std::to_string(reqJson[i]["amount"].asDouble());
        order_info.amount = std::to_string(reqJson[i]["amount"].asDouble());
        order_info.addrType = 1;
        order_info.sandbox = userAccountResp.useraccount.isinwhitelist;
        order_info.status = 3;
        order_info.productID = "";
        order_info.clubId = 0;
        order_info.fromAddr = reqJson[i]["fromAddress"].asString();
        order_info.toAddr = reqJson[i]["toAddress"].asString();
        order_info.createTime = g_app.getOuterFactoryPtr()->GetTimeFormat();

        iRet = ProcessorSingleton::getInstance()->createWalletOrder(order_info.outOrderID, order_info);
        if(iRet != 0)
        {
            ROLLLOG_ERROR<<" create recharge order err."<< endl;
            return iRet;
        }

        userinfo::ModifyUserWealthReq  modifyUserWealthReq;
        userinfo::ModifyUserWealthResp  modifyUserWealthResp;
        modifyUserWealthReq.uid = uid;
        modifyUserWealthReq.goldChange = S2I(order_info.amount) * 100;
        modifyUserWealthReq.changeType = XGameProto::GOLDFLOW::GOLDFLOW_ID_WALLET_RECHARGE;
        modifyUserWealthReq.relateId = txId;
        iRet = g_app.getOuterFactoryPtr()->getHallServantPrx(uid)->modifyUserWealth(modifyUserWealthReq, modifyUserWealthResp);
        if (iRet != 0)
        {
            LOG_ERROR << "cost gold err. iRet: " << iRet << endl;
            return iRet;
        }

    }
    __CATCH__
    return 0;
}

int OrderServantImp::bindTgCallback(const string& reqData, long &uid)
{
    int iRet = 0;

    __TRY__

    Json::Value dataJson;
    Json::Reader Reader;
    Reader.parse(reqData, dataJson);
    ROLLLOG_DEBUG << " dataJson: "<< dataJson << endl;

    uid = ProcessorSingleton::getInstance()->selectBindTgUidByToken(dataJson["token"].asString());
    if(uid <= 0)
    {
       ROLLLOG_ERROR << "token expired." << endl;
       return -1;
    }

    if(dataJson["flag"].asInt() != 1 && dataJson["flag"].asInt() != 2)
    {
        ROLLLOG_ERROR << "param err. flag:"<< dataJson["flag"].asInt() << endl;
        return -2;
    }

    string tgId = dataJson["tgid"].asString();
    if(tgId.empty())
    {
        ROLLLOG_ERROR << "param err tgId is empty." << endl;
        return -3;
    }
    if(dataJson["flag"].asInt() == 1 && ProcessorSingleton::getInstance()->checkBindTG(tgId))
    {
        ROLLLOG_ERROR << "param err tgId already bind tgId: " << tgId << endl;
        return -4;
    }

    //同步余额
    userinfo::GetUserBasicReq basicReq;
    basicReq.uid = uid;
    userinfo::GetUserBasicResp basicResp;
    iRet = g_app.getOuterFactoryPtr()->getHallServantPrx(uid)->getUserBasic(basicReq, basicResp);
    if (iRet != 0)
    {
        ROLLLOG_ERROR << " getUserBasic err. iRet:  " << iRet  << endl;
        return iRet;
    }

    userinfo::UpdateUserAccountReq updateUserAccountReq;
    updateUserAccountReq.uid = uid;
    updateUserAccountReq.updateInfo = {
        {"bindTgId", dataJson["flag"].asInt() == 1 ? dataJson["tgid"].asString() : ""},
        {"tgName", dataJson["flag"].asInt() == 1 ? dataJson["fullname"].asString() : ""},
        {"merchantId", dataJson["flag"].asInt() == 1 ? dataJson["merchantId"].asString() : ""}
    };
    userinfo::UpdateUserAccountResp updateUserAccountResp;
    iRet = g_app.getOuterFactoryPtr()->getHallServantPrx(uid)->updateUserAccount(updateUserAccountReq, updateUserAccountResp);
    if (iRet != 0)
    {
        ROLLLOG_ERROR << "updateUserAccount failed, uid: " <<uid << endl;
        iRet = XGameRetCode::LOGIN_SERVER_ERROR;
    }

    __CATCH__
    return iRet;
}

int OrderServantImp::getPropsByProductID(const string& product_id, map<string, string>& mapProps)
{
    int iRet = -1;
    auto mall_config = g_app.getOuterFactoryPtr()->getMalllConfig();
    for(auto cfg : mall_config)
    {
        if(cfg["product_id"] !=  product_id)
        {
            continue;
        }
        mapProps = cfg;
        iRet = 0;
    }
    return iRet;
}

int OrderServantImp::getWalletAddressAndKey(const long uid, Json::Value &dataJson)
{
    __TRY__

    int iRet = DBOperatorSingleton::getInstance()->selectWalletAddress(uid, dataJson);
    if(iRet != 0)
    {
        ROLLLOG_ERROR << "getWalletAddressAndKey failed, uid: " <<uid << endl;
        return -1;
    }

    if(dataJson["fromAddress"].isNull() || dataJson["fromAddress"].asString().empty() ||
       dataJson["privateKey"].isNull() || dataJson["privateKey"].asString().empty())
    {
        Json::Value postJson;
        postJson["memberId"] = Json::UInt(uid);

        Json::Value resJson;
        int iRet = CRSAEnCryptSingleton::getInstance()->sendPostMsg("/trc/createAddress", postJson, resJson);
        if(iRet == 0)//创建本地订单
        {
            if(resJson["code"].asInt() == 200)
            {
               /* iRet = DBOperatorSingleton::getInstance()->updateWalletAddress(uid, resJson["data"]["address"].asString(), resJson["data"]["secretKey"].asString());
                if(iRet != 0)
                {
                    ROLLLOG_ERROR << "update user address failed, uid: " << uid << ", resJson: "<< resJson << endl;
                    return -2;
                }
                dataJson["fromAddress"] = resJson["data"]["address"];
                dataJson["privateKey"] = resJson["data"]["secretKey"];*/

                iRet = DBOperatorSingleton::getInstance()->selectWalletAddress(uid, dataJson);
                if(iRet != 0)
                {
                    ROLLLOG_ERROR << "getWalletAddressAndKey failed, uid: " <<uid << endl;
                    return -2;
                }
            }
            else
            {
                return -3;
            }
        }
    }

    __CATCH__
    return 0;
}

int OrderServantImp::createWalletRechargeOrder(const long uid, const orderProto::CreateWalletRechargeOrderReq &req, orderProto::CreateWalletRechargeOrderResp &resp)
{
    int iRet = 0;
    __TRY__

    if(req.itype() != 1 || req.product_id().empty())
    {
        ROLLLOG_ERROR << "create rcaharge order param err."<< endl;
        return -1;
    }

    userinfo::GetUserAccountReq userAccountReq;
    userinfo::GetUserAccountResp userAccountResp;
    userAccountReq.uid = uid;
    iRet = g_app.getOuterFactoryPtr()->getHallServantPrx(userAccountReq.uid)->getUserAccount(userAccountReq, userAccountResp);
    if (iRet != 0)
    {
        ROLLLOG_ERROR << "getUserAccount failed, uid: " << userAccountReq.uid << endl;
        return -1;
    }

    //检测账号冻结
    if(userAccountResp.useraccount.isForbidden == 1)
    {
        return XGameRetCode::LOGIN_USER_FORBIDDEN;
    }

    //沙盒测试中只允许白名单玩家充值
    if(g_app.getOuterFactoryPtr()->isSandBox() && userAccountResp.useraccount.isinwhitelist != 1)
    {
        return XGameRetCode::LOGIN_USER_SERVER_UPDATE;
    }

    //获取商品价格
    std::map<string, string> mapProps;
    iRet = getPropsByProductID(req.product_id(), mapProps);
    if(iRet != 0)
    {
        ROLLLOG_ERROR << "select props info err.  product_id:"<< req.product_id() << endl;
        return -2;
    }
    int amount = S2I(mapProps["price"]);

    Json::Value postJson;
    postJson["amount"] = amount / 100.00;
    postJson["toAddress"] = g_app.getOuterFactoryPtr()->getWalletAddress();

    //获取充玩家钱包信息
    if(getWalletAddressAndKey(uid, postJson) != 0)
    {
        ROLLLOG_ERROR << "select wallet address and key err. uid:"<< uid << endl;
        return -3;
    }
    ROLLLOG_DEBUG << " data: "<< postJson << endl;

    string orderId = L2S(TNOWMS) + L2S(uid) + I2S(rand() % 10 + 1);//20位 唯一订单号(时间戳 + UID + 随机数)

   /* Json::Value resJson;
    //调用钱包
    iRet = CRSAEnCryptSingleton::getInstance()->sendPostMsg("/trc/transferContract", postJson, resJson);
    if(iRet == 0)//创建本地订单
    {
        if(resJson["code"].asInt() == 200)
        {
            orderId = resJson["data"]["txID"].asString();
            srand(TNOWMS);
            string inOrderId = L2S(TNOWMS) + L2S(uid) + I2S(rand() % 10 + 1);//20位 唯一订单号(时间戳 + UID + 随机数)

            WalletOrderInfo order_info;
            order_info.uid = uid;
            order_info.inOrderID = inOrderId;
            order_info.outOrderID = resJson["data"]["txID"].asString();
            order_info.tradeType = 1;
            order_info.channelType = 1;
            order_info.applyAmount = amount;
            order_info.amount = 0;
            order_info.addrType = 1;
            order_info.sandbox = userAccountResp.useraccount.isinwhitelist;
            order_info.productID = req.product_id();
            order_info.clubId = req.club_id();
            order_info.fromAddr = postJson["fromAddress"].asString();
            order_info.toAddr =  postJson["toAddress"].asString();
            order_info.createTime = g_app.getOuterFactoryPtr()->GetTimeFormat();
            iRet = ProcessorSingleton::getInstance()->createWalletOrder(order_info.outOrderID, order_info);
            if(iRet != 0)
            {
                ROLLLOG_ERROR<<" create recharge order err."<< endl;
                iRet = -2;
            }
        }
        else
        {
            return -4;
        }
    }*/

    resp.set_order_id(orderId);
    resp.mutable_addr_info()->set_hash_key(postJson["secretKey"].asString());
    resp.mutable_addr_info()->set_address(postJson["fromAddress"].asString());
    resp.set_recharge_amount(amount);
    resp.set_props_count(0);

    //for test
    CRSAEnCryptSingleton::getInstance()->callback(orderId, postJson, this);

    __CATCH__
    return iRet;
}

int OrderServantImp::onCreateWalletRechargeOrder(const XGameComm::TPackage &pkg, const std::string &sCurServrantAddr, const orderProto::CreateWalletRechargeOrderReq &req, tars::TarsCurrentPtr current)
{
    int iRet = 0;
    __TRY__
    orderProto::CreateWalletRechargeOrderResp resp;
    iRet = createWalletRechargeOrder(pkg.stuid().luid(), req, resp);
    resp.set_resultcode(iRet);

    ROLLLOG_DEBUG << "uid: "<< pkg.stuid().luid() << ", resp: "<< logPb(resp)<< endl;
    toClientPb(pkg, sCurServrantAddr, XGameProto::ActionName::WALLET_CREATE_RECHARGE_ORDER, XGameComm::MSGTYPE_RESPONSE, resp);
    __CATCH__
    return iRet;
}

int OrderServantImp::queryWalletOrderStatus(const long uid, const string order_id, orderProto::QueryWalletOrderStatusResp& resp)
{
    __TRY__

    __CATCH__
    return 0;
}

int OrderServantImp::onQueryWalletOrderStatus(const XGameComm::TPackage &pkg, const std::string &sCurServrantAddr, const orderProto::QueryWalletOrderStatusReq &req, tars::TarsCurrentPtr current)
{
    int iRet = 0;
    __TRY__
    ROLLLOG_DEBUG << "uid: "<< pkg.stuid().luid() << ", req: "<< logPb(req)<< endl;
    orderProto::QueryWalletOrderStatusResp resp;
    iRet = queryWalletOrderStatus(pkg.stuid().luid(), req.order_id(), resp);
    resp.set_resultcode(iRet);

    ROLLLOG_DEBUG << "uid: "<< pkg.stuid().luid() << ", resp: "<< logPb(resp)<< endl;
    toClientPb(pkg, sCurServrantAddr, XGameProto::ActionName::WALLET_ORDER_STATUS_QUERY, XGameComm::MSGTYPE_RESPONSE, resp);
    __CATCH__
    return iRet;
}

int OrderServantImp::onQueryWalletWithdrawCfg(const XGameComm::TPackage &pkg, const std::string &sCurServrantAddr, tars::TarsCurrentPtr current)
{
    __TRY__
    orderProto::QueryWalletWithdrawCfgResp resp;

    ROLLLOG_DEBUG << "uid: "<< pkg.stuid().luid() << ", resp: "<< logPb(resp)<< endl;
    toClientPb(pkg, sCurServrantAddr, XGameProto::ActionName::WALLET_WITHDRAW_CFG_QUERY, XGameComm::MSGTYPE_RESPONSE, resp);
    __CATCH__
    return 0;
}

bool OrderServantImp::checkAddress(const string src)
{
    std::vector<string> invaild_symbol_list = {"_", ",", ":", ".", "\"", "\'", " "};
    for(auto s : invaild_symbol_list)
    {
        if(src.find(s) != string::npos)
        {
            ROLLLOG_ERROR << "address exist invaild symbol. s: "<< s << endl;
            return false;
        }
    }
    return true;
}

//ATH5NA6I22CZ
int OrderServantImp::createWalletWithdrawOrder(const long uid, const long withdraw_amount, const string &address)
{
    int iRet = 0;
    return iRet;
}

int OrderServantImp::onCreateWalletWithdrawOrder(const XGameComm::TPackage &pkg, const std::string &sCurServrantAddr, const orderProto::CreateWalletWithdrawOrderReq &req, tars::TarsCurrentPtr current)
{
    int iRet = 0;
    __TRY__
    orderProto::CreateWalletWithdrawOrderResp resp;
    iRet = createWalletWithdrawOrder(pkg.stuid().luid(), req.withdraw_amount(), req.address());
    resp.set_resultcode(iRet);

    ROLLLOG_DEBUG << "uid: "<< pkg.stuid().luid()<< ", req: "<< logPb(req) << ", resp: "<< logPb(resp)<< endl;
    toClientPb(pkg, sCurServrantAddr, XGameProto::ActionName::WALLET_CREATE_WITHDRAW_ORDER, XGameComm::MSGTYPE_RESPONSE, resp);
    __CATCH__
    return iRet;
}

int OrderServantImp::onQueryWalletOrderList(const XGameComm::TPackage &pkg, const std::string &sCurServrantAddr, const orderProto::WalletOrderListReq &req, tars::TarsCurrentPtr current)
{
    __TRY__

    ROLLLOG_DEBUG << "uid: "<< pkg.stuid().luid() << ", req: "<< logPb(req)<< endl;
    orderProto::WalletOrderListResp resp;
    ProcessorSingleton::getInstance()->selectWalletOrderListByUid(pkg.stuid().luid(), req.ipage(), req.idays(), req.ipropsid(), resp);
    resp.set_curr_page(req.ipage());
    resp.set_ipropsid(req.ipropsid());

    ROLLLOG_DEBUG << "uid: "<< pkg.stuid().luid() << ", resp: "<< logPb(resp)<< endl;
    toClientPb(pkg, sCurServrantAddr, XGameProto::ActionName::WALLET_ORDER_LIST_QUERY, XGameComm::MSGTYPE_RESPONSE, resp);
    __CATCH__
    return 0;
}

int OrderServantImp::onQueryFlowRecordList(const XGameComm::TPackage &pkg, const std::string &sCurServrantAddr, const orderProto::FlowRecordReq &req, tars::TarsCurrentPtr current )
{
    __TRY__
    orderProto::FlowRecordResp resp;
    DBOperatorSingleton::getInstance()->selectFlowRecordListByUid(pkg.stuid().luid(), req, resp);
    resp.set_curr_page(req.ipage());

    ROLLLOG_DEBUG << "uid: "<< pkg.stuid().luid() << ", resp: "<< logPb(resp)<< endl;
    toClientPb(pkg, sCurServrantAddr, XGameProto::ActionName::FLOW_HISTORY_RECORD_LIST, XGameComm::MSGTYPE_RESPONSE, resp);
    __CATCH__
    return 0;
}

int OrderServantImp::onQueryCallMeInfo(const XGameComm::TPackage &pkg, const std::string &sCurServrantAddr, tars::TarsCurrentPtr current )
{
    __TRY__
    orderProto::CallMeInfo resp;
    for(auto item : g_app.getOuterFactoryPtr()->getCallMe())
    {
        auto ptr = resp.add_call_me_info();
        ptr->set_name(item.first);
        ptr->set_phone(item.second);
    }
    ROLLLOG_DEBUG << "uid: "<< pkg.stuid().luid() << ", resp: "<< logPb(resp)<< endl;
    toClientPb(pkg, sCurServrantAddr, XGameProto::ActionName::CALL_ME_INFO, XGameComm::MSGTYPE_RESPONSE, resp);
    __CATCH__
    return 0;
}

int OrderServantImp::onBindTG(const XGameComm::TPackage &pkg, const std::string &sCurServrantAddr, const orderProto::BindTGReq &req, tars::TarsCurrentPtr current )
{
    __TRY__

    userinfo::GetUserAccountReq userAccountReq;
    userinfo::GetUserAccountResp userAccountResp;
    userAccountReq.uid = pkg.stuid().luid();
    int iRet = g_app.getOuterFactoryPtr()->getHallServantPrx(pkg.stuid().luid())->getUserAccount(userAccountReq, userAccountResp);
    if (iRet != 0)
    {
        ROLLLOG_ERROR << "getUserAccount failed, uid: " << userAccountReq.uid << endl;
        return -1;
    }

    orderProto::BindTGResp resp;
    if(req.itype() == 0)
    {
        if(userAccountResp.useraccount.bindTgId.empty())
        {
            iRet = ORDER_NOT_BIND_TG;
            ROLLLOG_ERROR << "not bind tg error. uid " << pkg.stuid().luid() << endl;
        }
    }
    else//绑定
    {
        if(!userAccountResp.useraccount.bindTgId.empty() )
        {
            iRet = ORDER_NOT_BIND_TG;
            ROLLLOG_ERROR << "exist bind tg error. uid " << pkg.stuid().luid() << endl;
        }
    }

    string strToken = generateUUIDStr();
    if(iRet == 0)
    {
        iRet = ProcessorSingleton::getInstance()->insertBindTgInfo(pkg.stuid().luid(), strToken);
        if(iRet != 0)
        {
            ROLLLOG_ERROR << "insert into token failed, uid: " << pkg.stuid().luid() << endl;
        }
        string url = "https://t.me/" + g_app.getOuterFactoryPtr()->getRobotId() + "?start=" + I2S(pkg.stuid().luid()) + "_" + strToken + "_" + (req.itype() == 0 ? "2" : "1");
        resp.set_url(url);
    }

    resp.set_resultcode(iRet);
    ROLLLOG_DEBUG << "uid: "<< pkg.stuid().luid() << ", resp: "<< logPb(resp)<< endl;
    toClientPb(pkg, sCurServrantAddr, XGameProto::ActionName::BIND_TG, XGameComm::MSGTYPE_RESPONSE, resp);

    //for test
    //CRSAEnCryptSingleton::getInstance()->bindcallback(req.itype() == 0 ? 2 : 1, strToken, this);

    __CATCH__
    return 0;
}

int OrderServantImp::updateTgBalance(const long uid, const int type, const long amount, long &balance)
{
    userinfo::GetUserAccountReq userAccountReq;
    userinfo::GetUserAccountResp userAccountResp;
    userAccountReq.uid = uid;
    int iRet = g_app.getOuterFactoryPtr()->getHallServantPrx(uid)->getUserAccount(userAccountReq, userAccountResp);
    if (iRet != 0)
    {
        ROLLLOG_ERROR << "getUserAccount failed, uid: " << userAccountReq.uid << endl;
        return -1;
    }

/*    if(userAccountResp.useraccount.bindTgId.empty())
    {
        return ORDER_NOT_BIND_TG;
    }*/

    Json::Value subJson;
    subJson["type"] = Json::UInt(type);
    subJson["amount"] = std::abs(amount) / g_app.getOuterFactoryPtr()->getExchangeRate() / 100.00;
    subJson["gameId"] = Json::UInt(uid);
    subJson["merchantId"] = userAccountResp.useraccount.merchantId;
    subJson["tgid"] = userAccountResp.useraccount.bindTgId;
    ROLLLOG_DEBUG << " subJson: "<< subJson << endl;

    Json::Value postJson;
    postJson["sign"] = tars::TC_MD5::md5str(L2S(TNOW) + g_app.getOuterFactoryPtr()->getMd5Key());;
    postJson["timestamp"] = L2S(TNOW);
    postJson["data"] = CRSAEnCryptSingleton::getInstance()->rsaPublicKeyEncryptSplit(CRSAEnCryptSingleton::getInstance()->jsonToString(subJson));

    ROLLLOG_DEBUG << " postJson: "<< postJson << endl;

    Json::Value resJson;
    //调用钱包
    iRet = CRSAEnCryptSingleton::getInstance()->sendPostMsg("/club/updateBalance", postJson, resJson);

    ROLLLOG_DEBUG<< "resJson: "<< resJson << ", iRet: "<< iRet << endl;
    if(iRet == 0)//创建本地订单
    {
        if(resJson["code"].asInt() == 200)
        {
            balance = long(resJson["data"]["balance"].asDouble() * 100);
        }
        else
        {
            return resJson["code"].asInt();
        }
    }
    return 0;
}

int OrderServantImp::getTgBalance(const long uid, long &balance)
{
    userinfo::GetUserAccountReq userAccountReq;
    userinfo::GetUserAccountResp userAccountResp;
    userAccountReq.uid = uid;
    int iRet = g_app.getOuterFactoryPtr()->getHallServantPrx(uid)->getUserAccount(userAccountReq, userAccountResp);
    if (iRet != 0)
    {
        ROLLLOG_ERROR << "getUserAccount failed, uid: " << userAccountReq.uid << endl;
        return -1;
    }

    if(userAccountResp.useraccount.bindTgId.empty())
    {
        return ORDER_NOT_BIND_TG;
    }

    Json::Value postJson;
    postJson["sign"] =tars::TC_MD5::md5str(L2S(TNOW) + g_app.getOuterFactoryPtr()->getMd5Key());
    postJson["timestamp"] = L2S(TNOW);
    postJson["gameId"] = Json::UInt(uid);
    postJson["merchantId"] = userAccountResp.useraccount.merchantId ;
    postJson["tgid"] = userAccountResp.useraccount.bindTgId;
    ROLLLOG_DEBUG << " data: "<< postJson << endl;

    Json::Value resJson;
    //调用钱包
    iRet = CRSAEnCryptSingleton::getInstance()->sendPostMsg("/club/balance", postJson, resJson);

    ROLLLOG_DEBUG<< "resJson: "<< resJson << ", iRet: "<< iRet << endl;
    if(iRet == 0)//创建本地订单
    {
        if(resJson["code"].asInt() == 200)
        {
            balance = long(resJson["data"]["balance"].asDouble() * g_app.getOuterFactoryPtr()->getExchangeRate() * 100);
        }
        else
        {
            return resJson["code"].asInt();
        }
    }
    return 0;
}

int OrderServantImp::onGetTgBalance(const XGameComm::TPackage &pkg, const std::string &sCurServrantAddr, tars::TarsCurrentPtr current )
{
    __TRY__
    long balance = 0;
    int iRet = getTgBalance(pkg.stuid().luid(), balance);

    orderProto::TGBalanceResp resp;
    resp.set_resultcode(iRet);
    resp.set_balance(balance);
    ROLLLOG_DEBUG << "uid: "<< pkg.stuid().luid() << ", resp: "<< logPb(resp)<< endl;
    toClientPb(pkg, sCurServrantAddr, XGameProto::ActionName::TG_BALANCE, XGameComm::MSGTYPE_RESPONSE, resp);

    __CATCH__
    return 0;
}

int OrderServantImp::onCoinExchange(const XGameComm::TPackage &pkg, const orderProto::MallExchangeReq &req, const std::string &sCurServrantAddr, tars::TarsCurrentPtr current)
{
    FUNC_ENTRY("");
    int iRet = 0;

    return 0;

    LOG_DEBUG << "mall exchange req: "<< logPb(req)<< endl;

    __TRY__
    orderProto::MallExchangeResp resp;

    userinfo::GetUserAccountReq userAccountReq;
    userinfo::GetUserAccountResp userAccountResp;
    userAccountReq.uid = pkg.stuid().luid();
    iRet = g_app.getOuterFactoryPtr()->getHallServantPrx(pkg.stuid().luid())->getUserAccount(userAccountReq, userAccountResp);
    if (iRet != 0)
    {
        ROLLLOG_ERROR << "getUserAccount failed, uid: " << userAccountReq.uid << endl;
        return -1;
    }
    if(userAccountResp.useraccount.isForbidden == 1)
    {
        iRet = XGameRetCode::LOGIN_USER_FORBIDDEN;
    }
    else
    {
        mall::DispatchGoodsReq treq;
        treq.uid = pkg.stuid().luid();
        treq.product_id = req.product_id();
        treq.discount = 1.00;
        treq.orderID = "";
        mall::DispatchGoodsResp tresp;
        iRet = g_app.getOuterFactoryPtr()->getHallServantPrx(treq.uid)->dispatchGoods(treq, tresp);
        if (iRet != 0)
        {
            LOG_ERROR << "dispatch goods err. iRet: " << iRet << endl;
        }
        else
        {
            if(!userAccountResp.useraccount.bindTgId.empty())
            {
                long balance = 0;
                iRet = updateTgBalance(pkg.stuid().luid(), 711, tresp.count, balance);
                if(iRet != 0)
                {
                    ROLLLOG_ERROR << "update balance err. iRet: "<< iRet << endl;
                }
            }
        }
    }
    resp.set_resultcode(iRet);
    LOG_DEBUG << "mall exchange resp: "<< logPb(resp)<< endl;
    toClientPb(pkg, sCurServrantAddr, XGameProto::ActionName::MALL_EXCHANGE, XGameComm::MSGTYPE_RESPONSE, resp);

    __CATCH__

    FUNC_EXIT("", iRet);

    return iRet;
}

int OrderServantImp::onGetAgencyBill(const XGameComm::TPackage &pkg, const orderProto::AgencyBillReq &req, const std::string &sCurServrantAddr, tars::TarsCurrentPtr current )
{
    __TRY__

    ROLLLOG_DEBUG << "uid: "<< pkg.stuid().luid() << ", req: "<< logPb(req)<< endl;

    orderProto::AgencyBillResp resp;
    int iRet = DBOperatorSingleton::getInstance()->selectAgencyBill(pkg.stuid().luid(), req, resp);
    if(iRet != 0)
    {
        ROLLLOG_ERROR << "select agency bill err. iRet: "<< iRet << endl;
    }

    resp.set_resultcode(iRet);
    ROLLLOG_DEBUG << "uid: "<< pkg.stuid().luid() << ", resp: "<< logPb(resp)<< endl;
    toClientPb(pkg, sCurServrantAddr, XGameProto::ActionName::AGENCY_BILL, XGameComm::MSGTYPE_RESPONSE, resp);

    __CATCH__
    return 0;
}


int OrderServantImp::goodsTransfer(const XGameComm::TPackage &pkg, const orderProto::GoodsTransferReq &req)
{
    if(req.lplayerid() == pkg.stuid().luid())
    {
        return XGameRetCode::GOODS_TRANSFER_USER_ERR;
    }

    userinfo::GetUserAccountReq userAccountReq;
    userinfo::GetUserAccountResp userAccountResp;
    userAccountReq.uid = pkg.stuid().luid();
    int iRet = g_app.getOuterFactoryPtr()->getHallServantPrx(userAccountReq.uid)->getUserAccount(userAccountReq, userAccountResp);
    if (iRet != 0)
    {
        ROLLLOG_ERROR << "getUserAccount failed, uid: " << userAccountReq.uid << endl;
        return -1;
    }

    if(userAccountResp.useraccount.isForbidden == 1)
    {
        return XGameRetCode::LOGIN_USER_FORBIDDEN;
    }

    if(!userAccountResp.useraccount.bindTgId.empty())
    {
        ROLLLOG_ERROR << "transfer failed, uid: "<< userAccountReq.uid<< ", bindTgId: " << userAccountResp.useraccount.bindTgId << endl;
        return XGameRetCode::GOODS_TRANSFER_BINDTG_ERR;
    }

    userAccountReq.uid = req.lplayerid();
    iRet = g_app.getOuterFactoryPtr()->getHallServantPrx(userAccountReq.uid)->getUserAccount(userAccountReq, userAccountResp);
    if (iRet != 0)
    {
        ROLLLOG_ERROR << "getUserAccount failed, uid: " << userAccountReq.uid << endl;
        return -2;
    }

    if(!userAccountResp.useraccount.bindTgId.empty())
    {
        ROLLLOG_ERROR << "transfer failed, uid: "<<userAccountReq.uid<< ", bindTgId: " << userAccountResp.useraccount.bindTgId << endl;
        return XGameRetCode::GOODS_TRANSFER_BINDTG_ERR;
    }

    userinfo::ModifyUserWealthReq  modifyUserWealthReq;
    userinfo::ModifyUserWealthResp  modifyUserWealthResp;
    modifyUserWealthReq.uid = pkg.stuid().luid();
    if(req.ipropsid() == 10000)
    {
        modifyUserWealthReq.diamondChange = -req.lcount();
    }
    else if(req.ipropsid() == 20000)
    {
        modifyUserWealthReq.goldChange = -req.lcount();
    }
    else if (req.ipropsid() == 70000)
    {
        modifyUserWealthReq.pointChange = -req.lcount();
    }
    modifyUserWealthReq.changeType = XGameProto::GOLDFLOW::GOLDFLOW_ID_GOODS_OUT;
    modifyUserWealthReq.relateId = L2S(req.lplayerid());

    iRet = g_app.getOuterFactoryPtr()->getHallServantPrx(pkg.stuid().luid())->modifyUserWealth(modifyUserWealthReq, modifyUserWealthResp);
    if (iRet != 0)
    {
        LOG_ERROR << "transfer failed, . iRet: " << iRet << endl;
        return iRet;
    }

    modifyUserWealthReq.uid =  req.lplayerid();
    if(req.ipropsid() == 10000)
    {
        modifyUserWealthReq.diamondChange = req.lcount() * (100 - g_app.getOuterFactoryPtr()->getGoodsTransferRate()) / 100;
    }
    else if(req.ipropsid() == 20000)
    {
        modifyUserWealthReq.goldChange = req.lcount() * (100 - g_app.getOuterFactoryPtr()->getGoodsTransferRate()) / 100;
    }
    else if (req.ipropsid() == 70000)
    {
        modifyUserWealthReq.pointChange = req.lcount() * (100 - g_app.getOuterFactoryPtr()->getGoodsTransferRate()) / 100;
    }
    modifyUserWealthReq.changeType = XGameProto::GOLDFLOW::GOLDFLOW_ID_GOODS_IN;
    modifyUserWealthReq.relateId = L2S(pkg.stuid().luid());
    iRet = g_app.getOuterFactoryPtr()->getHallServantPrx(pkg.stuid().luid())->modifyUserWealth(modifyUserWealthReq, modifyUserWealthResp);
    if (iRet != 0)
    {
        LOG_ERROR << "transfer failed, . iRet: " << iRet << endl;
        return iRet;
    }
    return 0;
}

// 可能会抛出异常
int OrderServantImp::IOSVerify(const string &response, const string &cur_transaction_id, const string &cur_product_id)
{
    Document d;
    if (d.Parse(response.c_str()).HasParseError())
    {
        throw logic_error("parse json error. raw data : " + response);
    }

    //
    const Value &propertyStatus = getValue(d, "status");
    int status = parseValue<int>(propertyStatus);
    if (status != 0)
    {
        ROLLLOG_ERROR << "verify IOS failed, reason: status failed, status: " << status << ", transaction_id : " << cur_transaction_id << endl;
        return -1; //状态码出错
    }

    // 获取嵌套的对象的属性
    const Value &property_receipt = getValue(d, "receipt");
    // IOS7+
    if (property_receipt.HasMember("in_app"))
    {
        const Value &property_in_app = getValue(property_receipt, "in_app");
        for (SizeType i = 0; i < property_in_app.Size(); i++)
        {
            const Value &item = property_in_app[i];

            const Value &property_transaction_id = getValue(item, "transaction_id");
            string transaction_id = parseValue<string>(property_transaction_id);

            const Value &property_product_id = getValue(item, "product_id");
            string product_id = parseValue<string>(property_product_id);

            // if (transaction_id == cur_transaction_id)
            if ((transaction_id == cur_transaction_id) && (product_id == cur_product_id))
            {
                ROLLLOG_DEBUG << "verify IOS success, transaction_id: " << cur_transaction_id << endl;

                return 0; // 校验成功
            }
            else
            {
                ROLLLOG_ERROR << "verify err, transaction_id: " << transaction_id << ", cur_transaction_id: " << cur_transaction_id
                              << ", product_id: " << product_id << ", cur_product_id: " << cur_product_id << endl;
            }
        }
    }
    else
    {
        const Value &property_transaction_id = getValue(property_receipt, "transaction_id");
        string transaction_id = parseValue<string>(property_transaction_id);

        const Value &property_product_id = getValue(property_receipt, "product_id");
        string product_id = parseValue<string>(property_product_id);
        // if (transaction_id == cur_transaction_id)
        if ((transaction_id == cur_transaction_id) && (product_id == cur_product_id))
        {
            ROLLLOG_DEBUG << "verify IOS success, transaction_id: " << cur_transaction_id << endl;

            return 0; // 校验成功
        }
        else
        {
            ROLLLOG_ERROR << "verify err, transaction_id: " << transaction_id << ", cur_transaction_id: " << cur_transaction_id
                          << ", product_id: " << product_id << ", cur_product_id: " << cur_product_id << endl;
        }
    }

    return 0;
}

// 需要保存凭据 -- 客户端可能会丢单 TODO
//校验订单
int OrderServantImp::orderVerify(const long lUid, const orderProto::OrderVerifyReq &req, orderProto::OrderVerifyResp &rsp)
{
    //TODO 需要存放凭据,如果校验失败,下次可以继续校验
    FUNC_ENTRY("");

    int iRet = 0;
    std::string url = "https://buy.itunes.apple.com/verifyReceipt";

    int status = ProcessorSingleton::getInstance()->getWalletOrderStatus(req.ordernum());
    if(status != 0)
    {
        LOG_DEBUG << "order already veridfy. inOrderID:"<< req.ordernum() << endl;
        return 0;
    }

    string product_id = ProcessorSingleton::getInstance()->getWalletOrderPropsID(req.ordernum());
    if(product_id.empty())
    {
        ROLLLOG_ERROR << "select product_id err. " << endl;
        return -1;
    }
    std::string postData = req.credential();
    string response = Request::post(url, postData);
    iRet = IOSVerify(response, req.transaction_id(), product_id);
    if(iRet != 0 )
    {
        LOG_ERROR << "ios order verify fail. iRet :"<< iRet << endl;
        return iRet;
    }


    Json::Value dataJson;
    dataJson["outOrderID"] = req.transaction_id();
    dataJson["status"] = 3;
    dataJson["payTime"] = g_app.getOuterFactoryPtr()->GetTimeFormat();;
    iRet = ProcessorSingleton::getInstance()->updateWalletOrder(req.ordernum(), dataJson);
    if(iRet != 0)
    {
        ROLLLOG_ERROR<<" update order err. inOrderID:"<< req.ordernum() << endl;
        return iRet;
    }

    mall::DispatchGoodsReq dispatchGoodsReq;
    dispatchGoodsReq.uid = lUid;
    dispatchGoodsReq.product_id = product_id;
    dispatchGoodsReq.discount = 100;
    dispatchGoodsReq.orderID = req.ordernum();
    mall::DispatchGoodsResp dispatchGoodsResp;
    iRet = g_app.getOuterFactoryPtr()->getHallServantPrx(lUid)->dispatchGoods(dispatchGoodsReq, dispatchGoodsResp);
    if (iRet != 0)
    {
        LOG_ERROR << "dispatch goods err. iRet: " << iRet << endl;
        return iRet;
    }

    FUNC_EXIT("", iRet);
    return iRet;
}

int OrderServantImp::onGoodsTransfer(const XGameComm::TPackage &pkg, const orderProto::GoodsTransferReq &req, const std::string &sCurServrantAddr, tars::TarsCurrentPtr current )
{
    __TRY__

    ROLLLOG_DEBUG << "uid: "<< pkg.stuid().luid() << ", req: "<< logPb(req)<< endl;

    int iRet = goodsTransfer(pkg, req);
    if(iRet != 0)
    {
        LOG_ERROR << "transfer failed, . iRet: " << iRet << endl;
    }

    orderProto::GoodsTransferResp resp;
    resp.set_resultcode(iRet);
    ROLLLOG_DEBUG << "uid: "<< pkg.stuid().luid() << ", resp: "<< logPb(resp)<< endl;
    toClientPb(pkg, sCurServrantAddr, XGameProto::ActionName::GOODS_TRANSFER, XGameComm::MSGTYPE_RESPONSE, resp);

    __CATCH__
    return 0;
}

// TODO 在这里要打印用户的请求参数并且检查参数是否合法
//校验订单
int OrderServantImp::onOrderVerify(const XGameComm::TPackage &pkg, const std::string &sCurServrantAddr, const orderProto::OrderVerifyReq &req, tars::TarsCurrentPtr current)
{
    FUNC_ENTRY("");

    int iRet = 0;
    orderProto::OrderVerifyResp resp;
    iRet = orderVerify(pkg.stuid().luid(), req, resp);
    if (iRet != 0)
    {
        ROLLLOG_DEBUG << "order verify err, req: " << logPb(req) << ", resp: " << logPb(resp) << endl;
    }

    resp.set_resultcode(iRet);
    resp.set_identity("");

    toClientPb(pkg, sCurServrantAddr, XGameProto::ActionName::ORDER_VERRITY, XGameComm::MSGTYPE_RESPONSE, resp);

    FUNC_EXIT("", iRet);

    return iRet;
}

//ios产生订单
int OrderServantImp::onOrderYield(const XGameComm::TPackage &pkg, const std::string &sCurServrantAddr, const orderProto::OrderYieldReq &req, tars::TarsCurrentPtr current)
{
    FUNC_ENTRY("");
    int iRet = 0;
    ROLLLOG_DEBUG << "uid : " << pkg.stuid().luid() << ", req: " << logPb(req) << endl;

    orderProto::OrderYieldResp resp;

    std::map<string, string> mapProps;
    iRet = getPropsByProductID(req.product_id(), mapProps);
    if(iRet != 0)
    {
        ROLLLOG_ERROR << "select props info err.  product_id:"<< req.product_id() << endl;
        return -1;
    }

    int amount = S2I(mapProps["price"]);

    string merchantOrderId = L2S(TNOWMS) + L2S(pkg.stuid().luid()) + I2S(rand() % 10 + 1);//20位 唯一订单号(时间戳 + UID + 随机数)

    WalletOrderInfo order_info;
    order_info.uid = pkg.stuid().luid();
    order_info.outOrderID = "";
    order_info.tradeType = 1;
    order_info.channelType = 3;
    order_info.status = 0;
    order_info.applyAmount = amount;
    order_info.amount = amount;
    order_info.clubId = req.club_id();
    order_info.productID = req.product_id();
    order_info.payTime = "";
    order_info.createTime = g_app.getOuterFactoryPtr()->GetTimeFormat();
    iRet = ProcessorSingleton::getInstance()->createWalletOrder(merchantOrderId, order_info);
    if(iRet != 0)
    {
        ROLLLOG_ERROR<<" create rcaharge order err."<< endl;
        iRet = -2;
    }
    resp.set_resultcode(iRet);
    resp.set_ordernum(merchantOrderId);
    resp.set_product_id(req.product_id());

    toClientPb(pkg, sCurServrantAddr, XGameProto::ActionName::ORDER_YIELD, XGameComm::MSGTYPE_RESPONSE, resp);

    FUNC_EXIT("", iRet);
    return iRet;
}

//发送消息到客户端
template<typename T>
int OrderServantImp::toClientPb(const XGameComm::TPackage &tPackage, const std::string &sCurServrantAddr, XGameProto::ActionName actionName, XGameComm::MSGTYPE type, const T &t)
{

    XGameComm::TPackage rsp;
    auto mh = rsp.add_vecmsghead();
    mh->set_nmsgid(actionName);
    mh->set_nmsgtype(type); //此处根据实际情况更改
    mh->set_servicetype(XGameComm::SERVICE_TYPE::SERVICE_TYPE_ORDER); //此处根据实际情况更改
    rsp.add_vecmsgdata(pbToString(t));

    auto pPushPrx = Application::getCommunicator()->stringToProxy<JFGame::PushPrx>(sCurServrantAddr);
    if (pPushPrx)
    {
        ROLLLOG_DEBUG << "response : " << t.DebugString() << endl;
        pPushPrx->tars_hash(tPackage.stuid().luid())->async_doPushBuf(NULL, tPackage.stuid().luid(), pbToString(rsp));
    }
    else
    {
        ROLLLOG_ERROR << "pPushPrx is null : " << t.DebugString() << endl;
    }

    return 0;
}

