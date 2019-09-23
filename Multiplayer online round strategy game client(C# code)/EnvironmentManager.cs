using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Xml;     //xml操作
using System.IO;     //file操作

public class EnvironmentManager
{
    GameController MainGameController = null;

    public EnvironmentManager(GameController ParaGameController)
    {
        MainGameController = ParaGameController;
    }

    public void EstablishReference()
    {

    }

    public void CreateTerrainDecoration(string DecorationName, Vector3 DecorationPosition, Vector3 DecorationRotation, Vector3 DecorationScale)
    {
        GameObject TerrainDecorationPrefabs = (GameObject)Resources.Load("Models/TerrainDecoration/" + DecorationName + "/" + DecorationName);

        //Debug.Log("Models/TerrainDecoration/" + DecorationName + "/" + DecorationName);

        GameObject CurrentTerrainDecoration = GameObject.Instantiate(TerrainDecorationPrefabs);

        CurrentTerrainDecoration.transform.localPosition = DecorationPosition;
        CurrentTerrainDecoration.transform.rotation = Quaternion.Euler(DecorationRotation);
        CurrentTerrainDecoration.transform.localScale = DecorationScale;
    }

    public void LoadTerrainDecorationLayout(string LayoutName)
    {
        string filePath = Application.dataPath + "/Resources/TDL/" + LayoutName + ".xml";

        if (File.Exists(filePath))
        {
            XmlDocument xmlDoc = new XmlDocument();
            xmlDoc.Load(filePath);
            XmlNodeList node = xmlDoc.SelectSingleNode("TerrainDecorationLayout").ChildNodes;
            foreach (XmlElement DecorationInfo in node)
            {
                string TerrainDecorationName = "";
                Vector3 TerrainDecorationPosition = new Vector3();
                Vector3 TerrainDecorationRotation = new Vector3();
                Vector3 TerrainDecorationScale = new Vector3();

                foreach (XmlElement DecorationInfoItem in DecorationInfo.ChildNodes)
                {
                    if (DecorationInfoItem.Name == "DecorationItemName")
                    {
                        //Debug.Log(DecorationInfoItem.InnerText);
                        TerrainDecorationName = DecorationInfoItem.InnerText;
                    }
                    else if (DecorationInfoItem.Name == "DecorationItemPositionX")
                    {
                        TerrainDecorationPosition.x = PublicUtility.StringToFloat(DecorationInfoItem.InnerText);
                    }
                    else if (DecorationInfoItem.Name == "DecorationItemPositionY")
                    {
                        TerrainDecorationPosition.y = PublicUtility.StringToFloat(DecorationInfoItem.InnerText);
                    }
                    else if (DecorationInfoItem.Name == "DecorationItemPositionZ")
                    {
                        TerrainDecorationPosition.z = PublicUtility.StringToFloat(DecorationInfoItem.InnerText);
                    }
                    else if (DecorationInfoItem.Name == "DecorationItemScaleX")
                    {
                        TerrainDecorationScale.x = PublicUtility.StringToFloat(DecorationInfoItem.InnerText);
                    }
                    else if (DecorationInfoItem.Name == "DecorationItemScaleY")
                    {
                        TerrainDecorationScale.y = PublicUtility.StringToFloat(DecorationInfoItem.InnerText);
                    }
                    else if (DecorationInfoItem.Name == "DecorationItemScaleZ")
                    {
                        TerrainDecorationScale.z = PublicUtility.StringToFloat(DecorationInfoItem.InnerText);
                    }
                    else if (DecorationInfoItem.Name == "DecorationItemRotationX")
                    {
                        TerrainDecorationRotation.x = PublicUtility.StringToFloat(DecorationInfoItem.InnerText);
                    }
                    else if (DecorationInfoItem.Name == "DecorationItemRotationY")
                    {
                        TerrainDecorationRotation.y = PublicUtility.StringToFloat(DecorationInfoItem.InnerText);
                    }
                    else if (DecorationInfoItem.Name == "DecorationItemRotationZ")
                    {
                        TerrainDecorationRotation.z = PublicUtility.StringToFloat(DecorationInfoItem.InnerText);
                    }

                }

                CreateTerrainDecoration(TerrainDecorationName, TerrainDecorationPosition, TerrainDecorationRotation, TerrainDecorationScale);
            }
        }
    }

}
