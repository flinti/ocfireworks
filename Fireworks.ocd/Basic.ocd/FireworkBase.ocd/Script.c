/**
 * Firework Base
 * 
 * Fusing, saving and basic functionality of fireworks
 * 
 * @author Flinti
*/

local fireworkData = new FireworkData {}; 
local fireworkActivated = false;


func Initialize()
{
	DebugLog("The abstract object %i may not be created", GetID());
	RemoveObject();
}

/* Public Interface */

/** 
 * Call this function to ignite the firework 
 * @param by The object the ignition is caused by. May be nil.
*/
public func SetFused(object by)
{
	if(fireworkActivated)
		return;
	
	this->~OnFused(by);
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
 * @param data The firework data to set
*/
public func SetFireworkData(proplist data)
{
	fireworkData = data;
}



/* Implementation */

protected func OnFinished()
{
	while(RemoveEffect("Firework*", this));
	ClearScheduleCall(this, this.CreateFireworkTrail);
	ClearScheduleCall(this, this.CreateFireworkEffect);
	ClearScheduleCall(this, this.StartSalvo);
}

protected func StartFirework()
{
	if(fireworkActivated)
		return;
	fireworkActivated = true;
	
	this->DoStartFirework();
}

protected func DoStartFirework()
{
	// duration ()
	var duration = fireworkData.duration;
	if(duration)
	{
		ScheduleCall(this, this.OnFinished, FW_Distribution->GetValue(duration) ?? 1);
	}
	
	ScheduleTrails(fireworkData.trails);
	ScheduleEffects(fireworkData.effects);
	ScheduleShots(fireworkData.shots);
}

// trails (every 'timer' frames after 'delay' frames, lasting 'duration' frames)
protected func ScheduleTrails(trails, proplist parentFireworkData)
{
	if(!parentFireworkData)
		parentFireworkData = this.fireworkData;
	if(trails)
	{
		if(GetType(trails) != C4V_Array)
			trails = [trails];
			
		for(var trail in trails)
		{
			if(trail.delay)
				ScheduleCall(this, this.CreateFireworkTrail, FW_Distribution->GetValue(trail.delay) ?? 1, nil, trail, parentFireworkData);
			else
				CreateFireworkTrail(trail, parentFireworkData);
		}
	}
}

// effects (one-time after 'delay' frames)
protected func ScheduleEffects(effects, proplist parentFireworkData)
{
	if(effects)
	{
		if(GetType(effects) != C4V_Array)
			effects = [effects];
			
		for(var fweffect in effects)
			if(fweffect.delay)
				ScheduleCall(this, this.CreateFireworkEffect, FW_Distribution->GetValue(fweffect.delay) ?? 1, nil, fweffect, parentFireworkData);
			else
				CreateFireworkEffect(fweffect, parentFireworkData);
	}
}

// shots (automatically stepped)
protected func ScheduleShots(shots, proplist parentFireworkData)
{
	
	var delay, wholedelay;
	if(shots)
	{
		if(GetType(shots) != C4V_Array)
			shots = [shots];
		
		delay = shots[0].delay ?? 0;
		wholedelay = delay;
		
		for(var i = 0; i < GetLength(shots); ++i)
		{
			var shot = shots[i];
			
			if(i > 0)
			{
				var prevshot = shots[i - 1];
				delay = prevshot.delay + (prevshot.step * prevshot.amount);
				wholedelay += delay;
			}
			if(wholedelay)
				ScheduleCall(this, this.StartSalvo, wholedelay + shot.delay, nil, shot, parentFireworkData);
			else
				StartSalvo(shot, parentFireworkData);
		}
	}
}



protected func CreateFireworkEffect(proplist fweffect_, proplist parentFireworkData)
{
	if(!parentFireworkData)
		parentFireworkData = this.fireworkData;
		
	var fweffect = new fweffect_ {};
	
	fweffect.distributionData = parentFireworkData.distributionData;
	
	var type = fweffect.type;
	
	if(fweffect.sound)
	{
		var dummy = CreateObject(Dummy);
		dummy->Sound(fweffect.sound);
		dummy->RemoveObject();
	}
	
	if(fweffect.flash)
	{
		var particle;
		if(fweffect.flash != true)
			particle = Particles_Colored(Particles_Flash(), fweffect.flash);
		else
			particle = Particles_Flash();
		CreateParticle("Flash", 0, 0, 0, 0, 20, particle);
	}
	
	if(fweffect.execfn)
		Call(fweffect.execfn, fweffect);
	else
		DebugLog("CreateFireworkEffect: undefined effect type %d", type);
		
	this->~OnFireworkEffect(fweffect);
		
	if(fweffect.die)
		RemoveObject();
}

protected func CreateFireworkTrail(proplist trail_, proplist parentFireworkData)
{
	if(!parentFireworkData)
		parentFireworkData = this.fireworkData;
		
	var trail = new trail_ {};
	
	trail.distributionData = parentFireworkData.distributionData;
		
	var fx = CreateEffect(FireworkFx_FireworkTrail, trail.effectPriority ?? 300, trail.timer ?? 1, trail);
	
	if(trail.initfn)
		Call(trail.initfn, fx, trail);
	fx.trailFunc = trail.execfn;
	
	if(!(trail.initfn || trail.execfn))
	{
		RemoveEffect(fx);
		return DebugLog("AddFireworkTrail: undefined trail type %d", trail.type);
	}
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
		if(trail.distributionData)
		{
			this.trail.color = FW_Distribution->GetValue(trail.color, trail.distributionData.shotIndex, trail.distributionData.shotAmount, trail.distributionData.shotTypeIndex);
			this.trail.color2 = FW_Distribution->GetValue(trail.color2, trail.distributionData.shotIndex, trail.distributionData.shotAmount, trail.distributionData.shotTypeIndex);
			this.trail.lightColor = FW_Distribution->GetValue(trail.lightColor, trail.distributionData.shotIndex, trail.distributionData.shotAmount, trail.distributionData.shotTypeIndex);
		}
	},
	
	//Play the trail's sound looped and set the lights
	Start = func(int temp)
	{
		if(this.trail.sound)
			Target->Sound(this.trail.sound);
		if(this.trail.soundLoop)
			Target->Sound(this.trail.soundLoop, nil, nil, nil, 1);
			
		if(this.trail.light)
		{
			this.oldLightColor = Target->GetLightColor();
			this.oldLightRange = Target->GetObjectVal("LightRange");
			this.oldLightFadeoutRange = Target->GetObjectVal("LightFadeoutRange");
		
			Target->SetLightRange(this.trail.lightRange, this.trail.lightFadeoutRange);
			Target->SetLightColor(this.trail.lightColor ?? this.trail.color);//BlendColors(this.trail.lightColor ?? this.trail.color, this.oldLightColor));
		}
		
		
		if(temp)
			return;
		
		Target->~OnTrailStart(this.trail);
	},

	// Stop the trails sound loop and reset the lights as this firework has vanished
	Stop = func(int temp)
	{
		if(this.trail.soundLoop)
			Target->Sound(this.trail.soundLoop, nil, nil, nil, -1);
			
		if(this.trail.light)
		{
			Target->SetLightRange(this.oldLightRange, this.oldLightFadeoutRange);
			Target->SetLightColor(this.oldLightColor);
		}
		
		if(temp)
			return;
			
		Target->~OnTrailStop(this.trail, Time);
		
		if(this.trail.dieAfter)
			Target->RemoveObject();
	},

	// Do the Trail tick every 'timer' frames
	Timer = func(int time)
	{		
		Target->Call(this.trailFunc, this);
		
		if(this.trail.duration && Time >= this.trail.duration)
			return FX_Execute_Kill;
		
		return FX_OK;
	},
};

protected func StartSalvo(proplist shot, proplist parentFireworkData)
{
	if(!shot.amount)
		return;
	if(!parentFireworkData)
		parentFireworkData = fireworkData;
		
	//Do not pass any distribution data here, because that data is set by us
		
	CreateEffect(FxFireworkSalvo, 300, shot.step, shot);
}

protected func DoShot(proplist fx)
{
	var typeIndex = fx.index % fx.shotTypes;
	var shot = fx.shot.emitted[typeIndex];
	
	var angle = shot.angle;
	if(angle)
		angle = FW_Distribution->GetValue(angle, fx.index, fx.shot.amount, typeIndex, false) + GetR();
	
	var radius = shot.height;
	if(radius)
		radius = FW_Distribution->GetValue(radius, fx.index, fx.shot.amount, typeIndex);
		
	var xoffset = shot.xoffset;
	if(xoffset)
		xoffset = FW_Distribution->GetValue(xoffset, fx.index, fx.shot.amount, typeIndex);
//	Log("xoffset: %d; index: %d, amount: %d, typeIndex: %d", xoffset, fx.index, fx.shot.amount, typeIndex);
	
	var obj = CreateObject(FireworkBomb, Sin(angle, radius) + Cos(angle, xoffset), -Cos(angle, radius) + Sin(angle, xoffset));
	
	var rotation = shot.rotation ?? [0, 359];
	obj->SetR(FW_Distribution->GetValue(rotation, fx.index, fx.shot.amount, typeIndex, true));
	
	var rotationSpeed = shot.rotationSpeed;
	if(rotationSpeed)
		obj->SetRDir(FW_Distribution->GetValue(rotationSpeed, fx.index, fx.shot.amount, typeIndex), shot.rotationPrecision);
	
	var speed = shot.speed;
	if(speed)
	{
		speed = FW_Distribution->GetValue(speed, fx.index, fx.shot.amount, typeIndex);
		
		obj->SetXDir(Sin(angle, speed), shot.speedPrecision);
		obj->SetYDir(-Cos(angle, speed), shot.speedPrecision);
	}
	
	if(shot.sound)
	{
		var dummy = CreateObject(Dummy);
		dummy->Sound(shot.sound);
		dummy->RemoveObject();
	}
	
	obj.fireworkData = new shot.fireworkData {};
	if(!obj.fireworkData.distributionData)
		obj.fireworkData.distributionData = CreatePropList();
		
	obj.fireworkData.distributionData.shotIndex = fx.index;
	obj.fireworkData.distributionData.shotAmount = fx.shot.amount;
	obj.fireworkData.distributionData.shotTypeIndex = typeIndex;
	obj->SetFused(this);
	
	++fx.index;
	if(fx.index >= fx.shot.amount)
		return FX_Execute_Kill;
	return FX_OK;
}

local FxFireworkSalvo = new Effect {
	Name = "FireworkSalvo",
	
	Construction = func(proplist shot)
	{
		this.shot = shot;
		this.index = 0;
		this.shotTypes = GetLength(shot.emitted);
		if(shot.step)
			Timer();
		else
		{
			while(Timer() != FX_Execute_Kill);
			return FX_Execute_Kill;
		}
	},
	
	Timer = func()
	{
		return Target->DoShot(this);
	}
};


/* Saving */

private func Indent(string str, int depth)
{
	str = Format("%s		", str);
	var d = depth;
	while(d--)
		str = Format("%s	", str);
	return str;
}

private func GetSpecialSerializationString(string key, value)
{
	if((WildcardMatch(key, "color*") || 
	    WildcardMatch(key, "lightColor*")
	   ) 
	    
	  && GetType(value) == C4V_Int)
		return Format("RGBa(%d, %d, %d, %d)", GetRGBaValue(value, 1), GetRGBaValue(value, 2), GetRGBaValue(value, 3), GetRGBaValue(value, 0));
}

protected func SerializeFireworkDataForScript(data, int depth)
{ // TODO Make the serialization look more like the code that was actually written
	var serialized = "";
	if(GetType(data) == C4V_PropList)
	{
		// Save Prototype
		var needToSaveProto = false, prototypeIsFireworkData = false;
		var prototype = GetPrototype(data);
		if(prototype)
		{
			var protostr = Format("%v", GetPrototype(data));
			
			if(GetChar(protostr, 0) == 123 /* { */) // protostr begins with { -> prototype not constant. We will need to save this later
				needToSaveProto = true;
			else // protostr is the name of the constant prototype, use 'new [Prototype] {'
				serialized = Format("%snew %s ", serialized, protostr);
			
			if(protostr == "FireworkData")
				prototypeIsFireworkData = true;
		}
			
		serialized = Format("%s{\n", serialized);
		
		if(needToSaveProto) // If the prototype is not constant, it is saved in the Prototype property instead of using 'new [Prototype] {'
		{
			serialized = Indent(serialized, depth+1);
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
			// Do not save properties that are only defined in the prototype,
			// but save the properties defined by the prototype 'FireworkData', eg 'version'
			// If there will be breaking changes, one may handle the data according to its version
			if(prototype && !prototypeIsFireworkData) 
			{
				//   Let's hope that resetting and then setting the Prototype again works fine			
				SetPrototype(nil, data);
				// if the Prototype's value for key is the same as the data's or
				// if the key does exist in the Prototype, but not in data itself,
				// we don't need to save that.
				if(prototype[key] == value || (prototype[key] && !data[key]))
				{
					SetPrototype(prototype, data);
					continue;
				}
				SetPrototype(prototype, data);
			}
				
			serialized = Indent(serialized, depth+1);
			var type = GetType(value);
			
			var specialString;
			if(specialString = GetSpecialSerializationString(key, value))
				serialized = Format("%s%s = %s,\n", serialized, key, specialString);
			else if(type == C4V_Int || type == C4V_Bool || type == C4V_String || type == C4V_Def)
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
		var notonlyint = false;
		for(var i = 0; i < GetLength(data); ++i)
			if(GetType(data[i]) != C4V_Int)
				++notonlyint;
		
		serialized = Format("%s[", serialized);
		
		if(notonlyint)
				serialized = Format("%s\n", serialized);
		
		for(var i = 0; i < GetLength(data); ++i)
		{
			var val = data[i];
			
			if(notonlyint)
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
				serialized = Format("%s, ", serialized);
			else
				serialized = Format("%s", serialized);
				
			if(notonlyint)
				serialized = Format("%s\n", serialized);
		}
		if(notonlyint)
			serialized = Indent(serialized, depth);
		serialized = Format("%s]", serialized);
	}
	return serialized;
}

public func SaveScenarioObject(proplist props)
{
	if(!inherited(props, ...))
		return false;
	if(fireworkActivated)
		return false;
	
	var saveName = this->MakeScenarioSaveName();	
	props->Add("FireworkData", "var %s_fireworkData = %s;\n	%s->SetFireworkData(%s_fireworkData)", saveName, SerializeFireworkDataForScript(fireworkData), saveName, saveName);
	
	return true;
}

