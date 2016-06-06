/**
	Firework Effects
	
	Contains effect prototypes
	
	Contains effect functions. These have to be called from object context.
	These are internals, for public interfaces see /Basic.ocd/FireworkBomb.ocd and /System.ocg/Firework.c

	@author Flinti
*/

/* -- Effect prototypes -- */

static const FW_Effect_Die = { 
								execfn = FireworkEffects.None, 
								type = FWE_Die, 
								die = true,
							};
static const FW_Effect_Emit = { 
								execfn = FireworkEffects.Emit, 
								type = FWE_Emit, 
								sound = "Firework::Blast?", 
								emitted = [], 
								amount = nil,
							};
static const FW_Effect_SimpleFireworks = { 
								execfn = FireworkEffects.SimpleFireworks, 
								type = FWE_SimpleFireworks, 
								sound = "Firework::Blast?",
							};

static const FWE_Die = 0;
static const FWE_Emit = 1;
static const FWE_SimpleFireworks = 2;


global func FW_Distribution_Linear(int min, int max, int deviationMin, int deviationMax)
{
	return [min, max, deviationMin, deviationMax];
}

global func FW_Distribution_Random(int min, int max)
{
	return [min, max];
}


/*static const FW_Distribution = new Global {
		distribution = FW_Distribution_Random,
	
		SetDistribution = func(int dist) {
			distribution = dist;
			if(distribution == FW_Distribution_Random || !dist)
				this.GetValue = this.GetValueRandom;
			else if(distribution == FW_Distribution_Even)
				this.GetValue = this.GetValueEven;
		},
	
		GetValueRandom = func(int index, int max, array valueSet) {
			return RandomX(valueSet[0], valueSet[1]);
		},
	
		GetValueEven = func(int index, int max, array valueSet) {
			return ((valueSet[0] + valueSet[2])*100 + (valueSet[1] - valueSet[0]) * index * 100 / max) / 100;
		},
	
		GetValue = func(int index, int max, array valueSet) {}
	};*/

protected func GetValueFromDistribution(distribution, int index, int amount, int typeIndex, bool angleMode)
{
	var type = GetType(distribution);
	
	if(type == C4V_Int)
		return distribution;
	else if(type == C4V_Array)
	{
		// Two values: Return a random value between them
		if(GetLength(distribution) == 2)
			return RandomX(distribution[0], distribution[1]);
		// Four values: Interpolate linearly between [0] and [1] ([0] + ([1]-[0]) * index/(amount-1)), add a random value between [2] and [3]
		//              For angles, do not include the value [1] (divide by amount instead of amount-1)
		else if(GetLength(distribution) == 4)
			return (RandomX(distribution[2], distribution[3]) + distribution[0]) + (distribution[1] - distribution[0]) * index / (amount - !angleMode);
	}
	/*else if(type == C4V_PropList)
	{
	}*/
	else
	{
		DebugLog("GetValueFromDistribution: distribution has wrong type (distribution = %v)", distribution);
		return 0;
	}
}


/* -- None -- */

public func None(proplist fweffect)
{
}

/* -- Simple Fireworks -- */

public func SimpleFireworks(proplist fweffect)
{
	Fireworks();
}


/* -- Emit -- */

public func Emit(proplist fweffect)
{
	/*
	amount
	emitted []
	angle
	radius
	rotation
	rotationSpeed
	rotationPrecision
	speed
	speedPrecision
	*/
	var length = GetLength(fweffect.emitted);
	var amount = fweffect.amount ?? length;
	//var distribution = new FW_Distribution {};
	//distribution->SetDistribution(fweffect.distribution);
	
	for(var i = 0; i < amount; ++i)
	{
		var emittedIndex = i % length;
		var curEmitted = fweffect.emitted[emittedIndex];
		
		if(curEmitted.fireworkData)
		{
			var angle = curEmitted.angle ?? [0, 359];
			angle = GetValueFromDistribution(angle, i, amount, emittedIndex, true) + GetR();
			
			var radius = curEmitted.radius ?? [0, 0];
			radius = GetValueFromDistribution(radius, i, amount, emittedIndex);
			
			var obj = CreateObject(FireworkBomb, Sin(angle, radius), -Cos(angle, radius));
			obj->SetController(GetController());
			
			var rotation = curEmitted.rotation ?? [0, 359];
			obj->SetR(GetValueFromDistribution(rotation, i, amount, emittedIndex, true));
			
			var rotationSpeed = curEmitted.rotationSpeed ?? [0, 0];
			obj->SetRDir(GetValueFromDistribution(rotationSpeed, i, amount, emittedIndex), curEmitted.rotationPrecision);
			
			var speed = curEmitted.speed ?? [0, 0];
			speed = GetValueFromDistribution(speed, i, amount, emittedIndex);
			obj->SetXDir(Sin(angle, speed) + GetXDir(curEmitted.speedPrecision) * curEmitted.relativeVelocity, curEmitted.speedPrecision);
			obj->SetYDir(-Cos(angle, speed) + GetYDir(curEmitted.speedPrecision) * curEmitted.relativeVelocity, curEmitted.speedPrecision);
			
			obj.fireworkData = curEmitted.fireworkData;
			obj->SetFused(this);
		}
	}
}




