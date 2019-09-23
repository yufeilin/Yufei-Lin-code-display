using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EffectLoader
{
    // 定义效果弹匣
    Dictionary<string, Effect> EffectOfEffectName = new Dictionary<string, Effect>();

    public EffectLoader()
    {
        // 效果弹匣预装填
        EffectOfEffectName.Add("DealDamage", new DealDamage());
        EffectOfEffectName.Add("ApplyBehavior", new ApplyBehavior());
        EffectOfEffectName.Add("RemoveBehavior", new RemoveBehavior());
        EffectOfEffectName.Add("ModifyPlayer", new ModifyPlayer());
    }
	
    public Effect GetNewEffectByType(string EffectType)
    {
        // 从效果弹匣中取出一个对应类型的效果用于返回
        Effect ReturnEffect = EffectOfEffectName[EffectType];

        // 向弹匣中被取出的效果槽位重新装入一个新的相同类型的效果
        EffectOfEffectName[EffectType] = EffectOfEffectName[EffectType].ReloadNewEffect();

        // 将取出的效果返回
        return ReturnEffect;
    }
}
