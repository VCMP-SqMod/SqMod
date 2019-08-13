// Compute the path to the scripts folder (use the `ScriptFolder` option from `sqmod.ini`)
ScriptsPath <- SqSysPath.Working().Append(SqCore.GetOption("ScriptFolder"));
// Log it for debug purposes
SqLog.Inf("Booting script from: %s", ScriptsPath.String);
// Load the command manager
SqCore.LoadScript(true, SqSysPath(ScriptsPath.String).Append("cmd.nut"));
// Load event scripts (delay them `true` since we don't need them executed right away)
SqCore.LoadScript(true, SqSysPath(ScriptsPath.String).Append("events").Append("blip.nut"));
SqCore.LoadScript(true, SqSysPath(ScriptsPath.String).Append("events").Append("checkpoint.nut"));
SqCore.LoadScript(true, SqSysPath(ScriptsPath.String).Append("events").Append("custom.nut"));
SqCore.LoadScript(true, SqSysPath(ScriptsPath.String).Append("events").Append("keybind.nut"));
SqCore.LoadScript(true, SqSysPath(ScriptsPath.String).Append("events").Append("object.nut"));
SqCore.LoadScript(true, SqSysPath(ScriptsPath.String).Append("events").Append("pickup.nut"));
SqCore.LoadScript(true, SqSysPath(ScriptsPath.String).Append("events").Append("player.nut"));
SqCore.LoadScript(true, SqSysPath(ScriptsPath.String).Append("events").Append("server.nut"));
SqCore.LoadScript(true, SqSysPath(ScriptsPath.String).Append("events").Append("vehicle.nut"));
// Load command scripts (delay them `true` since we don't need them executed right away)
