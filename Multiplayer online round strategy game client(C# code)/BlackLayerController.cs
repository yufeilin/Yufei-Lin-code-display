using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class BlackLayerController : MonoBehaviour
{
    float StartAlpha = 0.0f;

    float EndAlpha = 0.0f;

    bool IsChangingAlpha = false;

    int FrameNum = 0;

    float Intervl = 0.0f;

    Image ThisImage = null;

    Color StartColor = new Color();

    Color EndColor = new Color();

    public void SetTurnBright(int NewFrameNum)
    {
        EndColor = ThisImage.color;
        EndColor.a = 0.0f;

        StartColor = ThisImage.color;
        StartColor.a = 1.0f;

        //ThisImage.color = new Color(0, 0, 0, 200);
        //FrameNum = NewFrameNum;
        //StartAlpha = 200.0f;
        //EndAlpha = 0.0f;
        //
        //Intervl = (EndAlpha - StartAlpha) / FrameNum;

        IsChangingAlpha = true;
    }

    public void SetTurnDark(int NewFrameNum)
    {
        ThisImage.color = new Color(0, 0, 0, 0);
        FrameNum = NewFrameNum;
        StartAlpha = 0.0f;
        EndAlpha = 200.0f;

        Intervl = (EndAlpha - StartAlpha) / FrameNum;

        IsChangingAlpha = true;
    }

	// Use this for initialization
	void Start ()
    {
        ThisImage = this.GetComponent<Image>();
    }
	
	// Update is called once per frame
	void Update ()
    {
		if(IsChangingAlpha)
        {
            Debug.Log("Black alpha:" + ThisImage.color);

            ThisImage.color = Color.Lerp(ThisImage.color, EndColor, 0.2f);

            //ThisImage.color = new Color(0, 0, 0, 200 + Intervl);
            //if(EndAlpha - ThisImage.color.a <=1.0f)
            //{
            //    ThisImage.color = new Color(0, 0, 0, EndAlpha);
            //    IsChangingAlpha = false;
            //}

            if(Mathf.Abs(ThisImage.color.a - EndColor.a) <=0.01f)
            {
                ThisImage.color = EndColor;
                IsChangingAlpha = false;
            }
        }
	}
}
