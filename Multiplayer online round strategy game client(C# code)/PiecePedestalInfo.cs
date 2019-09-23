using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PiecePedestalInfo
{
    string PiecePedestalID = "";

    string PiecePedestalName = "";

    Vector3 LocalPosition = new Vector3();

    Vector3 LocalScale = new Vector3(1,1,1);

    Quaternion LocalRotation = new Quaternion();

    // 棋子雕像底部起始位置
    Vector3 StatusBasePosition = new Vector3();

    GameObject PedestalPrefab = null;

    public string GetPiecePedestalID()
    {
        return PiecePedestalID;
    }

    public void SetPiecePedestalID(string NewPiecePedestalID)
    {
        PiecePedestalID = NewPiecePedestalID;
    }

    public void SetPiecePedestalName(string NewPiecePedestalName)
    {
        PiecePedestalName = NewPiecePedestalName;
    }

    public void SetLocalPosition(Vector3 NewLocalPosition)
    {
        LocalPosition = NewLocalPosition;
    }

    public void SetLocalScale(Vector3 NewLocalScale)
    {
        LocalScale = NewLocalScale;
    }

    public void SetLocalRotation(Vector3 NewLocalRotation)
    {
        LocalRotation = Quaternion.Euler(NewLocalRotation);
    }

    public void SetPedestalPrefab(string PedestalID)
    {
        PedestalPrefab = (GameObject)Resources.Load("Prefabs/Pedestal/" + PedestalID);
    }
	
    public GameObject LoadPedestal(GameObject MainPiece)
    {
        GameObject NewPedestal = GameObject.Instantiate(PedestalPrefab);

        //NewPedestal.transform.SetParent(MainPiece.transform.Find("PiecePedestal"));

        NewPedestal.transform.SetParent(MainPiece.transform);

        NewPedestal.transform.localPosition = LocalPosition;
        NewPedestal.transform.localScale = LocalScale;
        NewPedestal.transform.localRotation = LocalRotation;

        return NewPedestal;
    }
}
