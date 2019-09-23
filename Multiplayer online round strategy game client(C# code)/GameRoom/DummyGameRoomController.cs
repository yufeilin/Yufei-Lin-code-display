using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DummyGameRoomController : MonoBehaviour {

	// Use this for initialization
	void Start ()
    {
        // 找到真正的游戏房间管理器
        GameObject GameRoomManager = GameObject.Find("GameRoomManager");

        // 通知真正的游戏房间管理器刷新游戏房间显示
        GameRoomManager.GetComponent<GameRoomController>().RefreashRoomDisplay();

        // 销毁自身
        Destroy(this.gameObject);
    }
	
}
