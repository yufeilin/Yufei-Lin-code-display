using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class MainMenuController : MonoBehaviour
{
    GameObject NetworkManager = null;

    GameObject ClientPlayerManager = null;

    MessageToBeSent CurrentMessageToBeSent;

    public void MessageReceive(string ReceiveString)
    {
        string[] StringSplit = ReceiveString.Split(' ');

        if (StringSplit[1] == "UserEnterShop")
        {
            if (StringSplit[2] == "Success")
            {
                // 获取并设置商城订单号
                ClientPlayerManager.GetComponent<ClientPlayerController>().SetShopOrderNumber(int.Parse(StringSplit[3]));

                LoadSceneConfig.nextSceneName = "Shop";
                SceneManager.LoadScene("Loading");//加载进度条场景
            }
            else
            {
                // 一般不会进入这个分支
            }
        }

        if (StringSplit[1] == "UserEnterCollection")
        {
            if (StringSplit[2] == "Success")
            {
                LoadSceneConfig.nextSceneName = "MyCollection";
                SceneManager.LoadScene("Loading");//加载进度条场景
            }
        }


    }

    public void OnClickSinglePlayerMode()
    {
        Debug.Log("Click");
        LoadSceneConfig.nextSceneName = "SinglePlayerModeMenu";
        SceneManager.LoadScene("Loading");//加载进度条场景
    }

    public void OnClickMultiPlayerMode()
    {
        LoadSceneConfig.nextSceneName = "MultiPlayerModeMenu";
        SceneManager.LoadScene("Loading");//加载进度条场景
    }

    public void OnClickYard()
    {
        LoadSceneConfig.nextSceneName = "Yard";
        SceneManager.LoadScene("Loading");//加载进度条场景
    }

    public void OnClickCollection()
    {
        // 生成发送信息
        string SendMessage = "UserEnterShop ";

        // 通知网络管理器发送信息

        NetworkManager.GetComponent<NetworkController>().MessageSend(SendMessage);
    }

    public void OnClickShop()
    {
        // 生成发送信息
        string SendMessage = "UserEnterShop ";

        // 通知网络管理器发送信息
        NetworkManager.GetComponent<NetworkController>().MessageSend(SendMessage);
    }

	// Use this for initialization
	void Start ()
    {

        NetworkManager = GameObject.Find("NetworkManager");

        ClientPlayerManager = GameObject.Find("ClientPlayerManager");

        // 向网络管理器注册自己的数据包处理函数
        NetworkManager.GetComponent<NetworkController>().ManagerRegistion("MainMenuController", MessageReceive);

    }
	
	// Update is called once per frame
	void Update () {
		
	}

    void OnDestroy()
    {
        // 向网络管理器注销自己的数据包处理函数
        NetworkManager.GetComponent<NetworkController>().ManagerCancellation("MainMenuController");
    }
}
