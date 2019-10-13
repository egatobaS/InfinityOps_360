# InfinityOps [360]
Black Ops GSC loader created by Sabotage &amp; ImJtagModz

## Description
All the latest CoDs use a scripting language called GSC, the game uses these scripts during missions, killstreaks and many other aspects of a match or session. While injecting custom GSC files has been possible and known for Black Ops 1 for a very long time, this program allows to inject a custom scripts while retaining the ability to connect with others players.

### Functionality
This program will load any assets (GSCs, CSCs, CSVs, Cfgs, etc...) from a folder in the game's directory. For this to work you must create this folder. The path should be "BlackOps1/raw/", BlackOps1 being the folder containing all the games files, eg: default_mp.xex and raw being the folder you must create to place the custom assets.

### Example

The following is an example of a script that could be loaded using this program. The script must be placed in "BlackOps1/raw/mp/map/gametype/_clientids.gsc", this will overwrite the script _clientids.gsc and run any custom code within this file. 

Note: you can also add any custom GSC files by including them as an include in this file or any other you decide to overwrite.

```c

#include common_scripts\utility;
#include maps\_utility;
#include maps\_hud_util;

init()
{
	level thread onPlayerConnect();
}

onPlayerConnect()
{
	for(;;)
	{
		level waittill( "connected", player );
		player thread onPlayerSpawned();
	}
}

onPlayerSpawned()
{
	for(;;)
	{
		self waittill("spawned_player");
		self iprintln("Infinity Ops Loaded!");
	}
}
```
## Credits

ImJtagModz, Sabotage, Blasts Mods and Stridder

