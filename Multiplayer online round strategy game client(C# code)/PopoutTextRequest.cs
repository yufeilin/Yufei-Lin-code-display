using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PopoutTextRequest
{
    string MainText = "";

    float ExistTime = 3.0f;

    public string GetMainText()
    {
        return MainText;
    }

    public void SetMainText(string NewText)
    {
        MainText = NewText;
    }

    public float GetExistTime()
    {
        return ExistTime;
    }

    public void SetExisTime(float NewExistTime)
    {
        ExistTime = NewExistTime;
    }
	
}
