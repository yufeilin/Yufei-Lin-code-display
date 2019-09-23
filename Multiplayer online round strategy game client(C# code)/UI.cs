using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class UI : MonoBehaviour
{

    public Image blood;

    // Use this for initialization
    void Start ()
    {
		
	}
	
	// Update is called once per frame
	void Update ()
    {
        if (blood != null)
        {
            blood.fillAmount -= Time.deltaTime / 50.0f;
            blood.gameObject.transform.position = Camera.main.WorldToScreenPoint(transform.position);


        }
    }
}
