using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class PopoutTextController : MonoBehaviour
{

    GameObject PopoutText = null;

    string MainText = "";

    bool ReadyToAnimate = true;

    // 飘字向上移动速度
    float PopoutSpeed = 0.8f;

    // 飘字透明渐变速度
    float FadeSpeed = 0.01f;

    // 飘字存在时间
    float ExistTime = 5.0f;

    public string GetMainText()
    {
        return MainText;
    }

    public void SetMainText(string NewText)
    {
        MainText = NewText;

        PopoutText = this.transform.Find("PopoutText").gameObject;
        PopoutText.GetComponent<Text>().text = MainText;
    }

    public void SetTextColor(Color NewColor)
    {
        PopoutText = this.transform.Find("PopoutText").gameObject;
        PopoutText.GetComponent<Text>().color = NewColor;
    }

    public void ActivatePopoutText()
    {
        ReadyToAnimate = true;
    }

	// Use this for initialization
	void Start ()
    {
        PopoutText = this.transform.Find("PopoutText").gameObject;

    }
	
	// Update is called once per frame
	void Update ()
    {
		if(ReadyToAnimate == true)
        {
            Vector3 OldLocalPosition = PopoutText.GetComponent<RectTransform>().localPosition;

            PopoutText.GetComponent<RectTransform>().localPosition = new Vector3(OldLocalPosition.x, OldLocalPosition.y + PopoutSpeed, OldLocalPosition.z);

            Color OldColor = PopoutText.GetComponent<Text>().color;

            if(OldColor.a - FadeSpeed < 0)
            {
                OldColor.a = 0;
            }
            else
            {
                OldColor.a -= FadeSpeed;
            }


            PopoutText.GetComponent<Text>().color = new Color(OldColor.r, OldColor.g, OldColor.b, OldColor.a);
        }
	}
}
