using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RoomPlayerSlot
{
    // 玩家槽位ID
    int SlotID = -1;

    // 该玩家槽位所属的队伍ID
    int TeamID = -1;

    // 玩家槽位包含的玩家
    RoomPlayer ContainRoomPlayer = null;

    public int GetSlotID() { return SlotID; }
    public void SetSlotID(int NewSlotID) { SlotID = NewSlotID; }

    public int GetTeamID() { return TeamID; }
    public void SetTeamID(int NewTeamID) { TeamID = NewTeamID; }

    public RoomPlayer GetContainRoomPlayer() { return ContainRoomPlayer; }
    public void SetContainRoomPlayer(RoomPlayer NewContainRoomPlayer) { ContainRoomPlayer = NewContainRoomPlayer; }

}
