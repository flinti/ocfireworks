/**
	Firework Bomb
	Basic functionality for all firework bombs

	@author Flinti
*/


local fireworkData = {};
local fireworkActivated = false;

/* Public Interface */

/** 
 * Call this function to ignite the bomb 
 * @param by The object the ignition is caused by. May be nil.
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
 * Set the fireworkData
 * You should only assign static const proplists as Prototypes!
 * @param data The firework data to set
*/
public func SetFireworkData(proplist data)
{
	fireworkData = data;
}


/* Implementation */

protected func StartFirework()
{
	if(fireworkActivated)
		return;
	fireworkActivated = true;
	
	// duration
	var duration = fireworkData.duration;
	if(duration)
	{
		if(GetType(duration) == C4V_Int)
			ScheduleCall(this, Global.RemoveObject, duration);
		else
			ScheduleCall(this, Global.RemoveObject, RandomX(duration[0], duration[1]));
	}
	
	// trails
	var trails = fireworkData.trails;
	if(trails)
	{
		if(GetType(trails) != C4V_Array)
			trails = [trails];
			
		for(var trail in trails)
		{
			if(trail.delay)
				ScheduleCall(this, this.AddFireworkTrail, trail.delay, nil, trail);
			else
				AddFireworkTrail(trail);
		}
	}
	
	// effects
	var effects = fireworkData.effects;
	if(effects)
	{
		if(GetType(effects) != C4V_Array)
			effects = [effects];
			
		for(var fweffect in effects)
			if(fweffect.delay)
				ScheduleCall(this, this.CreateFireworkEffect, fweffect.delay, nil, fweffect);
			else
				CreateFireworkEffect(fweffect);
	}
}

protected func CreateFireworkEffect(proplist fweffect)
{
	var type = fweffect.type;
	
	if(fweffect.sound)
	{
		var dummy = CreateObject(Dummy);
		dummy->Sound(fweffect.sound);
		dummy->RemoveObject();
	}
	
	if(fweffect.execfn)
		Call(fweffect.execfn, fweffect);
	else
		DebugLog("CreateFireworkEffect: undefined effect type %d", type);
		
	this->~OnFireworkEffect(fweffect);
		
	if(fweffect.die)
		RemoveObject();
}

protected func AddFireworkTrail(proplist trail)
{
	var fx = CreateEffect(FireworkFx_FireworkTrail, 300, 1, trail);
	
	if(trail.initfn)
		Call(trail.initfn, fx, trail);
	fx.trailFunc = trail.execfn;
	
	if(!(trail.initfn || trail.execfn))
		return DebugLog("AddFireworkTrail: undefined trail type %d", trail.type);
}

protected func RemoveFireworkTrails(proplist trail)
{
	var ret = 0;
	while(RemoveEffect("FireworkTrail", this))
		++ret;
	return !!ret;
}

local FireworkFx_FireworkTrail = new Effect
{
	Name = "FireworkTrail",
	
	Construction = func(proplist trail)
	{
		this.trail = trail;
	},
	
	//Play the trail's sound looped and set the lights
	Start = func(int temp)
	{
		if(this.trail.soundLoop)
			Target->Sound(this.trail.soundLoop, nil, nil, nil, 1);
			
		this.oldLightColor = Target->GetLightColor();
		this.oldLightRange = Target->GetObjectVal("LightRange");
		this.oldLightFadeoutRange = Target->GetObjectVal("LightFadeoutRange");
		
		Target->SetLightRange(this.trail.lightRange, this.trail.lightFadeoutRange);
		Target->SetLightColor(this.trail.lightColor ?? this.trail.color);
		
		if(temp)
			return;
		
		Target->~OnTrailStart(this.trail);
	},

	// Stop the trails sound loop and reset the lights as this firework has vanished
	Stop = func(int temp)
	{
		if(this.trail.soundLoop)
			Target->Sound(this.trail.soundLoop, nil, nil, nil, -1);
		Target->SetLightRange(this.oldLightRange, this.oldLightFadeoutRange);
		Target->SetLightColor(this.oldLightColor);
		
		if(temp)
			return;
			
		Target->~OnTrailStop(this.Trail, Time);
		
		if(this.trail.dieAfter)
			Target->RemoveObject();
	},

	// Do the Trail tick every frame
	Timer = func(int time)
	{		
		Target->Call(this.trailFunc, this);
		
		if(this.trail.float)
			Target->SetYDir(Target->GetYDir(10000) - (100 - this.trail.gravity) * GetGravity(), 10000);
		
		if(this.trail.accel)
		{
			var newSpeed = Target->GetSpeed(1000) + this.trail.accel /* * dt (= 1)*/;
			if(newSpeed < 0)
				;
			else
			{
				var angle = Target->GetR();
				Target->SetXDir(Sin(angle, newSpeed), 1000);
				Target->SetYDir(-Cos(angle, newSpeed), 1000);
			}
		}
		
		if(this.trail.duration && time >= this.trail.duration)
			return FX_Execute_Kill;
		
		return FX_OK;
	},
};

protected func Initialize()
{

}




/** 
 * Interactions with other objects
 * You can use this with the cannon
*/

public func Fuse(bool shallExplodeOnHit)
{
	SetFused();
}

public func OnCannonShot(object cannon)
{
	SetFused(cannon);
}

public func IsFireworkBomb() { return true; }


local Collectible = 1;
local Name = "$Name$";
local Description = "$Description$";
local Components = { Coal = 1 };






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
		var needToSaveProto = false;
		if(GetPrototype(data))
		{
			var protostr = Format("%v", GetPrototype(data));
			
			if(GetChar(protostr, 0) == 123 /* { */) // protostr begins with { -> prototype not constant. We will need to save this later
				needToSaveProto = true;
			else // protostr is the name of the constant prototype, use 'new [Prototype] {'
				serialized = Format("%snew %s ", serialized, protostr);
		}
			
		serialized = Format("%s{\n", serialized);
		
		if(needToSaveProto) // If the prototype is not constant, it is saved in the Prototype property instead of using 'new [Prototype] {'
		{
			serialized = Indent(serialized, depth);
			serialized = Format("%sPrototype = %s,\n", serialized, SerializeFireworkDataForScript(GetPrototype(data), depth+1));
		}
		
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
				//   Let's hope that resetting and then setting the Prototype again works fine
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
	if(fireworkActivated)
		return false;
	if(!inherited(props, ...))
		return false;
	
	
	var saveName = this->MakeScenarioSaveName();	
	props->Add("FireworkData", "var %s_fireworkData = %s;\n	%s->SetFireworkData(%s_fireworkData)", saveName, SerializeFireworkDataForScript(fireworkData), saveName, saveName);
	
	return true;
}

