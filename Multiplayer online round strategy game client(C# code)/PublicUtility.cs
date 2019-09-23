using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PublicUtility
{
    public static int StringToInt(string s)
    {
        float Tmp = float.Parse(s);

        return (int)Tmp;
    }

    public static float StringToFloat(string s)
    {
        float Tmp = float.Parse(s);

        return Tmp;
    }

    public static Vector3 StringToVector3(string ParaString)
    {
        Vector3 Res = new Vector3();

        string[] StringArray = ParaString.Split(' ');
        float[] ConvertArray = new float[3];

        Res.x = StringToFloat(StringArray[0]);
        Res.y = StringToFloat(StringArray[1]);
        Res.z = StringToFloat(StringArray[2]);

        return Res;
    }

    public static Vector2Int StringToVector2Int(string ParaString)
    {
        Vector2Int Res = new Vector2Int();

        string[] StringArray = ParaString.Split(' ');
        int[] ConvertArray = new int[2];

        Res.x = StringToInt(StringArray[0]);
        Res.y = StringToInt(StringArray[1]);

        return Res;
    }

}
