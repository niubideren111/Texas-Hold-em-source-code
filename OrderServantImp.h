#ifndef _OrderServantImp_H_
#define _OrderServantImp_H_

#include "servant/Application.h"
#include "OrderServant.h"
#include "Order.pb.h"
#include "XGameComm.pb.h"
#include "CommonStruct.pb.h"
#include "Push.h"

#include <json/json.h>

/**
 *订单服务接口
 *
 */
class OrderServantImp : public order::OrderServant
{
public:
    /**
     *
     */
    virtual ~OrderServantImp() {}

    /**
     *
     */
    virtual void initialize();

    /**
     *
     */
    virtual void destroy();

public:
    //tcp请求处理接口
    virtual tars::Int32 onRequest(tars::Int64 lUin, const std::string &sMsgPack, const std::string &sCurServrantAddr, const JFGame::TClientParam &stClientParam,  const JFGame::UserBaseInfoExt &stUserBaseInfo, tars::TarsCurrentPtr current);
    //http请求处理接口
    virtual tars::Int32 doRequestJson(const string &reqBuf, const string &urlKey, string &rspBuf, tars::TarsCurrentPtr current);
    //后台扣除
    virtual tars::Int32 takeRecharge(tars::Int64 lUid, const string &sDesc, const string &sOptUser, const vector<tars::Int64> &vecUid, const map<tars::Int64, tars::Int64> &mapRecharge, tars::TarsCurrentPtr current);
    //后台赠送
    virtual tars::Int32 giveRecharge(tars::Int64 lUid, const string &sDesc, const string &sOptUser, const vector<tars::Int64> &vecUid, const map<tars::Int64, tars::Int64> &mapRecharge, tars::TarsCurrentPtr current);
    //人工充值
    virtual tars::Int32 manualRecharge(tars::Int64 lUin, tars::Int64 lCid, const map<tars::Int64, tars::Int64> &mapRecharge, const map<tars::Int64, tars::Int64> &mapRealRecharge, tars::TarsCurrentPtr current);
    //创建订单
    virtual tars::Int32 createOrder(const map<string, string> &mapOrderInfo, tars::TarsCurrentPtr current);
    //更新订单
    virtual tars::Int32 updateOrder(const map<string, string> &mapOrderInfo, tars::TarsCurrentPtr current);
    //绑定成功通知
    virtual tars::Int32 bindWalletNotify(tars::Int64 lUin, tars::TarsCurrentPtr current);
    //更新余额
    virtual tars::Int32 modifyWalletBalance(const order::ModifyWalletBalanceReq &req, order::ModifyWalletBalanceResp &resp, tars::TarsCurrentPtr current);
    //查询余额
    virtual tars::Int64 selectWalletBalance(tars::Int64 lUin, tars::TarsCurrentPtr current);

public:
    int getPropsByProductID(const string& product_id, map<string, string>& mapProps);

    int getWalletAddressAndKey(const long uid, Json::Value &dataJson);

    int createWalletRechargeOrder(const long uid, const orderProto::CreateWalletRechargeOrderReq &req, orderProto::CreateWalletRechargeOrderResp &resp);
    //
    int queryWalletOrderStatus(const long uid, const string order_id, orderProto::QueryWalletOrderStatusResp& resp);
    //
    int walletOrderCallback(const string& reqData, const int order_type);
    //
    int bindTgCallback(const string& reqData, long &lUid);
    //
    bool checkAddress(const string src);
    //
    int createWalletWithdrawOrder(const long uid, const long withdraw_amount, const string &address);
    //
    int updateTgBalance(const long uid, const int type, const long amount, long &balance);
    //
    int getTgBalance(const long uid, long &balance);
    //
    int goodsTransfer(const XGameComm::TPackage &pkg, const orderProto::GoodsTransferReq &req);
    //
    int IOSVerify(const string &response, const string &cur_transaction_id, const string &cur_product_id);
    //
    int orderVerify(const long lUid, const orderProto::OrderVerifyReq &req, orderProto::OrderVerifyResp &rsp);

private:
    //
    int onCreateWalletRechargeOrder(const XGameComm::TPackage &pkg, const std::string &sCurServrantAddr, const orderProto::CreateWalletRechargeOrderReq &req, tars::TarsCurrentPtr current);
    //
    int onQueryWalletOrderStatus(const XGameComm::TPackage &pkg, const std::string &sCurServrantAddr, const orderProto::QueryWalletOrderStatusReq &req, tars::TarsCurrentPtr current);
    //
    int onQueryWalletWithdrawCfg(const XGameComm::TPackage &pkg, const std::string &sCurServrantAddr, tars::TarsCurrentPtr current);
    //
    int onCreateWalletWithdrawOrder(const XGameComm::TPackage &pkg, const std::string &sCurServrantAddr, const orderProto::CreateWalletWithdrawOrderReq &req, tars::TarsCurrentPtr current);
    //
    int onQueryWalletOrderList(const XGameComm::TPackage &pkg, const std::string &sCurServrantAddr, const orderProto::WalletOrderListReq &req, tars::TarsCurrentPtr current);
    //
    int onQueryFlowRecordList(const XGameComm::TPackage &pkg, const std::string &sCurServrantAddr, const orderProto::FlowRecordReq &req, tars::TarsCurrentPtr current );
    //
    int onQueryCallMeInfo(const XGameComm::TPackage &pkg, const std::string &sCurServrantAddr, tars::TarsCurrentPtr current );
    //tg账户
    int onBindTG(const XGameComm::TPackage &pkg, const std::string &sCurServrantAddr, const orderProto::BindTGReq &req, tars::TarsCurrentPtr current );
    //
    int onGetTgBalance(const XGameComm::TPackage &pkg, const std::string &sCurServrantAddr, tars::TarsCurrentPtr current );
    //
    int onCoinExchange(const XGameComm::TPackage &pkg, const orderProto::MallExchangeReq &req, const std::string &sCurServrantAddr, tars::TarsCurrentPtr current);

    int onGetAgencyBill(const XGameComm::TPackage &pkg, const orderProto::AgencyBillReq &req, const std::string &sCurServrantAddr, tars::TarsCurrentPtr current);
    //
    int onGoodsTransfer(const XGameComm::TPackage &pkg, const orderProto::GoodsTransferReq &req, const std::string &sCurServrantAddr, tars::TarsCurrentPtr current );

    //ios校验订单
    int onOrderVerify(const XGameComm::TPackage &pkg, const std::string &sCurServrantAddr, const orderProto::OrderVerifyReq &req, tars::TarsCurrentPtr current);
    //ios产生订单
    int onOrderYield(const XGameComm::TPackage &pkg, const std::string &sCurServrantAddr, const orderProto::OrderYieldReq &req, tars::TarsCurrentPtr current);

private:
    //发送消息到客户端
    template<typename T>
    int toClientPb(const XGameComm::TPackage &tPackage, const std::string &sCurServrantAddr, XGameProto::ActionName actionName, XGameComm::MSGTYPE type, const T &t);

private:
    std::map<int64_t, JFGame::UserBaseInfoExt> _userInfo; //用户基本信息
    std::map<int64_t, string> _userIp;  //用户IP
};

/////////////////////////////////////////////////////
#endif
