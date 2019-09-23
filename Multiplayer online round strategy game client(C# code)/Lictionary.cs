using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Lictionary <Key, Value>
{
    public int Count = 0;

    Dictionary<Key, Value> ValueOfKey = new Dictionary<Key, Value>();

    Dictionary<int, Key> KeyOfIndex = new Dictionary<int, Key>();

    public void Add(Key NewKey, Value NewValue)
    {
        
    }
	
}
