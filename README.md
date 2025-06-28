# PW2Code
Code Injection Patches to upgrade the Pokémon White 2 Edition gameplay

## Features
To personalize the patch modify the ``settings.h`` file (I don't recomend modifying the non-boolean definitions unless you know what you are doing)
### Hard Level Caps
  - Limits the maximum level a Pokémon can be
  - If the level cap variable is set to 0 the level cap is ignored (0 is the default value)
  - Rare Candies ignore the level cap, you can change that in the ``settings.h`` file
  - Set the game variable 16415 to the level cap you want using CTRMap or some other editor:
    - To change the levelcap use the command WorkSet ``EventWorks.Set(16415, <new level cap>)`` in a script
### Disable EVs
  - Removes the gain of EVs from defeating Pokémons, independetly of the EV gain value in the Personal NARC
  - Does NOT prevent the gain of EVs through EV items like *Protein*
### Infinite Rare Candy Item
  - Allows you to level up a Pokémon until the level cap (100 by default).
  - Respects the hard level cap if that patch is active
  - The item ID for this is 622
### Infinite Repel Item
  - Allows you to toggle the repel effect on and off
  - Removes the functionality of regular repel effect items like Super Repel
  - The item ID for this is 623
### Repel Effect Removes Shaking Grass
  - Removes the chance of a shaking grass or a fishing spot appearing while the repel effect is active
### Dynamic Speed
  - Instead of calculating action order at the begining of every turn, the game updates it depending on the most actualized speed stats and priority
  - Fully functional with speed/priority items and the moves After You and Quash
### Gen6 crit changes
  - Crits damage boost updated to Gen6 (1.5 multiplier instead of doubling the damage)
  - Crit stages updated to Gen6
### Gen6 weather abilities
  - Weather triggered by abilites like Drizzle now last 5 turns
### Grass is immune to powder moves
  - Grass type Pokémon are unaffected by powder moves like *Spore* or *Rage Powder*
### Electric is immune to paralysis
  - Electric type Pokémon can't get paralized
### Ghosts ignore traps
  - Ghost type Pokémon can't get trapped by moves or abilities
### Mega-Evolution (BETA)
  - Activate Mega-Evolution by pressing the *Start* button while in the move selection menu
  - Known Issues:
    - Currently all megastones are replaced by the Master Ball
    - Mega forms are not implemented
    - There is no UI button or feedback for this yet
	- THIS FEATURE IS NOT RECOMENDED TO BE USED (it also requires the NitroKernell DLL to work)
### Full Ability Expansion
  - Every single ability from Gen6, Gen7, Gen8 and Gen9 has been added to the battle engine
  - Updated Intimidate interactions with existing abilities like *Inner Focus* or *Rattled*
  - To change a Pokémon ability to an expanded just modify the ability ID in that particular Pokémon Personal NARC (the ability IDs are preserved from vanilla games, but you can check them in ``BattleUpgrade.h``)
    - To give a Pokémon an ability with an ID higher than 255 you will need to use the 2 last bits of the respective wild item data, the code to do so is as follows:
    ```cpp
    // Set the item value
    u16 itemValue = itemID;
    itemValue &= 0x3FFF;
    itemValue += (ability & 0x0300) << 6;
    // Set the ability value
    u8 abilityValue = ability;
    ```
  - Known Issues:
    - RKS System is not implemented
    - Zero to Hero, displays the form change animation before switching out
    - Commander probably breaks in Rotation Battles (not tested)
    - Commander is compatible with Triple Battles (Tatsugiri can enter an "Out of Reach" Dondozo)
    - Hadron Engines message it's too long if used by an enemy, so part of the message is not shown
    - Coestar will choose a random ally in Triple Battles if it's in the center (so if one of the allies is boosted and the other one is not Flamigo could not boost on switch in)
    - Let me know any bugs or wrong mechanics that you find
### Full Battle Item Expansion
  - Every single battle item from Gen6, Gen7, Gen8 and Gen9 has been added to the battle engine
  - You can check the item IDs for the new items in ``BattleUpgrade.h``
  - Known Issues:
    - Items don't currectly have the correct sprites

## How to Install
### Installing tools
  - Set up a *Pokémon White 2 Edition USA* CTRMap project following this [guide](https://ds-pokemon-hacking.github.io/docs/generation-v/guides/bw_b2w2-using_ctrmap/)
  - Download and install [PW2Builder](https://github.com/Paideieitor/PW2Builder/releases)
  - Copy the ``Assets``, ``Global``, ``Headers``, ``Libraries`` and ``Patches`` folders to the ``PW2Builder`` folder
  - Copy the ``ESDB.yml`` and ``settings.h`` files to the ``PW2Builder`` folder

### Building the patch
  - Open a Command Prompt in the ``Builder`` folder and execute the following command:  
    ``PW2Builder.exe -build -whitelist-all``

## Reporting Issues/Contributing
You can message me through Discord, @dararoraro
If you are having troubles building the patches or running CTRMap, I recommend joining [Kingdom of DS Hacking](https://discord.gg/zAtqJDW2jC) and the [DS Modding Community](https://discord.gg/YBtdN3aXfv)

## Credits
* Hello007 - CTRMap and the Gen V code injection system
* PlatinumMaster - Gen V code injection system
* totally_anonymous - IDB documentation
* SpagoAsparago - Original Gen6 ability expansion
* CH340, totally_anonymous, Sunk, klark, Trusty Peaches and Brom - Aswering my questions, resolving doubts and hanging around
