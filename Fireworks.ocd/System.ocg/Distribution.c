
global func FW_Distribution_Linear(int min, int max, int deviation)
{
	return [min, max, deviation];
}

global func FW_Distribution_Random(int min, int max)
{
	return [min, max];
}

global func FW_Distribution_Set(array set)
{
	return new FW_Distribution { type = FW_Distribution.Set, values = set };
}

global func FW_Distribution_DoubleSet(array set)
{
	return new FW_Distribution { type = FW_Distribution.DoubleSet, values = set };
}

global func FW_Distribution_LinearModulo(int min, int max, int stepAmount, int deviation)
{
	return new FW_Distribution { type = FW_Distribution.LinearModulo, values = [min, max, deviation], amount = stepAmount };
}

static const FW_Distribution = new Global {
	Random = 0,
	Linear = 1,
	Set = 2,
	DoubleSet = 3,
	LinearModulo = 4,

	GetValue =  func (distribution, int index, int amount, int typeIndex, bool circularMode)
	{
		var type = GetType(distribution);

		if(type == C4V_Int)
			return distribution;
		else if(type == C4V_Array)
		{
			var length = GetLength(distribution);
			// Two values: Return a random value between them
			if(length == 2)
				return RandomX(distribution[0], distribution[1]);
			// Three values: Interpolate linearly
			else if(length == 3)
				return GetValueLinear(distribution, index, amount, typeIndex, circularMode);
			// Some other number of values: Interpret as value set
			else
				return distribution[index % length];

		}
		else if(type == C4V_PropList)
		{
			var values = distribution.values;
			if(distribution.type == Random)
				return RandomX(values[0], values[1]);
			else if(distribution.type == Linear)
				return GetValueLinear(values, index, amount, typeIndex, circularMode);
			else if(distribution.type == Set)
				return values[index % GetLength(values)];
			else if(distribution.type == DoubleSet)
				return values[typeIndex][index % GetLength(values[typeIndex])];
			else if(distribution.type == LinearModulo)
				return GetValueLinear(values, index % distribution.amount, distribution.amount, nil, false);
			else
			{
				DebugLog("FW_Distribution.GetValue: Bad distribution definition (distribution = %v, distribution.type = %v)", distribution, distribution.type);
				return;
			}
		}
		return distribution;
		/*else
		{
			DebugLog("FW_Distribution.GetValue: distribution has wrong type (distribution = %v)", distribution);
			return;
		}*/
	},

	GetValueRandom = func(array distribution, int index, int amount, int typeIndex, bool circularMode)
	{
		return RandomX(distribution[0], distribution[1]);
	},
	// Interpolate linearly between [0] and [1] ([0] + ([1]-[0]) * index/(amount-1)), add a random value between -[2] and [2]
	// For circular intervals, do not include the value [1] (divide by amount instead of amount-1)
	GetValueLinear = func(array distribution, int index, int amount, int typeIndex, bool circularMode)
	{
		return (RandomX(-distribution[2], distribution[2]) + distribution[0]) + (distribution[1] - distribution[0]) * index / (amount - !circularMode);
	}
};

