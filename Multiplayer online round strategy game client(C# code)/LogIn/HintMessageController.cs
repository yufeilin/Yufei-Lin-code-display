using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class HintMessageController : MonoBehaviour
{
    // 提示框物体预制体引用
    GameObject HintBoxPrefab = null;

    // 提示框物体引用
    public GameObject HintBox = null;

    // 提示框中的信息文本物体引用
    public GameObject MessageText = null;



    // 提示框已经存在时间
    float MessageExistTime = 0.0f;

    // 提示框需要存在的时间
    float MessageLastTime = 0.0f;

    // 提示框显示标旗
    bool IsHintBoxShowing = false;

    // 不带消息持续时间参数的消息显示函数
    public void ShowHintMessage(string MessageString)
    {
        if(IsHintBoxShowing)
        {
            // 如果提示框已经显示则刷新存在时间
            MessageExistTime = 0.0f;

            // 设置提示框需要存在时间为一个很大的值
            MessageLastTime = 100.0f;

            // 更改提示框中的提示文本
            MessageText.GetComponent<Text>().text = MessageString;
        }
        else
        {
            // 如果提示框此时未显示则实例化一个提示框并且设置已存在时间为0

            // 实例化一个提示框并获取该提示框的引用
            HintBox = GameObject.Instantiate(HintBoxPrefab);

            HintBox.GetComponent<RectTransform>().SetParent(GameObject.Find("Canvas").transform);

            HintBox.GetComponent<RectTransform>().localPosition = new Vector3(0, 33, 0);
            // 获取提示框文本的引用
            MessageText = HintBox.transform.Find("MessageText").gameObject;

            // HintBox.SetActive(true);
            MessageExistTime = 0.0f;
            MessageLastTime = 100.0f;
            IsHintBoxShowing = true;

            // 更改提示框中的提示文本
            MessageText.GetComponent<Text>().text = MessageString;
        }
    }

    public void ShowHintMessage(string MessageString, float LastTime)
    {
        if (IsHintBoxShowing)
        {
            // 如果提示框已经显示则刷新存在时间
            MessageExistTime = 0.0f;

            // 设置提示框需要存在时间为一个很大的值
            MessageLastTime = LastTime;

            // 更改提示框中的提示文本
            MessageText.GetComponent<Text>().text = MessageString;
        }
        else
        {
            // 如果提示框此时未显示则实例化一个提示框并且设置已存在时间为0

            // 实例化一个提示框并获取该提示框的引用
            HintBox = GameObject.Instantiate(HintBoxPrefab);

            HintBox.GetComponent<RectTransform>().SetParent(GameObject.Find("Canvas").transform);

            HintBox.GetComponent<RectTransform>().localPosition = new Vector3(0, 33, 0);
            // 获取提示框文本的引用
            MessageText = HintBox.transform.Find("MessageText").gameObject;

            MessageExistTime = 0.0f;
            MessageLastTime = LastTime;
            IsHintBoxShowing = true;

            // 更改提示框中的提示文本
            MessageText.GetComponent<Text>().text = MessageString;
        }
    }

    public void ConcealHintMessage()
    {
        if (IsHintBoxShowing)
        {
            Destroy(HintBox);
            HintBox = null;
            MessageText = null;
            //HintBox.SetActive(false);
            MessageExistTime = 0.0f;
            IsHintBoxShowing = false;
        }
    }

    // Use this for initialization
    void Start ()
    {
        GameObject.DontDestroyOnLoad(gameObject);
        // 获取提示信息框物体的预制体
        HintBoxPrefab = (GameObject)Resources.Load("Prefabs/UIPrefabs/HintBox");

        if(HintBoxPrefab == null)
        {
            Debug.Log("1233334");
        }
    }
	
	// Update is called once per frame
	void Update ()
    {
		if(IsHintBoxShowing)
        {
            MessageExistTime += Time.deltaTime;
            if(MessageExistTime >= MessageLastTime)
            {
                ConcealHintMessage();
                //HintBox.SetActive(false);
                //MessageExistTime = 0.0f;
                //IsHintBoxShowing = false;
            }
        }


	}
}
