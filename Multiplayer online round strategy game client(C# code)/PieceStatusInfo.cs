using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PieceStatusInfo
{
    string PieceStatusID = "";

    string PieceStatusName = "";

    //Vector3 LocalPosition = new Vector3();

    Vector3 LocalScale = new Vector3(1, 1, 1);

    Quaternion LocalRotation = new Quaternion();

    GameObject StatusPrefab = null;

    public string GetPieceStatusID()
    {
        return PieceStatusID;
    }

    public void SetPieceStatusID(string NewPieceStatusID)
    {
        PieceStatusID = NewPieceStatusID;
    }

    public void SetPieceStatusName(string NewPieceStatusName)
    {
        PieceStatusName = NewPieceStatusName;
    }

    //public void SetLocalPosition(Vector3 NewLocalPosition)
    //{
    //    LocalPosition = NewLocalPosition;
    //}

    public void SetLocalScale(Vector3 NewLocalScale)
    {
        LocalScale = NewLocalScale;
    }

    public void SetLocalRotation(Vector3 NewLocalRotation)
    {
        LocalRotation = Quaternion.Euler(NewLocalRotation);
    }

    public void SetStatusPrefab(string StatusID)
    {
        StatusPrefab = (GameObject)Resources.Load("Prefabs/Status/" + StatusID);
    }

    public GameObject LoadStatus(GameObject MainPiece, GameObject MainPedestal)
    {
        GameObject NewStatus = GameObject.Instantiate(StatusPrefab);

        NewStatus.transform.SetParent(MainPiece.transform);

        // 将棋子雕像放置在棋子底座的雕像基座位置
        NewStatus.transform.position = MainPedestal.transform.Find("StatusBase").position;

        NewStatus.transform.localScale = LocalScale;
        NewStatus.transform.localRotation = LocalRotation;

        return NewStatus;
    }


}
