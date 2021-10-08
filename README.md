# Air Race Training
The Air Race Training Plugin is intended to be used with Panic's Air Race map. The map can be found in the steam workshop [here](https://steamcommunity.com/sharedfiles/filedetails/?id=2204381361 "Panics Air Race Map"). The plugin allows you to teleport to any of the checkpoints on the map. Check out the [Air Race Discord](https://discord.gg/CVmFzn4 "Air Race Discord") while you're at it too!

## How It Works
This plugin works by repeatedly teleporting your car to each trigger on the map until you reach the desired checkpoint. Be warned that this teleporting is very rapid and jarring the first time that you use the plugin.

## How To Use Plugin
Make sure that the plugin is enabled and that you are in an air race map. Select the checkpoint that you want to teleport to either by the plugins tab, or by use of commands. Press your reset button and you will be teleported to your desired checkpoint.

## How To Enable
- Click the checkbox on the bakkesmod settings page for the Air Race Training Plugin

## Commands
- `airracetraining_enabled` This is a true/false flag that determines if the plugin should perform teleports.
- `airracetraining_toggle_enabled` This will switch the enabled status from true to false and vice versa only if you are in the map.
- `airracetraining_movecheckup` This will move the selected checkpoint to the previous one.
- `airracetraining_movecheckdown` This will move the selected checkpoint to the next one.
- `airracetraining_panicsairrace_selectedcheck` This stores the value of your selected checkpoint.
- `airracetraining_chatbox_messages` This is a true/false flag that determines if messages should be sent to the chatbox.

An important note is that you can interact with all of the stored values in the settings page.

## Recommended Usage
- Bind the command `airracetraining_toggle_enabled` to a button or key. I use the up dpad.
- Bind the command `airracetraining_movecheckup` to a button or key. I use the left dpad.
- Bind the command `airracetraining_movecheckdown` to a button or key. I use the right dpad.

This set up allows you to easily turn the plugin on and off as well as change the checkpoint with the dpad.
