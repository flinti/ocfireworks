/**
	Firework Bomb
	Basic functionality for all firework bombs

	@author Flinti
*/


local fireworkData = {};
local activated = false;


/** 
	Call this function to ignite the bomb 
*/
public func SetFused(object by)
{
	this.Collectible = false;
	if(by)
		SetController(by->GetController());
	if(fireworkData.fuseTime)
		this->ScheduleCall(this, "StartFirework", fireworkData.fuseTime);
	else
		StartFirework();
	return true;
}

/**
	Set the fireworkData
	You should only assign static const proplists as Prototypes!
*/
public func SetFireworkData(proplist data)
{
	fireworkData = data;
}



protected func StartFirework()
{
	activated = true;
	
	if(fireworkData.trail)
		AddFireworkTrail(fireworkData.trail);
	
	if(fireworkData.duration)
		ScheduleCall(this, Global.RemoveObject, fireworkData.duration);
	
	if(fireworkData.effects)
		for(var fweffect in fireworkData.effects)
			if(fweffect.delay)
				ScheduleCall(this, Global.CreateFireworkEffect, fweffect.delay, nil, fweffect);
			else
				CreateFireworkEffect(fweffect);
}

protected func Initialize()
{
	SetFireworkData( { 
		effects = [ new FW_Effect_Die {} ], 
		trail = FW_Trail_None 
	});
}

/* Saving */

private func Indent(string str, int depth)
{
	str = Format("%s		", str);
	var d = depth;
	while(d--)
		str = Format("%s	", str);
	return str;
}


protected func SerializeFireworkDataForScript(data, int depth)
{ // TODO Make the serialization look more like the code that was actually written
	var serialized = "";
	if(GetType(data) == C4V_PropList)
	{
		// Save Prototype
		if(GetPrototype(data))
			serialized = Format("%snew %v ", serialized, data.Prototype);
			
		serialized = Format("%s{\n", serialized);
		
		// We have to take care of that there are duplicates in the array returned from GetProperties if
		// the proplist has a prototype
		var properties = {};
		if(GetPrototype(data))
		{
			for(var prop in GetProperties(data))
				if(!properties[prop])
					properties[prop] = true;
		}
		else
			properties = data;
			
		for(var key in GetProperties(properties)) 
		{
			var value = data[key];
			if(GetPrototype(data))
			{
				var savproto = GetPrototype(data);
				
				SetPrototype(nil, data);
				// if the Prototype's value for key is the same as the data's or
				// if the key does exist in the Prototype, but not in data itself,
				// we don't need to save that.
				if(savproto[key] == value || (savproto[key] && !data[key]))
				{
					SetPrototype(savproto, data);
					continue;
				}
				SetPrototype(savproto, data);
			}
				
			serialized = Indent(serialized, depth);
			var type = GetType(value);
			
			if(type == C4V_Int || type == C4V_Bool || type == C4V_String || type == C4V_Def)
				serialized = Format("%s%s = %v,\n", serialized, key, value);
			else if(type == C4V_PropList || type == C4V_Array)
				serialized = Format("%s%s = %s,\n", serialized, key, SerializeFireworkDataForScript(value, depth+1));
			else if(type == C4V_C4Object)
				serialized = Format("%s%s = %s,\n", serialized, key, value->MakeScenarioSaveName());
			else
				Log("%i::SerializeFireworkData: ERROR! (%s = %v)", GetID(), key, value);
		}
		serialized = Indent(serialized, depth);
		serialized = Format("%s}", serialized);
	}
	else if(GetType(data) == C4V_Array)
	{
		serialized = Format("%s[\n", serialized);
		for(var i = 0; i < GetLength(data); ++i)
		{
			var val = data[i];
			
			serialized = Indent(serialized, depth+1);
			
			var type = GetType(val);
			if(type == C4V_Int || type == C4V_Bool || type == C4V_String || type == C4V_Def)
				serialized = Format("%s%v", serialized, val);
			else if(type == C4V_Array || type == C4V_PropList)
				serialized = Format("%s%s", serialized, SerializeFireworkDataForScript(val, depth+1));
			else if(type == C4V_C4Object)
				serialized = Format("%s%s", serialized, val->MakeScenarioSaveName());
			else
				Log("%i::SerializeFireworkData: ERROR! (%v, index %d)", GetID(), value, i);
				
			if(i < GetLength(data) - 1)
				serialized = Format("%s,\n", serialized);
			else
				serialized = Format("%s\n", serialized);
		}
		serialized = Indent(serialized, depth);
		serialized = Format("%s]", serialized);
	}
	return serialized;
}

public func SaveScenarioObject(proplist props)
{
	if(activated)
		return false;
	if(!inherited(props, ...))
		return false;
	
	
	var saveName = this->MakeScenarioSaveName();	
	props->Add("FireworkData", "var %s_temp = %s;\n	%s->SetFireworkData(%s_temp)", saveName, SerializeFireworkDataForScript(fireworkData), saveName, saveName);
	
	return true;
}


/* Interactions with other objects */
/* You can use this with the grenade lancher & the cannon */

public func Fuse(bool shallExplodeOnHit)
{
	SetFused();
}

public func OnCannonShot(object cannon)
{
	SetFused(cannon);
}

public func IsFireworkBomb() { return true; }
public func IsGrenadeLauncherAmmo() { return true; }


local Collectible = 1;
local Name = "$Name$";
local Description = "$Description$";
local Components = { Coal = 1, Firestone = 1 };
