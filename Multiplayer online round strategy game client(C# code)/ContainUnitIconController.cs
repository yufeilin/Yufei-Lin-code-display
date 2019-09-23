using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class ContainUnitIconController : MonoBehaviour
{

    GameObject PieceEntity = null;

    GameObject UnitHealthBar = null;

    GameObject UnitIcon = null;

    PieceStateController ThisPieceStateController = null;

    bool IsMovingUpward = false;

    bool IsMovingDownward = false;

    Vector3 TargetPosition = new Vector3();

    float CurveA = 0.02f;

    float X1 = 0.0f;
    float X2 = 0.0f;

    float Time = 20.0f;

    float IntervalPerFrame = 0.0f;

    float CalculateY(float InputX)
    {
        return CurveA * (InputX - X1) * (InputX - X2);
    }

    public void SetMoveUpward(Vector3 NewTargetPosition)
    {
        Debug.Log("move up set.");

        IsMovingUpward = true;
        TargetPosition = NewTargetPosition;

        IntervalPerFrame = (NewTargetPosition.y - this.GetComponent<RectTransform>().localPosition.y)/ Time;

        X1 = TargetPosition.y;
        X2 = this.GetComponent<RectTransform>().localPosition.y;
    }

    public void SetMoveDownward(Vector3 NewTargetPosition)
    {
        IsMovingDownward = true;
        TargetPosition = NewTargetPosition;

        IntervalPerFrame = (NewTargetPosition.y - this.GetComponent<RectTransform>().localPosition.y) / Time;
    }

    public GameObject GetPieceEntity()
    {
        return PieceEntity;
    }

    public void SetPieceEntity(GameObject NewPieceEntity)
    {
        if(NewPieceEntity != null)
        {
            PieceEntity = NewPieceEntity;
            ThisPieceStateController = NewPieceEntity.GetComponentInChildren<PieceStateController>();

            if(ThisPieceStateController.GetUnitIconImage() == null)
            {
                //Debug.Log("=====================================");
            }

            SetUnitIconImage(ThisPieceStateController.GetUnitIconImage());
        }
        else
        {
            PieceEntity = null;
            ThisPieceStateController = null;
        }
    }

    public void SetUnitIconImage(Texture NewUnitIconImage)
    {
        UnitIcon = this.transform.Find("UnitIcon").gameObject;
        if (UnitIcon == null)
        {
            //Debug.Log("=====================================");
        }

        if(UnitIcon.GetComponent<RawImage>() == null)
        {
            //Debug.Log("+++++++++++++++++++++++++++++++++++++");
        }

        UnitIcon.GetComponent<RawImage>().texture = NewUnitIconImage;
    }

	// Use this for initialization
	void Start ()
    {
        UnitHealthBar = this.transform.Find("UnitHealthBar").gameObject;

        UnitIcon = this.transform.Find("UnitIcon").gameObject;
    }
	
	// Update is called once per frame
	void Update ()
    {
        if(PieceEntity != null)
        {
            // 如果该UI对应的棋子不再存活则UI自行销毁
            if (PieceEntity.GetComponentInChildren<PieceStateController>().GetAliveUnitNum() == 0)
            {
                Destroy(this);
            }

            if (UnitHealthBar != null)
            {
                float HealthBarWidth = 71 * ThisPieceStateController.GetRationOfHealth();

                // 设置RectTransform的Width和Height数值
                UnitHealthBar.GetComponent<RectTransform>().sizeDelta = new Vector2(HealthBarWidth, 10);

                // 设置RectTransform的PosX, PosY 和PosZ数值
                UnitHealthBar.GetComponent<RectTransform>().localPosition = new Vector3((float)((-1) * ((71 - HealthBarWidth) / 2.0)), -30, 0);
            }
            else
            {
                UnitHealthBar = this.transform.Find("UnitHealthBar").gameObject;
            }

            if(IsMovingUpward == true)
            {
                float NewY = this.GetComponent<RectTransform>().localPosition.y + IntervalPerFrame;

                float NewX = CalculateY(NewY);

                this.GetComponent<RectTransform>().localPosition = new Vector3(NewX, NewY, 0);

                if(Mathf.Abs(TargetPosition.y - NewY) <= 0.1f)
                {
                    IsMovingUpward = false;
                }
            }

            if(IsMovingDownward == true)
            {
                float NewY = this.GetComponent<RectTransform>().localPosition.y + IntervalPerFrame;

                float NewX = this.GetComponent<RectTransform>().localPosition.x;

                this.GetComponent<RectTransform>().localPosition = new Vector3(NewX, NewY, 0);

                if (Mathf.Abs(TargetPosition.y - NewY) <= 0.1f)
                {
                    IsMovingDownward = false;
                }
            }
        }       
	}
}
