using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraController : MonoBehaviour
{
    // 摄像机初始预设位置
    public Vector3 InitCameraPosition = new Vector3(0.0f,12.8f,-7.1f);

    // 缩放速率
    public float ZoomSpeed = 2.5f;

    // 摄像机向Forward方向移动的最大距离
    public float FarwardBound = 3.0f;

    // 摄像机向Forward反方向移动的最大距离
    public float BackwardBound = 3.0f;

    GameObject MainCamera = null;

    float sensitivityAmt = 1.0f;

    Vector3 OriginalCameraPosition;

    // Use this for initialization
    void Start ()
    {
        // 将主摄像机的位置修正到初始预设位置
        Camera.main.transform.position = InitCameraPosition;

        OriginalCameraPosition = InitCameraPosition;
    }
	
	// Update is called once per frame
	void Update ()
    {
        //Vector3 p0 = Camera.main.transform.position;
        //Vector3 p01 = p0 - Camera.main.transform.right * Input.GetAxisRaw("Mouse X") * sensitivityAmt * Time.timeScale;
        //Vector3 p03 = p01 - Camera.main.transform.up * Input.GetAxisRaw("Mouse Y") * sensitivityAmt * Time.timeScale;
        //Camera.main.transform.position = p03;  


        float DistanceFromInit = Vector3.Distance(OriginalCameraPosition, Camera.main.transform.position);

        //if()

        Camera.main.transform.position = Camera.main.transform.position + Camera.main.transform.forward * Input.GetAxis("Mouse ScrollWheel") * ZoomSpeed;



    }
}
