using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class PlayerResourceAreaController : MonoBehaviour
{
    int PlayerID = 0;

    public GameObject MainGameManager = null; 

    ResourceManager MainResourceManager = null;

    GameObject MineralBar = null;
    GameObject GasBar = null;

    GameObject MineralValueText = null;
    GameObject GasValueText = null;

    float MineralBarWidth = 0;
    float MineralBarHeight = 0;

    float GasBarWidth = 0;
    float GasBarHeight = 0;

    bool UIOn = false;

    public void Initialize()
    {
        MainResourceManager = MainGameManager.GetComponent<GameController>().GetResourceManager();

        MineralBar = this.transform.Find("MineralBarBackGround/MineralBar").gameObject;
        GasBar = this.transform.Find("GasBarBackGround/GasBar").gameObject;

        MineralValueText = this.transform.Find("MineralBarBackGround/MineralValueText").gameObject;
        GasValueText = this.transform.Find("GasBarBackGround/GasValueText").gameObject;

        MineralBarWidth = MineralBar.GetComponent<RectTransform>().sizeDelta.x;
        MineralBarHeight = MineralBar.GetComponent<RectTransform>().sizeDelta.y;

        GasBarWidth = MineralBar.GetComponent<RectTransform>().sizeDelta.x;
        GasBarHeight = MineralBar.GetComponent<RectTransform>().sizeDelta.y;

        //Debug.Log("SetFalse");
        //this.gameObject.SetActive(false);
    }

    public void UIUpdate()
    {
        int PlayerMineral = MainResourceManager.GetClientPlayerMineral();
        int PlayerGas = MainResourceManager.GetClientPlayerGas();
        int PlayerMineralCapacity = MainResourceManager.GetClientPlayerMineralCapacity();
        int PlayerGasCapacity = MainResourceManager.GetClientPlayerGasCapacity();

        MineralValueText.GetComponent<Text>().text = PlayerMineral + " / " + PlayerMineralCapacity;
        GasValueText.GetComponent<Text>().text = PlayerGas + " / " + PlayerGasCapacity;

        
        // 晶体矿UI更新
        float MineralRatio = (float)PlayerMineral / (float)PlayerMineralCapacity;
        MineralBar.GetComponent<RectTransform>().sizeDelta = new Vector2(MineralBarWidth * MineralRatio, MineralBarHeight);
        MineralBar.GetComponent<RectTransform>().localPosition = new Vector3( (float)((-1) * ((1 - MineralRatio) * MineralBarWidth / 2.0)), 0, 0);

        // 瓦斯UI更新
        float GasRatio = (float)PlayerGas / (float)PlayerGasCapacity;
        GasBar.GetComponent<RectTransform>().sizeDelta = new Vector2(GasBarWidth * GasRatio, GasBarHeight);
        GasBar.GetComponent<RectTransform>().localPosition = new Vector3((float)((-1) * ((1 - GasRatio) * GasBarWidth / 2.0)), 0, 0);

    }

    public void DisplayResourceUI()
    {
        this.gameObject.SetActive(true);
        UIOn = true;
    }

    // Use this for initialization
    void Start ()
    {
        PlayerID = 0;

        Initialize();
    }
	
	// Update is called once per frame
	void Update ()
    {
        if(UIOn)
        {
            UIUpdate();
        }
    }
}
