using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Xml;     //xml操作
using System.IO;     //file操作

public class GameLoader
{
    GameController MainGameController = null;

    WorldManager MainWorldManager = null;

    ResourceManager MainResourceManager = null;

    string GameMapID = "";

    public GameLoader(GameController ParaGameController)
    {
        MainGameController = ParaGameController;
    }

    public void SetGameMapID(string NewGameMapID) { GameMapID = NewGameMapID; }

    public string GetGameMapID() { return GameMapID; }

    public void EstablishReference()
    {
        MainWorldManager = MainGameController.GetWorldManager();
        MainResourceManager = MainGameController.GetResourceManager();
    }

    public void LoadChessBoardFromXML()
    {
        string filePath = Application.dataPath + "/Resources/XML/ChessBoardConfig/"+ GameMapID + ".xml";

        if (File.Exists(filePath))
        {
            XmlDocument xmlDoc = new XmlDocument();
            xmlDoc.Load(filePath);
            XmlNodeList node = xmlDoc.SelectSingleNode("ChessBoardConfig").ChildNodes;
            foreach (XmlElement ChessBoardInfo in node)
            {
                foreach (XmlElement ChessBoardInfoItem in ChessBoardInfo.ChildNodes)
                {
                    if (ChessBoardInfoItem.Name == "Coordinate")
                    {
                        MainWorldManager.CreateChessboard(PublicUtility.StringToVector2Int(ChessBoardInfoItem.InnerText));
                    }
                    else if(ChessBoardInfoItem.Name == "DisableCheckerBoard")
                    {
                        foreach (XmlElement DisableCheckerBoardInfoItem in ChessBoardInfoItem.ChildNodes)
                        {
                            Debug.Log(DisableCheckerBoardInfoItem.InnerText);
                        }
                    }
                }
            }
        }
    }

    public void LoadInitialPieceFromXML()
    {
        string filePath = Application.dataPath + "/Resources/XML/InitialPieceConfig/" + GameMapID + ".xml";

        if (File.Exists(filePath))
        {
            XmlDocument xmlDoc = new XmlDocument();
            xmlDoc.Load(filePath);
            XmlNodeList node = xmlDoc.SelectSingleNode("InitialPieceConfig").ChildNodes;
            foreach (XmlElement InitialPieceInfo in node)
            {
                string CurrentPieceID = "";
                Vector2Int CurrentOuterCoordinate = new Vector2Int();
                Vector2Int CurrentInnerCoordinate = new Vector2Int();

                foreach (XmlElement InitialPieceInfoItem in InitialPieceInfo.ChildNodes)
                {
                    if (InitialPieceInfoItem.Name == "PieceID")
                    {
                        CurrentPieceID = InitialPieceInfoItem.InnerText;
                    }
                    else if(InitialPieceInfoItem.Name == "OuterCoordinate")
                    {
                        CurrentOuterCoordinate = PublicUtility.StringToVector2Int(InitialPieceInfoItem.InnerText);
                    }
                    else if(InitialPieceInfoItem.Name == "InnerCoordinate")
                    {
                        CurrentInnerCoordinate = PublicUtility.StringToVector2Int(InitialPieceInfoItem.InnerText);
                    }
                    else if(InitialPieceInfoItem.Name == "Orientation")
                    {
                        Debug.Log(InitialPieceInfoItem.InnerText);
                    }
                }

                MainWorldManager.CreatePieceOnChessboard(CurrentPieceID, CurrentInnerCoordinate, CurrentOuterCoordinate);
            }
        }
    }

    public void LoadClientPlayerResource()
    {
        string filePath = Application.dataPath + "/Resources/GamePlay/" + GameMapID + "/ResourceConfig.xml";

        if (File.Exists(filePath))
        {
            

            XmlDocument xmlDoc = new XmlDocument();
            xmlDoc.Load(filePath);
            XmlNodeList node = xmlDoc.SelectSingleNode("ResourceConfig").ChildNodes;

            int CurrentPlayerID = -1;
            int CurrentMineral = -1;
            int CurrentMineralCapacity = -1;
            int CurrentGas = -1;
            int CurrentGasCapacity = -1;

            foreach (XmlElement PlayerResourceInfo in node)
            {


                foreach (XmlElement PlayerResourceInfoItem in PlayerResourceInfo.ChildNodes)
                {
                    if (PlayerResourceInfoItem.Name == "PlayerID")
                    {
                        CurrentPlayerID = PublicUtility.StringToInt(PlayerResourceInfoItem.InnerText);
                    }
                    else if(PlayerResourceInfoItem.Name == "Mineral")
                    {
                        CurrentMineral = PublicUtility.StringToInt(PlayerResourceInfoItem.InnerText);
                    }
                    else if (PlayerResourceInfoItem.Name == "MineralCapacity")
                    {
                        CurrentMineralCapacity = PublicUtility.StringToInt(PlayerResourceInfoItem.InnerText);
                    }
                    else if(PlayerResourceInfoItem.Name == "Gas")
                    {
                        CurrentGas = PublicUtility.StringToInt(PlayerResourceInfoItem.InnerText);
                    }
                    else if (PlayerResourceInfoItem.Name == "GasCapacity")
                    {
                        CurrentGasCapacity = PublicUtility.StringToInt(PlayerResourceInfoItem.InnerText);
                    }
                }

                if(CurrentPlayerID == MainGameController.GetPlayerID())
                {
                    break;
                }
            }

            Debug.Log("233"+ CurrentPlayerID);

            MainResourceManager.SetClientPlayerMineral(CurrentMineral);
            MainResourceManager.SetClientPlayerMineralCapacity(CurrentMineralCapacity);
            MainResourceManager.SetClientPlayerGas(CurrentGas);
            MainResourceManager.SetClientPlayerGasCapacity(CurrentGasCapacity);
            MainResourceManager.DisplayResourceUI();

            GameObject.Find("PlayerResourceArea").GetComponent<PlayerResourceAreaController>().DisplayResourceUI();

        }
    }

    public void LoadGame()
    {
        LoadChessBoardFromXML();

        LoadInitialPieceFromXML();

        LoadClientPlayerResource();

        MainGameController.GetUIController().LoadUI();
    }
}
