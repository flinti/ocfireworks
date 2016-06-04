static const FWT_None = 0;
static const FWT_Glow = 1;
static const FWT_Sparkle = 2;

static const FWE_Die = 0;
static const FWE_Emit = 1;
static const FWE_SimpleFireworks = 2;

static const FWD_None = 0;
static const FWD_Random = 1;
static const FWD_Even = 2;



static const FW_Trail_None = { type = FWT_None };
static const FW_Trail_Glow = { type = FWT_Glow, size = 3, color = -1, lightRange = 4, lightFadeoutRange = 100 };
static const FW_Trail_Sparkle = { type = FWT_Sparkle, size = 3, color = -1, lightRange = 4, lightFadeoutRange = 100 };

static const FW_Effect_Die = { type = FWE_Die, die = true };
static const FW_Effect_Emit = { type = FWE_Emit, sound = "Firework::Blast?", emitted = [], amount = nil };
static const FW_Effect_SimpleFireworks = { type = FWE_SimpleFireworks, sound = "Firework::Blast?" };



/*global func FW_Distribution(distribution, int start_resp_par0, int end_resp_par1, array moreparams)
{ return [distribution, start_resp_par0, end_resp_par1, moreparams]; }

global func FW_Distribution_Same(int value)
{ return [FWD_None, value, value]; }

global func FW_Distribution_FullAngle(distribution)
{ return [distribution, 0, 359]; }

global func FW_Distribution_FromSet(array set)
{ return [FWD_FromSet, 0, 0, set ?? [0]]; }


global func FW_Trail_None() 
{ return [FWT_None]; }

global func FW_Trail_Glow(int color, int strength) 
{ return [FWT_Glow, color ?? RGB(255, 0, 0), strength ?? 10]; }

global func FW_Trail_Sparkle()
{ return [FWT_Sparkle]; }


global func FW_Action_Die() 
{ return [FWA_Die]; }

global func FW_Action_FuseContents() 
{ return [FWA_FuseContents]; }

global func FW_Action_CastContents(array speedDistribution, array angleDistribution, array rotationDistribution, array rotationSpeedDistribution, string sound, int amount) 
{ return [FWA_CastContents, 
			speedDistribution,
			angleDistribution ?? FW_Distribution_FullAngle(FWD_Random),
			rotationDistribution ?? FW_Distribution_FullAngle(FWD_Random),
			rotationSpeedDistribution ?? FW_Distribution_Same(0),
			sound ?? "Fire::Blast?",
			amount ?? -1
			]; }

global func FW_Action_CastSimpleFireworks() 
{ return [FWA_CastSimpleFireworks]; }*/
