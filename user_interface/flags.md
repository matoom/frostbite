## Server-side flags and settings

Changes in server-side flags are shared between all game clients.

The crossed out settings do not have any effect in the Frostbite frontend. 

It is important to note while changes in some settings might not be immediately apparent in the GUI, disabling them 
could disrupt data aggregation for the scripting engine that offers on-demand data access 
in the scripting API.

### Flags

The following commands are available from the game server

Syntax - `flag ConciseThoughts on`


|Flag           |Status|Behavior for this setting
|---------------|------|-------------------------
|LogOn          |ON    |Show logon messages.
|LogOff         |ON    |Show logoff messages.
|~~Disconnect~~ |~~OFF~~   |~~Do not show disconnection messages.~~
|ShowDeaths     |ON    |Show death messages when someone dies.
|RoomNames      |ON    |Display the name of the room in which you are located.
|Description    |ON    |Display room descriptions.
|RoomBrief      |OFF   |Display the full text of the room description.
|BattleBrief    |OFF   |Display normal battle descriptions.
|CombatBrief    |OFF   |Display normal battle messages for yourself and others.
|MonsterBold    |OFF   |Do not highlight monster names.
|Inactivity     |ON    |Increase time before the system logs you off when inactive.
|~~Portrait~~   |~~OFF~~   |~~Do not view character portraits.~~
|~~Dialogs~~    |~~OFF~~   |~~Do not display most dialog boxes.~~
|Inventory      |ON    |Display all inventory and container windows.
|AvoidJoiners   |OFF   |Allow people to join your group.
|AvoidHolders   |OFF   |Allow people to hold your hand and adding you to their group.
|AvoidDancers   |OFF   |Allow people to dance with you, and add you to their group.
|AvoidWhispers  |OFF   |Allow people to whisper to you.
|AvoidDraggers  |ON    |Prevent people from dragging your body.
|AvoidTeachers  |OFF   |Allow people to teach you.
|AvoidSinging   |OFF   |Allow yourself to sing.
|NoHarnessShare |OFF   |Allow the shareing of harness with your group.
|HarnessWarning |ON    |Receive notifications about harness limitations.
|HarnessVerbose |ON    |Receive detailed notifications about harness limitations.
|Autosneak      |OFF   |Do not automatically sneak from room to room when in hiding.
|BriefExp       |OFF   |Display verbose information in the Field Experience window in Stormfront.
|ConciseThoughts|ON    |Gweth messages will be shorter.


### Set

Syntax - `set LogOn" or "set !LogOn"`

|Setting|Behaviour for the setting|
|--- |---|
|LogOn         |(receive messages when someone logs on) |
|LogOff        |(receive messages when someone logs off)|
|!Disconnect   | (receive messages when someone disconnects)|
|Death         |(receive messages when someone dies)|
|RoomNames     |(displays the name of the room you are in)|
|Description   |(determines if room descriptions are displayed or not)|
|!RoomBrief    | (will shorten the output of the room description)|
|!BattleBrief  | (shortens messages during battle)|
|!CombatBrief  | (leaves your battle messages full while shortening others)|
|!MonsterBold  | (highlights monster names, requires ANSI emulation)|
|!StatusPrompt | (displays status information in front of the prompt)|
|!DesignerNotes| (determines inclusion in this game's mailing lists)|
|!Profession   | (toggles display of your profession)|
|Inactivity    |(increases time before system logs you off when inactive)|
