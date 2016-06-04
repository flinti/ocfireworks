#include Library_Map


// Called be the engine: draw the complete map here.
protected func InitializeMap(proplist map)
{	
	var surface = {Algo = MAPALGO_Rect, X = 0, Y = map.Hgt/2, Wdt = map.Wdt, Hgt = map.Hgt};
	Draw("Earth", surface);	
	
	// Return true to tell the engine a map has been successfully created.
	return true;
}
