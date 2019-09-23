using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HandCardController : MonoBehaviour {

    // 卡牌显示控制变量
    List<GameObject> UnitCardUIReferenceList = new List<GameObject>();
    // ==================================================
    // 单位卡牌显示UI对应引用
    // --------------------------------------------------
    public GameObject MainDisplayCard = null;
    // --------------------------------------------------

    // 法术卡牌显示UI对应的引用

    // 装备卡牌显示UI对应的引用

    // ==================================================

    Card RelativeCard = null;
    
    // 动画控制变量
    // ==================================================
    public bool IsRepositioning = false;

    Vector3 StartPosition = new Vector3();
    Vector3 TargetPosition = new Vector3();

    Quaternion TargetRotation = new Quaternion();


    Vector3 RepositionInterval = new Vector3();

    float RepositionSpeed = 15.0f;

    float RotateSpeed = 5.0f;

    float RepositionTime = 0.5f;
    float PastTime = 0.0f;
    // ==================================================

    public void HandCardDisplayOn()
    {
        this.gameObject.SetActive(true);
    }

    public void HandCardDisplayOff()
    {
        this.gameObject.SetActive(false);
    }

    public void SetCard(Card NewCard)
    {
        RelativeCard = NewCard;
    }

    public Card GetCard()
    {
        return RelativeCard;
    }

    void InitializeGameObjectReference()
    {
        if (MainDisplayCard == null) { MainDisplayCard = this.transform.Find("MainDisplayCard").gameObject; }
    }

    public void SetReposition(Vector3 NewPosition, Quaternion NewRotation)
    {
        IsRepositioning = true;

        StartPosition = this.transform.localPosition;
        TargetPosition = NewPosition;
        TargetRotation = NewRotation;

        RepositionInterval = TargetPosition - StartPosition;
        //
        RepositionSpeed = Vector3.Distance(StartPosition, TargetPosition) / RepositionTime;

        RepositionInterval = Vector3.Normalize(RepositionInterval);

        
    }

	// Use this for initialization
	void Start ()
    {
        InitializeGameObjectReference();

    }
	
	// Update is called once per frame
	void Update ()
    {
        if(RelativeCard != null)
        {
            // 因为是手牌所以每帧都要刷新手牌的卡面
            RelativeCard.FetchInfoFromCard(MainDisplayCard);
        }

		if(IsRepositioning == true)
        {
            transform.rotation = Quaternion.Slerp(transform.localRotation, TargetRotation, RotateSpeed * Time.deltaTime);

            //if (PastTime + Time.deltaTime < RepositionTime)
            //{
            //    this.transform.localPosition = this.transform.localPosition + RepositionInterval * RepositionSpeed * Time.deltaTime;
            //}
            //else
            //{
            //    this.transform.localPosition = this.transform.localPosition + RepositionInterval * RepositionSpeed * (RepositionTime - Time.deltaTime);
            //}
            
            Vector3 NewPosition = this.transform.localPosition + RepositionInterval * RepositionSpeed * Time.deltaTime;

            Vector3 V1 = NewPosition - TargetPosition;
            Vector3 V2 = this.transform.localPosition - TargetPosition;

            if(Vector3.Dot(V1,V2) < 0.0f)
            {
                this.transform.localPosition = TargetPosition;
            }
            else
            {
                transform.localPosition = NewPosition;
            }


            if (Vector3.Distance(this.transform.localPosition, TargetPosition) <= 1.0f)
            {
                this.transform.localPosition = TargetPosition;
                IsRepositioning = false;

                PastTime = 0.0f;
            }
        }
	}
}
