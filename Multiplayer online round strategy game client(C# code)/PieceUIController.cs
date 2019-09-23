using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class PieceUIController : MonoBehaviour
{
    public GameObject PieceUIBasePrefab = null;

    public GameObject PopoutTextPrefab = null;

    PieceStateController ThisPieceStateController = null;

    bool DisplayHealthBar = true;

    GameObject PieceUIBase;
    bool PieceUIBaseVisibale = true;

    GameObject HealBar = null;

    // 飘字请求队列
    List<PopoutTextRequest> PopoutTextRequestList = new List<PopoutTextRequest>();

    // 相邻两次飘字的时间间隔
    float PopoutInterval = 1.0f;

    // 在上一次飘字到当前帧的时间
    float TimeAfterLastPopout = 0.0f;

    public bool GetDisplayHealthBar()
    {
        return DisplayHealthBar;
    }

    public void SetDisplayHealthBar(bool NewState)
    {
        DisplayHealthBar = NewState;
    }

    // 向飘字请求队列中加入新的飘字请求
    public void AddPopoutTextRequest(PopoutTextRequest NewRequest)
    {
        PopoutTextRequestList.Add(NewRequest);
    }

    public void HidePieceUIBase()
    {
        //Debug.Log("tttttttttttttttttttttttttttttt");

        if (PieceUIBase != null)
        {
            //Debug.Log("CAOCAOCAOCAOCAOCAO");

            PieceUIBase.SetActive(false);
            PieceUIBaseVisibale = false;
        }          
    }

    public void DisplayPieceUIBase()
    {
        if(PieceUIBase != null)
        {
            PieceUIBase.SetActive(true);
            PieceUIBaseVisibale = true;
        }       
    }

    public void CreatePopoutText(PopoutTextRequest NewRequest)
    {
        GameObject PopoutTextBase = Instantiate(PopoutTextPrefab);

        //PopoutTextBase.transform.parent = GameObject.Find("Canvas").gameObject.transform;
        PopoutTextBase.transform.SetParent(GameObject.Find("Canvas").gameObject.transform);

        PopoutTextBase.GetComponent<PopoutTextController>().SetMainText(NewRequest.GetMainText());
        PopoutTextBase.GetComponent<PopoutTextController>().SetTextColor(new Color(255, 0, 0));

        PopoutTextBase.gameObject.transform.position = Camera.main.WorldToScreenPoint(transform.position);

        // 设置飘字在NewRequest.GetExistTime()预设时间后销毁
        Destroy(PopoutTextBase, NewRequest.GetExistTime());

    }

    public void InitializePieceUI()
    {
        //Debug.Log("sasasasasa");

        PieceUIBase = Instantiate(PieceUIBasePrefab);

        //PieceUIBase.transform.parent = GameObject.Find("Canvas").gameObject.transform;
        PieceUIBase.transform.SetParent(GameObject.Find("Canvas/PieceUILayer").gameObject.transform);

        // 在父对象的子对象中查找目标物体
        HealBar = PieceUIBase.transform.Find("HealthBar").gameObject;

        ThisPieceStateController = this.GetComponent<PieceStateController>();

    }

    // Use this for initialization
    void Start ()
    {

        
    }

    // Update is called once per frame
    void Update()
    {
        // 棋子生命值状态更新
        //=========================================================================================================================
        if (PieceUIBase != null && PieceUIBaseVisibale == true)
        {
            //PieceUIBase.fillAmount -= Time.deltaTime / 50.0f;
            PieceUIBase.gameObject.transform.position = Camera.main.WorldToScreenPoint(transform.position);
       
            float HealthBarWidth = 75 * ThisPieceStateController.GetRationOfHealth();

            // 设置RectTransform的Width和Height数值
            HealBar.GetComponent<RectTransform>().sizeDelta = new Vector2(HealthBarWidth, 10);

            // 设置RectTransform的PosX, PosY 和PosZ数值
            HealBar.GetComponent<RectTransform>().localPosition = new Vector3((float)((-1) * ((75 - HealthBarWidth) / 2.0)), -35, 0);

        }
        //=========================================================================================================================

        if (TimeAfterLastPopout >= PopoutInterval)
        {
            if (PopoutTextRequestList.Count > 0)
            {
                CreatePopoutText(PopoutTextRequestList[0]);
                PopoutTextRequestList[0] = null;
                PopoutTextRequestList.Remove(PopoutTextRequestList[0]);

                TimeAfterLastPopout = 0.0f;
            }

        }
        else
        {
            TimeAfterLastPopout += Time.deltaTime;
        }

        if (Input.GetKeyDown(KeyCode.L))
        {
            PopoutTextRequest NewPopoutRequest = new PopoutTextRequest();

            NewPopoutRequest.SetMainText("999");

            AddPopoutTextRequest(NewPopoutRequest);

            HidePieceUIBase();
        }
    }
}
