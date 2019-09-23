using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ShopController : MonoBehaviour {

    GameObject NetworkManager = null;

    GameObject HintMessageManager = null;

    GameObject ClientPlayerManager = null;

    MessageToBeSent CurrentMessageToBeSent;

    public void MessageReceive(string ReceiveString)
    {
        Debug.Log(ReceiveString);

        string[] StringSplit = ReceiveString.Split(' ');

        int CurrentShopOrderNumber = ClientPlayerManager.GetComponent<ClientPlayerController>().GetShopOrderNumber();

        // 检查信息类型以及商城订单号是否是当前订单
        if (StringSplit[1] == "UserBuy" && StringSplit[4] == CurrentShopOrderNumber.ToString())
        {
            if (StringSplit[5] == "Success")
            {
                // 如果购买成功

                Debug.Log("buy successful");

                // 显示UI刷新钻石数目显示

                // 隐藏 正在连接 提示信息
                HintMessageManager.GetComponent<HintMessageController>().ConcealHintMessage();

                // 将购买信息发送请求设为不活跃以便让网络控制器停止发送该信息并在下一个帧循环中删除该信息发送请求
                CurrentMessageToBeSent.SetEnable(false);

                // 显示 购买成功 提示信息
                HintMessageManager.GetComponent<HintMessageController>().ShowHintMessage("购买成功", 3.0f);
            }
            else
            {
                // 如果购买失败

                // 隐藏 正在连接 提示信息
                HintMessageManager.GetComponent<HintMessageController>().ConcealHintMessage();

                // 将购买信息发送请求设为不活跃以便让网络控制器停止发送该信息并在下一个帧循环中删除该信息发送请求
                CurrentMessageToBeSent.SetEnable(false);

                // 显示 购买成功 提示信息
                HintMessageManager.GetComponent<HintMessageController>().ShowHintMessage("购买失败", 3.0f);
            }

            // 递增商城订单号
            ClientPlayerManager.GetComponent<ClientPlayerController>().SetShopOrderNumber(CurrentShopOrderNumber + 1);
        }
    }


    public void OnClickBuyDiamond6480()
    {
        // 将当前商城订单号包含在数据包中
        int CurrentShopOrderNumber = ClientPlayerManager.GetComponent<ClientPlayerController>().GetShopOrderNumber();

        // 生成发送信息
        string SendMessage = "UserBuy Diamond 6480 "+ CurrentShopOrderNumber.ToString()+" ";

        // 通知网络管理器发送信息
        CurrentMessageToBeSent = NetworkManager.GetComponent<NetworkController>().AddMessageInSendList(SendMessage);

        // 显示 正在连接 提示信息
        HintMessageManager.GetComponent<HintMessageController>().ShowHintMessage("Connecting");
    }

    public void OnClickBuyCardPack120()
    {
        // 将当前商城订单号包含在数据包中
        int CurrentShopOrderNumber = ClientPlayerManager.GetComponent<ClientPlayerController>().GetShopOrderNumber();

        // 生成发送信息
        string SendMessage = "UserBuy CardPack 120 " + CurrentShopOrderNumber.ToString() + " ";

        // 通知网络管理器发送信息
        CurrentMessageToBeSent = NetworkManager.GetComponent<NetworkController>().AddMessageInSendList(SendMessage);

        // 显示 正在连接 提示信息
        HintMessageManager.GetComponent<HintMessageController>().ShowHintMessage("Connecting");
    }


	// Use this for initialization
	void Start ()
    {
        // 找到网络管理器的引用
        NetworkManager = GameObject.Find("NetworkManager");

        // 找到提示信息管理器的引用
        HintMessageManager = GameObject.Find("HintMessageManager");

        ClientPlayerManager = GameObject.Find("ClientPlayerManager");

        // 向网络管理器注册自己的数据包处理函数
        NetworkManager.GetComponent<NetworkController>().ManagerRegistion("ShopController", MessageReceive);

        Debug.Log("Enter Shop successful.");

    }
	
	// Update is called once per frame
	void Update ()
    {
		
	}

    void OnDestroy()
    {
        // 向网络管理器注销自己的数据包处理函数
        NetworkManager.GetComponent<NetworkController>().ManagerCancellation("ShopController");
    }
}
