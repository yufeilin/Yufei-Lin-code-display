using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class PlayerMessageAreaController : MonoBehaviour
{
    GameObject MessageTextPrefab = null;

    GameObject MessageText = null;

    public void CurrentPlayerEndRound()
    {
        MessageText = GameObject.Instantiate(MessageTextPrefab);

        MessageText.transform.SetParent(GameObject.Find("Canvas").gameObject.transform);
        MessageText.GetComponent<RectTransform>().localPosition = new Vector3(0, 0, 0);

        MessageText.GetComponent<Text>().text = "你的回合结束";

        Destroy(MessageText, 3.0f);
    }

    public void OtherPlayerEndRound(int PlayerID)
    {
        MessageText = GameObject.Instantiate(MessageTextPrefab);

        MessageText.transform.SetParent(GameObject.Find("Canvas").gameObject.transform);
        MessageText.GetComponent<RectTransform>().localPosition = new Vector3(0, 40, 0);

        MessageText.GetComponent<Text>().text = PlayerID + "的回合结束";

        Destroy(MessageText, 3.0f);
    }

    public void CurrentPlayerBeginRound()
    {
        MessageText = GameObject.Instantiate(MessageTextPrefab);

        MessageText.transform.SetParent(GameObject.Find("Canvas").gameObject.transform);
        MessageText.GetComponent<RectTransform>().localPosition = new Vector3(0, 0, 0);

        MessageText.GetComponent<Text>().text = "你的回合开始";

        Destroy(MessageText, 3.0f);
    }

    public void OtherPlayerBeginRound(int PlayerID)
    {
        MessageText = GameObject.Instantiate(MessageTextPrefab);

        MessageText.transform.SetParent(GameObject.Find("Canvas").gameObject.transform);
        MessageText.GetComponent<RectTransform>().localPosition = new Vector3(0, 40, 0);

        MessageText.GetComponent<Text>().text = PlayerID + "的回合开始";

        Destroy(MessageText, 3.0f);
    }

    public void PrivateMessage(string Message)
    {
        MessageText = GameObject.Instantiate(MessageTextPrefab);

        MessageText.transform.SetParent(GameObject.Find("Canvas").gameObject.transform);
        MessageText.GetComponent<RectTransform>().localPosition = new Vector3(0, 0, 0);

        MessageText.GetComponent<Text>().text = Message;

        Destroy(MessageText, 3.0f);
    }

    // Use this for initialization
    void Start ()
    {
        MessageTextPrefab = (GameObject)Resources.Load("Prefabs/UIPrefabs/MessageText");

        if(MessageTextPrefab == null)
        {
            Debug.Log("++++");
        }

    }
	
	// Update is called once per frame
	void Update () {
		
	}
}
